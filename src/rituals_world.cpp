/*
 * rituals_world.cpp
 */

isize _tw, _th;
uint8* _tiles;
uint8 _get_at(isize x, isize y)
{
	if((x < 0) || (x > _tw) || (y < 0) || (y > _th)) return false;
	isize index = y * _tw + x;
	if((index < 0) || (index >= _tw * _th)) return false;
	return _tiles[index];

}

void generate_statics_for_tilemap(Simulator* sim, Tilemap* tilemap)
{
	start_temp_arena(game->temp_arena);
	_tw = tilemap->w;
	_th = tilemap->h;
	isize map_size = tilemap->w * tilemap->h;
	uint8* tiles = Arena_Push_Array(game->temp_arena, uint8, map_size + 1);
	for(isize i = 0; i < map_size; ++i) {
		tiles[i] = tilemap->info[tilemap->tiles[i]].solid;
	}
	_tiles = tiles;
	isize work = 0;

	Rect2i* rects = Arena_Push_Array(game->temp_arena, Rect2i, map_size / 2);
	isize rects_count = 0;
	isize last_rects = 0;
	do {
		last_rects = rects_count;
		for(isize y = 0; y < tilemap->h; ++y) {
			for(isize x = 0; x < tilemap->w; ++x) {
				if(_get_at(x, y)) {
					if(!_get_at(x, y - 1)) {
						Rect2i* r = rects + rects_count++;
						r->x = x;
						r->y = y;
						r->w = 1;
						r->h = 1;
						do {
							x++;
						}
						while(_get_at(x, y) && !_get_at(x, y - 1) && (x < tilemap->w));

						if(x != r->x) {
							r->w = x - r->x;
						}
					}	
				}
			}
		}

		for(isize i = last_rects; i < rects_count; ++i) {
			Rect2i* r = rects + i;
			bool solid = true;
			isize y = r->y;
			while(solid && (y < tilemap->h)) {
				for(isize local_x = 0; local_x < r->w; ++local_x) {
					solid = solid && _get_at(r->x + local_x, y + 1);
					if(!solid) break;
				}
				if(solid) {
					y++;
					r->h++;
				}
			}
		}

		for(isize i = 0; i < rects_count; ++i) {
			Rect2i* r = rects + i;
			for(isize local_y = 0; local_y < r->h; ++local_y) {
				for(isize local_x = 0; local_x < r->w; ++local_x) {
					isize index = (local_y + r->y) * tilemap->w + (local_x + r->x);
					//printf("%d ", index);
					tiles[index] = false;
				}
			}
		}
		work = 0;
		for(isize i = 0; i < map_size; ++i) {
			work += tiles[i];
		}
	} while(work);
	
	for(isize i = 0; i < rects_count; ++i) {
		Rect2i* r = rects + i;
		Sim_Body* e = sim_get_next_body(sim);
		e->shape.center.x = (r->x + r->w / 2.0f) * Tile_Size;//+ Half_TS;
		e->shape.center.y = (r->y + r->h / 2.0f) * Tile_Size;// + Half_TS;
		e->shape.hw = r->w * Half_TS;
		e->shape.hh = r->h * Half_TS;
		e->flags = Body_Flag_Static;
	}
	end_temp_arena(game->temp_arena);
}

typedef struct World_Area World_Area;

struct Entity
{
	isize id;
	isize body_id;
	Sim_Body* body;
	Sprite sprite;

	World_Area* area;
};

#define _entity_get_id(e) (e.id)
Generate_Quicksort_For_Type(entity_sort_on_id, Entity, _entity_get_id)
Generate_Binary_Search_For_Type(entity_search_for_id, Entity, isize, _entity_get_id)


typedef struct World_Area World_Area;
struct Area_Link
{
	Vec2i position;
	World_Area* link;
};

#define World_Area_Entity_Capacity (8192)
#define World_Area_Tilemap_Width (64)
#define World_Area_Tilemap_Height (64)
struct World_Area
{
	Simulator sim;
	Tilemap map;
	Vec2 offset;

	Entity* player;
	Sim_Body* player_body;
	Entity* entities;
	bool entities_dirty;
	isize entities_count, entities_capacity, next_entity_id;

	Area_Link north;
	Area_Link south;
	Area_Link west;
	Area_Link east;
};


