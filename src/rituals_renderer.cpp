
/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 * rituals_renderer.cpp
 *
 * This file is dedicated to the d7samurai, for without his help,
 * none of this would work.
 */

enum Sprite_Anchor
{
	Anchor_Center = 0,
	Anchor_Top_Left = 1,
	Anchor_Top = 2,
	Anchor_Top_Right = 3,
	Anchor_Right = 4,
	Anchor_Bottom_Right = 5,
	Anchor_Bottom = 6 ,
	Anchor_Bottom_Left = 7,
	Anchor_Left = 8
};

uint32 Sprite_Anchor_Mask = 0xF;

enum Sprite_Flags
{
	SpriteFlag_FlipHoriz = Flag(4),
	SpriteFlag_FlipVert = Flag(5)
};

real SpriteAnchorX[] = {
	0.0f,
	-0.5f,
	0.0f,
	0.5f, 
	0.5f,
	0.5f, 
	0.0f, 
	-0.5f,
	-0.5f
};

real SpriteAnchorY[] = {
	0.0f,
	-0.5f,
	-0.5f,
	-0.5f,
	0.0f,
	0.5f,
	0.5f,
	0.5f,
	0.0f
};

struct Sprite
{
	Vec2 position;
	Vec2 center;
	real angle;
	Vec2 size;
	Rect2 texture;
	Vec4 color;
	uint32 flags;
	real sort_offset;
};

struct Sprite4
{
	Sprite e[4];
};
#define _get_sprite_y_base(s) (s.position.y - s.center.y + s.sort_offset)
GenerateIntrosortForType(sort_sprites_on_y_base, Sprite, 12, _get_sprite_y_base)

void init_sprite(Sprite* s)
{
	s->position = v2(0, 0);
	s->center = v2(0, 0);
	s->angle = 0;
	s->size = v2(32, 32);
	s->texture = rect2(0, 0, 1, 1);
	s->color = v4(1, 1, 1, 1);
	s->flags = Anchor_Center;
	s->sort_offset = 0;
}

struct Render_Group
{
	GLuint texture;
	Vec2i texture_size;
	Vec2 offset;
	Rect2 clip;
	real ortho[16];

	real night_amount;
	real night_cutoff;

	Sprite* sprites;
	isize sprites_count, sprites_capacity;
};

struct OpenGL_Renderer
{
	GLuint shader_program, vbo, vao;
	isize u_texturesize, u_orthomat, u_night_amount, u_night_cutoff;
	
	Render_Group* groups;
	isize groups_count;
};

void init_group(Render_Group* group, isize sprites_capacity, Memory_Arena* arena)
{
	group->offset = v2(0, 0);
	group->clip = rect2(0, 0, 0, 0);
	group->night_amount = 0;
	group->night_cutoff = 0;

	group->sprites_capacity = sprites_capacity;
	group->sprites_count = 0;
	group->sprites = arena_push_array(arena, Sprite, sprites_capacity);
}

