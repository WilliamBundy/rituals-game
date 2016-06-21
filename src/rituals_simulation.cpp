/*
 * rituals_simulation.cpp
 */

struct Entity
{
	isize id;
	AABB body;
	Vec2 velocity, force;
	real inv_mass;
	Sprite sprite;
	
	//rolled into flags
	bool is_static;
	bool use_custom_size;
};

#define _entity_get_min_x(e) (e->body.center.x - e->body.hw)
Generate_Quicksort_For_Type(entity_sort_on_x, Entity*, _entity_get_min_x)
#define _entity_get_min_y(e) (e->body.center.y - e->body.hh)
Generate_Quicksort_For_Type(entity_sort_on_y, Entity*, _entity_get_min_y)

#define _entity_get_not_static(e) (!e.is_static)
Generate_Quicksort_For_Type(entity_sort_static_first, Entity, _entity_get_not_static)

#define _entity_get_id(e) (e->id)
Generate_Quicksort_For_Type(entity_sort_on_id, Entity*, _entity_get_id)
Generate_Binary_Search_For_Type(entity_search_for_id, Entity*, isize, _entity_get_id)

void init_entity(Entity* e)
{
	init_sprite(&e->sprite);
	e->body = aabb(v2(0, 0), 0, 0);
	e->inv_mass = 1.0f;
	e->velocity = v2(0, 0);
	e->force = v2(0, 0);
	e->is_static = false;
}

struct Simulator
{
	Entity* entities;
	Entity** spacial_entities;
	Entity** id_ordered_entities;
	isize entities_count, entities_capacity, next_entity_id;
	isize sort_axis;
};

Entity* sim_get_next_entity(Simulator* sim)
{
	if(sim->entities_count + 1 > sim->entities_capacity) {
		Log_Error("Ran out of entities");
		return NULL;
	}

	Entity* e = sim->entities + sim->entities_count++;
	init_entity(e);
	e->id = sim->next_entity_id++;
	sim->spacial_entities[sim->entities_count - 1] = e;
	sim->id_ordered_entities[sim->entities_count - 1] = e;

	return e;
}

void init_simulator(Simulator* sim, isize cap, Memory_Arena* arena)
{
	sim->entities_count = 0;
	sim->entities_capacity = cap;
	sim->sort_axis = 0;
	sim->next_entity_id = 0;
	sim->entities = Arena_Push_Array(arena, Entity, cap);
	sim->spacial_entities = Arena_Push_Array(arena, Entity*, cap);
	sim->id_ordered_entities = Arena_Push_Array(arena, Entity*, cap);
}

void sim_sort_entities(Simulator* sim)
{
	entity_sort_static_first(sim->entities, sim->entities_count);
	for(isize i = 0; i < sim->entities_count; ++i) {
		sim->spacial_entities[i] = sim->entities + i;
		sim->id_ordered_entities[i] = sim->entities + i;
	}
	entity_sort_on_id(sim->id_ordered_entities, sim->entities_count);
}

Entity* sim_find_entity(Simulator* sim, isize id)
{
	isize index = entity_search_for_id(id, sim->id_ordered_entities, sim->entities_count);
	return index == -1? NULL: sim->id_ordered_entities[index];
}

void sim_update(Simulator* sim, real dt)
{
	Entity *a, *b;
	real damping = powf(0.5, 8);
	for(isize times = 0; times < 8; ++times) {
		if(sim->sort_axis == 0) {
			entity_sort_on_x(sim->spacial_entities, sim->entities_count);
		} else if(sim->sort_axis == 1) {
			entity_sort_on_y(sim->spacial_entities, sim->entities_count);
		}

		Vec2 center_sum1 = v2(0, 0);
		Vec2 center_sum2 = v2(0, 0);
		Vec2 variance = v2(0, 0);
		for(isize i = 0; i < sim->entities_count; ++i) {
			a = sim->spacial_entities[i];

			//sweep and prune stuff
			center_sum1 += a->body.center;
			for(isize q = 0; q < 2; ++q) {
				center_sum2.e[q] += a->body.center.e[q] * a->body.center.e[q];
			}

			//if(a->is_static) continue;

			for(isize j = i + 1; j < sim->entities_count; ++j) {
				b = sim->spacial_entities[j];

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

					if(a->is_static && b->is_static) continue;
					if(a->is_static && !b->is_static) {
						b->body.center += overlap;
					} else if(!a->is_static && b->is_static) {
						a->body.center -= overlap;
					} else {
						overlap *= 0.5f;
						a->body.center -= overlap;
						b->body.center += overlap;
					}

				}
			}
		}

		for(isize i = 0; i < 2; ++i) {
			//TODO(will) check if it's (sum2 - sum1 ** 2) / count or sum2 - (sum1 ** 2) / count ?
			//seems to be correct
			variance.e[i] = center_sum2.e[i] - center_sum1.e[i] * center_sum1.e[i] / sim->entities_count;
		}
		
		if(variance.x > variance.y) {
			sim->sort_axis = 0;
		} else {
			sim->sort_axis = 1;
		}

		for(isize i = 0; i < sim->entities_count; ++i) {
			a = sim->entities + i;
			if(a->is_static) continue;
			//TODO(will) sort entities by static, skip block of statics
			Vec2 new_vel = a->velocity + (dt * a->force);
			Vec2 dpos = (a->velocity + new_vel) * 0.5f;
			a->body.center += dpos * dt;
			a->velocity = new_vel;
			a->velocity *= damping;
		}

	}
	
}
