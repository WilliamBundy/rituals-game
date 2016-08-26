/* 
 * wtb_sprite_renderer.h
 * In the public domain, otherwise
 * Copyright 2016, William Bundy
 * Under the MIT and WTFPL licences.
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

struct Draw_List
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
	
	Draw_List* draw_lists;
	isize draw_lists_count;
};
extern OpenGL_Renderer* Renderer;

void init_draw_list(Draw_List* list, isize sprites_capacity, Memory_Arena* arena)
{
	list->offset = v2(0, 0);
	list->clip = rect2(0, 0, 0, 0);
	list->night_amount = 0;
	list->night_cutoff = 0;

	list->sprites_capacity = sprites_capacity;
	list->sprites_count = 0;
	list->sprites = arena_push_array(arena, Sprite, sprites_capacity);
}

#define _get_member_address(s, m) ((void*)&(((s*)(NULL))->m))
#define _gl_offset(name) ((GLvoid*)(_get_member_address(Sprite, name)))
void init_renderer(OpenGL_Renderer* r, isize list_count, isize list_size, char* vertex_source, char* frag_source, Memory_Arena* arena)
{
	r->draw_lists = arena_push_array(arena, Draw_List, list_count);
	r->draw_lists_count = list_count;
	for(isize i = 0; i < list_count; ++i) {
		init_draw_list(r->draw_lists + i, list_size / sizeof(Sprite), arena);
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

void render_draw_list_start(OpenGL_Renderer* r, Draw_List* list)
{
	list->sprites_count = 0;
	list->clip = {0, 0, 0, 0};
}

void render_start(OpenGL_Renderer* r, isize list_index)
{
	render_draw_list_start(r, r->draw_lists + list_index);
}

void render_start(isize list_index = 0)
{
	render_draw_list_start(Renderer, Renderer->draw_lists + list_index);
}

static inline bool render_draw_list_has_clip_rect(Draw_List* list)
{
	return 0 != (list->clip.w * list->clip.h);
}

static inline void render_draw_list_set_clip_rect(Draw_List* list, real x, real y, real w, real h)
{
	list->clip = Rect2 {
		x, y, w, h
	};
}

void render_set_clip_rect(OpenGL_Renderer* r, isize list_index, real x, real y, real w, real h)
{
	r->draw_lists[list_index].clip = Rect2{
		x, y, w, h
	};
}

void render_set_clip_rect(real x, real y, real w, real h, isize list_index = 0)
{
	Renderer->draw_lists[list_index].clip = Rect2{
		x, y, w, h
	};
}

void render_draw_list_sort(Draw_List* list, isize offset)
{
	sort_sprites_on_y_base(list->sprites + offset, list->sprites_count - offset);
}

void render_sort(OpenGL_Renderer* r, isize list_index, isize offset)
{
	Draw_List* list = r->draw_lists + list_index;
	sort_sprites_on_y_base(list->sprites + offset, list->sprites_count - offset);
}

void render_sort(isize offset, isize list_index = 0)
{
	Draw_List* list = Renderer->draw_lists + list_index;
	sort_sprites_on_y_base(list->sprites + offset, list->sprites_count - offset);
}

void render_draw_list_add(Draw_List* list, Sprite* sprite)
{
	Sprite sp = *sprite;

	if(render_draw_list_has_clip_rect(list)) {
		Rect2 c = list->clip;
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
		sp.anchor = Anchor_Top_Left;
		sp.angle = 0;
	}
	
	sp.texture.x /= list->texture_size.x;
	sp.texture.w /= list->texture_size.x;
	sp.texture.y /= list->texture_size.y;
	sp.texture.h /= list->texture_size.y;

	list->sprites[list->sprites_count++] = sp;
}

void render_add(OpenGL_Renderer* r, Sprite* sprite, isize list_index)
{
	Draw_List* list = r->draw_lists + list_index;
	render_draw_list_add(list, sprite);
}

void render_add(OpenGL_Renderer* r, Sprite4* s4, isize list_index = 0) 
{
	Draw_List* list = r->draw_lists + list_index;
	render_draw_list_add(list, s4->e + 0);
	render_draw_list_add(list, s4->e + 1);
	render_draw_list_add(list, s4->e + 2);
	render_draw_list_add(list, s4->e + 3);
}

void render_add(Sprite* sprite, isize list_index = 0)
{
	Draw_List* list = Renderer->draw_lists + list_index;
	render_draw_list_add(list, sprite);
}

void render_add(Sprite4* s4, isize list_index = 0) 
{
	Draw_List* list = Renderer->draw_lists + list_index;
	render_draw_list_add(list, s4->e + 0);
	render_draw_list_add(list, s4->e + 1);
	render_draw_list_add(list, s4->e + 2);
	render_draw_list_add(list, s4->e + 3);
}

void render_calculate_ortho_matrix(real* ortho, Vec4 screen, real near, real far)
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
	ortho[10] = -2.0f / (far - near);
	ortho[11] = (-1.0f * (far + near) / (far - near));

	ortho[12] = 0;
	ortho[13] = 0;
	ortho[14] = 0;
	ortho[15] = 1.0f;
}


void render_draw_list(OpenGL_Renderer* r, Draw_List* list, Vec2 size, real scale)
{
	glUseProgram(r->shader_program);
	list->offset.x = roundf(list->offset.x);
	list->offset.y = roundf(list->offset.y);

	glUniform2f(r->u_texturesize,
		list->texture_size.x,
		list->texture_size.y);

	glUniform1f(r->u_night_amount, 1.0f - list->night_amount);
	glUniform1f(r->u_night_cutoff, list->night_cutoff);

	Vec4 screen = v4(
		list->offset.x, list->offset.y, 
		size.x + list->offset.x,
		size.y + list->offset.y);
	render_calculate_ortho_matrix(list->ortho, screen, 1, -1);
	glUniformMatrix4fv(r->u_orthomat, 
		1, 
		GL_FALSE,
		list->ortho);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, list->texture);
	glBindVertexArray(r->vao);
	glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
	glBufferData(GL_ARRAY_BUFFER, list->sprites_count * sizeof(Sprite), list->sprites, GL_STREAM_DRAW);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, list->sprites_count);
	glBindVertexArray(0);
}

void render_draw(OpenGL_Renderer* r, isize list_index)
{
	Draw_List* list = r->draw_lists + list_index;
	render_draw_list(r, list);
}

void render_draw(isize list_index = 0)
{
	render_draw_list(Renderer, Renderer->draw_lists + list_index);
}

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
		printf("There was an error adding a texture: %d \n", error));
	}
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

	
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

void render_box_primitive(Vec2 pos, Vec2 size, Vec4 color, isize list_index = 0)
{
	Sprite s = create_box_primitive(pos, size, color);
	render_add(&s, list_index);
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
		s = get_box_sprite(start + v2(dline.x / 2, 0), v2(dline.x, thickness), color);
	} else if(dline.x == 0) {
		if(dline.y < 0) {
			dline.y *= -1;
			Vec2 temp = end;
			end = start;
			start = temp;
		}
		s = get_box_sprite(start + v2(0, dline.y / 2), v2(thickness, dline.y), color);
	} else {
		s = get_box_sprite(start + dline/2, v2(sqrtf(v2_dot(dline, dline)), thickness), color);
		real angle = atan2f(dline.y, dline.x);
		s.angle = -angle;
	}
	return s;
}
void render_line_primitive(Vec2 start, Vec2 end, Vec4 color, int32 thickness, isize list_index = 0)
{
	Sprite s = create_line_primitive(start, end, color, thickness);
	render_add(&s, list_index);
}

Sprite4 create_box_outline_primitive(Vec2 center, Vec2 size, Vec4 color, int32 thickness)
{
	size *= 0.5f;	
	Vec2 tl = center - size;
	Vec2 br = center + size;
	Sprite s[4];
	s[0] = create_line_primitive(tl, v2(br.x, tl.y), color, thickness);
	s[1] = create_line_primitive(v2(br.x, tl.y), br, color, thickness);
	s[2] = create_line_primitive(br, v2(tl.x, br.y), color, thickness);
	s[3] = create_line_primitive(v2(tl.x, br.y), tl, color, thickness);
	Sprite4 s4;
	s4.e = s;
	return s4;
}

void render_box_outline_primitive(Vec2 center, Vec2 size, Vec4 color, int32 thickness, isize list_index = 0)
{
	Sprite4 s = create_box_outline_primitive(center, size, color, thickness);
	render_add(&s, list_index);
}

Sprite4 create_box_outline_primitive(Vec2 center, Vec2 size, Vec4 color[4], int32 thickness)
{
	size *= 0.5f;	
	Vec2 tl = center - size;
	Vec2 br = center + size;
	Sprite s[4];
	s[0] = create_line_primitive(tl, v2(br.x, tl.y), color[0], thickness);
	s[1] = create_line_primitive(v2(br.x, tl.y), br, color[1], thickness);
	s[2] = create_line_primitive(br, v2(tl.x, br.y), color[2], thickness);
	s[3] = create_line_primitive(v2(tl.x, br.y), tl, color[3], thickness);
	Sprite4 s4;
	s4.e = s;
	return s4;
}

Sprite4 render_box_outline_primitive(Vec2 center, Vec2 size, Vec4 color[4], int32 thickness, isize list_index = 0)
{
	Sprite4 s = create_box_outline_primitive(center, size, color, thickness);
	render_add(&s, list_index);
}


