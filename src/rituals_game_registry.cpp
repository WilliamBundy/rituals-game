/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* 
 * game_registry.cpp
 *
 */ 

usize hash_str(const char* str)
{
	isize len = strlen(str);
	usize hash = 1;
	for(isize i = 0; i < len; ++i) {
		hash = hash * 65599 + str[i];	
	}
	return hash;
}

struct Game_Registry
{
	Tile_Info* tiles;
	usize* tiles_hash;
	isize tiles_count;

	Item_Info* items;
	usize* items_hash;
	isize items_count;
};

Game_Registry* Registry;

#define MaxRegistryInfoEntries (256)
void init_game_registry(Game_Registry* registry, Memory_Arena* arena)
{
#define _set_registry_arrays(type, base_name) do { \
	registry->base_name = arena_push_array(arena, type, MaxRegistryInfoEntries); \
	registry->base_name##_hash = arena_push_array(arena, usize, MaxRegistryInfoEntries); \
	registry->base_name##_count = 0; \
	} while(0)

	_set_registry_arrays(Tile_Info, tiles);
	_set_registry_arrays(Item_Info, items);
}

#define _game_registry__key_macro(t) (t.k)
#define _generate_registry_lookup(lookup_func_name, sort_func_name , return_type, array_base_name) \
return_type* lookup_func_name(const char* name) \
{ \
	usize hash = hash_str(name); \
	isize index = usize_search(hash, Registry->array_base_name##_hash, Registry->array_base_name##_count); \
	return Registry->array_base_name + index; \
} \
struct _game_registry__##return_type##_pair { return_type v; usize k; }; \
GenerateQuicksortForType(_game_registry__##return_type##_pair_sort, _game_registry__##return_type##_pair, _game_registry__key_macro) \
void sort_func_name() \
{ \
	start_temp_arena(Game->temp_arena); \
	_game_registry__##return_type##_pair* pairs = arena_push_array( \
			Game->temp_arena,  \
			_game_registry__##return_type##_pair, \
			Registry->array_base_name##_count); \
	for(isize i = 0; i < Registry->array_base_name##_count; ++i) { \
		auto pair = pairs + i; \
		pair->v = Registry->array_base_name[i]; \
		pair->k = Registry->array_base_name##_hash[i]; \
	} \
	_game_registry__##return_type##_pair_sort(pairs, Registry->array_base_name##_count); \
	for(isize i = 0; i < Registry->array_base_name##_count; ++i) { \
		auto pair = pairs + i; \
		Registry->array_base_name[i] = pair->v; \
		Registry->array_base_name##_hash[i] = pair->k; \
	} \
} 
 
_generate_registry_lookup(lookup_tile, sort_registered_tiles, Tile_Info, tiles) 
_generate_registry_lookup(lookup_item, sort_registered_items, Item_Info, items)

#define Tile_Size (32)
#define Half_TS (16)

#define _tile_texture(x, y) Get_Texture_Coordinates(Tile_Size * (x), Tile_Size * (y), Tile_Size, Tile_Size)
#define _new_tile(name, mvt, frc, x, y, solid) Tile_Info* tile_##name = add_tile_info(#name, (real)(mvt), (real)frc, _tile_texture(x, y), solid, Tile_Dug_Earth)

Tile_Info* add_tile_info(const char* name, real movement_modifier, real friction, Rect2 texture, bool solid, isize break_to_id)
{
	Tile_Info* t = Registry->tiles + Registry->tiles_count++;
	t->id = Registry->tiles_count - 1;
	t->hash = hash_str(name);
	t->texture = texture;
	t->name = name;
	t->solid = solid;
	t->movement_modifier = movement_modifier;
	t->friction = friction;
	t->max_damage = 5;
	t->immune_to_damage = false;
	t->break_to_id = break_to_id;

	Registry->tiles_hash[Registry->tiles_count - 1] = t->hash;

	return t;
}

void register_all_rituals_tile_info()
{
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
}

#define _add_item(name, s, x, y) Item_Info* item_##name = add_item_type(#name, (s), _tile_texture(x, y)) 

Item_Info* add_item_type(const char* name, int32 max_stack, Rect2 texture)
{
	Item_Info* item = Registry->items + Registry->items_count++; 
	item->id = Registry->items_count - 1;
	item->name = name;
	item->max_stack = max_stack;
	item->texture = texture;
	item->hash = hash_str(name);
	Registry->items_hash[Registry->items_count - 1] = item->hash;
	return item;
}

void register_all_rituals_item_info()
{
	_add_item(none, 0, 0, 0);
	_add_item(hooknife, 1, 0, 5);
	_add_item(rope, 8, 1, 5);
	_add_item(book, 64, 2, 5);
	_add_item(rock, 64, 3, 0);
}

void register_everything_in_rituals()
{
	register_all_rituals_item_info();
	register_all_rituals_tile_info();
}
