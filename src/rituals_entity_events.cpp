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
#define _scancode(s1) ((Input->scancodes[SDL_SCANCODE_##s1]))
void rituals_walk_entities(Entity* entities, isize count, World_Area* area, World* world)
{
	for(isize i = 0; i < count; ++i) {
		Entity* e = entities + i;
		e->walk_impulse = Vec2{};	
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
			auto enemy = &e->userdata.enemy;
			Vec2 walk = Vec2{};
			Vec2 dpos = e->body->shape.center - area->player->body->shape.center;
			real mag = v2_dot(dpos, dpos);

			switch(enemy->kind) {
				case EnemyKind_Slime:
				case EnemyKind_Goblin_Knight:
				case EnemyKind_Snake:
					if(enemy->mode == 0) {
						if(mag < (enemy->alert_dist * enemy->alert_dist)) {
							enemy->mode = 1;
						}
					} else if(enemy->mode == 1) {
						if(mag < (enemy->follow_dist * enemy->follow_dist)) {
							real angle = atan2f(dpos.y, dpos.x);
							walk.x = cosf(angle) * -enemy->speed;
							walk.y = sinf(angle) * -enemy->speed;
						} else {
							enemy->mode = 0;
						}
					}
					break;
				case EnemyKind_Bat:
					if(enemy->mode == 0) {
						if(mag < (enemy->alert_dist * enemy->alert_dist)) {
							enemy->mode = 1;
						} else {
							dpos = e->body->shape.center - enemy->bat.perch;
							mag = v2_dot(dpos, dpos);
							if(mag > 16) {
								real angle = atan2f(dpos.y, dpos.x);
								walk.x = cosf(angle) * -enemy->speed;
								walk.y = sinf(angle) * -enemy->speed;

							}


						}
					} else if(enemy->mode == 1) {
						enemy->bat.arc_perc += 0.05;
						Vec2 target;
						if(enemy->bat.arc_perc <= 0.2) {
							target = enemy->bat.perch + v2(enemy->follow_dist, 0);
						} else {
							real angle = Math_Tau * (enemy->bat.arc_perc - 0.2 / 0.8);
							target = enemy->bat.perch + 
								v2(cosf(angle), sinf(angle)) * enemy->follow_dist;
						}
						real angle = atan2f(target.y, target.x);
						walk.x = cosf(angle) * enemy->speed;
						walk.y = sinf(angle) * enemy->speed;
						if(enemy->bat.arc_perc >= 1) {
							enemy->mode = 0;
							enemy->bat.arc_perc = 0;
						}
					}

			}

			switch(enemy->kind) {
				case EnemyKind_Slime:
				case EnemyKind_Goblin_Knight:
				case EnemyKind_Bat:
					e->walk_impulse = walk;
					break;
				case EnemyKind_Snake:
					e->walk_impulse = enemy->mode == 1 ? 
						enemy->snake.chase_speed_modifier * walk :
						walk;
					break;
			}
		}
	}
}

void rituals_animate_entities(Entity* entities, isize count, World_Area* area, World* world)
{
	for(isize i = 0; i < count; ++i) {
		Entity* e = entities + i;
		if(e->kind == EntityKind_Player) {
			Direction old_direction = e->direction;
			Vec2 walk = e->walk_impulse;
			if(walk.y < 0) {
				e->direction = Direction_North;
			} else if(walk.y > 0) {
				e->direction = Direction_South;
			}

			if(walk.x < 0) {
				e->facing = -1;
				e->direction = Direction_West;
			} else if(walk.x > 0) {
				e->facing = 1;
				e->direction = Direction_East;
			}
			if(Input->scancodes[SDL_SCANCODE_SPACE] == State_Pressed) {
				e->direction = old_direction;
			}
			int32 frame = 0;
			if(v2_dot(walk, walk) > 0){
				e->counter++;
				frame = 1;
				if(e->counter > 15) {
					frame = 0;
					if(e->counter > 30) {
						e->counter = 0;
					}
				}
			} else {
				e->counter = 0;
				frame = 0;
			}

			Sprite* s = &e->sprite;
			if(e->facing == -1) {
				s->texture = Get_Texture_Coordinates(32 + frame * 32, 0, -32, 32);
			} else if(e->facing == 1) {
				s->texture = Get_Texture_Coordinates(0  + frame * 32, 0, 32, 32);
			}
		}
	}
}
void rituals_interact_entities(Entity* entities, isize count, World_Area* area, World* world)
{
	for(isize i = 0; i < count; ++i) {
		Entity* e = entities + i;
	}
}
