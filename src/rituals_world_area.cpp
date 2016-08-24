
/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 * rituals_world_area.cpp
 */

typedef struct Entity Entity;
struct Hitbox_Contact
{
	isize a_id;
	isize b_id;

	Entity* a;
	Entity* b;
};

struct Hitbox
{
	isize id;
	Entity* ref;
	uint64 mask;
	AABB box;
};
#define _hitbox_get_x1(h) (AABB_x1(h.box))
GenerateIntrosortForType(_hitbox_sort_on_x_axis, Hitbox, 12, _hitbox_get_x1)
#define _hitbox_get_y1(h) (AABB_y1(h.box))
GenerateIntrosortForType(_hitbox_sort_on_y_axis, Hitbox, 12, _hitbox_get_y1)

struct Entity
{
	isize id;
	isize body_id;
	Sim_Body* body;
	Vec2 walk_impulse;

	Hitbox hitbox;
	int32 health;
	int32 attack;
	real attack_interval;
	uint64 hitbox_mask;

	Sprite sprite;
	isize kind;

	int32 counter;

	int32 facing;
	Direction direction;

	uint64 events;
	//void* userdata;
	Rituals_Entity_Userdata userdata;
};

#define _entity_get_id(e) (e.id)
GenerateIntrosortForType(entity_sort_on_id, Entity, 12,  _entity_get_id)
GenerateBinarySearchForType(entity_search_for_id, Entity, isize, _entity_get_id)

typedef struct World_Area_Stub World_Area_Stub;
struct Area_Link
{
	Vec2i position;
	World_Area_Stub* link;
};

enum World_Area_Biome
{
	AreaBiome_Grassland,
	AreaBiome_Desert
};

#define WorldAreaTilemapWidth (64)
#define WorldAreaTilemapHeight (64)
#define WorldAreaEntityCapacity (WorldAreaTilemapWidth * WorldAreaTilemapHeight * 4)
struct World_Area_Stub
{
	isize id;
	usize seed;
	Area_Link north;
	Area_Link south;
	Area_Link west;
	Area_Link east;
	World_Area_Biome biome;
};

struct World_Area
{
	isize id;
	World_Area_Stub* stub;
	World* world;
	Simulator sim;
	Tilemap map;
	Vec2 offset;
	Vec2 target;

	Entity* entities;
	bool entities_dirty;
	isize entities_count, entities_capacity, next_entity_id;

	isize* removed_entities;
	isize removed_entities_count, removed_entities_capacity;


	Hitbox* hitboxes;
	isize hitboxes_count, hitboxes_capacity;
	int32 hitbox_sort_axis;
	Hitbox_Contact* hitbox_contacts;
	isize hitbox_contacts_count, hitbox_contacts_capacity;

	
	Entity* player;

};

void init_world_area(World_Area* area, Memory_Arena* arena)
{
	init_simulator(&area->sim, WorldAreaEntityCapacity, arena);
	init_tilemap(&area->map, 
			WorldAreaTilemapWidth,
			WorldAreaTilemapHeight,
			arena);

	area->id = -1;
	area->entities = arena_push_array(arena, Entity, WorldAreaEntityCapacity);
	area->entities_count = 0;
	area->entities_capacity = WorldAreaEntityCapacity;
	area->removed_entities_capacity = 256;
	area->removed_entities = arena_push_array(arena, isize, WorldAreaEntityCapacity);
	area->removed_entities_count = 0;
	area->next_entity_id = 0;
	area->entities_dirty = false;
	area->hitboxes = arena_push_array(arena, Hitbox, WorldAreaEntityCapacity);
	area->hitboxes_count = 0;
	area->hitboxes_capacity = WorldAreaEntityCapacity;
	area->hitbox_contacts = arena_push_array(arena, Hitbox_Contact, WorldAreaEntityCapacity);
	area->hitbox_contacts_count = 0;
	area->hitbox_contacts_capacity = WorldAreaEntityCapacity;
}

void init_entity(Entity* entity)
{
	entity->id = 0;
	entity->body_id = 0;
	init_sprite(&entity->sprite);
	entity->counter = 0;
	//ntity->userdata = NULL;
}

