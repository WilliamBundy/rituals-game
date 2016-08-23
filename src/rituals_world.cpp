/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 * rituals_world.cpp
 */

struct World
{
	char* name;

	World_Area_Stub* area_stubs;
	isize next_area_id;
	isize areas_count, areas_capacity;
	isize areas_width, areas_height;

	World_Area* current_area;
	usize seed;
};

void init_world(World* world, isize width, isize height, usize seed, Memory_Arena* arena)
{
	world->seed = seed;
	world->areas_capacity = width * height * 2;
	world->area_stubs = arena_push_array(arena, World_Area_Stub, world->areas_capacity);
	world->areas_count = 0;
	world->areas_width = width;
	world->areas_height = height;
	world->next_area_id = 0;
	world->current_area = NULL;
}

void deserialize_area(World_Area* area, FILE* file, Memory_Arena* arena);
FILE* get_area_file(const char* name, isize id, const char* mode);
FILE* get_world_file(const char* name, const char* mode);
int check_path(char* path);
World_Area* world_load_area(World* world, isize id, Memory_Arena* arena)
{
	FILE* fp = get_area_file(world->name, id, "rb");
	World_Area* area = NULL;
	if(fp != NULL) {
		area = arena_push_struct(arena, World_Area);
		deserialize_area(area, fp, arena);
		area->world = world;
		area->stub = world->area_stubs + id;
		for(isize i = 0; i < area->entities_count; ++i) {
			Entity* e = area->entities + i;
		}
		world_area_synchronize_entities_and_bodies(area);
		world_area_init_player(area, v2i(0,0), false);

		fclose(fp);
	}

	return area;
}

//TODO(will) call init_world_area on area before calling
Entity* rituals_spawn_enemy(World_Area* area, isize enemykind, Vec2 position);
void generate_world_area(World* world, World_Area* area, World_Area_Stub* stub)
{
	area->stub = stub;
	area->world = world;
	Random rand;
	Random* r = &rand;
	init_random(r, stub->seed);
	generate_tilemap(&area->map, stub->seed);
	area->id = stub->id;
	for(isize i = 0; i < WorldAreaTilemapWidth / 4; ++i) {
		Entity* e = world_area_get_next_entity(area);
		Sim_Body* b = sim_find_body(&area->sim, e->body_id);
		e->sprite.texture = Get_Texture_Coordinates(8*32, 16, 32, 48);
		b->shape.hw = 15;
		b->shape.hh = 11;
		b->inv_mass = 1.0f;
		e->sprite.size = v2(32, 48);
		e->sprite.anchor = Anchor_Bottom;
		//e->sprite.center = v2(0, 20);
		do {
			b->shape.center = v2(
					rand_range(r, 0, area->map.w * 32),
					rand_range(r, 0, area->map.h * 32));
		}
		while (Registry->tiles[tilemap_get_at(&area->map, b->shape.center)].solid);
	}

	for(isize i = 0; i < WorldAreaTilemapWidth / 4; ++i) {
		Entity* e = world_area_get_next_entity(area);
		Sim_Body* b = sim_find_body(&area->sim, e->body_id);
		e->sprite.texture = Get_Texture_Coordinates(0, 5*32, 96, 144);
		b->shape.hw = 16;
		b->shape.hh = 15;
		b->inv_mass = 1.0f;
		b->flags = Body_Flag_Static;
		e->sprite.size = v2(96, 144) * 2;
		e->sprite.anchor = Anchor_Bottom;
		e->sprite.sort_point_offset = v2(0, -60);
		e->sprite.center = v2(2, -b->shape.hh * 2);
		
		do {
			b->shape.center = v2(
					rand_range(r, 0, area->map.w * 32),
					rand_range(r, 0, area->map.h * 32));
		}
		while (Registry->tiles[tilemap_get_at(&area->map, b->shape.center)].solid);
	}

	for(isize i = 0; i < 64; ++i) {
		Vec2 pos;
		do {
			pos = v2(
					rand_range(r, 0, area->map.w * 32),
					rand_range(r, 0, area->map.h * 32));
		}
		while (Registry->tiles[tilemap_get_at(&area->map, pos)].solid);
		rituals_spawn_enemy(area, rand_range_int(r, 0, 4), pos);
	}

	generate_statics_for_tilemap(&area->sim, &area->map);
	world_area_init_player(area, v2i(WorldAreaTilemapWidth / 2, WorldAreaTilemapHeight /2));
}

