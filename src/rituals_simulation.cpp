/*
 * rituals_simulation.cpp
 */

struct Entity
{
	usize id;
	AABB body;
	Vec2 velocity, force;
	real inv_mass;
	Sprite sprite;
};

#define _entity_get_min_x(e) (e->body.center.x - e->body.hw)
Generate_Quicksort_For_Type(entity_sort_on_x, Entity*, _entity_get_min_x)
#define _entity_get_min_y(e) (e->body.center.y - e->body.hh)
Generate_Quicksort_For_Type(entity_sort_on_y, Entity*, _entity_get_min_y)


void init_entity(Entity* e)
{
	init_sprite(&e->sprite);
	e->body = aabb(v2(0, 0), 0, 0);
	e->inv_mass = 1.0f;
	e->velocity = v2(0, 0);
	e->force = v2(0, 0);
}

struct Simulator
{
	Entity* entities;
	Entity** sorted_entities;
	isize entities_count, entities_capacity;
	isize sort_axis;
};

void init_simulator(Simulator* sim, isize cap, Memory_Arena* arena)
{
	sim->entities_count = 0;
	sim->entities_capacity = cap;
	sim->sort_axis = 0;

	sim->entities = Arena_Push_Array(arena, Entity, cap);
	sim->sorted_entities = Arena_Push_Array(arena, Entity*, cap);
}

void sim_refresh_sorted(Simulator* sim)
{
	for(isize i = 0; i < sim->entities_count; ++i) {
		sim->sorted_entities[i] = sim->entities + i;
	}
}

void sim_update(Simulator* sim, real dt)
{
	Entity *a, *b;
	real damping = powf(0.5, 8);
	for(isize times = 0; times < 8; ++times) {
		if(sim->sort_axis == 0) {
			entity_sort_on_x(sim->sorted_entities, sim->entities_count);
		} else if(sim->sort_axis == 1) {
			entity_sort_on_y(sim->sorted_entities, sim->entities_count);
		}

		Vec2 center_sum1 = v2(0, 0);
		Vec2 center_sum2 = v2(0, 0);
		Vec2 variance = v2(0, 0);
		for(isize i = 0; i < sim->entities_count; ++i) {
			a = sim->sorted_entities[i];

			//sweep and prune stuff
			center_sum1 += a->body.center;
			for(isize q = 0; q < 2; ++q) {
				center_sum2.e[q] += a->body.center.e[q] * a->body.center.e[q];
			}

			for(isize j = i + 1; j < sim->entities_count; ++j) {
				b = sim->sorted_entities[j];

				if(sim->sort_axis == 0) {
					if(_entity_get_min_x(b) > (a->body.center.x + a->body.hw)) {
						break;
					}
				} else if(sim->sort_axis == 1) {
					if(_entity_get_min_y(b) > (a->body.center.y + a->body.hh)) {
						break;
					}
				}
			
			
				if(aabb_intersect(&a->body, &b->body)) {
					// TODO(will) fix velocities for real momentum collision
					// resolution....
					
					Vec2 overlap;
					aabb_overlap(&a->body, &b->body, &overlap);
					overlap *= 0.5f;
					a->body.center -= overlap;
					b->body.center += overlap;
				}
			}
		}

		for(isize i = 0; i < 2; ++i) {
			//TODO(will) check if it's (sum2 - sum1 ** 2) / count or sum2 - (sum1 ** 2) / count ?
			variance.e[i] = center_sum2.e[i] - center_sum1.e[i] * center_sum1.e[i] / sim->entities_count;
		}
		
		if(variance.x > variance.y) {
			sim->sort_axis = 0;
		} else {
			sim->sort_axis = 1;
		}

		for(isize i = 0; i < sim->entities_count; ++i) {
			a = sim->entities + i;
			Vec2 new_vel = a->velocity + (dt * a->force);
			new_vel *= damping;
			Vec2 new_pos = a->body.center + (dt * new_vel);
			a->body.center = new_pos;
		}

	}
	
}
