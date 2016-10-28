
/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/



#ifndef REFLECTED
struct Particle
{
	Vec3 position;
	Vec3 velocity;
	real scale;
	real angle;
	real angular_vel;
	int32 frame;
	int32 time;
	int32 total_time;
	int32 style_index;
};

struct Particle_Style
{
	int32 id;
	Rect2 texture;
	Vec2 size;
	Vec4 color;
	Vec4 shadow_color;

	Vec3 acceleration;

	real impulse_min;
	real impulse_max;
	
	real angle_min;
	real angle_max;

	real angle_vel_min;
	real angle_vel_max;

	real scale_min;
	real scale_max;

	int32 frame_min;
	int32 frame_max;
	int32 max_frames;
	int32 framerate;

	int32 time_min;
	int32 time_max;

	bool time_scaling;
	bool time_alpha;
	real ground_restitution;
	real skid_on_bounce_min;
	real skid_on_bounce_max;
	real jitter_on_bounce_mag;
};
#endif

Particle_Style copy_particle_style(Particle_Style s,
		Vec2 impulse_range, Vec2i time_range)
		 
{
	s.impulse_min = impulse_range.x;
	s.impulse_max = impulse_range.y;
	s.time_min = time_range.x;
	s.time_max = time_range.y;
	return s;
}

Particle_Style make_particle_style(
		Rect2 texture, 
		Vec2 size, 
		Vec4 color, 
		Vec4 shadow_color,
		Vec3 acl, 
		Vec2 impulse_range, 
		Vec2 angle_range, 
		Vec2 angle_vel_range, 
		Vec2 scale_range, 
		Vec2i frame_range, 
		int32 max_frames, 
		int32 framerate, 
		Vec2i time_range, 
		bool time_scaling,
		real ground_restitution,
		Vec2 skid_on_bounce_range,
		real jitter_on_bounce_mag)
{
	Particle_Style s;
	s.texture = texture;
	s.size = size;
	s.color = color;
	s.shadow_color = shadow_color;
	s.acceleration = acl;
	s.impulse_min = impulse_range.x;
	s.impulse_max = impulse_range.y;
	s.angle_min = angle_range.x;
	s.angle_max = angle_range.y;
	s.angle_vel_min = angle_vel_range.x;
	s.angle_vel_max = angle_vel_range.y;
	s.scale_min = scale_range.x;
	s.scale_max = scale_range.y;
	s.frame_min = frame_range.x;
	s.frame_max = frame_range.y;
	s.max_frames = max_frames;
	s.framerate = framerate;
	s.time_min = time_range.x;
	s.time_max = time_range.y;
	s.time_scaling = time_scaling;
	s.ground_restitution = ground_restitution;
	s.skid_on_bounce_min = skid_on_bounce_range.x;
	s.skid_on_bounce_max = skid_on_bounce_range.y;
	s.jitter_on_bounce_mag = jitter_on_bounce_mag;
	return s;
}

void init_particle(Particle* p, Vec3 pos, Vec3 vel, real scale, real angle, real anglev, int32 frame, int32 time, int32 style_index)
{
	p->position = pos;
	p->velocity = vel;
	p->scale = scale;
	p->angle = angle;
	p->angular_vel = anglev;
	p->frame = frame;
	p->time = time;
	p->total_time = time;
	p->style_index = style_index;
}

#define EmitterStyleCapacity (256)
#ifndef REFLECTED
struct Emitter
{
	Particle* particles;
	isize particles_count, particles_capacity;

	Particle_Style* styles;
	isize styles_count, styles_capacity;
};
#endif

void init_emitter(Emitter* e, isize max_particles, Memory_Arena* arena)
{
	e->particles_count = 0; 
	e->styles_capacity = EmitterStyleCapacity;
	e->styles = arena_push_array(arena, Particle_Style, EmitterStyleCapacity);
	e->particles_capacity = max_particles;
	e->particles = arena_push_array(arena, Particle, max_particles);
}