Entity* world_area_get_next_entity(World_Area* area)
{
	if(area->entities_count + 1 >= area->entities_capacity) {
		Log_Error("Ran out of entities");
		return NULL;
	}

	Entity* e = area->entities + area->entities_count++;
	init_entity(e);
	e->body = sim_get_next_body(&area->sim);
	e->body_id = e->body->id;
	e->id = area->next_entity_id++;
	e->body->entity = e;
	e->body->entity_id = e->id;
//	e->attack_hitbox = aabb(0, 0, 0, 0);
//	e->health_hitbox = aabb(0, 0, 0, 0);
	e->health = 100;
	e->attack = 0;
	e->attack_interval = 0.5f;
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

void world_area_sort_entities_on_id(World_Area* area)
{
	entity_sort_on_id(area->entities, area->entities_count);
}

void world_area_synchronize_entities_and_bodies(World_Area* area)
{
	world_area_sort_entities_on_id(area);
	sim_sort_bodies_on_id(&area->sim);
	for(isize i = 0; i < area->entities_count; ++i) {
		Entity* e = area->entities + i;
		if(e->body_id == -1) continue;
		Sim_Body* b = sim_find_body(&area->sim, e->body_id);
		if(b == NULL) continue;
		b->entity = e;
		b->entity_id = e->id;
		e->body = b;
	}
}



//Returns 1 if failed
bool world_area_remove_entity(World_Area* area, Entity* entity)
{
	if(area->removed_entities_count >= area->removed_entities_capacity) {
		printf("Ran out of room for removing entities\n");
		return true;
	}
	area->removed_entities[area->removed_entities_count++] = entity->id;
	return false;
}

void world_area_remove_entity_internal(World_Area* area, isize id)
{
	isize index = entity_search_for_id(id, area->entities, area->entities_count);
	Entity* entity = area->entities + index;
	sim_remove_body(&area->sim, entity->body_id);
	area->entities[index] = area->entities[--area->entities_count];
	world_area_synchronize_entities_and_bodies(area);
}

void world_area_process_removed_entities(World_Area* area)
{
	for(isize i = 0; i < area->removed_entities_count; ++i) {
		isize e = area->removed_entities[i];
		world_area_remove_entity_internal(area, e);
	}
	area->removed_entities_count = 0;
}

//Make a player struct?
void world_area_init_player(World_Area* area, Vec2i tile_pos, bool move_player=true)
{
	Entity* player_entity = world_area_find_entity(area, 0);
	Sim_Body* player = sim_find_body(&area->sim, player_entity->body_id);
	if(player == NULL) {
		printf("Something went wrong! Couldn't find player entity....?");
	}
	///player->shape.center = v2(area->map.w * 16, area->map.h * 16);
	if(move_player) {
		player->shape.center = v2(tile_pos.x * Tile_Size, tile_pos.y * Tile_Size);
	}
	player_entity->sprite.texture = Get_Texture_Coordinates(0, 0, 32, 32);
	player->shape.hext = v2(5, 5);
	player_entity->sprite.size = v2(32, 32);
	player->group = 1;
	//player_entity->sprite.center = v2(0,11);
	player_entity->sprite.anchor = Anchor_Bottom;
	player->damping = 0.5f;
	player->restitution = 0;
	player->flags = Body_Flag_No_Friction;
	player_entity->kind = EntityKind_Player;
	area->offset = player->shape.center;	
	area->target = player->shape.center;
}

void world_area_deinit_player(World_Area* area, bool move_player=true)
{
	Entity* player_entity = world_area_find_entity(area, 0);
	Sim_Body* player = sim_find_body(&area->sim, player_entity->body_id);
	if(move_player) {
		player->shape.center = v2(-1000, -1000);
	}
}

void world_area_build_hitboxes(World_Area* area)
{
	area->hitboxes_count = 0;
	for(isize i = 0; i < area->entities_count; ++i) {
		Entity* e = area->entities + i;
		if(e->body == NULL) continue;
		Hitbox* h = area->hitboxes + area->hitboxes_count++;
		h->id = e->id;
		h->ref = e;
		AABB box = e->hitbox.box;
		h->box.center = e->sprite.position + box.center;
		//if the entity's box is set
		if(v2_dot(box.hext, box.hext) > 1) {
			h->box.hext = box.hext;
		} else {
			h->box.hext = e->body->shape.hext;
			e->hitbox.box.hext = h->box.hext;
			e->hitbox.box.center.y -= e->body->shape.hh;
		}
	}
}

void world_area_process_hitboxes(World_Area* area)
{
	if(area->hitboxes_count == 0) return;

	area->hitbox_contacts_count = 0;

	if(area->hitbox_sort_axis == 0) {
		_hitbox_sort_on_x_axis(area->hitboxes, area->hitboxes_count);
	} else {
		_hitbox_sort_on_y_axis(area->hitboxes, area->hitboxes_count);
	}
	Vec2 center_sum1 = v2(0, 0);
	Vec2 center_sum2 = v2(0, 0);
	Vec2 variance = v2(0, 0);
	for(isize i = 0; i < area->hitboxes_count; ++i) {
		Hitbox* a = area->hitboxes + i;
		center_sum1 += a->box.center;
		for(isize q = 0; q < 2; ++q) {
			center_sum2.e[q] += a->box.center.e[q] * a->box.center.e[q];
		}

		for(isize j = i + 1; j < area->hitboxes_count; ++j) {
			Hitbox* b = area->hitboxes + j;
			if(!(a->mask == 0 && b->mask == 0)) {
				//if(!(a->mask & b->mask)) break;
			}

			if(area->hitbox_sort_axis == 0) {
				if(AABB_x1(b->box) > AABB_x2(a->box)) {
					break;
				}
			} else if(area->hitbox_sort_axis == 1) {
				if(AABB_y1(b->box) > AABB_y2(a->box)) {
					break;
				}
			}

			if(aabb_intersect(&a->box, &b->box)) {
				Hitbox_Contact* c = area->hitbox_contacts + area->hitbox_contacts_count++;
				c->a = a->ref;
				c->b = b->ref;
				c->a_id = a->id;
				c->b_id = b->id;
			}
		}
	}
	for(isize i = 0; i < 2; ++i) {
		variance.e[i] = center_sum2.e[i] - center_sum1.e[i] * center_sum1.e[i] / 
			area->hitboxes_count;
	}

	if(variance.x > variance.y) {
		area->hitbox_sort_axis = 0;
	} else {
		area->hitbox_sort_axis = 1;
	}
}