void init_world_area(World_Area* area, Memory_Arena* arena)
{
	init_simulator(&area->sim, World_Area_Entity_Capacity, arena);
	init_tilemap(&area->map, 
			World_Area_Tilemap_Width,
			World_Area_Tilemap_Height,
			arena);

	area->entities = Arena_Push_Array(arena, Entity, World_Area_Entity_Capacity);
	area->entities_count = 0;
	area->entities_capacity = World_Area_Entity_Capacity;
	area->next_entity_id = 0;
	area->entities_dirty = false;

}

void init_entity(Entity* entity)
{
	entity->id = 0;
	entity->body_id = 0;
	init_sprite(&entity->sprite);
	entity->area = NULL;
}

Entity* world_area_get_next_entity(World_Area* area)
{
	if(area->entities_count + 1 >= area->entities_capacity) {
		Log_Error("Ran out of entities");
		return NULL;
	}

	Entity* e = area->entities + area->entities_count++;
	init_entity(e);
	e->area = area;
	e->body_id = sim_get_next_body(&area->sim)->id;
	e->id = area->next_entity_id++;
	return e;
}

Entity* world_area_find_entity(World_Area* area, isize id)
{
	if(area->entities_dirty) {
		entity_sort_on_id(area->entities, area->entities_count);
	}
	isize index = entity_search_for_id(id, area->entities, area->entities_count);
	return index == -1 ? NULL : area->entities + index;
}

void world_area_init_player(World_Area* area, Vec2i tile_pos)
{
	Entity* player_entity = world_area_find_entity(area, 0);
	Sim_Body* player = sim_find_body(&area->sim, player_entity->body_id);
	if(player == NULL) {
		printf("Something went wrong! Couldn't find player entity....?");
	}
	///player->shape.center = v2(area->map.w * 16, area->map.h * 16);
	player->shape.center = v2(tile_pos.x * Tile_Size, tile_pos.y * Tile_Size);
	player_entity->sprite.texture = Get_Texture_Coordinates(0, 0, 32, 32);
	player->shape.hext = v2(5, 5);
	player_entity->sprite.size = v2(32, 32);
	player_entity->sprite.center = v2(0,11);
	area->offset = player->shape.center;	
}

void world_area_deinit_player(World_Area* area)
{
	Entity* player_entity = world_area_find_entity(area, 0);
	Sim_Body* player = sim_find_body(&area->sim, player_entity->body_id);
	player->shape.center = v2(-1000, -1000);
}

struct World
{
	World_Area* areas;
	isize areas_count, areas_capacity;
	isize areas_width, areas_height;

	World_Area* current_area;
};

void world_switch_current_area(World* world, Area_Link link)
{
	if(link.link == NULL) return;
	World_Area* new_area = link.link;
	world_area_init_player(new_area, link.position);
	world_area_deinit_player(world->current_area);
	world->current_area = new_area;
}

void init_world(World* world, isize width, isize height, Memory_Arena* arena)
{
	world->areas_capacity = width * height * 2;
	world->areas = Arena_Push_Array(arena, World_Area, world->areas_capacity);
	world->areas_count = 0;
	world->areas_width = width;
	world->areas_height = height;
}

void generate_world(World* world, Tile_Info* info, isize ti_count, uint64 seed, Memory_Arena* arena)
{
	Random r_s;
	Random* r = &r_s;
	init_random(r, seed);


	for(isize i = 0; i < world->areas_height; ++i) {
		for(isize j = 0; j < world->areas_width; ++j) {
			isize index = i * world->areas_width + j;
			World_Area* area = world->areas + index;
			init_world_area(area, arena);
			area->map.info = info;
			area->map.info_count = ti_count;
			generate_tilemap(&area->map, next_random_uint64(r));
			for(isize i = 0; i < 256; ++i) {
				Entity* e = world_area_get_next_entity(area);
				Sim_Body* b = sim_find_body(&area->sim, e->body_id);
				e->sprite.texture = Get_Texture_Coordinates(0, 96, 32, 64);
				b->shape.hw = 16;
				b->shape.hh = 12;
				e->sprite.size = v2(32, 64);
				e->sprite.center = v2(0, 20);
				b->shape.center = v2(
						rand_range(r, 0, area->map.w * 32),
						rand_range(r, 0, area->map.h * 32));
			}
			generate_statics_for_tilemap(&area->sim, &area->map);

			area->north = Area_Link {
				v2i(World_Area_Tilemap_Width / 2,  World_Area_Tilemap_Height - 1), 
				world->areas + (((i - 1) % world->areas_height) * world->areas_width + (j)),
			};
			area->south = Area_Link {
				v2i(World_Area_Tilemap_Width / 2, 1),
				world->areas + ((i + 1) % world->areas_height) * world->areas_width + (j)
			};
			area->west = Area_Link {
				v2i(World_Area_Tilemap_Width - 1, World_Area_Tilemap_Height / 2),
				world->areas + ((i) * world->areas_width + ((j - 1) % world->areas_width))
			};
			area->east = Area_Link {
				v2i(1, World_Area_Tilemap_Height / 2),
				world->areas + ((i) * world->areas_width + ((j + 1) % world->areas_width))
			};
		}
	}

}