void serialize_world(World* world);
void world_switch_current_area(World* world, Area_Link link, Memory_Arena* arena)
{
	if(link.link == NULL) return;
	//world_area_deinit_player(world->current_area);
	//TODO(will) free old current area
	
	serialize_world(world);
	clear_arena(arena);
	World_Area* new_area = world_load_area(world, link.link->id, arena);
	if(new_area == NULL) {
		new_area = arena_push_struct(arena, World_Area);
		init_world_area(new_area, arena);
		generate_world_area(world, new_area, link.link);
	}
	world_area_init_player(new_area, link.position);
	world->current_area = new_area;
}

void world_start_in_area(World* world, World_Area_Stub* area, Memory_Arena* arena)
{
	World_Area* new_area = world_load_area(world, area->id, arena);
	if(new_area == NULL) {
		new_area = arena_push_struct(arena, World_Area);
		init_world_area(new_area, arena);
		generate_world_area(world, new_area, area);
	}
	world_area_init_player(new_area, v2i(0, 0),  false);
	world->current_area = new_area;
}

void generate_world(char* name, World* world)
{
	world->name = name;

	for(isize i = 0; i < world->areas_height; ++i) {
		for(isize j = 0; j < world->areas_width; ++j) {
			isize index = i * world->areas_width + j;
			World_Area_Stub* stub = world->area_stubs + world->areas_count++; 
			stub->id = world->next_area_id++;
			stub->seed = world->seed + stub->id;
			isize north_link = modulus(i - 1, world->areas_height) * world->areas_width + j;
			isize south_link = modulus(i + 1, world->areas_height) * world->areas_width + j;
			isize west_link = i * world->areas_width + modulus(j - 1, world->areas_width);
			isize east_link = i * world->areas_width + modulus(j + 1, world->areas_width);

			stub->north = Area_Link {
				v2i(WorldAreaTilemapWidth / 2,  WorldAreaTilemapHeight - 1), 
					world->area_stubs + north_link
			};
			stub->south = Area_Link {
				v2i(WorldAreaTilemapWidth / 2, 1),
					world->area_stubs + south_link
			};
			stub->west = Area_Link {
				v2i(WorldAreaTilemapWidth - 1, WorldAreaTilemapHeight / 2),
					world->area_stubs + west_link
			};
			stub->east = Area_Link {
				v2i(1, WorldAreaTilemapHeight / 2),
					world->area_stubs + east_link
			};

		}
	}
}
//TODO(will) Implement packages -- hook up using function pointers
//For the time being we're just going to use forward declarations
void rituals_walk_entities(Entity* entities, isize count, World_Area* area, World* world);
void rituals_animate_entities(Entity* entities, isize count, World_Area* area, World* world);
void rituals_interact_entities(Entity* entities, isize count, World_Area* area, World* world);
void rituals_hit_entities(Hitbox_Contact* contacts, isize count, World_Area* area, World* world);
void rituals_contact_entities(Sim_Contact* contacts, isize count, World_Area* area, World* world);

void world_area_walk_entities(World_Area* area, World* world)
{
	rituals_walk_entities(area->entities, area->entities_count, area, world);
}

void world_area_animate_entities(World_Area* area, World* world)
{
	rituals_animate_entities(area->entities, area->entities_count, area, world);
}

void world_area_hit_entities(World_Area* area, World* world)
{
	rituals_hit_entities(area->hitbox_contacts, area->hitbox_contacts_count, area, world);
}

void world_area_contact_entities(World_Area* area, World* world)
{
	rituals_contact_entities(area->sim.contacts, area->sim.contacts_count, area, world);
}

void world_area_interact(World_Area* area, World* world)
{
	rituals_interact_entities(area->entities, area->entities_count, area, world);
}

