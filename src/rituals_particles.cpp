struct Particle
{
	Vec3 position;
	Vec3 velocity;
	f32 scale;
	f32 angle;
	f32 angular_vel;
	i32 frame;
	i32 time;
	i32 total_time;
	i32 style_index;
};

struct Particle_Style
{
	i32 id;
	u32 color;
	u32 shadow_color;
	Rect2i texture;
	Vec2 size;

	Vec3 acceleration;

	f32 impulse_min;
	f32 impulse_max;
	
	f32 angle_min;
	f32 angle_max;

	f32 angle_vel_min;
	f32 angle_vel_max;

	f32 scale_min;
	f32 scale_max;

	i32 frame_min;
	i32 frame_max;
	i32 max_frames;
	i32 framerate;

	i32 time_min;
	i32 time_max;

	bool time_scaling;
	bool time_alpha;
	f32 ground_restitution;
	f32 skid_on_bounce_min;
	f32 skid_on_bounce_max;
	f32 jitter_on_bounce_mag;
};

Particle_Style copy_particle_style(Particle_Style s,
		Vec2 impulse_range, Vec2i time_range)
		 
{
	s.impulse_min = impulse_range.x;
	s.impulse_max = impulse_range.y;
	s.time_min = time_range.x;
	s.time_max = time_range.y;
	return s;
}

//TODO(will) Add particle styles to the registry
Particle_Style make_particle_style(
		Rect2i texture, 
		Vec2 size, 
		u32 color, 
		u32 shadow_color,
		Vec3 acl, 
		Vec2 impulse_range, 
		Vec2 angle_range, 
		Vec2 angle_vel_range, 
		Vec2 scale_range, 
		Vec2i frame_range, 
		i32 max_frames, 
		i32 framerate, 
		Vec2i time_range, 
		bool time_scaling,
		f32 ground_restitution,
		Vec2 skid_on_bounce_range,
		f32 jitter_on_bounce_mag)
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

void init_particle(Particle* p, Vec3 pos, Vec3 vel, f32 scale, f32 angle, f32 anglev, i32 frame, i32 time, i32 style_index)
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

void init_emitter(Emitter* e, isize max_particles, MemoryArena* arena)
{
	e->particles_count = 0; 
	e->styles_capacity = EmitterStyleCapacity;
	e->styles = (Particle_Style*)arenaPush(arena,
			sizeof(Particle_Style) *  EmitterStyleCapacity);
	e->particles_capacity = max_particles;
	e->particles = (Particle*)arenaPush(arena, sizeof(Particle) * max_particles);
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
		f32 mag = rand_range(style.impulse_min, style.impulse_max);
		Vec2 impulse = v2_from_angle(rand_range(angle_range.x, angle_range.y)) * mag;
		f32 quarter_h = pos.z / 4;
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

void emitter_render(Emitter* e, Simulator* sim, f32 dt)
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
		f32 tscale = (f32)p->time / (f32)p->total_time;
		tscale /= 2;
		tscale += 0.5f;
		p->time--;

		Sprite s;
		wInitSprite(&s);
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
		s.pos = v2(p->position.x, p->position.y - p->position.z);
		s.sort_offset = p->position.z;
		s.angle = p->angle;
		s.size = style.size * p->scale * tscale;
		s.texture = style.texture;
		s.texture.x += s.texture.w * p->frame;
		s.color = style.color;
		if(style.time_alpha) {
			f32 a = (f32)(s.color & 0xFF) / 255.0;
			a *= tscale;
			s.color |= (u8)(a * 255.0);
			//s.color. *= tscale;
		}
		render_add(&s);
		s.pos= v2(p->position.x, p->position.y);
		s.color = style.shadow_color;
		if(style.time_alpha) {
			f32 a = (f32)(s.color & 0xFF) / 255.0;
			a *= tscale;
			s.color |= (u8)(a * 255.0);
		}
		s.sort_offset = -1;
		render_add(&s);
	}
}