void update_world_area(World_Area* area)
{
	game_set_scale(2.0);
	real movespeed = 800;
	Vec2 move_impulse = v2(0, 0);
	if(input->scancodes[SDL_SCANCODE_LEFT] == State_Pressed) {
		move_impulse.x -= movespeed;
	}
	if(input->scancodes[SDL_SCANCODE_RIGHT] == State_Pressed) {
		move_impulse.x += movespeed;
	}
	if(input->scancodes[SDL_SCANCODE_UP] == State_Pressed) {
		move_impulse.y -= movespeed;
	}
	if(input->scancodes[SDL_SCANCODE_DOWN] == State_Pressed) {
		move_impulse.y += movespeed;
	}


	if(fabsf(move_impulse.x * move_impulse.y) > 0.01f) {
		move_impulse *= Math_InvSqrt2;
	}

	play_state->current_time = SDL_GetTicks();
	real dt = (play_state->current_time - play_state->prev_time) / 1000.0;
	dt = clamp(dt, 0, 1.2f);
	play_state->accumulator += dt;
	play_state->prev_time = play_state->current_time;

	while(play_state->accumulator >= Time_Step) {
		play_state->accumulator -= Time_Step;
		sim_update(&area->sim, Time_Step);
	}
	Entity* player_entity = world_area_find_entity(area, 0);
	Sim_Body* player = sim_find_body(&area->sim, player_entity->body_id);
		player->velocity += move_impulse;

	Vec2 target = player->shape.center;

	if(target.x < 0) {
		world_switch_current_area(play_state->world, area->west);
		play_state->world_xy.x--;
	} else if(target.x > area->map.w * Tile_Size) {
		world_switch_current_area(play_state->world, area->east);
		play_state->world_xy.x++;
	} else if(target.y < 0) {
		world_switch_current_area(play_state->world, area->north);
		play_state->world_xy.y--;
	} else if(target.y > area->map.h * Tile_Size) {
		world_switch_current_area(play_state->world, area->south);
		play_state->world_xy.y++;
	}

	area->offset += (target - area->offset) * 0.1f;
	area->offset -= game->size * 0.5f;
	if(area->offset.x < 0) 
		area->offset.x = 0;
	else if((area->offset.x + game->size.x) > area->map.w * Tile_Size)
		area->offset.x = area->map.w * Tile_Size - game->size.x;

	if(area->offset.y < 0) 
		area->offset.y = 0;
	else if((area->offset.y + game->size.y) > area->map.h * Tile_Size)
		area->offset.y = area->map.h * Tile_Size - game->size.y;
	renderer->offset = area->offset;
	area->offset += game->size * 0.5f;

	renderer_start();

	Rect2 screen = rect2(
			area->offset.x - game->size.x / 2,
			area->offset.y - game->size.y / 2, 
			game->size.x, game->size.y);

	render_tilemap(&area->map, v2(0,0), screen);
	isize sprite_count_offset = renderer->sprite_count;

	for(isize i = 0; i < area->entities_count; ++i) {
		Entity* e = area->entities + i;
		Sim_Body* b = sim_find_body(&area->sim, e->body_id);

		if (b == NULL) continue;
		e->sprite.position = b->shape.center;
		//e->sprite.size = v2(b->shape.hw * 2, b->shape.hh * 2);
		
		//TODO(will) align entity sprites by their bottom center
		renderer_push_sprite(&e->sprite);
	}

	renderer_sort(sprite_count_offset);

	renderer_draw();

	renderer->offset = v2(0, 0);
	game_set_scale(2.0);
	renderer_start();

	Sprite s; 
	s.size = v2(100, 16);
	s.position = v2(50, 18);
	s.color = v4(1, 1, 1, 0.5f);
	s.texture = Get_Texture_Coordinates(16, 16, 1, 1); 
	renderer_push_sprite(&s);

	render_body_text("You", player->shape.center - v2(1.5f * body_font->glyph_width, 32));
	char str[256];
	isize len = snprintf(str, 256, "X: %d, Y: %d", play_state->world_xy.x, play_state->world_xy.y);

	render_body_text(str, v2(16, 16));

	renderer_draw();
}




