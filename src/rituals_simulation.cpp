/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 * rituals_simulation.cpp
 */

enum Sim_Body_Flags
{
	Body_Flag_None,
	Body_Flag_Static = Flag(1),
	Body_Flag_No_Friction = Flag(2),
	Body_Flag_Sensor = Flag(3)
};


typedef struct Entity Entity;
struct Sim_Body
{
	isize id;
	AABB shape;
	Vec2 velocity, force, collision_vel;
	real inv_mass, restitution, damping;
	uint64 flags;
	isize entity_id;
	Entity* entity;
};

struct Sim_Contact
{
	isize a_id;
	isize b_id;
	Vec2 overlap;
	real mag;
	Vec2 normal;
};

#define _body_get_min_x(e) (e.shape.center.x - e.shape.hw)
#define _body_get_min_y(e) (e.shape.center.y - e.shape.hh)
GenerateIntrosortForType(body_sort_on_x, Sim_Body, 12, _body_get_min_x)
GenerateIntrosortForType(body_sort_on_y, Sim_Body, 12, _body_get_min_y)

#define _body_get_not_static(e) (!Has_Flag(e.flags, Body_Flag_Static))
GenerateIntrosortForType(body_sort_static_first, Sim_Body, 12, _body_get_not_static)

#define _body_get_id(e) (e.id)
GenerateIntrosortForType(body_sort_on_id, Sim_Body, 12, _body_get_id)
GenerateBinarySearchForType(body_search_for_id, Sim_Body, isize, _body_get_id)

void init_body(Sim_Body* b)
{
	b->shape = aabb(v2(0, 0), 0, 0);
	b->inv_mass = 1.0f;
	b->restitution = 0.3f;
	b->velocity = v2(0,0);
	b->damping = 0.5f;
	b->force = v2(0, 0);
	b->flags = Body_Flag_None;
}

struct Simulator
{
	Sim_Body* bodies;
	isize bodies_count, bodies_capacity, next_body_id;
	Sim_Contact* contacts;
	isize contacts_count, contacts_capacity;

	isize sort_axis;
};

Sim_Body* sim_get_next_body(Simulator* sim)
{
	if(sim->bodies_count + 1 > sim->bodies_capacity) {
		Log_Error("Ran out of bodies");
		return NULL;
	}

	Sim_Body* e = sim->bodies + sim->bodies_count++;
	init_body(e);
	e->id = sim->next_body_id++;

	return e;
}

void init_simulator(Simulator* sim, isize cap, Memory_Arena* arena)
{
	sim->bodies_count = 0;
	sim->bodies_capacity = cap;
	sim->sort_axis = 0;
	sim->next_body_id = 0;
	sim->bodies = arena_push_array(arena, Sim_Body, cap);
	sim->contacts_capacity = Max(512, cap / 16);
	sim->contacts = arena_push_array(arena, Sim_Contact, sim->contacts_capacity);
}

Sim_Body* sim_find_body(Simulator* sim, isize id)
{
	isize index = body_search_for_id(id, sim->bodies, sim->bodies_count);
	return index == -1? NULL: sim->bodies + index;
}

void sim_sort_bodies_on_id(Simulator* sim)
{
	body_sort_on_id(sim->bodies, sim->bodies_count);
}

void sim_remove_body(Simulator* sim, isize body)
{
	isize index = body_search_for_id(body, sim->bodies, sim->bodies_count);
	sim->bodies[index] = sim->bodies[--sim->bodies_count];
	sim_sort_bodies_on_id(sim);
}

void sim_remove_body(Simulator* sim, Sim_Body* body)
{
	sim_remove_body(sim, body->id);
}

Sim_Body* sim_query_aabb(Simulator* sim, AABB query)
{
	for(isize i = 0; i < sim->bodies_count; ++i) {
		Sim_Body* a = sim->bodies + i;
		if(aabb_intersect(&a->shape, &query)) {
			return a;
		}
	}
	return NULL;
}

