/* 
 * game_info.cpp
 *
 * NOTE(will): all <thing>_Info types need to have an id member
 *
 */ 

//typedef void (*Entity_On_Activate)(Entity* entity, World_Area* area);
//
typedef void* Entity_On_Activate;

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

