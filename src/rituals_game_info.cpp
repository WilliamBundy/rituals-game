/* 
 * game_info.cpp
 *
 * NOTE(will): all <thing>_Info types need to have an id member
 *
 */ 

//typedef void (*Entity_On_Activate)(Entity* entity, World_Area* area);


typedef void* Entity_On_Activate;
#ifndef WirmphtEnabled
struct Tile_Info
{
	isize id;
	usize hash;

	Rect2i texture;
	//TODO(will) support sided/connected textures
	Rect2i bottom_texture;
	Rect2i top_texture;

	f32 movement_modifier;
	f32 friction;
	
	const char* name;

	i32 max_damage;
	bool immune_to_damage;
	char* break_to_name;
	isize break_to_id;

	//maybe change to flags
	bool solid;
	u64 body_mask;
	u64 body_group;

	bool has_top_texture;
	bool has_bottom_texture;
};

struct Tile_State
{
	isize id;
	i32 damage;
};

struct Game_Registry
{
	Tile_Info* tiles;
	isize tiles_count, tiles_capacity;
};
#endif

void init_tile_state(Tile_State* state, isize id)
{
	state->id = id;
	state->damage = 0;
}

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

#ifndef WirmphtEnabled
struct Rituals_Bat_Userdata
{
    Vec2 perch;
};

struct Rituals_Snake_Userdata
{
    f32 chase_speed_modifier;
};

struct Rituals_Goblin_Knight_Userdata
{
    Vec2 patrol_start;
    Vec2 patrol_end;
};

struct Rituals_Enemy_Userdata
{
    isize kind;
    isize mode;
    f32 speed;
    f32 alert_dist;
    f32 follow_dist;

    union {
        Rituals_Bat_Userdata bat;
        Rituals_Snake_Userdata snake;
        Rituals_Goblin_Knight_Userdata goblin_knight;
    };
};

struct Rituals_Player_Userdata {
    isize held_entity_id;
    f32 heal_cooldown;
    f32 heal_timer;
    i32 heal_rate;
    i32 heal_to_interval;
};

struct Rituals_Prop_Userdata {
    Rituals_Entity_Kinds contains;
    isize subtype;
    isize amount;
    isize quality;
};

struct Rituals_Item_Pickup_Userdata
{
    isize id, count;
};

struct Rituals_Health_Pickup_Userdata
{
    isize amount;
};

struct Rituals_Pickup_Userdata
{
    isize kind;
    Rituals_Item_Pickup_Userdata item;
    Rituals_Health_Pickup_Userdata health;
};

struct Unknown_Userdata {
    void* ptr;
    isize size;
};

union Rituals_Entity_Userdata
{
    Rituals_Prop_Userdata prop;
    Rituals_Player_Userdata player;
    Rituals_Enemy_Userdata enemy;
    Rituals_Pickup_Userdata pickup;
    Unknown_Userdata unknown;
};
#endif

void registryInit(Game_Registry* reg, isize capacity, MemoryArena* arena)
{
	reg->tiles = (Tile_Info*)arenaPush(arena, sizeof(Tile_Info) * capacity);
	reg->tiles_count = 0;
	reg->tiles_capacity = capacity;
}

enum {
	Tile_Void, 
	Tile_Sand, 
	Tile_Grass, 
	Tile_Dense_Grass, 
	Tile_Dug_Earth, 
	Tile_Tree_Wall, 
	Tile_Shallow_Water, 
	Tile_Water, 
	Tile_Stone_Road, 
	Tile_Cracked_Road, 
	Tile_Pristine_Road, 
	Tile_Slab_Wall, 
};

#define TiSz (32)
#define hTiSz (16)

#define _tile_texture(x, y) rect2i(TiSz * (x), TiSz * (y), TiSz, TiSz)
#define _new_tile(name, mvt, frc, x, y, solid) Tile_Info* tile_##name = add_tile_info(Registry, name, #name, (f32)(mvt), (f32)frc, _tile_texture(x, y), solid, Tile_Dug_Earth)

Tile_Info* add_tile_info(
		Game_Registry* Registry,
		isize index, 
		const char* name,
		f32 movement_modifier, 
		f32 friction, 
		Rect2i texture, 
		bool solid, 
		isize break_to_index)
{
	Tile_Info* t = Registry->tiles + index;
	Registry->tiles_count++;
	t->id = index;
	//t->hash = hash_str(name);
	t->texture = texture;
	t->name = name;
	t->solid = solid;
	t->movement_modifier = movement_modifier;
	t->friction = friction;
	t->max_damage = 5;
	t->immune_to_damage = false;
	t->break_to_id = break_to_index;

	return t;
}

void register_all_rituals_tile_info(Game_Registry* Registry)
{
	_new_tile(Tile_Void, 1.0, 0.5, 0, 0, true);
	tile_Tile_Void->texture = rect2i(0, 0, 0, 0);

	_new_tile(Tile_Sand, 0.9, 0.8, 0, 1, false);
	_new_tile(Tile_Grass, 1, 0.5, 1, 1, false);
	_new_tile(Tile_Dense_Grass, 1, 0.5, 2, 1, false);
	_new_tile(Tile_Dug_Earth, 0.9, 0.5, 3, 1, false);
	tile_Tile_Dug_Earth->immune_to_damage = true;

	_new_tile(Tile_Tree_Wall, 1, 1, 1, 4, true);
	tile_Tile_Tree_Wall->has_top_texture = true;
	tile_Tile_Tree_Wall->top_texture = _tile_texture(1, 3);
	tile_Tile_Tree_Wall->has_bottom_texture = true;
	tile_Tile_Tree_Wall->bottom_texture = _tile_texture(2, 4);


	_new_tile(Tile_Water, 1, 1, 1, 2, true);
	tile_Tile_Water->body_group = 1;
	_new_tile(Tile_Shallow_Water, 0.7, 0.99, 0, 2, false);
	_new_tile(Tile_Stone_Road, 1.2, 0.99, 2, 2, false);
	_new_tile(Tile_Cracked_Road, 1.2, 0.99, 3, 2, false);
	_new_tile(Tile_Pristine_Road, 1.2, 0.99, 4, 2, false);
	tile_Tile_Stone_Road->max_damage = 20;
	_new_tile(Tile_Slab_Wall, 1, 1, 0, 3, true);
	//tile_slab_wall->has_top_texture = true;
	//tile_slab_wall->top_texture = _tile_texture(0, 3);
	tile_Tile_Slab_Wall->has_bottom_texture = true;
	tile_Tile_Slab_Wall->bottom_texture = _tile_texture(0, 4);
}

void register_everything_in_rituals(Game_Registry* Registry)
{
	register_all_rituals_tile_info(Registry);
}
