
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
	uint64 body_mask;
	uint64 body_group;

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
	EntityEvent_None = Flag(0),
	EntityEvent_Walk = Flag(1),
	EntityEvent_Interact = Flag(2),
	EntityEvent_Animate = Flag(3),
	EntityEvent_FrameTick = Flag(4),
	EntityEvent_SlowTick = Flag(5),
	EntityEvent_Destroy = Flag(6), // singular
	EntityEvent_Activate = Flag(7), // singular
	//Entity health_hitbox was hit by a attack_hitbox
	EntityEvent_Hit = Flag(8),
	EntityEvent_Contact = Flag(9),
};	

enum Rituals_Entity_Kinds
{
	EntityKind_Prop,
	EntityKind_Static,
	EntityKind_Player,
	EntityKind_Enemy,
	EntityKind_Bullet,
	EntityKind_Pickup,
};


enum Rituals_Enemy_Kind
{
	EnemyKind_Slime,
	EnemyKind_Bat,
	EnemyKind_Snake,
	EnemyKind_Goblin_Knight,
};

enum Rituals_Pickup_Kind
{
	PickupKind_Item,
	PickupKind_Health
};

union Rituals_Entity_Userdata
{
	struct {
		Rituals_Entity_Kinds contains;
		isize subtype;
		isize amount;
		isize quality;
	} prop;
	struct {
		isize held_entity_id;
		real heal_cooldown;
		real heal_timer;
		int32 heal_rate;
		int32 heal_to_interval;
		
	} player;
	struct {
		isize kind;
		isize mode;
		real speed;
		real alert_dist;
		real follow_dist;
		
		union {
			struct {
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
		isize kind;
		union {
			struct {
				isize id, count;
			} item;
			struct {
				isize amount;
			} health;
		};
	} pickup;
	struct {
		void* ptr;
		isize size;
	} unknown;
};


