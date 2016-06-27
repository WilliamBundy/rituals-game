
/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 * rituals_world_area.cpp
 */


/*
enum Entity_Event_Type
{
	Entity_Event_On_Activate
}
struct Entity_Event 
{
	union {
		Entity_On_Activate on_activate;

	} event;
	isize hash;
	const char* name;
	real priority;
};
*/

typedef struct World_Area World_Area;

enum Direction 
{
	Direction_North,
	Direction_South,
	Direction_East,
	Direction_West
};



#define EntityMaxCallbacksPerFrame (16)


struct Entity
{
	isize id;
	isize body_id;
	Sim_Body* body;
	Sprite sprite;

	int32 counter;

	int32 facing;
	Direction direction;
	
	World_Area* area;
	
	//TODO(will) Implement entity callback hashtable
	// Right now save and load with function pointers will break
	// Can't verify whether pointers will be the same between dll loads
	Entity_On_Activate* event_on_activate;
	isize event_on_activate_count;

	//userdata
	isize held_entity_id;
};

#define _entity_get_id(e) (e.id)
GenerateQuicksortForType(entity_sort_on_id, Entity, _entity_get_id)
GenerateBinarySearchForType(entity_search_for_id, Entity, isize, _entity_get_id)

typedef struct World_Area World_Area;
struct Area_Link
{
	Vec2i position;
	World_Area* link;
};

#define WorldAreaEntityCapacity (8192)
#define WorldAreaTilemapWidth (64)
#define WorldAreaTilemapHeight (64)
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
	init_simulator(&area->sim, WorldAreaEntityCapacity, arena);
	init_tilemap(&area->map, 
			WorldAreaTilemapWidth,
			WorldAreaTilemapHeight,
			arena);

	area->entities = arena_push_array(arena, Entity, WorldAreaEntityCapacity);
	area->entities_count = 0;
	area->entities_capacity = WorldAreaEntityCapacity;
	area->next_entity_id = 0;
	area->entities_dirty = false;
}

void init_entity(Entity* entity)
{
	entity->id = 0;
	entity->body_id = 0;
	init_sprite(&entity->sprite);
	entity->counter = 0;
	entity->area = NULL;
	entity->event_on_activate = NULL;
	entity->event_on_activate_count = 0;
}

isize entity_add_event_on_activate(Entity* e, Entity_On_Activate event)
{
	if(e != NULL) {
		if(e->event_on_activate == NULL) {
			e->event_on_activate = arena_push_array(
					Game->play_arena, 
					Entity_On_Activate,
					EntityMaxCallbacksPerFrame);
		}
		if(e->event_on_activate_count < EntityMaxCallbacksPerFrame) {
			e->event_on_activate[e->event_on_activate_count++] = event;
		} else {
			Log_Error("Entity exceeded on_activate_count callbacks");
		}

	}
	return e->event_on_activate_count - 1;
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
	e->body = sim_get_next_body(&area->sim);
	e->body_id = e->body->id;
	e->id = area->next_entity_id++;
	e->body->entity = e;
	e->body->entity_id = e->id;
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

//Make a player struct?
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
	player->damping = 0.5f;
	player->restitution = 0;
	player->flags = Body_Flag_No_Friction;
	area->offset = player->shape.center;	
}

void world_area_deinit_player(World_Area* area)
{
	Entity* player_entity = world_area_find_entity(area, 0);
	Sim_Body* player = sim_find_body(&area->sim, player_entity->body_id);
	player->shape.center = v2(-1000, -1000);
}

void world_area_sort_entities_on_id(World_Area* area)
{
	entity_sort_on_id(area->entities, area->entities_count);
}

void world_area_synchronize_entities_and_bodies(World_Area* area)
{
	for(isize i = 0; i < area->entities_count; ++i) {
		Entity* e = area->entities + i;
		if(e->body_id == -1) continue;
		Sim_Body* b = sim_find_body(&area->sim, e->body_id);
		b->entity = e;
		b->entity_id = e->id;
		e->body = b;
	}
}

void world_area_remove_entity(World_Area* area, Entity* entity)
{
	sim_remove_body(&area->sim, entity->body_id);
	isize index = entity_search_for_id(entity->id, area->entities, area->entities_count);
	area->entities[index] = area->entities[--area->entities_count];
	world_area_sort_entities_on_id(area);
	world_area_synchronize_entities_and_bodies(area);
}

EntityOnActivateDecl(test_on_activate)
{
	printf("%d was clicked \n", entity->id);
}
EntityOnActivateDecl(delete_on_activate)
{
	world_area_remove_entity(area, entity);
}