#define TimeStep (1.0f/60.0f)
#define SimIter_i (8)
#define SimIter ((real)SimIter_i)
void sim_update(Simulator* sim, Tilemap* map, real dt, bool capture_contacts = true)
{
	if(capture_contacts)
		sim->contacts_count = 0;
	Sim_Body *a, *b;
	for(isize times = 0; times < SimIter_i; ++times) {
		if(sim->sort_axis == 0) {
			body_sort_on_x(sim->bodies, sim->bodies_count);
		} else if(sim->sort_axis == 1) {
			body_sort_on_y(sim->bodies, sim->bodies_count);
		}
		Vec2 center_sum1 = v2(0, 0);
		Vec2 center_sum2 = v2(0, 0);
		Vec2 variance = v2(0, 0);
		for(isize i = 0; i < sim->bodies_count; ++i) {
			a = sim->bodies + i;

			//sweep and prune stuff
			center_sum1 += a->shape.center;
			for(isize q = 0; q < 2; ++q) {
				center_sum2.e[q] += a->shape.center.e[q] * a->shape.center.e[q];
			}

			//if(a->is_static) continue;

			for(isize j = i + 1; j < sim->bodies_count; ++j) {
				b = sim->bodies + j;

				uint64 a_is_static = Has_Flag(a->flags, Body_Flag_Static);
				uint64 b_is_static = Has_Flag(b->flags, Body_Flag_Static);
				if(a_is_static && b_is_static) continue;

				if(sim->sort_axis == 0) {
					if(AABB_x1(b->shape) > AABB_x2(a->shape)) {
						break;
					}
				} else if(sim->sort_axis == 1) {
					if(AABB_y1(b->shape) > AABB_y2(a->shape)) {
						break;
					}
				}
			
				if(aabb_intersect(&a->shape, &b->shape)) {
					Vec2 overlap;
					aabb_overlap(&a->shape, &b->shape, &overlap);
					real ovl_mag = sqrtf(v2_dot(overlap, overlap));
					if (ovl_mag < 0.0001f) continue;
					Vec2 normal = overlap * (1.0f / ovl_mag);

					if(a->id == 0 || b->id  == 0) {
						aabb_intersect(&a->shape, &b->shape);
					}

					if(capture_contacts && times == 0) {
						Sim_Contact c;
						c.a_id = a->id;
						c.b_id = b->id;
						c.overlap = overlap;
						c.normal = normal;
						c.mag = ovl_mag;
						if(sim->contacts_count < sim->contacts_capacity) {
							sim->contacts[sim->contacts_count++] = c;
						}
					}


					#define _collision_slop (0.8f)
					if(a_is_static && !b_is_static) {
						b->shape.center += overlap;
						Vec2 relative_velocity = b->velocity;
						real velocity_on_normal = v2_dot(relative_velocity, normal);
						if(velocity_on_normal > 0) continue;

						real e = Min(a->restitution, b->restitution);
						real mag = -1.0f * (1.0f + e) * velocity_on_normal;
						mag /= b->inv_mass;
						Vec2 impulse = mag * normal;
						b->collision_vel += b->inv_mass * impulse;
					} else if(!a_is_static && b_is_static) {
						a->shape.center -= overlap;

						Vec2 relative_velocity = -a->velocity;
						real velocity_on_normal = v2_dot(relative_velocity, normal);
						if(velocity_on_normal > 0) continue;

						real e = Min(a->restitution, b->restitution);
						real mag = -1.0f * (1.0f + e) * velocity_on_normal;
						mag /= a->inv_mass + 0;
						Vec2 impulse = mag * normal;
						a->collision_vel -= a->inv_mass * impulse;
					} else {
						Vec2 separation = Max(ovl_mag - _collision_slop, 0) 
							* (1.0f / (a->inv_mass + b->inv_mass)) * 0.5f * normal;
						a->shape.center -= a->inv_mass * separation;
						b->shape.center += b->inv_mass * separation;

						Vec2 relative_velocity = b->velocity - a->velocity;
						real velocity_on_normal = v2_dot(relative_velocity, normal);
						if(velocity_on_normal > 0) continue;

						real e = Min(a->restitution, b->restitution);
						real mag = -1.0f * (1.0f + e) * velocity_on_normal;
						mag /= a->inv_mass + b->inv_mass;
						Vec2 impulse = mag * normal;
						a->collision_vel -= a->inv_mass * impulse;
						b->collision_vel += b->inv_mass * impulse;
					}
				}
			}
		}

		for(isize i = 0; i < 2; ++i) {
			variance.e[i] = center_sum2.e[i] - center_sum1.e[i] * center_sum1.e[i] / 
				sim->bodies_count;
		}
		
		if(variance.x > variance.y) {
			sim->sort_axis = 0;
		} else {
			sim->sort_axis = 1;
		}

		for(isize i = 0; i < sim->bodies_count; ++i) {
			a = sim->bodies + i;
			if(Has_Flag(a->flags, Body_Flag_Static)) continue;
			Vec2 iter_acl = (a->force * a->inv_mass) / SimIter;
			Vec2 new_vel = a->velocity + (dt * iter_acl);
			Vec2 dpos = (a->velocity + new_vel) * 0.5f;
			dpos *= 1.0f / SimIter;
			a->shape.center += dpos * dt;
			a->shape.center += a->collision_vel / SimIter * dt;
			a->velocity = new_vel;
			Tile_Info* tile = Registry->tiles + tilemap_get_at(map, a->shape.center);
			real damping = 1.0f;
			if(Has_Flag(a->flags, Body_Flag_No_Friction)) {
				damping = a->damping;
			} else {
				damping = sqrtf(a->damping * a->damping + 
					tile->friction * tile->friction) * Math_InvSqrt2;
			}
			a->velocity *= powf(damping, SimIter);
			a->velocity += a->collision_vel;
			a->collision_vel = v2(0, 0);
		}

	}
	body_sort_on_id(sim->bodies, sim->bodies_count);
}

