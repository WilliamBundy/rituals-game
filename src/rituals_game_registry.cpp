struct Game_Registry
{
	Tile_Info* tiles;
	isize tiles_count, tiles_capacity;
};

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
