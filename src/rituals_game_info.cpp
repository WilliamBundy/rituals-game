
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