isize _tw, _th;
uint8* _tiles;
uint8 _get_at(isize x, isize y)
{
	if((x < 0) || (x > _tw) || (y < 0) || (y > _th)) return false;
	isize index = y * _tw + x;
	if((index < 0) || (index >= _tw * _th)) return false;
	return _tiles[index];

}

void generate_statics_for_tilemap(Simulator* sim, Tilemap* tilemap)
{
	start_temp_arena(Game->temp_arena);
	_tw = tilemap->w;
	_th = tilemap->h;
	isize map_size = tilemap->w * tilemap->h;
	uint8* tiles = arena_push_array(Game->temp_arena, uint8, map_size + 1);
	for(isize i = 0; i < map_size; ++i) {
		tiles[i] = Registry->tiles[tilemap->tiles[i]].solid;
	}
	_tiles = tiles;
	isize work = 0;

	Rect2i* rects = arena_push_array(Game->temp_arena, Rect2i, map_size / 2);
	isize rects_count = 0;
	isize last_rects = 0;
	do {
		last_rects = rects_count;
		for(isize y = 0; y < tilemap->h; ++y) {
			for(isize x = 0; x < tilemap->w; ++x) {
				if(_get_at(x, y)) {
					if(!_get_at(x, y - 1)) {
						Rect2i* r = rects + rects_count++;
						r->x = x;
						r->y = y;
						r->w = 1;
						r->h = 1;
						do {
							x++;
						}
						while(_get_at(x, y) && !_get_at(x, y - 1) && (x < tilemap->w));

						if(x != r->x) {
							r->w = x - r->x;
						}
					}	
				}
			}
		}

		for(isize i = last_rects; i < rects_count; ++i) {
			Rect2i* r = rects + i;
			bool solid = true;
			isize y = r->y;
			while(solid && (y < tilemap->h)) {
				for(isize local_x = 0; local_x < r->w; ++local_x) {
					solid = solid && _get_at(r->x + local_x, y + 1);
					if(!solid) break;
				}
				if(solid) {
					y++;
					r->h++;
				}
			}
		}

		for(isize i = 0; i < rects_count; ++i) {
			Rect2i* r = rects + i;
			for(isize local_y = 0; local_y < r->h; ++local_y) {
				for(isize local_x = 0; local_x < r->w; ++local_x) {
					isize index = (local_y + r->y) * tilemap->w + (local_x + r->x);
					//printf("%d ", index);
					tiles[index] = false;
				}
			}
		}
		work = 0;
		for(isize i = 0; i < map_size; ++i) {
			work += tiles[i];
		}
	} while(work);
	
	for(isize i = 0; i < rects_count; ++i) {
		Rect2i* r = rects + i;
		Sim_Body* e = sim_get_next_body(sim);
		e->shape.center.x = (r->x + r->w / 2.0f) * Tile_Size;//+ Half_TS;
		e->shape.center.y = (r->y + r->h / 2.0f) * Tile_Size;// + Half_TS;
		e->shape.hw = r->w * Half_TS;
		e->shape.hh = r->h * Half_TS;
		e->restitution = 0.3f;
		e->inv_mass = 0.0f;
		e->flags = Body_Flag_Static;
	}
	end_temp_arena(Game->temp_arena);
}

