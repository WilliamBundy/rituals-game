/* 
 * game_info.cpp
 *
 * NOTE(will): all <thing>_Info types need to have an id member
 *
 */ 

//typedef void (*Entity_On_Activate)(Entity* entity, World_Area* area);


typedef void* Entity_On_Activate;
#ifndef WirmphtEnabled
struct TileInfo
{
	isize id;
	usize hash;

	Rect2i texture;
	//TODO(will) support sided/connected textures
	Rect2i bottomTexture;
	Rect2i topTexture;

	f32 movementModifier;
	f32 friction;
	
	const char* name;

	i32 maxDamage;
	bool immuneToDamage;
	isize breakToId;

	//maybe change to flags
	bool solid;
	u64 bodyMask;
	u64 bodyGroup;

	bool hasTopTexture;
	bool hasBottomTexture;
};

struct TileState
{
	isize id;
	i32 damage;
};

struct Game_Registry
{
	TileInfo* tiles;
	isize tileCount, tileCapacity;
};
#endif

void initTileState(TileState* state, isize id)
{
	state->id = id;
	state->damage = 0;
}
void registryInit(Game_Registry* reg, isize capacity, MemoryArena* arena)
{
	reg->tiles = (TileInfo*)arenaPush(arena, sizeof(TileInfo) * capacity);
	reg->tileCount = 0;
	reg->tileCapacity = capacity;
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
#define _new_tile(name, mvt, frc, x, y, solid) TileInfo* tile_##name = add_tile_info(Registry, name, #name, (f32)(mvt), (f32)frc, _tile_texture(x, y), solid, Tile_Dug_Earth)

TileInfo* add_tile_info(
		Game_Registry* Registry,
		isize index, 
		const char* name,
		f32 movement_modifier, 
		f32 friction, 
		Rect2i texture, 
		bool solid, 
		isize break_to_index)
{
	TileInfo* t = Registry->tiles + index;
	Registry->tileCount++;
	t->id = index;
	//t->hash = hash_str(name);
	t->texture = texture;
	t->name = name;
	t->solid = solid;
	t->movementModifier = movement_modifier;
	t->friction = friction;
	t->maxDamage = 5;
	t->immuneToDamage = false;
	t->breakToId = break_to_index;

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
	tile_Tile_Dug_Earth->immuneToDamage = true;

	_new_tile(Tile_Tree_Wall, 1, 1, 1, 4, true);
	tile_Tile_Tree_Wall->hasTopTexture = true;
	tile_Tile_Tree_Wall->topTexture = _tile_texture(1, 3);
	tile_Tile_Tree_Wall->hasBottomTexture = true;
	tile_Tile_Tree_Wall->bottomTexture = _tile_texture(2, 4);


	_new_tile(Tile_Water, 1, 1, 1, 2, true);
	tile_Tile_Water->bodyGroup = 1;
	_new_tile(Tile_Shallow_Water, 0.7, 0.99, 0, 2, false);
	_new_tile(Tile_Stone_Road, 1.2, 0.99, 2, 2, false);
	_new_tile(Tile_Cracked_Road, 1.2, 0.99, 3, 2, false);
	_new_tile(Tile_Pristine_Road, 1.2, 0.99, 4, 2, false);
	tile_Tile_Stone_Road->maxDamage = 20;
	_new_tile(Tile_Slab_Wall, 1, 1, 0, 3, true);
	//tile_slab_wall->hasTopTexture = true;
	//tile_slab_wall->topTexture = _tile_texture(0, 3);
	tile_Tile_Slab_Wall->hasBottomTexture = true;
	tile_Tile_Slab_Wall->bottomTexture = _tile_texture(0, 4);
}

void register_everything_in_rituals(Game_Registry* Registry)
{
	register_all_rituals_tile_info(Registry);
}
