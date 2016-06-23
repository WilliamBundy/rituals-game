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

#define Tile_Size (32)
#define Half_TS (16)
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

void generate_tilemap(Tilemap* tilemap, uint64 seed)
{
	start_temp_arena(game->temp_arena);
	isize minified_w = tilemap->w / 8;
	isize minified_h = tilemap->h / 8;
	real* initial = Arena_Push_Array(game->temp_arena, real, minified_w * minified_h);

	Random rs;
	Random* r = &rs;
	init_random(r, seed);

	for(isize i = 0; i < minified_h; ++i) {
		for(isize j = 0; j < minified_w; ++j) {
			real rr = rand_range(r, 0, 2);
			initial[i * minified_w + j] = (rr * rr) / 4.0f;
		}
	}

	minified_w *= 2;
	minified_h *= 2;
	real* second = Arena_Push_Array(game->temp_arena, real, minified_w * minified_h);
		
	for(isize i = 0; i < minified_h; ++i) {
		for(isize j = 0; j < minified_w; ++j) {
			isize sample_x = j / 2;
			isize sample_y = i / 2;
			real val = rand_range(r, 0, 2.0f);
			real prev = initial[sample_y * minified_w / 2 + sample_x]; 
			second[i * minified_w + j] = prev + (val * val) / 4.0f;
		}
	}


	minified_w *= 2;
	minified_h *= 2;
	real* third = Arena_Push_Array(game->temp_arena, real, minified_w * minified_h);
		
	for(isize i = 0; i < minified_h; ++i) {
		for(isize j = 0; j < minified_w; ++j) {
			isize sample_x = j / 2;
			isize sample_y = i / 2;
			third[i * minified_w + j] = second[sample_y * minified_w / 2 + sample_x] + rand_range(r, 0, 1.0f);
			real val = rand_range(r, 0, 2.0f);
			real prev = second[sample_y * minified_w / 2 + sample_x]; 
			third[i * minified_w + j] = prev + (val * val) / 4.0f;
		}
	}

	minified_w *= 2;
	minified_h *= 2;
	real* fourth = Arena_Push_Array(game->temp_arena, real, minified_w * minified_h);
		
	for(isize i = 0; i < minified_h; ++i) {
		for(isize j = 0; j < minified_w; ++j) {
			isize sample_x = j / 2;
			isize sample_y = i / 2;
			real val = rand_range(r, 0, 2.0f);
			real prev = third[sample_y * minified_w / 2 + sample_x]; 
			fourth[i * minified_w + j] = prev + (val * val) / 4.0f;
			fourth[i * minified_w + j] /= 4.0f;
		}
	}

	for(isize i = 0; i < tilemap->h; ++i) {
		for(isize j = 0; j < tilemap->w; ++j) {
			isize index = i * tilemap->w + j;
			if(i == 0 || i == tilemap->h - 1 || j == 0 || j == tilemap->w - 1) {
				tilemap->tiles[index] = 7;
				continue;
			}

			real height = fourth[index];
			if(height < 0.25f) {
				//water
				tilemap->tiles[index] = 8;
			} else if(height < 0.35f) {
				//sand
				tilemap->tiles[index] = 1;

			} else if(height < 0.5f) {
				//sparse grass
				tilemap->tiles[index] = 3;
			} else if(height < 0.65f) {
				//dense grass
				tilemap->tiles[index] = 4;
			} else if(height < 0.8f) {
				// trees
				tilemap->tiles[index] = 7;
			} else {
				tilemap->tiles[index] = 9;

			}
		}
	}


	end_temp_arena(game->temp_arena);

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
void render_tilemap(Tilemap* tilemap, Vec2 pos)
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

void render_tilemap(Tilemap* tilemap, Vec2 pos, Rect2 area)
{
	isize startx = area.x / Tile_Size - 1;
	isize starty = area.y / Tile_Size - 1;
	isize endx = (area.x + area.w) / Tile_Size + 1;
	isize endy = (area.y + area.h) / Tile_Size + 1;
	if(startx < 0) startx = 0;
	if(starty < 0) starty = 0;
	if(endx > tilemap->w) endx = tilemap->w;
	if(endy > tilemap->h) endy = tilemap->h;
	Sprite s;
	for(isize i = starty; i < endy; ++i) {
		for(isize j = startx; j < endx; ++j) {
			Tile_Info* t = tilemap->info + tilemap->tiles[i * tilemap->w + j];
			init_sprite(&s);
			s.position = v2(j*Tile_Size + Half_TS, i*Tile_Size + Half_TS) + pos;
			s.size = v2(Tile_Size, Tile_Size);
			s.texture = t->texture_clip;
			renderer_push_sprite(&s);
		}
	}

	
	
}