#define _get_member_address(s, m) ((void*)&(((s*)(NULL))->m))
#define _gl_offset(name) ((GLvoid*)(_get_member_address(Sprite, name)))
void init_renderer(OpenGL_Renderer* r, isize group_count, isize group_size, char* vertex_source, char* frag_source, Memory_Arena* arena)
{
	r->groups = arena_push_array(arena, Render_Group, group_count);
	r->groups_count = group_count;
	for(isize i = 0; i < group_count; ++i) {
		init_group(r->groups + i, group_size / sizeof(Sprite), arena);
	}

	glGenVertexArrays(1, &r->vao);
	glBindVertexArray(r->vao);
	glGenBuffers(1, &r->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, r->vbo);

	usize stride = sizeof(Sprite);
	usize vertex_count = 1;

	//position
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, _gl_offset(position));
	glEnableVertexAttribArray(0);  
	glVertexAttribDivisor(0, vertex_count);

	//center
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, _gl_offset(center));
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, vertex_count);

	//angle
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, stride, _gl_offset(angle));
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, vertex_count);

	//size
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, _gl_offset(size));
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, vertex_count);

	//texcoords
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, stride, _gl_offset(texture));
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, vertex_count);

	//color
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, stride, _gl_offset(color));
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(5, vertex_count);

	//anchor
	glVertexAttribIPointer(6, 1, GL_UNSIGNED_INT, stride, _gl_offset(flags));
	glEnableVertexAttribArray(6);
	glVertexAttribDivisor(6, vertex_count);

	glBindVertexArray(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//TODO(will) consider adding depth information? 


	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_source, NULL);
	glCompileShader(vertex_shader);
	{
		GLint success;
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
		GLsizei log_size;
		char shader_log[4096];
		glGetShaderInfoLog(vertex_shader, 4096, &log_size, shader_log); 
		if(!success) {
			printf("Error compiling vertex shader \n%s \n", shader_log);
		} else {
			printf("Vertex shader compiled successfully\n");
		}
	}	

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &frag_source, NULL);
	glCompileShader(fragment_shader);
	{
		GLint success;
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
		GLsizei log_size;
		char shader_log[4096];
		glGetShaderInfoLog(fragment_shader, 4096, &log_size, shader_log); 
		if(!success) {
			printf("Error compiling frag shader \n%s \n", shader_log);
		} else {
			printf("Frag shader compiled successfully\n");
		}
	}	
	r->shader_program = glCreateProgram();
	glAttachShader(r->shader_program, vertex_shader);
	glAttachShader(r->shader_program, fragment_shader);
	glLinkProgram(r->shader_program);
	glUseProgram(r->shader_program);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	r->u_texturesize = glGetUniformLocation(r->shader_program, "u_texturesize");
	r->u_orthomat = glGetUniformLocation(r->shader_program, "u_orthomat");
	r->u_night_amount = glGetUniformLocation(r->shader_program, "u_night_amount");
	r->u_night_cutoff = glGetUniformLocation(r->shader_program, "u_night_cutoff");
}

Render_Group* render_group(OpenGL_Renderer* r, isize index)
{
	return r->groups + index;
}

/* Explicit state functions */

void render_start(Render_Group* group)
{
	group->sprites_count = 0;
	group->clip = {0, 0, 0, 0};
}

bool render_has_clip_rect(Render_Group* group)
{
	return 0 != (group->clip.w * group->clip.h);
}

void render_set_clip_rect(Render_Group* group, real x, real y, real w, real h)
{
	group->clip = Rect2 {
		x, y, w, h
	};
}

void render_sort(Render_Group* group, isize offset)
{
	sort_sprites_on_y_base(group->sprites + offset, group->sprites_count - offset);
}

void render_add(Render_Group* group, Sprite* sprite)
{
	Sprite sp = *sprite;

	if(render_has_clip_rect(group)) {
		Rect2 c = group->clip;
		Rect2 r;
		r.position = sp.position;
		r.size = sp.size;
		r.x -= r.w * (0.5f + SpriteAnchorX[sp.flags & Sprite_Anchor_Mask]);
		r.y -= r.h * (0.5f + SpriteAnchorY[sp.flags & Sprite_Anchor_Mask]);

		if(r.x > (c.x + c.w)) return;
		if((r.x + r.w) < c.x) return;
		if(r.y > (c.y + c.h)) return;
		if((r.y + r.h) < c.y) return;

		Rect2_Clip_Info clip = rect2_clip(r, c);

		Vec2 tex_scale = v2(sp.texture.w / sp.size.x, sp.texture.h / sp.size.y);
		Vec2 tdp1 = clip.diff1 * tex_scale;
		Vec2 tdp2 = clip.diff2 * tex_scale;
		Vec2 tp1 = v2(sp.texture.x, sp.texture.y);
		Vec2 tp2 = tp1 + v2(sp.texture.w, sp.texture.h);
		tp1 += tdp1;
		tp2 -= tdp2;
		sp.texture.position = tp1;
		sp.texture.size = tp2 - tp1;
		sp.position = clip.rp1;
		sp.size = clip.rp2 - clip.rp1;
		sp.flags = (~Sprite_Anchor_Mask & sp.flags) | Anchor_Top_Left;
		sp.angle = 0;
	}
	
	sp.texture.x /= group->texture_size.x;
	sp.texture.w /= group->texture_size.x;
	sp.texture.y /= group->texture_size.y;
	sp.texture.h /= group->texture_size.y;

	group->sprites[group->sprites_count++] = sp;

}

