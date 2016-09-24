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

void rituals_prop_drop_on_break(World_Area* area, Entity* a) 
{
	if(a->kind != EntityKind_Prop) {
		printf("Could not drop on break: recieved entity type %d", a->kind);
		return;
	}
	auto p = &a->userdata.prop;
	if(p->amount == 0) return;
	Vec2 pos = a->body->shape.center;
	switch(p->contains) {
		case EntityKind_Prop:
			break;
		case EntityKind_Enemy:
			break;
		case EntityKind_Pickup:
			switch(p->subtype) {
				case PickupKind_Item:
					break;
				case PickupKind_Health:
					for(isize i = 0; i < p->amount; ++i) {
						Entity* e = world_area_get_next_entity(area);
						e->kind = EntityKind_Pickup;
						auto eud = &e->userdata.pickup;
						eud->kind = PickupKind_Health;
						eud->health.amount = p->quality;
						real size_mag = (a->body->shape.hw + a->body->shape.hh);
						e->body->shape.center = pos + v2(
								rand_range(&Game->r, -1, 1) * size_mag,
								rand_range(&Game->r, -1, 1) * size_mag
								);
						e->body->shape.hext = v2(4,4);
						e->sprite.flags = Anchor_Bottom;
						e->sprite.texture = rect2(
								9*32, 0, 16, 16);
						e->sprite.size = v2(8, 8);
						e->body->group = 1;
						e->hitbox.box.hext = e->body->shape.hext * 2.0;
					}
					break;
			}
			break;
		default: 
			break;
	}
}


Entity* rituals_spawn_enemy(World_Area* area, isize enemykind, Vec2 position) 
{
	Entity* e = world_area_get_next_entity(area);
	e->kind = EntityKind_Enemy;
	e->userdata.enemy.kind = enemykind;
	e->sprite.texture = rect2(
			enemykind * 32, 10*32, 32, 32);
	e->sprite.size = v2(32, 32);
	e->hitbox.box = aabb(v2(0, e->sprite.size.y * -0.5f), 16, 16);
	e->hitbox.mask = Flag(2);
	e->body->shape.center = position;
	e->sprite.flags = Anchor_Bottom;
	e->body->shape.hext = v2(8, 5);
	e->body->group = 2;
	e->attack = 5;
	e->attack_interval = 0.25f;
	e->knockback = 5000;
	auto enemy = &e->userdata.enemy;
	enemy->mode = 0;
	switch(enemy->kind) {
		case EnemyKind_Slime:
			enemy->speed = 200;
			enemy->alert_dist = 128;
			enemy->follow_dist = 512;
			e->health = 100;
			break;
		case EnemyKind_Bat:
			enemy->speed = 600;
			enemy->alert_dist = 64;
			enemy->follow_dist = 512;
			enemy->bat.perch = e->body->shape.center;
			e->body->flags = Body_Flag_No_Friction;
			e->hitbox.box.hh *= 0.75;
			e->hitbox.box.center.y += e->sprite.size.y / 4;
			e->sprite.size *= 0.5f;
			e->body->shape.hext *= 0.5f;
			e->hitbox.box.hext *= 0.5f;
			e->health = 50;
			break;
		case EnemyKind_Snake:
			enemy->speed = 200;
			enemy->alert_dist = 128; 
			enemy->follow_dist = 512;
			enemy->snake.chase_speed_modifier = 3.0f;
			e->hitbox.box.hh *= 0.5;
			e->hitbox.box.center.y += e->sprite.size.y / 4;
			e->sprite.size *= 0.75f;
			e->body->shape.hext *= 0.75f;
			e->hitbox.box.hext *= 0.75f;
			e->health = 50;
			break;
		case EnemyKind_Goblin_Knight:
			enemy->goblin_knight.patrol_start = e->sprite.position;
			enemy->goblin_knight.patrol_end = e->sprite.position + v2(512, 0);
			enemy->alert_dist = 128;
			enemy->follow_dist = 800;
			enemy->speed = 250;
			e->health = 300;
			break;
	}
	return e;
}

void rituals_on_activate_entity(Entity* entity, World_Area* area, World* world)
{

}

void rituals_on_destroy_entity(Entity* e, World_Area* area, World* world)
{
	if(e->kind == EntityKind_Bullet) {
			emitter_spawn(&world->emitter, 
					v3(e->sprite.position, 16), 
					v2(-Math_Pi, Math_Pi),
					4,
					copy_particle_style(world->base_style, v2(50, 150), v2i(10, 45)));
	}
}

