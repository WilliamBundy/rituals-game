/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* 
 * game_info.cpp
 *
 */ 

typedef struct Entity Entity;
typedef struct World_Area World_Area;
typedef struct World World;
#define EntityOnActivateDecl(name) void name(Entity* entity, World_Area* area)
typedef EntityOnActivateDecl((*Entity_On_Activate));
EntityOnActivateDecl(test_on_activate)
{
	printf("%d was clicked \n", entity->id);
}
EntityOnActivateDecl(delete_on_activate)
{
	world_area_remove_entity(area, entity);
}

typedef int32 Tile;

struct Tile_Info
{
	isize id;
	Rect2 texture;
	//TODO(will) support sided/connected textures
	Rect2 bottom_texture;
	Rect2 top_texture;

	real movement_modifier;
	real friction;
	
	const char* name;

	int32 max_damage;
	bool immune_to_damage;
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


Tile_Info* add_tile_info(Tile_Info* list, isize* info_count, const char* name, real movement_modifier, real friction, Rect2 texture, bool solid, isize break_to_id)
{
	Tile_Info* t = list + *info_count;
	*info_count += 1;
	t->texture = texture;
	t->name = name;
	t->solid = solid;
	t->movement_modifier = movement_modifier;
	t->friction = friction;
	t->max_damage = 5;
	t->immune_to_damage = false;
	t->break_to_id = break_to_id;

	return t;
}


#define Tile_Size (32)
#define Half_TS (16)

#define _tile_texture(x, y) Get_Texture_Coordinates(Tile_Size * (x), Tile_Size * (y), Tile_Size, Tile_Size)
#define _new_tile(name, mvt, frc, x, y, solid) Tile_Info* tile_##name = add_tile_info(tile_info, &tile_count, #name, (real)(mvt), (real)frc, _tile_texture(x, y), solid, Tile_Dug_Earth)

enum Rituals_Tiles 
{
	Tile_Void,
	Tile_Sand,
	Tile_Grassy_Sand,
	Tile_Grass,
	Tile_Dense_Grass,
	Tile_Desert_Sand,
	Tile_Dug_Earth,
	Tile_Rock_Wall,
	Tile_Tree_Wall,
	Tile_Water,
	Tile_Stone_Road,
	Tile_Earthen_Wall
};



void register_all_rituals_tile_info(Tile_Info* tile_info, isize* tile_count_out)
{
	isize tile_count = *tile_count_out;
	_new_tile(void, 1.0, 0.5, 0, 0, true);
	tile_void->texture = rect2(0, 0, 0, 0);

	_new_tile(sand, 0.7, 0.999, 0, 1, false);
	_new_tile(grassy_sand, 0.7, 0.999, 1, 1, false);
	_new_tile(grass, 0.9, 0.5, 2, 1, false);
	_new_tile(dense_grass, 0.9, 0.5, 3, 1, false);
	_new_tile(desert_sand, 0.7, 0.999, 4, 1, false);
	_new_tile(dug_earth, 0.9, 0.5, 5, 1, false);
	tile_dug_earth->immune_to_damage = true;

	_new_tile(rock_wall, 1, 1, 0, 2, true);
	_new_tile(tree_wall, 1, 1, 1, 2, true);
	tile_tree_wall->has_bottom_texture = true;
	tile_tree_wall->has_top_texture = true;
	tile_tree_wall->top_texture = _tile_texture(1, 3);
	tile_tree_wall->bottom_texture = _tile_texture(1, 4);


	_new_tile(water, 1, 1, 2, 2, true);
	_new_tile(stone_road, 1.2, 0.99, 3, 2, false);
	tile_stone_road->max_damage = 20;
	_new_tile(earthen_wall, 1, 1, 4, 2, true);
	*tile_count_out = tile_count;
}



