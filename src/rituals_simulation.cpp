/*
 * rituals_simulation.cpp
 */

#define Flag(x) (1 << x)
#define Has_Flag(x, y) (x & y)

enum Sim_Body_Flags
{
	Body_Flag_None,
	Body_Flag_Static = Flag(1),
	Body_Flag_Custom_Size = Flag(2)
};
struct Sim_Body
{
	isize id;
	AABB shape;
	Vec2 velocity, force;
	real inv_mass;
	uint64 flags;
};

#define _body_get_min_x(e) (e.shape.center.x - e.shape.hw)
#define _body_get_min_y(e) (e.shape.center.y - e.shape.hh)
Generate_Insertion_Sort_For_Type(body_sort_on_x, Sim_Body, _body_get_min_x)
Generate_Insertion_Sort_For_Type(body_sort_on_y, Sim_Body, _body_get_min_y)
//Generate_Quicksort_For_Type(body_sort_on_x, Sim_Body*, _body_get_min_x)
//Generate_Quicksort_For_Type(body_sort_on_y, Sim_Body*, _body_get_min_y)

#define _body_get_not_static(e) (!Has_Flag(e.flags, Body_Flag_Static))
Generate_Quicksort_For_Type(body_sort_static_first, Sim_Body, _body_get_not_static)

#define _body_get_id(e) (e.id)
Generate_Quicksort_For_Type(body_sort_on_id, Sim_Body, _body_get_id)
Generate_Binary_Search_For_Type(body_search_for_id, Sim_Body, isize, _body_get_id)

void init_body(Sim_Body* b)
{
	b->shape = aabb(v2(0, 0), 0, 0);
	b->inv_mass = 1.0f;
	b->velocity = v2(0,0);
	b->force = v2(0, 0);
	b->flags = Body_Flag_None;
}

struct Simulator
{
	Sim_Body* bodies;
	isize bodies_count, bodies_capacity, next_body_id;
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
	sim->bodies = Arena_Push_Array(arena, Sim_Body, cap);
}

Sim_Body* sim_find_body(Simulator* sim, isize id)
{
	isize index = body_search_for_id(id, sim->bodies, sim->bodies_count);
	return index == -1? NULL: sim->bodies + index;
}

void sim_update(Simulator* sim, real dt)
{
	Sim_Body *a, *b;
	real damping = powf(0.5, 8);
	for(isize times = 0; times < 4; ++times) {
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
					// TODO(will) fix velocities for real momentum collision
					// resolution....
					
					Vec2 overlap;
					aabb_overlap(&a->shape, &b->shape, &overlap);

					if(a_is_static && !b_is_static) {
						b->shape.center += overlap;
					} else if(!a_is_static && b_is_static) {
						a->shape.center -= overlap;
					} else {
						overlap *= 0.5f;
						a->shape.center -= overlap;
						b->shape.center += overlap;
					}

				}
			}
		}

		for(isize i = 0; i < 2; ++i) {
			//TODO(will) check if it's (sum2 - sum1 ** 2) / count or sum2 - (sum1 ** 2) / count ?
			//seems to be correct
			variance.e[i] = center_sum2.e[i] - center_sum1.e[i] * center_sum1.e[i] / sim->bodies_count;
		}
		
		if(variance.x > variance.y) {
			sim->sort_axis = 0;
		} else {
			sim->sort_axis = 1;
		}

		for(isize i = 0; i < sim->bodies_count; ++i) {
			a = sim->bodies + i;
			if(Has_Flag(a->flags, Body_Flag_Static)) continue;
			//TODO(will) sort bodies by static, skip block of statics
			Vec2 new_vel = a->velocity + (dt * a->force);
			Vec2 dpos = (a->velocity + new_vel) * 0.5f;
			a->shape.center += dpos * dt;
			a->velocity = new_vel;
			a->velocity *= damping;
		}

	}
	body_sort_on_id(sim->bodies, sim->bodies_count);
}