void render_add_unclipped(Render_Group* group, Sprite* sprite)
{
	Sprite sp = *sprite;

	sp.texture.x /= group->texture_size.x;
	sp.texture.w /= group->texture_size.x;
	sp.texture.y /= group->texture_size.y;
	sp.texture.h /= group->texture_size.y;

	group->sprites[group->sprites_count++] = sp;
}

void render_add(Render_Group* group, Sprite4* s4)
{
	render_add(group, s4->e + 0);
	render_add(group, s4->e + 1);
	render_add(group, s4->e + 2);
	render_add(group, s4->e + 3);
}

void render_calculate_ortho_matrix(real* ortho, Vec4 screen, real nearplane, real farplane)
{
	ortho[0] = 2.0f / (screen.z - screen.x);
	ortho[1] = 0;
	ortho[2] = 0;
	ortho[3] = -1.0f * (screen.x + screen.z) / (screen.z - screen.x);

	ortho[4] = 0;
	ortho[5] = 2.0f / (screen.y - screen.w);
	ortho[6] = 0;
	ortho[7] = -1 * (screen.y + screen.w) / (screen.y - screen.w);

	ortho[8] = 0;
	ortho[9] = 0;
	ortho[10] = (-2.0f / (farplane - nearplane));
	ortho[11] = (-1.0f * (farplane + nearplane) / (farplane - nearplane));

	ortho[12] = 0;
	ortho[13] = 0;
	ortho[14] = 0;
	ortho[15] = 1.0f;
}

void render_draw(OpenGL_Renderer* r, Render_Group* group, Vec2 size, real scale)
{
	glUseProgram(r->shader_program);
	group->offset.x = roundf(group->offset.x);
	group->offset.y = roundf(group->offset.y);

	glUniform2f(r->u_texturesize,
		group->texture_size.x,
		group->texture_size.y);

	glUniform1f(r->u_night_amount, group->night_amount);
	glUniform1f(r->u_night_cutoff, group->night_cutoff);

	Vec4 screen = v4(
		group->offset.x, group->offset.y, 
		size.x + group->offset.x,
		size.y + group->offset.y);
	render_calculate_ortho_matrix(group->ortho, screen, 1, -1);
	glUniformMatrix4fv(r->u_orthomat, 
		1, 
		GL_FALSE,
		group->ortho);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, group->texture);
	glBindVertexArray(r->vao);
	glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
	glBufferData(GL_ARRAY_BUFFER, 
			group->sprites_count * sizeof(Sprite), 
			group->sprites, 
			GL_STREAM_DRAW);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, group->sprites_count);

	glBindVertexArray(0);
}

/* Implicit state functions */

void render_start()
{
	render_start(CurrentGroup);
}

bool render_has_clip_rect()
{
	return render_has_clip_rect(CurrentGroup);
}

void render_set_clip_rect(real x, real y, real w, real h)
{
	render_set_clip_rect(CurrentGroup, x, y, w, h);
}

void render_sort(isize offset)
{
	render_sort(CurrentGroup, offset);
}

void render_add(Sprite* sprite)
{
	render_add(CurrentGroup, sprite);
}

void render_add_unclipped(Sprite* sprite)
{
	render_add_unclipped(CurrentGroup, sprite);
}

void render_add(Sprite4* s4)
{
	render_add(CurrentGroup, s4);
}

void render_draw(Vec2 size, real scale)
{
	render_draw(Renderer, CurrentGroup, size, scale);
}


/* Texture loading from file and uploading */

GLuint ogl_add_texture(uint8* data, isize w, isize h) 
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	uint32 error = glGetError();
	if(error != 0) {
		printf("There was an error adding a texture: %d \n", error);
	}
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

GLuint ogl_load_texture(char* filename, isize* w_o, isize* h_o)
{
	int w, h, n;
	char file[FilePathMaxLength];
	const char* base_path = SDL_GetBasePath();
	isize len = snprintf(file, FilePathMaxLength, "%s%s", base_path, filename);
	uint8* data = (uint8*)stbi_load(file, &w, &h, &n, STBI_rgb_alpha);
	//TODO(will) do error checking
	GLuint texture = ogl_add_texture(data, w, h);
	if(texture == NULL) {
		printf("There was an error loading %s \n", filename);
	}
	if(w_o != NULL) *w_o = w;
	if(h_o != NULL) *h_o = h;

	STBI_FREE(data);
	return texture;
}
	