void emitter_spawn(Emitter* e, Vec3 pos, Vec2 angle_range, isize count, Particle_Style style) 
{
	//this is mostly just hacks until we bother serializing it
	if(e == NULL || e->particles == NULL) return;

	isize next_style = e->styles_count++;
	next_style %= e->styles_capacity;
	style.id = next_style;
	e->styles[next_style] = style;

	for(isize i = 0; i < count; ++i) {
		isize next_particle = e->particles_count++;
		next_particle %= e->particles_capacity;
		Particle* p = e->particles + next_particle;
		real mag = rand_range(style.impulse_min, style.impulse_max);
		Vec2 impulse = v2_from_angle(rand_range(angle_range.x, angle_range.y)) * mag;
		real quarter_h = pos.z / 4;
		init_particle(p, pos,
				v3(impulse, rand_range(-quarter_h, quarter_h)), 
				rand_range(style.scale_min, style.scale_max), 
				rand_range(style.angle_min, style.angle_max),
				rand_range(style.angle_vel_min, style.angle_vel_max),
				rand_range_int(style.frame_min, style.frame_max), 
				rand_range_int(style.time_min, style.time_max),
				next_style);
	}
}

void emitter_render(Emitter* e, Simulator* sim, real dt)
{
	if(e == NULL || e->particles == NULL) return;

	isize count = e->particles_count;
	if(count > e->particles_capacity) {
		count = e->particles_capacity;
	}

	Particle_Style style = {0};
	style.id = -1;
	Sim_Static_Grid* grid = sim->grid;
	Vec2i gridsize = grid->size;
	for(isize i = 0; i < count; ++i) {
		Particle* p = e->particles + i;
		if(p->time <= 0) continue;
		if(style.id != p->style_index) {
			style = e->styles[p->style_index];
		}
		real tscale = (real)p->time / (real)p->total_time;
		tscale /= 2;
		tscale += 0.5f;
		p->time--;

		Sprite s;
		init_sprite(&s);
		Vec3 prev_vel = p->velocity;
		p->velocity += style.acceleration * dt;
		p->position += (p->velocity + prev_vel) * 0.5f * dt;
		p->velocity *= 0.99f;
		
		if(p->position.z < 0) {
			p->position.z = 0;
			p->velocity.z *= -1 * style.ground_restitution;
			p->velocity += style.jitter_on_bounce_mag * v3(
					rand_range(-1, 1),
					rand_range(-1, 1), 0);
			p->velocity.x *= rand_range(
					style.skid_on_bounce_min,
					style.skid_on_bounce_max);
			p->velocity.y *= rand_range(
					style.skid_on_bounce_min,
					style.skid_on_bounce_max);
		}

		isize gx,gy;
		gx = p->position.x / SimGridCellSide;
		gy = p->position.y / SimGridCellSide;
		if(gx < 0) gx = 0;
		else if(gx >= gridsize.x) gx = gridsize.x - 1;
		if(gy < 0) gy = 0;
		else if(gy >= gridsize.y) gy = gridsize.y - 1;
		Sim_Grid_Cell* c = grid->cells[gx + gy * gridsize.x];
		if(c != NULL) {
			do {
				Sim_Body* b = c->body;
				if(b->group == 1) continue;
				AABB shape = b->shape;
				AABB point = aabb(v2(p->position), 0, 0);
				if(aabb_intersect(shape, point)) {
					Vec2 overlap;
					aabb_overlap(shape, point, &overlap);
					Vec2 newpos = v2(p->position);
					newpos += overlap;

					if(overlap.y == 0) {
						p->velocity.x *= -1;
					} else {
						p->velocity.y *= -1;
					}

					p->velocity *= style.ground_restitution;

					p->position.x = newpos.x;
					p->position.y = newpos.y;
					
					
				}
			} while(c = c->next);
		}

		p->angle += p->angular_vel * dt;
		s.position = v2(p->position.x, p->position.y - p->position.z);
		s.sort_offset = p->position.z;
		s.angle = p->angle;
		s.size = style.size * p->scale * tscale;
		s.texture = style.texture;
		s.texture.x += s.texture.w * p->frame;
		s.color = style.color;
		if(style.time_alpha) {
			s.color.w *= tscale;
		}
		render_add(&s);
		s.position = v2(p->position.x, p->position.y);
		s.color = style.shadow_color;
		if(style.time_alpha) {
			s.color.w *= tscale;
		}
		s.sort_offset = -1;
		render_add(&s);
	}
}