void world_area_render(World_Area* area, World* world)
{	
	Vec2 target = area->target;
	area->offset += (target - area->offset) * 0.1f;
	area->offset -= Game->size * 0.5f;
	if(area->offset.x < 0) 
		area->offset.x = 0;
	else if((area->offset.x + Game->size.x) > area->map.w * Tile_Size)
		area->offset.x = area->map.w * Tile_Size - Game->size.x;

	if(area->offset.y < 0) 
		area->offset.y = 0;
	else if((area->offset.y + Game->size.y) > area->map.h * Tile_Size)
		area->offset.y = area->map.h * Tile_Size - Game->size.y;

	Renderer->offset = area->offset;
	area->offset += Game->size * 0.5f;

	renderer_start();

	Rect2 screen = rect2(
			area->offset.x - Game->size.x / 2,
			area->offset.y - Game->size.y / 2, 
			Game->size.x, Game->size.y);

	isize sprite_count_offset = render_tilemap(&area->map, v2(0,0), screen);

	for(isize i = 0; i < area->entities_count; ++i) {
		Entity* e = area->entities + i;
		Sim_Body* b = sim_find_body(&area->sim, e->body_id);

		if (b == NULL) continue;
		e->sprite.position = b->shape.center;
		e->sprite.position.y += b->shape.hh;
		//e->sprite.size = v2(b->shape.hw * 2, b->shape.hh * 2);
		
		//TODO(will) align entity sprites by their bottom center
		renderer_push_sprite(&e->sprite);
#if 0
		draw_box_outline(e->hitbox.box.center + e->sprite.position + v2(0, 1), e->hitbox.box.hext * 2, v4(1, 1, 1, 1), 1);
#endif
	}
	renderer_sort(sprite_count_offset);

#if 0
	for(isize i = 0; i < area->sim.bodies_count; ++i) {
		Sim_Body* b = area->sim.bodies + i;

		if (b == NULL) continue;
		draw_box_outline(b->shape.center, b->shape.hext * 2, v4(.6f, 1, .6f, 1), 1);
	}
#endif
	renderer_draw();

}

void world_area_update(World_Area* area, World* world)
{
	game_set_scale(2.0);
	game_calc_mouse_pos(area->offset - Game->size * 0.5f);
	//Simulation timing
	
	area->player = world_area_find_entity(area, 0);
	if(!play_state->running) {
		world_area_render(area, world);
		return;
	}

	play_state->current_time = SDL_GetTicks();
	real dt = (play_state->current_time - play_state->prev_time) / 1000.0;
	dt = clamp(dt, 0, 1.2f);
	play_state->accumulator += dt;
	play_state->prev_time = play_state->current_time;
	sim_sort_bodies_on_id(&area->sim);
	area->player = world_area_find_entity(area, 0);

	world_area_walk_entities(area, world);
	//TODO(will) use same sorted array as world_area_walk_entities
	for(isize i = 0; i < area->entities_count; ++i) {
		Entity* e = area->entities + i;
		e->body->velocity += e->walk_impulse;
	}

	isize times = 0;
	while(play_state->accumulator >= TimeStep) {
		play_state->accumulator -= TimeStep;
		sim_update(&area->sim, &area->map, TimeStep, times == 0);
		times++;
	}

	world_area_build_hitboxes(area);
	world_area_process_hitboxes(area);
	
	Vec2 target = area->player->body->shape.center;
	area->target = target;
	world_area_animate_entities(area, world);
	if(target.x < 0) {
		world_switch_current_area(play_state->world, area->stub->west, Game->play_arena);
		play_state->world_xy.x--;
	} else if(target.x > area->map.w * Tile_Size) {
		world_switch_current_area(play_state->world, area->stub->east, Game->play_arena);
		play_state->world_xy.x++;
	} else if(target.y < 0) {
		world_switch_current_area(play_state->world, area->stub->north, Game->play_arena);
		play_state->world_xy.y--;
	} else if(target.y > area->map.h * Tile_Size) {
		world_switch_current_area(play_state->world, area->stub->south, Game->play_arena);
		play_state->world_xy.y++;
	}
	world_area_interact(area, world);
	world_area_render(area, world);

	if(Input->mouse[SDL_BUTTON_LEFT] == State_Just_Pressed) {
		for(isize i = 0; i < 8; ++i) {
			Entity* e = world_area_get_next_entity(area);
			e->kind = EntityKind_Prop;
			e->sprite = get_box_sprite(area->player->sprite.position - v2(0, 16), v2(2, 2), v4(1, 1, 1, 1));
			e->sprite.position += v2(
					rand_range(&Game->r, -2, 2),
					rand_range(&Game->r, -2, 2));
			e->body->shape.center = e->sprite.position;
			e->sprite.anchor = Anchor_Bottom;
			e->body->shape.hext = v2(1, 1);
			e->body->flags = Body_Flag_No_Friction | Body_Flag_Sensor;
			e->body->damping = 1.0f;
			e->hitbox.mask = Flag(2);

			Vec2 dmouse =  Input->mouse_pos - e->sprite.position; 
			real a = v2_to_angle(dmouse);
			a += rand_range(&Game->r, -5, 5) * Math_Deg2Rad;

			e->body->velocity = v2_from_angle(a) * 800;
		}
	}
	world_area_hit_entities(area, world);
	world_area_contact_entities(area, world);
}




