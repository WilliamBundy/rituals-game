/*
 * rituals_inventory.cpp
 *
 */

enum Item_Info_Types
{
	Item_None,

	Item_Hooknife,
	Item_Rope,
	Item_Book,
	Item_Rock,

	Item_Info_Types_Count
};

#define Max_Item_Info_Count (256)
struct Item_Info
{
	isize id;
	const char* name;
	int32 max_stack;

	Rect2 texture;
};

Item_Info* add_item_type(Item_Info* info, isize* count, const char* name, int32 max_stack, Rect2 texture)
{

	Item_Info* item = info + *count; 
	item->id = *count;
	*count += 1;
	item->name = name;
	item->max_stack = max_stack;
	item->texture = texture;
	return item;
}


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
	Item_Stack* stack = Arena_Push_Struct(arena, Item_Stack);
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
	inventory->contents = Arena_Push_Array(arena, Item_Stack*, size);
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
			s.position = position + v2(Tile_Size * j, Tile_Size * i);
			s.anchor = Anchor_Top_Left;
			s.size = v2(32, 32);

			s.texture = Get_Texture_Coordinates(
					renderer->texture_width - 7 * 16, 
					6 * 16, 
					32, 32);
					
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

