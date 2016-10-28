/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 * rituals_tilemap.cpp
 * 
 */

#ifndef REFLECTED
struct Tilemap
{
	isize w, h;
	Tile* tiles;
	Tile_State* states;
};
#endif

#define WorldAreaTilemapWidth (256)
#define WorldAreaTilemapHeight (256)

void init_tilemap(Tilemap* tilemap, isize w, isize h, Memory_Arena* arena)
{
	tilemap->w = w;
	tilemap->h = h;
	tilemap->tiles = arena_push_array(arena, Tile, w * h);
	tilemap->states = arena_push_array(arena, Tile_State,  w * h);
}

void generate_tilemap(Tilemap* tilemap, uint64 seed)
{
	start_temp_arena(Game->temp_arena);
	isize minified_w = tilemap->w / 8;
	isize minified_h = tilemap->h / 8;
	real* initial = arena_push_array(Game->temp_arena, real, minified_w * minified_h);

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
	real* second = arena_push_array(Game->temp_arena, real, minified_w * minified_h);
		
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
	real* third = arena_push_array(Game->temp_arena, real, minified_w * minified_h);
		
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
	real* fourth = arena_push_array(Game->temp_arena, real, minified_w * minified_h);
		
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

	bool flag = false;
	for(isize i = 0; i < tilemap->h; ++i) {
		for(isize j = 0; j < tilemap->w; ++j) {
			isize index = i * tilemap->w + j;
			if( i == 0 || 
				i == tilemap->h - 1 || 
				j == 0 ||
				j == tilemap->w - 1) {
				tilemap->tiles[index] = Tile_Tree_Wall;
				flag = true;
			}

			if((i >= (tilemap->h/2 - 1) &&
				i <= (tilemap->h/2 + 1)) ||
			   (j >= (tilemap->w/2 - 1) && 
				j <= (tilemap->w/2 + 1))) {
				tilemap->tiles[index] = Tile_Stone_Road;
				if(rand_range(r, 0, 1) < 0.25f) {
					tilemap->tiles[index] = Tile_Cracked_Road;
				//	if(rand_range(r, 0, 1) < 0.1f) {
				//		tilemap->tiles[index] = Tile_Pristine_Road;
				//	}
				}
				flag = true;
			}

			if(flag) {
				flag = false;
				continue;
			}


			real height = fourth[index];
			if(height < 0.10f) {
				tilemap->tiles[index] = Tile_Water;
			}
			else if(height < 0.22f) {
				//water
				tilemap->tiles[index] = Tile_Shallow_Water;
			} else if(height < 0.25f) {
				//sand
				tilemap->tiles[index] = Tile_Sand;
			} else if(height < 0.4f) {
				//sparse grass
				tilemap->tiles[index] = Tile_Grass;
			} else if(height < 0.5f) {
				//dense grass
				tilemap->tiles[index] = Tile_Dense_Grass;
			} else if(height < 0.7f) {
				// trees
				tilemap->tiles[index] = Tile_Tree_Wall;
			} else  {
				tilemap->tiles[index] = Tile_Tree_Wall;
			}
		}
	}
	for(isize i = 0; i < tilemap->h; ++i) {
		for(isize j = 0; j < tilemap->w; ++j) {
			isize index = i * tilemap->w + j;
			init_tile_state(tilemap->states + index, tilemap->tiles[index]);
		}
	}

	end_temp_arena(Game->temp_arena);

}

Tile tilemap_get_at(Tilemap* tilemap, isize x, isize y)
{
	isize i = y * tilemap->w + x;
	if(i < 0 || i >= (tilemap->w * tilemap->h)) return 0;
	return tilemap->tiles[i];
}

Tile tilemap_get_at(Tilemap* tilemap, Vec2 pos)
{
	isize x = (isize)(pos.x / Tile_Size);
	isize y = (isize)(pos.y / Tile_Size);
	return tilemap_get_at(tilemap, x, y);
}

Tile_State* tilemap_get_state_at(Tilemap* tilemap, isize x, isize y)
{
	isize i = y * tilemap->w + x;
	if(i < 0 || i >= (tilemap->w * tilemap->h)) return NULL;
	return tilemap->states + i;
}

