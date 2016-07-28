/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* 
 * rituals_entity_events.cpp
 *
 */ 

#define _check(s1, s2, state) ((Input->scancodes[SDL_SCANCODE_##s1] == state) || (Input->scancodes[SDL_SCANCODE_##s2] == state))
void rituals_walk_entities(Entity* entities, isize count, World_Area* area, World* world)
{
	for(isize i = 0; i < count; ++i) {
		Entity* e = entities + i;
		if(e->kind == EntityKind_Player) {
			real movespeed = 800;
			Vec2 move_impulse = v2(0, 0);

			if(_check(LEFT, A, State_Pressed)) {
				move_impulse.x -= movespeed;
			}
			if(_check(RIGHT, D, State_Pressed)) {
				move_impulse.x += movespeed;
			}
			if(_check(UP, W, State_Pressed)) {
				move_impulse.y -= movespeed;
			}
			if(_check(DOWN, S, State_Pressed)) {
				move_impulse.y += movespeed;
			}

			if(fabsf(move_impulse.x * move_impulse.y) > 0.01f) {
				move_impulse *= Math_InvSqrt2;
			}

			Tile_Info* player_tile = Registry->tiles + tilemap_get_at(&area->map, e->body->shape.center);

			move_impulse *= player_tile->movement_modifier;
			e->walk_impulse = move_impulse;
		} else if(e->kind == EntityKind_Enemy) {
			real movespeed = 200;
			Vec2 walk = Vec2{};
			Vec2 dpos = e->body->shape.center - area->player->body->shape.center;
			real angle = atan2f(dpos.y, dpos.x);
			walk.x = cosf(angle) * movespeed;
			walk.y = sinf(angle) * movespeed;

			e->walk_impulse = walk;
		}
	}
}