bool rituals_frametick_entities(Entity* entities, isize count, World_Area* area, World* world)
{
	for(isize i = 0; i < count; ++i) {
		Entity* e = entities + i;
		if(e->kind == EntityKind_Player) {
			auto p = &e->userdata.player;
			if(p->heal_timer > 0) {
				p->heal_timer -= TimeStep;
			}

			int32 last_health = e->health;
			int32 ivl = p->heal_to_interval;
			if(last_health % ivl != 0)  {
				if(p->heal_timer <= 0) {
					int32 new_health = e->health + p->heal_rate;
					if(((new_health % ivl) < (last_health % ivl))) {
						while(new_health % ivl != 0) {
							new_health--;
						}
					}
					e->health = new_health;
				}
			}

			if(e->health <= 0) {
				play_state->delete_world_on_stop = true;
				play_state->save_world_on_stop = false;
				switch_state(Game_State_Menu);
				return true;
			}
		}
	}
	return false;
}

void rituals_slowtick_entities(Entity* entities, isize count, World_Area* area, World* world)
{
	for(isize i = 0; i < count; ++i) {
		Entity* e = entities + i;
	}
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
				s->texture = rect2(32 + frame * 32, 0, -32, 32);
			} else if(e->facing == 1) {
				s->texture = rect2(0  + frame * 32, 0, 32, 32);
			}
		}

		Sim_Body* b = e->body;
		if (b != NULL) {
			e->sprite.position = b->shape.center;
			e->sprite.position.y += b->shape.hh;
			if(Has_Flag(e->flags, EntityFlag_Tail)) {
				Vec2 v = b->velocity / 30.0f; 
				Sprite s = e->sprite;
				for(isize i = 0; i < 16; ++i) {
					render_add(&s);
					s.position -= v / 16;
					s.color = Color_White;
					s.color.w = lerp(1.0f, 0.0f, i/16.0);
					s.color.w *= s.color.w;
					s.sort_offset -= 10;
				}
			}
		}
		render_add(&e->sprite);
#if 0
		draw_box_outline(e->hitbox.box.center + e->sprite.position + v2(0, 1), e->hitbox.box.hext * 2, v4(1, 1, 1, 1), 1);
#endif
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
				s.texture = rect2(64, 0, 16, 16);
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
			case EntityKind_Prop:
				if(b->kind == EntityKind_Bullet) {
					a->health -= b->attack;
					if(a->health <= 0) {
						rituals_prop_drop_on_break(area, a);
						world_area_remove_entity(area, a);
					}
					world_area_remove_entity(area, b);
				}
				break;
			case EntityKind_Player:
				if(b->kind == EntityKind_Enemy) {
					if(b->attack_timer <= 0.0f) {
						b->attack_timer = b->attack_interval;
						a->health -= b->attack;
						real angle = v2_to_angle(b->walk_impulse);
						angle += rand_range(&Game->r, -10, 10) * Math_DegToRad;
						a->body->velocity += (v2_from_angle(angle) * b->knockback);
						a->userdata.player.heal_timer = a->userdata.player.heal_cooldown;
					}
				} else if(b->kind == EntityKind_Pickup) {
					if(b->userdata.pickup.kind == PickupKind_Health) {
						a->health += b->userdata.pickup.health.amount;
						world_area_remove_entity(area, b);
					}
				}
				break;
			case EntityKind_Enemy:
				if(b->kind == EntityKind_Bullet) {
					a->health -= b->attack;
					a->body->velocity += b->body->velocity * 4;
					a->userdata.enemy.mode = 1;
					if(a->health <= 0) {
						world_area_remove_entity(area, a);
					}
					world_area_remove_entity(area, b);
				}
				break;
		}
		
		
	}
}

void rituals_entity_on_contact_terrain(Entity* e, World_Area* area, World* world)
{
	if(e->kind == EntityKind_Bullet) {
		world_area_remove_entity(area, e);
	}
}

void rituals_contact_entities(Sim_Contact* contacts, isize count, World_Area* area, World* world)
{
	Simulator* sim = &area->sim;
	for(isize i = 0; i < count; ++i) {
		Sim_Contact* c = contacts + i;
		Sim_Body* body_a = sim_find_body(sim, c->a_id);
		if(body_a == NULL) continue;
		Sim_Body* body_b;
		if(c->b_id == -1) {
			body_b = c->static_b;
		} else {
			body_b = sim_find_body(sim, c->b_id);
		}
		if(body_b == NULL) continue;

		Entity* a = body_a->entity;
		Entity* b = body_b->entity;
		if(a == NULL && b != NULL) {
			rituals_entity_on_contact_terrain(b, area, world);
		} else if(a != NULL && b == NULL) {
			rituals_entity_on_contact_terrain(a, area, world);
		} else if(a != NULL && b != NULL) {
			if(a->kind > b->kind) {
				Entity* tmp = b;
				b = a;
				a = tmp;
			}

			if(b->kind == EntityKind_Bullet) {
				if(a->kind != EntityKind_Bullet && a->kind != EntityKind_Player) {
					world_area_remove_entity(area, b);
				}
			}
		}

	}
}