/* Primitive generation/rendering functions */

Sprite create_box_primitive(Vec2 pos, Vec2 size, Vec4 color)
{
	Sprite s;
	init_sprite(&s);
	s.position = pos;
	s.texture = rect2(64, 0, 32, 32);
	s.size = size;
	s.color = color;
	return s;
}

void render_box_primitive(Render_Group* group, Vec2 pos, Vec2 size, Vec4 color)
{
	Sprite s = create_box_primitive(pos, size, color);
	render_add(group, &s);
}

Sprite create_line_primitive(Vec2 start, Vec2 end, Vec4 color, int32 thickness)
{
	Vec2 dline = end - start;
	Sprite s;
	if(dline.y == 0) {
		if(dline.x < 0) {
			dline.x *= -1;
			Vec2 temp = end;
			end = start;
			start = temp;
		}
		s = create_box_primitive(start + v2(dline.x / 2, 0), v2(dline.x, thickness), color);
	} else if(dline.x == 0) {
		if(dline.y < 0) {
			dline.y *= -1;
			Vec2 temp = end;
			end = start;
			start = temp;
		}
		s = create_box_primitive(start + v2(0, dline.y / 2), v2(thickness, dline.y), color);
	} else {
		s = create_box_primitive(start + dline/2, v2(sqrtf(v2_dot(dline, dline)), thickness), color);
		real angle = atan2f(dline.y, dline.x);
		s.angle = -angle;
	}
	return s;
}
void render_line_primitive(Render_Group* group, Vec2 start, Vec2 end, Vec4 color, int32 thickness)
{
	Sprite s = create_line_primitive(start, end, color, thickness);
	render_add(group, &s);
}

Sprite4 create_box_outline_primitive(Vec2 center, Vec2 size, Vec4 color, int32 thickness)
{
	size *= 0.5f;	
	Vec2 tl = center - size;
	Vec2 br = center + size;
	Sprite4 s;
	s.e[0] = create_line_primitive(tl, v2(br.x, tl.y), color, thickness);
	s.e[1] = create_line_primitive(v2(br.x, tl.y), br, color, thickness);
	s.e[2] = create_line_primitive(br, v2(tl.x, br.y), color, thickness);
	s.e[3] = create_line_primitive(v2(tl.x, br.y), tl, color, thickness);
	return s;
}

void render_box_outline_primitive(Render_Group* group, Vec2 center, Vec2 size, Vec4 color, int32 thickness)
{
	Sprite4 s = create_box_outline_primitive(center, size, color, thickness);
	render_add(group, &s);
}

Sprite4 create_box_outline_primitive(Vec2 center, Vec2 size, Vec4 color[4], int32 thickness)
{
	size *= 0.5f;	
	Vec2 tl = center - size;
	Vec2 br = center + size;
	Sprite4 s;
	s.e[0] = create_line_primitive(tl, v2(br.x, tl.y), color[0], thickness);
	s.e[1] = create_line_primitive(v2(br.x, tl.y), br, color[1], thickness);
	s.e[2] = create_line_primitive(br, v2(tl.x, br.y), color[2], thickness);
	s.e[3] = create_line_primitive(v2(tl.x, br.y), tl, color[3], thickness);
	return s;
}

void render_box_outline_primitive(Render_Group* group, Vec2 center, Vec2 size, Vec4 color[4], int32 thickness)
{
	Sprite4 s = create_box_outline_primitive(center, size, color, thickness);
	render_add(group, &s);
}

void render_box_primitive(Vec2 pos, Vec2 size, Vec4 color)
{
	render_box_primitive(CurrentGroup, pos, size, color);
}

void render_line_primitive(Vec2 start, Vec2 end, Vec4 color, int32 thickness)
{
	render_line_primitive(CurrentGroup, start, end, color, thickness);
}

void render_box_outline_primitive(Vec2 center, Vec2 size, Vec4 color, int32 thickness)
{
	render_box_outline_primitive(CurrentGroup, center, size, color, thickness);
}

