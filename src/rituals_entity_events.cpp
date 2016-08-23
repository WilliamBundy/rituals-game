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

Entity* rituals_spawn_enemy(World_Area* area, isize enemykind, Vec2 position) 
{
	Entity* e = world_area_get_next_entity(area);
	e->kind = EntityKind_Enemy;
	e->userdata.enemy.kind = enemykind;
	e->sprite.texture = Get_Texture_Coordinates(
			enemykind * 32, 10*32, 32, 32);
	e->sprite.size = v2(32, 32);
	e->hitbox.box = aabb(v2(0, e->sprite.size.y * -0.5f), 16, 16);
	e->hitbox.mask = Flag(2);
	e->body->shape.center = position;
	e->sprite.anchor = Anchor_Bottom;
	e->body->shape.hext = v2(8, 5);
	auto enemy = &e->userdata.enemy;
	enemy->mode = 0;
	switch(enemy->kind) {
		case EnemyKind_Slime:
			enemy->speed = 200;
			enemy->alert_dist = 128;
			enemy->follow_dist = 128;
			break;
		case EnemyKind_Bat:
			enemy->speed = 600;
			enemy->alert_dist = 64;
			enemy->follow_dist = 128;
			enemy->bat.perch = e->body->shape.center;
			e->body->flags = Body_Flag_No_Friction;
			e->hitbox.box.hh *= 0.75;
			e->hitbox.box.center.y += e->sprite.size.y / 4;
			e->sprite.size *= 0.5f;
			e->body->shape.hext *= 0.5f;
			e->hitbox.box.hext *= 0.5f;
			break;
		case EnemyKind_Snake:
			enemy->speed = 200;
			enemy->alert_dist = 128; 
			enemy->follow_dist = 128;
			enemy->snake.chase_speed_modifier = 3.0f;
			e->hitbox.box.hh *= 0.5;
			e->hitbox.box.center.y += e->sprite.size.y / 4;
			e->sprite.size *= 0.75f;
			e->body->shape.hext *= 0.75f;
			e->hitbox.box.hext *= 0.75f;
			break;
		case EnemyKind_Goblin_Knight:
			enemy->goblin_knight.patrol_start = e->sprite.position;
			enemy->goblin_knight.patrol_end = e->sprite.position + v2(512, 0);
			enemy->alert_dist = 128;
			enemy->follow_dist = 384;
			enemy->speed = 250;
			break;
	}
	return e;
}

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
					//TODO(will) fix all of this; idea: get close to bat
					// then the bat moves in a ring around its perch
					// but.... this code doesn't do that.
				case EnemyKind_Bat:
					if(enemy->mode == 0) {
						dpos = e->body->shape.center - enemy->bat.perch;
						real player_dist = mag;
						mag = v2_dot(dpos, dpos);
						if(mag > 16) {
							real angle = atan2f(dpos.y, dpos.x);
							walk.x = cosf(angle) * -enemy->speed;
							walk.y = sinf(angle) * -enemy->speed;
						} else if(player_dist < (enemy->alert_dist * enemy->alert_dist)) {
							enemy->mode = 1;
						}
					} else if(enemy->mode == 1) {
						Vec2 perch_dpos = e->body->shape.center - enemy->bat.perch;
						real perch_dist2 = v2_dot(perch_dpos, perch_dpos);
						if(perch_dist2 < (enemy->follow_dist * enemy->follow_dist)) {
							real angle = atan2f(dpos.y, dpos.x);
							walk.x = cosf(angle) * -enemy->speed;
							walk.y = sinf(angle) * -enemy->speed;
						} else {
							enemy->mode = 0;
						}
					}
			}

			switch(enemy->kind) {
				case EnemyKind_Slime:
					//TODO(will) make goblin kights patrol 
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
		if(e->kind == EntityKind_Player) {
			Sim_Body* player = e->body;
			auto ud = &e->userdata.player;
			if(Input->scancodes[SDL_SCANCODE_SPACE] >= State_Pressed) {
				Sprite s;
				init_sprite(&s);
				s.position = player->shape.center;
				s.size = v2(16, 16);
				s.texture = Get_Texture_Coordinates(0, Renderer->texture_height - 16, 16, 16);
				s.color = v4(1, 1, 1, 1);
				switch(e->direction) {
					case Direction_North:
						s.position.y -= s.size.y + player->shape.hh;
						break;
					case Direction_South:
						s.position.y += s.size.y + player->shape.hh;
						break;
					case Direction_East:
						s.position.x += s.size.x + player->shape.hw;
						break;
					case Direction_West:
						s.position.x -= s.size.x + player->shape.hh;
						break;
				}

				if(Input->scancodes[SDL_SCANCODE_SPACE] == State_Just_Pressed) {
					//TODO(will) implement good space queries	
					Sim_Body* touching = sim_query_aabb(&area->sim, 
							aabb(s.position, s.size.x / 2, s.size.y / 2));
					if(touching != NULL) {
						if(!Has_Flag(touching->flags, Body_Flag_Static)) 
							ud->held_entity_id = touching->entity_id;
					}
				}
			} else {
				ud->held_entity_id = -1;
			}

			if(ud->held_entity_id > 0) {
				Entity* s = world_area_find_entity(area, ud->held_entity_id);
				if(s != NULL) {
					Sim_Body* b = s->body;
					Vec2 target = player->shape.center; 
					Vec2 diff = b->shape.hext + player->shape.hext + v2(8, 8);
					switch(e->direction) {
						case Direction_North:
							target.y -= diff.y;
							break;
						case Direction_South:
							target.y += diff.y;
							break;
						case Direction_East:
							target.x += diff.x;
							break;
						case Direction_West:
							target.x -= diff.x;
							break;
					}

					Vec2 impulse = (target - b->shape.center);
					if(v2_dot(impulse, impulse) > (4 * Tile_Size * Tile_Size)) {
						ud->held_entity_id = -1;
					}
					impulse *= 60;
					if(v2_dot(impulse, impulse) < (1000 * 1000)) 
						b->velocity += impulse;// * b->inv_mass;
				}
			}

		}
	}
}
void rituals_hit_entities(Hitbox_Contact* contacts, isize count, World_Area* area, World* world)
{
	for(isize i = 0; i < count; ++i) {
		Hitbox_Contact* c = contacts + i;
		Entity* a = c->a;
		Entity* b = c->b;

		//This saves us permutations:
		//the order of a/b doesn't matter, so we sort by
		//their kind, from rituals_game_info.cpp
		if(a->kind > b->kind) {
			Entity* tmp = b;
			b = a;
			a = tmp;
		}

		switch(a->kind) {
			case EntityKind_Player:
				break;
			case EntityKind_Enemy:
				if(b->kind == EntityKind_Bullet) {
					a->health -= b->attack;
					if(a->health <= 0) {
						a->kind = EntityKind_Prop;
						a->body->flags = Body_Flag_Sensor;
						a->sprite.color.w = 0.5f;
					}
					b->kind = EntityKind_Prop;
					b->attack = 0;
					b->sprite.color.w = 0;

				}
				break;
		}
		
		
	}
}

void rituals_contact_entities(Sim_Contact* contacts, isize count, World_Area* area, World* world)
{
	
}
