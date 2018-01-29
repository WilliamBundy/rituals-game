//#define TiSz (32.0f)
typedef i32 Tile;
#ifndef WirmphtEnabled
struct Tilemap
{
	isize w, h;
	Tile* tiles;
	Tile_State* states;
};
#endif

#define WorldAreaTilemapWidth (64)
#define WorldAreaTilemapHeight (64)

void init_tilemap(Tilemap* tilemap, isize w, isize h, MemoryArena* arena)
{
	tilemap->w = w;
	tilemap->h = h;
	tilemap->tiles = (Tile*)arenaPush(arena, sizeof(Tile) * w * h);
	tilemap->states = (Tile_State*)arenaPush(arena, sizeof(Tile_State) * w * h);
}

void generate_tilemap(Tilemap* tilemap, u64 seed)
{
	for(isize i = 0; i < tilemap->w * tilemap->h; ++i) {
		tilemap->tiles[i] = (i%Tile_Slab_Wall)+1;
	}
}

Tile tilemap_get_at(Tilemap* tilemap, isize x, isize y)
{
	isize i = y * tilemap->w + x;
	if(i < 0 || i >= (tilemap->w * tilemap->h)) return 0;
	return tilemap->tiles[i];
}

Tile tilemap_get_at(Tilemap* tilemap, Vec2 pos)
{
	isize x = (isize)(pos.x / TiSz);
	isize y = (isize)(pos.y / TiSz);
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
	isize x = (isize)(pos.x / TiSz);
	isize y = (isize)(pos.y / TiSz);
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
	isize x = (isize)(pos.x / TiSz);
	isize y = (isize)(pos.y / TiSz);
	return tilemap_set_at(tilemap, x, y, value);
}

void update_tile_state_at(Game_Registry* Registry, Tilemap* map, isize x, isize y)
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

void update_tile_state_at(Game_Registry* Registry, Tilemap* map, Vec2 pos)
{
	isize x = (isize)(pos.x / TiSz);
	isize y = (isize)(pos.y / TiSz);
	return update_tile_state_at(Registry, map, x, y);
}



void render_tilemap(Game_Registry* Registry, Tilemap* tilemap, Vec2 pos)
{
	Sprite s;
	for(isize i = 0; i < tilemap->h; ++i) {
		for(isize j = 0; j < tilemap->w; ++j) {
			Tile_Info* t = Registry->tiles + tilemap->tiles[i * tilemap->w + j];
			wInitSprite(&s);
			s.flags = Sprite_NoAA;
			s.pos = v2(j*TiSz, i*TiSz) + pos;
			s.size = v2(TiSz, TiSz);
			s.texture = t->texture;
			render_add(&s);
		}
	}
}

isize render_tilemap(Game_Registry* Registry, Tilemap* tilemap, Vec2 pos, Rect2 area)
{
	isize startx = area.x / TiSz - 1;
	isize starty = area.y / TiSz - 1;
	isize endx = (area.x + area.w) / TiSz + 1;
	isize endy = (area.y + area.h) / TiSz + 1;
	if(startx < 0) startx = 0;
	if(starty < 0) starty = 0;
	if(endx > tilemap->w) endx = tilemap->w;
	if(endy > tilemap->h) endy = tilemap->h;
	Sprite s;
	for(isize i = starty; i < endy; ++i) {
		for(isize j = startx; j < endx; ++j) {
			Tile tile = tilemap->tiles[i * tilemap->w + j];
			Tile_Info* t = Registry->tiles + tile;
			wInitSprite(&s);
			s.pos = v2(j*TiSz + hTiSz, i*TiSz + hTiSz) + pos;
			s.size = v2(TiSz, TiSz);

			if(t->has_bottom_texture && tilemap_get_at(tilemap, j, i + 1) != tile) {
				s.texture = t->bottom_texture;
			} else {
				s.texture = t->texture;
				render_add(&s);
			}
			render_add(&s);

			Tile_State* state = tilemap->states + (i * tilemap->w + j);
			if(state->damage > 0) {
				f32 dmgp = (f32)(state->damage) / (f32)(t->max_damage);
				dmgp *= 3;
				i32 frame = (i32)dmgp;
				s.texture = _tile_texture((5 + frame), 0);
				render_add(&s);
			}
		}
	}
	isize unsorted_index = Game->render->count;
	for(isize i = starty; i < endy; ++i) {
		for(isize j = startx; j < endx; ++j) {
				Tile tile = tilemap->tiles[i * tilemap->w + j];
				Tile bottom_tile = tilemap_get_at(tilemap, j, i + 1);
				if(tile != bottom_tile) {
					Tile_Info* bottom = Registry->tiles + tilemap_get_at(tilemap, j, i + 1);
					if(bottom->has_top_texture) {
						wInitSprite(&s);
						s.pos = v2(j*TiSz + hTiSz, (i+1)*TiSz) + pos;
						s.size = v2(TiSz, TiSz);
						s.texture = bottom->top_texture;
						s.flags = Anchor_BottomCenter;
						s.sort_offset = 4;
						render_add(&s);
					}
				}

		}
	}
	return unsorted_index;
}