void render_box_outline_primitive(Vec2 center, Vec2 size, Vec4 color[4], int32 thickness)
{
	render_box_outline_primitive(CurrentGroup, center, size, color, thickness);
}

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
};

void init_particle(Particle* p, Vec3 pos, Vec3 vel, real scale, real angle, real anglev, int32 frame, int32 time)
{
	p->position = pos;
	p->velocity = vel;
	p->scale = scale;
	p->angle = angle;
	p->angular_vel = anglev;
	p->frame = frame;
	p->time = time;
	p->total_time = time;
}

struct Emitter
{
	Particle* particles;
	isize particles_count, particles_capacity;

	Rect2 texture;
	Vec2 particle_size;
	Vec4 color;

	Vec3 acceleration;

	int32 min_time;
	int32 max_time;
};

void init_emitter(Emitter* e, isize max_particles, Rect2 texture, Vec4 color, Vec2 size, int32 min_time, int32 max_time, Memory_Arena* arena)
{
	e->particles_count = 0; 
	e->particles_capacity = max_particles;
	e->particles = arena_push_array(arena, Particle, max_particles);
	e->texture = texture;
	e->particle_size = size;
	e->color = color;

	e->acceleration = v3(0, 0, -400);
	e->min_time = min_time;
	e->max_time = max_time;
}

void emitter_spawn(Emitter* e, Vec2 pos, real height_off_ground, isize count, Vec2 impulse_mag_range, Vec2 scale_range, Vec2 impulse_angle_range)
{
	for(isize i = 0; i < count; ++i) {
		isize next_particle = e->particles_count++;
		next_particle %= e->particles_capacity;
		Particle* p = e->particles + next_particle;
		real mag = rand_range(&Game->r, impulse_mag_range.x, impulse_mag_range.y);
		Vec2 impulse = v2_from_angle(rand_range(&Game->r, impulse_angle_range.x, impulse_angle_range.y)) * mag;
		init_particle(p, v3(pos, height_off_ground), 
				v3(impulse, rand_range(&Game->r, -height_off_ground/4, height_off_ground/4)), 
				rand_range(&Game->r, scale_range.x, scale_range.y), 
				0.0f,
				10 * ((mag - impulse_mag_range.x) / (impulse_mag_range.y - impulse_mag_range.x)) - 0.5f * Math_Tau,
				0, 
				rand_range_int(&Game->r, e->min_time, e->max_time));
	}
}

void emitter_render(Emitter* e, real dt)
{
	isize count = e->particles_count;
	if(count > e->particles_capacity) {
		count = e->particles_capacity;
	}
	for(isize i = 0; i < count; ++i) {
		Particle* p = e->particles + i;
		if(p->time <= 0) continue;
		real tscale = (real)p->time / (real)p->total_time;
		tscale /= 2;
		tscale += 0.5f;
		p->time--;
		Sprite s;
		init_sprite(&s);
		Vec3 prev_vel = p->velocity;
		p->velocity += e->acceleration * dt;
		p->position += (p->velocity + prev_vel) * 0.5f * dt;
		p->velocity *= 0.99f;
		if(p->position.z < 0) {
			p->position.z = 0;
			//the .88 is ground restitution
			p->velocity.z *= -1 * 0.5f;
			p->velocity += 50 * v3(
					rand_range(&Game->r, -1, 1),
					rand_range(&Game->r, -1, 1), 0);
			p->velocity.x *= rand_range(&Game->r, 0.4f, 0.6f); //skid
			p->velocity.y *= rand_range(&Game->r, 0.4f, 0.6f);//skid
		}
		p->angle += p->angular_vel * dt;
		s.position = v2(p->position.x, p->position.y - p->position.z);
		s.sort_offset = p->position.z;
		s.angle = p->angle;
		s.size = e->particle_size * p->scale * tscale;
		s.texture = e->texture;
		s.texture.x += s.texture.w * p->frame;
		s.color = e->color;
		render_add(&s);
		s.position = v2(p->position.x, p->position.y);
		s.color = v4(0, 0, 0, 0.3f);
		s.sort_offset = -1;
		render_add(&s);
	}
}

