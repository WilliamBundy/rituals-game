/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 * rituals_inventory.cpp
 *
 */


struct Item_Stack
{
	isize id;
	Item_Info* info;

	int32 number;
};

void init_item_stack(Item_Stack* stack, Item_Info* info)
{
	stack->id = 0;
	stack->info = info;
	stack->number = 1;
}

//TODO(will) think of new type of arena for 
// semipersistent game data? 
// or, maybe implement free-list allocator?
Item_Stack* new_item_stack(Item_Info* info, Memory_Arena* arena)
{
	Item_Stack* stack = arena_push_struct(arena, Item_Stack);
	init_item_stack(stack, info);
	return stack;
}

struct Inventory
{
	Item_Stack** contents;
	isize w, h, size;
};

void init_inventory(Inventory* inventory, isize w, isize h, Memory_Arena* arena)
{
	isize size = w * h;
	inventory->w = w;
	inventory->h = h;
	inventory->size = size;
	inventory->contents = arena_push_array(arena, Item_Stack*, size);
}

bool inventory_add_item(Inventory* inventory, Item_Stack** origin_slot)
{
	for(isize i = 0; i < inventory->h; ++i) {
		for(isize j = 0; j < inventory->w; ++j) {
			Item_Stack** slot = inventory->contents + (i * inventory->w + j);
			//TODO(will) fix for stacking
			if (*slot == NULL || 
				(*slot)->info == NULL || 
				(*slot)->info->id == Item_None) {

				*slot = *origin_slot;
				*origin_slot = NULL;
				return true;
			}
		}
	}
	return false;
}



void render_inventory(Inventory* inventory, Vec2 position)
{
	Sprite s;
	for(isize i = 0; i < inventory->h; ++i) {
		for(isize j = 0; j < inventory->w; ++j) {
			init_sprite(&s);
			//TODO(will) rewrite most of this
			s.position = position + v2(Tile_Size * j, Tile_Size * i);
			s.flags = Anchor_Top_Left;
			s.size = v2(32, 32);

					
			renderer_push_sprite(&s);

			Item_Stack* at = inventory->contents[i * inventory->w + j];
			if(at == NULL || at->info == NULL || at->info->id == Item_None) {
				continue;
			}

			s.texture = at->info->texture;
			renderer_push_sprite(&s);
		}
	}
}

