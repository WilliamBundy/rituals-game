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

#if 0
Vec2 _player_controls(World_Area* area, Entity* player_entity, Sim_Body* player)
{
	real movespeed = 800;
	Vec2 move_impulse = v2(0, 0);

	if(_check(LEFT, A, State_Pressed)) {
		move_impulse.x -= movespeed;
	}
	if(_check(RIGHT, D, State_Pressed)) {
		move_impulse.x += movespeed;
	}
	if(_check(UP, W, State_Pressed)) {
		move_impulse.y -= movespeed;
	}
	if(_check(DOWN, S, State_Pressed)) {
		move_impulse.y += movespeed;
	}

	if(fabsf(move_impulse.x * move_impulse.y) > 0.01f) {
		move_impulse *= Math_InvSqrt2;
	}

	Tile_Info* player_tile = Registry->tiles + tilemap_get_at(&area->map, player->shape.center);

	move_impulse *= player_tile->movement_modifier;
	return move_impulse;
}

//TODO(will) store move impulse on entities to reference later
void _player_animate(World_Area* area, Entity* player_entity, Sim_Body* player, Vec2 move_impulse)
{
	Direction old_direction = player_entity->direction;
	if(move_impulse.y < 0) {
		player_entity->direction = Direction_North;
	} else if(move_impulse.y > 0) {
		player_entity->direction = Direction_South;
	}

	if(move_impulse.x < 0) {
		player_entity->facing = -1;
		player_entity->direction = Direction_West;
	} else if(move_impulse.x > 0) {
		player_entity->facing = 1;
		player_entity->direction = Direction_East;
	}
	if(Input->scancodes[SDL_SCANCODE_SPACE] == State_Pressed) {
		player_entity->direction = old_direction;
	}
	Sprite* plr_spr = &player_entity->sprite;
	int32 player_frame = 0;
	if(v2_dot(move_impulse, move_impulse) > 0){
		player_entity->counter++;
		player_frame = 1;
		if(player_entity->counter > 15) {
			player_frame = 0;
			if(player_entity->counter > 30) {
				player_entity->counter = 0;
			}
		}
	} else {
		player_entity->counter = 0;
		player_frame = 0;
	}

	if(player_entity->facing == -1) {
		plr_spr->texture = Get_Texture_Coordinates(32 + player_frame * 32, 0, -32, 32);
	} else if(player_entity->facing == 1) {
		plr_spr->texture = Get_Texture_Coordinates(0  + player_frame * 32, 0, 32, 32);
	}
}

