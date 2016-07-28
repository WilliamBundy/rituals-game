/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* 
 * rituals_entity.cpp
 *
 */ 


enum Direction 
{
	Direction_North,
	Direction_South,
	Direction_East,
	Direction_West
};

enum Entity_Event_Type
{
	EntityEvent_Sim = Flag(0),
	EntityEvent_Walk = Flag(1),
	EntityEvent_Interact = Flag(2),
	EntityEvent_Animate = Flag(3),
	EntityEvent_Create = Flag(4),
	EntityEvent_FrameTick = Flag(5),
	EntityEvent_SlowTick = Flag(6),
	EntityEvent_Destroy = Flag(7)
};	

//TODO(will) go into game registry
struct Rituals_Entity_Kinds
{
	isize PropEntity,
		  StaticEntity,
		  PlayerEntity,
		  EnemyEntity;
};
Rituals_Entity_Kinds RitualsEntityKinds;

//TODO(will) serialize -- translate from old to new on deserialization
isize _next_entity_kind = 0;
static inline isize get_next_entity_kind()
{
	return _next_entity_kind++;
}

isize Rituals_PropEntity,
	Rituals_StaticEntity,
	Rituals_PlayerEntity;

void rituals_assign_entity_kinds(Rituals_Entity_Kinds* kinds)
{
	kinds->PropEntity = get_next_entity_kind();
	kinds->StaticEntity = get_next_entity_kind();
	kinds->PlayerEntity = get_next_entity_kind();
	kinds->EnemyEntity = get_next_entity_kind();

}

struct Rituals_Entity_Userdata
{
	union {
		struct {
			isize held_entity_id;
		} player;
	} data;
};

struct Entity
{
	isize id;
	isize body_id;
	Sim_Body* body;
	Vec2 walk_impulse;

	Sprite sprite;
	isize kind;

	int32 counter;

	int32 facing;
	Direction direction;

	uint64 events;
	void* userdata;
};

#define _entity_get_id(e) (e.id)
GenerateIntrosortForType(entity_sort_on_id, Entity, 12,  _entity_get_id)
GenerateBinarySearchForType(entity_search_for_id, Entity, isize, _entity_get_id)

typedef void (*ProcessEntitiesFunc)(Entity* entities, isize count, World_Area* area, World* world);
struct Package
{
	ProcessEntitiesFunc sim, walk, interact, animate, 
						create, frame_tick, slow_tick, 
						destroy;
}

#define _check(s1, s2, state) ((Input->scancodes[SDL_SCANCODE_##s1] == state) || (Input->scancodes[SDL_SCANCODE_##s2] == state))
void rituals_walk_entities(Entity* entities, isize count, World_Area* area, World* world)
{
	for(isize i = 0; i < count; ++i) {
		Entity* e = entities + i;
		if(e->kind == RitualsEntityKinds.PlayerEntity) {
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

			Tile_Info* player_tile = Registry->tiles + tilemap_get_at(&area->map, e->body->shape.center);

			move_impulse *= player_tile->movement_modifier;

		}
	}
}

