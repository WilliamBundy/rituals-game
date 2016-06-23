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
	Sprite sprite;

	World_Area* area;
};

#define _entity_get_id(e) (e.id)
Generate_Quicksort_For_Type(entity_sort_on_id, Entity, _entity_get_id)
Generate_Binary_Search_For_Type(entity_search_for_id, Entity, isize, _entity_get_id)


#define World_Area_Entity_Capacity (8192)
#define World_Arena_Tilemap_Width (48)
#define World_Arena_Tilemap_Height (32)
struct World_Area
{
	Simulator sim;
	Tilemap map;

	Entity* player;
	Sim_Body* player_body;
	Entity* entities;
	bool entities_dirty;
	isize entities_count, entities_capacity, next_entity_id;
};

void init_world_area(World_Area* area, Memory_Arena* arena)
{
	init_simulator(&area->sim, World_Area_Entity_Capacity, arena);
	init_tilemap(&area->map, 
			World_Arena_Tilemap_Width,
			World_Arena_Tilemap_Height,
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

