/*
 * rituals_tilemap.cpp
 * 
 */

typedef int32 Tile;

struct Tile_Info
{
	isize id;
	Rect2 texture_clip;

	//maybe change to flags
	bool solid;
};

struct Tilemap
{
	isize w, h;
	Tile* tiles;

	Tile_Info* info;
	isize info_count;
};

#define Max_Tile_Info_Count (256)

void init_tilemap(Tilemap* tilemap, isize w, isize h, Memory_Arena* arena)
{
	tilemap->w = w;
	tilemap->h = h;
	tilemap->tiles = Arena_Push_Array(arena, Tile, w * h);
	tilemap->info = Arena_Push_Array(arena, Tile_Info, Max_Tile_Info_Count);
}

Tile_Info* add_tile_info(Tilemap* tilemap, Rect2 texture_clip, bool solid)
{
	Tile_Info* t = tilemap->info + tilemap->info_count++;
	t->texture_clip = texture_clip;
	t->solid = solid;

	return t;
}

void generate_tilemap(Tilemap* tilemap)
{
	for(isize i = 0; i < tilemap->h; ++i) {
		for(isize j = 0; j < tilemap->w; ++j) {
			if(next_random(&game->r) > .5) {
				tilemap->tiles[i * tilemap->w + j] = 4;
			} else {
				tilemap->tiles[i * tilemap->w + j] = 3;
			}
		}
	}

}

bool tilemap_get_at(Tilemap* tilemap, isize x, isize y, Tile* out)
{
	isize i = y * tilemap->w + x;
	if(i < 0 || i >= (tilemap->w * tilemap->h)) return true;
	*out = tilemap->tiles[i];
	return false;
}

Tile tilemap_get_at(Tilemap* tilemap, isize x, isize y)
{
	isize i = y * tilemap->w + x;
	if(i < 0 || i >= (tilemap->w * tilemap->h)) return 0;
	return tilemap->tiles[i];
}

bool tilemap_set_at(Tilemap* tilemap, isize x, isize y, Tile value)
{
	isize i = y * tilemap->w + x;
	if(i < 0 || i >= (tilemap->w * tilemap->h)) return true;
	tilemap->tiles[i] = value;
	return false;
}

bool b = 0;
void render_tilemap(Tilemap* tilemap, Vec2 pos, real scale)
{
	Sprite s;
	for(isize i = 0; i < tilemap->h; ++i) {
		for(isize j = 0; j < tilemap->w; ++j) {
			Tile_Info* t = tilemap->info + tilemap->tiles[i * tilemap->w + j];
			init_sprite(&s);
			s.position = v2(j*32, i*32) + pos;
			s.size = v2(32, 32);
			s.texture = t->texture_clip;
			renderer_push_sprite(&s);
		}
	}
}