void serialize_world(World* world);
void _player_handle_interactions(World* world, World_Area* area, Entity* player_entity, Sim_Body* player)
{
	if(Input->mouse[SDL_BUTTON_LEFT] == State_Just_Pressed) {
		Entity* ball_entity = world_area_get_next_entity(area);
		Sim_Body* ball = ball_entity->body;
		Vec2 dmouse = v2(Input->mouse_x / Game->scale, 
						Input->mouse_y / Game->scale) + area->offset;

		dmouse -= player->shape.center;
		real angle = atan2f(dmouse.y, dmouse.x);
		Vec2 normal = v2(cosf(angle), sinf(angle));
		//entity_add_event_on_activate(ball_entity, delete_on_activate);

		ball->damping = 0.9999f;
		ball->shape.hext = v2(8, 16);
		ball->shape.center = normal * ball->shape.hw * 4 + player->shape.center; 
		ball->velocity += normal * 2000;
		ball->shape.hext = v2(8, 6);
		//ball->flags = Body_Flag_No_Friction;
		ball_entity->sprite.size = v2(16, 32);
		ball_entity->sprite.texture  = Get_Texture_Coordinates(8*32, 0, 32, 64);
		ball_entity->sprite.anchor = Anchor_Bottom;
		
	}


	/*
	if(Input->mouse[SDL_BUTTON_RIGHT] == State_Just_Pressed) {
		Vec2 dmouse = v2(
			Input->mouse_x / Game->scale, 
			Input->mouse_y / Game->scale) + area->offset;
		AABB mbb = aabb(dmouse, 0, 0);
		world_area_synchronize_entities_and_bodies(area);
		for(isize i = 0; i < area->entities_count; ++i) {
			Entity* e = area->entities + i;
			if(aabb_intersect(e->body->shape, mbb)) {
				if(e->id != 0) {
					for(isize j = 0; j < e->event_on_activate_count; ++j) {
						e->event_on_activate[j](e, area);
					}
					break;
				}
			}
		}
	}
*/
	if(Input->scancodes[SDL_SCANCODE_F] == State_Just_Pressed) {
		//tilemap_set_at(&area->map, player->shape.center, Tile_Dug_Earth);
		Tile_State* state = tilemap_get_state_at(&area->map, player->shape.center);
		if(state != NULL) {
			state->damage++;
			update_tile_state_at(&area->map, player->shape.center);
		}
	}

	if(Input->scancodes[SDL_SCANCODE_ESCAPE] >= State_Just_Pressed) {
		serialize_world(world);
		Game->state = Game_State_Menu;
	}

	Sprite s;


#if 0	
	if(Input->scancodes[SDL_SCANCODE_SPACE] >= State_Pressed) {
		init_sprite(&s);
		s.position = player->shape.center;
		s.size = v2(16, 16);
		s.texture = Get_Texture_Coordinates(0, Renderer->texture_height - 16, 16, 16);
		s.color = v4(1, 1, 1, 1);
		switch(player_entity->direction) {
			case Direction_North:
				s.position.y -= s.size.y + player->shape.hh;
				break;
			case Direction_South:
				s.position.y += s.size.y + player->shape.hh;
				break;
			case Direction_East:
				s.position.x += s.size.x + player->shape.hw;
				break;
			case Direction_West:
				s.position.x -= s.size.x + player->shape.hh;
				break;
		}

		if(Input->scancodes[SDL_SCANCODE_SPACE] == State_Just_Pressed) {
			//TODO(will) implement good space queries	
			Sim_Body* touching = sim_query_aabb(&area->sim, 
					aabb(s.position, s.size.x / 2, s.size.y / 2));
			if(touching != NULL) {
				if(!Has_Flag(touching->flags, Body_Flag_Static)) 
					player_entity->held_entity_id = touching->entity_id;
			}
		}
	} else {
		player_entity->held_entity_id = -1;
	}
	

	char debug_str[256];
	if(player_entity->held_entity_id > 0) {
		Entity* e = world_area_find_entity(area, player_entity->held_entity_id);
		if(e != NULL) {
			Sim_Body* b = e->body;
			Vec2 target = player->shape.center; 
			Vec2 diff = b->shape.hext + player->shape.hext + v2(8, 8);
			switch(player_entity->direction) {
				case Direction_North:
					target.y -= diff.y;
					break;
				case Direction_South:
					target.y += diff.y;
					break;
				case Direction_East:
					target.x += diff.x;
					break;
				case Direction_West:
					target.x -= diff.x;
					break;
			}
			
			Vec2 impulse = (target - b->shape.center);
			if(v2_dot(impulse, impulse) > (4 * Tile_Size * Tile_Size)) {
				player_entity->held_entity_id = -1;
			}
			impulse *= 60;
			{
				snprintf(debug_str, 256, "T(%.2f %.2f) j(%.2f %.2f)", target.x, target.y,
						impulse.x, impulse.y);

			}
			if(v2_dot(impulse, impulse) < (1000 * 1000)) 
				b->velocity += impulse;// * b->inv_mass;
		}
	}
#endif
}

#endif
void world_area_update(World_Area* area, World* world)
{
	game_set_scale(2.0);
	//Simulation timing
	play_state->current_time = SDL_GetTicks();
	real dt = (play_state->current_time - play_state->prev_time) / 1000.0;
	dt = clamp(dt, 0, 1.2f);
	play_state->accumulator += dt;
	play_state->prev_time = play_state->current_time;
	sim_sort_bodies_on_id(&area->sim);
	area->player = world_area_find_entity(area, 0);
	//Entity* player_entity = world_area_find_entity(area, 0);
	//Sim_Body* player = player_entity->body;

	//Vec2 move_impulse = _player_controls(area, player_entity, player);

	while(play_state->accumulator >= TimeStep) {
		play_state->accumulator -= TimeStep;
		//player->velocity += move_impulse;
		sim_update(&area->sim, &area->map, TimeStep);
	}
	
#if 0
	_player_animate(area, player_entity, player, move_impulse);
	Vec2 target = player->shape.center;
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

	_player_handle_interactions(world, area, player_entity, player);

	Renderer->offset = area->offset;
	area->offset += Game->size * 0.5f;
	// throw a ball

#endif 
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
	}
	renderer_sort(sprite_count_offset);

#if 0
	for(isize i = 0; i < area->sim.bodies_count; ++i) {
		Sim_Body* b = area->sim.bodies + i;

		if (b == NULL) continue;
		draw_box_outline(b->shape.center, b->shape.hext * 2, v4(1, 1, 1, 1), 1);
	}
#endif
	renderer_draw();
}




