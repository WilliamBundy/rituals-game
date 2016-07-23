/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 * rituals_renderer.cpp
 *
 * This file is dedicated to D7Samurai, for without his help,
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
	uint32 anchor;
	Vec2 sort_point_offset;
};

struct OpenGL_Renderer
{
	GLuint shader_program, vbo, vao, texture;
	AABB screen;
	real texture_width, texture_height;
	Vec2 offset;
	Rect2 clip;

	isize screen_loc, texture_size_loc, window_loc;
	Vec4 ortho;

	Sprite* sprite_data;
	isize data_index, sprite_count;

	isize last_sprite_id; 
};


void init_sprite(Sprite* s)
{
	s->position = v2(0, 0);
	s->center = v2(0, 0);
	s->angle = 0;
	s->size = v2(100, 100);
	s->texture = rect2(0, 0, 1, 1);
	s->color = v4(1, 1, 1, 1);
	s->anchor = Anchor_Center;
	s->sort_point_offset = v2(0, 0);
}


#define _gl_offset(a) ((GLvoid*)(a*sizeof(real)))
int32 t = 0;
#define _glerror printf("OpenGL Error at #%d: %0x\n", t++, glGetError());
void renderer_init(OpenGL_Renderer* renderer, Memory_Arena* arena)
{
	renderer->offset = v2(0, 0);
	renderer->last_sprite_id = 0;
	renderer->sprite_data = arena_push_array(arena, Sprite, Megabytes(32) / sizeof(Sprite)); 
	glGenVertexArrays(1, &renderer->vao);
	glBindVertexArray(renderer->vao);
	glGenBuffers(1, &renderer->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);

	usize stride = sizeof(Sprite);
	//position
	usize vertex_count = 1;
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, _gl_offset(0));
	glEnableVertexAttribArray(0);  
	glVertexAttribDivisor(0, vertex_count);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, _gl_offset(2));
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, vertex_count);

	//angle
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, stride, _gl_offset(4));
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, vertex_count);

	//size
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, _gl_offset(5));
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, vertex_count);

	//texcoords
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, stride, _gl_offset(7));
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, vertex_count);

	//color
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, stride, _gl_offset(11));
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(5, vertex_count);

	glVertexAttribIPointer(6, 1, GL_UNSIGNED_INT, stride, _gl_offset(15));
	glEnableVertexAttribArray(6);
	glVertexAttribDivisor(6, vertex_count);

	glBindVertexArray(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertex_shader_src = 
#include "vert.glsl"
		;
	glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
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

	const char* fragment_shader_src = 
#include "frag.glsl"
		;
	glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
	glCompileShader(fragment_shader);
	{
		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
		if(!success) {
			printf("Error compiling frag shader %.*s \n", 512, infoLog);
		} else {
			printf("Frag shader compiled successfully\n");
		}
	}	

	renderer->shader_program = glCreateProgram();
	glAttachShader(renderer->shader_program, vertex_shader);
	glAttachShader(renderer->shader_program, fragment_shader);
	glLinkProgram(renderer->shader_program);

	glUseProgram(renderer->shader_program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	renderer->screen_loc = glGetUniformLocation(renderer->shader_program, "screen");
	renderer->window_loc = glGetUniformLocation(renderer->shader_program, "window");
	renderer->texture_size_loc = glGetUniformLocation(renderer->shader_program, "texture_size");
}

GLuint ogl_add_texture(uint8* data, isize w, isize h) 
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
#if 0
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#else 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#endif

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//TODO(will) do error checking?
	printf("Adding texture: %d \n", glGetError());
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

#define Get_Texture_Coordinates(x, y, w, h) rect2((x) / Renderer->texture_width, (y) / Renderer->texture_height, (w) / Renderer->texture_width, (h) / Renderer->texture_height)

void renderer_start()
{
	Renderer->data_index = 0;
	Renderer->sprite_count = 0;
	Renderer->screen.center = Renderer->offset;
	Renderer->screen.hext = Game->size * 0.5;
	Renderer->screen.center += v2(Renderer->screen.hw, Renderer->screen.hh);
	Renderer->clip = {0, 0, 0, 0};

	glUseProgram(Renderer->shader_program);
	//Renderer->offset.x = roundf(Renderer->offset.x);
	//Renderer->offset.y = roundf(Renderer->offset.y);
	glUniform2f(Renderer->texture_size_loc, 
			Renderer->texture_width,
			Renderer->texture_height);
	glUniform3f(Renderer->window_loc,
			Game->window_size.x, 
			Game->window_size.y,
			Game->scale);
	glUniform4f(Renderer->screen_loc, 
			Renderer->offset.x, Renderer->offset.y, 
			Game->size.x + Renderer->offset.x,
			Game->size.y + Renderer->offset.y);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Renderer->texture);
}

static inline bool renderer_has_clip_rect()
{
	return 0 != (Renderer->clip.w * Renderer->clip.h);
}

static inline void renderer_set_clip_rect(real x, real y, real w, real h)
{
	Renderer->clip = Rect2 {
		x, y, w, h
	};
}

