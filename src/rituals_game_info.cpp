
/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* 
 * game_info.cpp
 *
 * NOTE(will): all <thing>_Info types need to have an id member
 *
 */ 

typedef struct Entity Entity;
typedef struct World_Area World_Area;
typedef struct World World;
#define EntityOnActivateDecl(name) void name(Entity* entity, World_Area* area)
typedef EntityOnActivateDecl((*Entity_On_Activate));

typedef int32 Tile;

struct Tile_Info
{
	isize id;
	usize hash;

	Rect2 texture;
	//TODO(will) support sided/connected textures
	Rect2 bottom_texture;
	Rect2 top_texture;

	real movement_modifier;
	real friction;
	
	const char* name;

	int32 max_damage;
	bool immune_to_damage;
	char* break_to_name;
	isize break_to_id;

	//maybe change to flags
	bool solid;
	bool has_top_texture;
	bool has_bottom_texture;
};

struct Tile_State
{
	isize id;
	int32 damage;
};

void init_tile_state(Tile_State* state, isize id)
{
	state->id = id;
	state->damage = 0;
}


enum Item_Info_Types
{
	Item_None,

	Item_Hooknife,
	Item_Rope,
	Item_Book,
	Item_Rock,

	Item_Info_Types_Count
};

struct Item_Info
{
	isize id;
	usize hash;
	const char* name;
	int32 max_stack;

	Rect2 texture;
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

enum Rituals_Entity_Kinds
{
	EntityKind_Prop,
	EntityKind_Static,
	EntityKind_Player,
	EntityKind_Enemy,
};


/*
typedef void (*ProcessEntitiesFunc)(Entity* entities, isize count, World_Area* area, World* world);
struct Package
{
	ProcessEntitiesFunc sim, walk, interact, animate, 
						create, frame_tick, slow_tick, 
						destroy;
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
	

void rituals_assign_entity_kinds(Rituals_Entity_Kinds* kinds)
{
	kinds->PropEntity = get_next_entity_kind();
	kinds->StaticEntity = get_next_entity_kind();
	kinds->PlayerEntity = get_next_entity_kind();
	kinds->EnemyEntity = get_next_entity_kind();
}*/

enum Rituals_Enemy_Kinds
{
	EnemyKind_Slime,
	EnemyKind_Bat,
	EnemyKind_Snake,
	EnemyKind_Goblin_Knight,
};

 

union Rituals_Entity_Userdata
{
	struct {
		isize held_entity_id;
	} player;
	struct {
		isize kind;
		isize mode;
		real speed;
		real alert_dist;
		real follow_dist;
		
		union {
			struct {
				real last_dist_to_player;
				real arc_perc;
				Vec2 perch;
			} bat;
			struct {
				real chase_speed_modifier;
			} snake;
			struct {
				Vec2 patrol_start;
				Vec2 patrol_end;
			} goblin_knight;
		};
	} enemy;
	struct {
		void* ptr;
	} unknown;
};