Tile_State* tilemap_get_state_at(Tilemap* tilemap, Vec2 pos)
{
	isize x = (isize)(pos.x / Tile_Size);
	isize y = (isize)(pos.y / Tile_Size);
	return tilemap_get_state_at(tilemap, x, y);
}

bool tilemap_set_at(Tilemap* tilemap, isize x, isize y, Tile value)
{
	isize i = y * tilemap->w + x;
	if(i < 0 || i >= (tilemap->w * tilemap->h)) return true;
	tilemap->tiles[i] = value;
	return false;
}

bool tilemap_set_at(Tilemap* tilemap, Vec2 pos, Tile value)
{
	isize x = (isize)(pos.x / Tile_Size);
	isize y = (isize)(pos.y / Tile_Size);
	return tilemap_set_at(tilemap, x, y, value);
}

void update_tile_state_at(Tilemap* map, isize x, isize y)
{
	Tile_State* state = tilemap_get_state_at(map, x, y);
	if(state != NULL) {
		Tile_Info* info = Registry->tiles + tilemap_get_at(map, x, y);
		if(info->immune_to_damage) {
			state->damage = 0;
		} else {
			if(state->damage >= info->max_damage) {
				init_tile_state(state, info->break_to_id);
				tilemap_set_at(map, x, y, info->break_to_id);
			}
		}

	}
}

void update_tile_state_at(Tilemap* map, Vec2 pos)
{
	isize x = (isize)(pos.x / Tile_Size);
	isize y = (isize)(pos.y / Tile_Size);
	return update_tile_state_at(map, x, y);
}



void render_tilemap(Tilemap* tilemap, Vec2 pos)
{
	Sprite s;
	for(isize i = 0; i < tilemap->h; ++i) {
		for(isize j = 0; j < tilemap->w; ++j) {
			Tile_Info* t = Registry->tiles + tilemap->tiles[i * tilemap->w + j];
			init_sprite(&s);
			s.position = v2(j*32, i*32) + pos;
			s.size = v2(32, 32);
			s.texture = t->texture;
			render_add(&s);
		}
	}
}

isize render_tilemap(Tilemap* tilemap, Vec2 pos, Rect2 area)
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
			Tile tile = tilemap->tiles[i * tilemap->w + j];
			Tile_Info* t = Registry->tiles + tile;
			init_sprite(&s);
			s.position = v2(j*Tile_Size + Half_TS, i*Tile_Size + Half_TS) + pos;
			s.size = v2(Tile_Size, Tile_Size);

			if(t->has_bottom_texture && tilemap_get_at(tilemap, j, i + 1) != tile) {
				s.texture = t->bottom_texture;
			} else {
				s.texture = t->texture;
				render_add(&s);
			}
			render_add(&s);

			Tile_State* state = tilemap->states + (i * tilemap->w + j);
			if(state->damage > 0) {
				real dmgp = (real)(state->damage) / (real)(t->max_damage);
				dmgp *= 3;
				int32 frame = (int32)dmgp;
				s.texture = _tile_texture((5 + frame), 0);
				render_add(&s);
			}
		}
	}
	isize unsorted_index = Renderer->groups[0].sprites_count;
	for(isize i = starty; i < endy; ++i) {
		for(isize j = startx; j < endx; ++j) {
				Tile tile = tilemap->tiles[i * tilemap->w + j];
				Tile bottom_tile = tilemap_get_at(tilemap, j, i + 1);
				if(tile != bottom_tile) {
					Tile_Info* bottom = Registry->tiles + tilemap_get_at(tilemap, j, i + 1);
					if(bottom->has_top_texture) {
						init_sprite(&s);
						s.position = v2(j*Tile_Size + Half_TS, (i+1)*Tile_Size) + pos;
						s.size = v2(Tile_Size, Tile_Size);
						s.texture = bottom->top_texture;
						s.flags = Anchor_Bottom;
						s.sort_offset = 4;
						render_add(&s);
					}
				}

		}
	}
	return unsorted_index;
}