void draw_box_outline(Vec2 center, Vec2 size, Vec4 color, int32 thickness);
void renderer_push_sprite(Sprite* s)
{
	Sprite sp = *s;
	if(renderer_has_clip_rect()) {
		Rect2 r;
		r.x = sp.position.x;
		r.y = sp.position.y;
		r.w = sp.size.x;
		r.h = sp.size.y;
		r.x += r.w * SpriteAnchorX[sp.anchor];
		r.y += r.h * SpriteAnchorX[sp.anchor];
		AABB raabb = rect_to_aabb(&r);
		AABB clipaabb = rect_to_aabb(&Renderer->clip);
		if(!aabb_intersect(&raabb, &clipaabb)) return;
		Rect2 rclip = Renderer->clip;
		Renderer->clip = rect2(0, 0, 0, 0);
		draw_box_outline(sp.position + sp.size /2 , sp.size, v4(1, 1, 1, 1), .5);
		draw_box_outline(v2(rclip.x, rclip.y) + v2(rclip.w/2, rclip.h/2), v2(rclip.w, rclip.h), v4(0.5, 1, 0.5, 1), 2);

		Renderer->clip = rclip;
		Rect2_Clip_Info clip = rect2_clip(r, Renderer->clip);
		if(clip.r == rect2(0, 0, 0, 0)) return;
		sp.position.x = clip.r.x;
		sp.position.y = clip.r.y;
		sp.size.x = clip.r.w;
		sp.size.y = clip.r.h;
		sp.texture.x += clip.diff.x / Renderer->texture_width;
		sp.texture.y += clip.diff.y / Renderer->texture_height;
		sp.texture.w += clip.diff.w / Renderer->texture_width;
		sp.texture.h += clip.diff.h / Renderer->texture_height;
		sp.anchor = Anchor_Top_Left;
		Renderer->clip = rect2(0, 0, 0, 0);
		draw_box_outline(sp.position + sp.size /2 , sp.size, v4(1, 1, 1, 1), 0.5);
		Renderer->clip = rclip;
	}


	//sp = *s;
	Renderer->sprite_data[Renderer->sprite_count++] = sp;
}

#define _get_sprite_y_base(s) (s.position.y - s.center.y + s.sort_point_offset.y)
GenerateIntrosortForType(sort_sprites_on_y_base, Sprite, 12, _get_sprite_y_base)

void renderer_sort(isize offset)
{
	sort_sprites_on_y_base(Renderer->sprite_data + offset,
			Renderer->sprite_count - offset);
#if 0 // Debug draw sprite base
	isize initial_count = Renderer->sprite_count;
	for(isize i = offset; i < initial_count; ++i) {
		Sprite s; 
		init_sprite(&s);
		Sprite spr = Renderer->sprite_data[i];
		s.position = v2(
				spr.position.x,
				_get_sprite_y_base(spr));
		s.size = v2(4,4);
		s.texture = Get_Texture_Coordinates(32 * 2, 0, 32, 32);
		renderer_push_sprite(&s);
	}
#endif
}
void renderer_draw()
{
	glBindVertexArray(Renderer->vao);
	glBindBuffer(GL_ARRAY_BUFFER, Renderer->vbo);
	glBufferData(GL_ARRAY_BUFFER, Renderer->sprite_count * sizeof(Sprite),
			Renderer->sprite_data, GL_STREAM_DRAW);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, Renderer->sprite_count);
	glBindVertexArray(0);
}


Sprite get_box_sprite(Vec2 pos, Vec2 size, Vec4 color)
{
	Sprite s;
	init_sprite(&s);
	s.position = pos;
	s.texture = Get_Texture_Coordinates(64, 0, 32, 32);
	s.size = size;
	s.color = color;
	return s;
}

void draw_line(Vec2 start, Vec2 end, Vec4 color, int32 thickness)
{
	Vec2 dline = end - start;
	real angle = atan2f(dline.y, dline.x);
	Sprite s;
	init_sprite(&s);
	s.position = start + dline / 2;
	s.texture = Get_Texture_Coordinates(64, 0, 32, 32);
	s.size = v2(sqrtf(v2_dot(dline, dline)), thickness);
	s.angle = -angle;
	s.color = color;
	renderer_push_sprite(&s);
}

void draw_box_outline(Vec2 center, Vec2 size, Vec4 color, int32 thickness)
{
	size *= 0.5f;	
	Vec2 tl = center - size;
	Vec2 br = center + size;
	draw_line(tl, v2(br.x, tl.y), color, thickness);
	draw_line(v2(br.x, tl.y), br, color, thickness);
	draw_line(br, v2(tl.x, br.y), color, thickness);
	draw_line(v2(tl.x, br.y), tl, color, thickness);
}
void draw_box_outline(Vec2 center, Vec2 size, Vec4 colors[4], int32 thickness)
{
	size *= 0.5f;
	Vec2 tl = center - size;
	Vec2 br = center + size;
	draw_line(tl, v2(br.x, tl.y), colors[0], thickness);
	draw_line(v2(br.x, tl.y), br, colors[1], thickness);
	draw_line(br, v2(tl.x, br.y), colors[2], thickness);
	draw_line(v2(tl.x, br.y), tl, colors[3], thickness);
}
void draw_box_outline(Vec2 center, Vec2 size, real angle, Vec4 color, int32 thickness)
{
	size *= 0.5f;
	Vec2 tl = -size;
	Vec2 br = size;
	Vec2 tr = v2(br.x, tl.y);
	Vec2 bl = v2(tl.x, br.y);
	Vec2 rot = v2(cosf(angle), sinf(angle));
	Mat2 rotmat = {
		rot.x, rot.y, 
		-rot.y, rot.x
	};
	tl = v2(rot.x * tl.x + rot.y * tl.y, -rot.y * tl.x + rot.x * tl.y);	
	br = v2(rot.x * br.x + rot.y * br.y, -rot.y * br.x + rot.x * br.y);	
	bl = v2(rot.x * bl.x + rot.y * bl.y, -rot.y * bl.x + rot.x * bl.y);	
	tr = v2(rot.x * tr.x + rot.y * tr.y, -rot.y * tr.x + rot.x * tr.y);	
	tl += center;
	br += center;
	bl += center;
	tr += center;
	draw_line(tl, tr, color, thickness);
	draw_line(tr, br, color, thickness);
	draw_line(br, bl, color, thickness);
	draw_line(bl, tl, color, thickness);
}






