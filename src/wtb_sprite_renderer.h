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

struct Draw_List
{
	Vec2 offset;
	Rect2 clip;
	float[16] ortho;

	Sprite* sprites;
	isize sprites_count, sprites_capacity;
};

struct Renderer
{
	GLuint shader_program, vbo, vao, texture;
	isize u_texturesize, u_orthomat;
	
	Draw_List* draw_lists;
	isize draw_lists_count;
};

void init_draw_list(Draw_List* list, isize sprites_capacity, Memory_Arena* arena)
{
	list->offset = v2(0, 0);
	list->clip = rect2(0, 0, 0, 0);

	list->sprites_capacity = sprites_capacity;
	list->sprites_count = 0;
	list->sprites = arena_push_array(arena, Sprite, sprites_capacity);
}

#define _get_member_address(s, m) ((void*)&(((s*)(NULL))->m))
#define _gl_offset(name) ((GLvoid*)(_get_member_address(Sprite, name)))
void init_renderer(Renderer* r, isize list_count, isize list_size, char* vertex_source, char* frag_source, Memory_Arena* arena)
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
	glVertexAttribIPointer(6, 1, GL_UNSIGNED_INT, stride, _gl_offset(anchor));
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
	r->u_texturesize = glGetUnformLocation(r->shader_program, "u_texturesize");
	r->u_orthomat = glGetUniformLocation(r->shader_program, "u_orthomat");

}

void render_draw_list_start(Renderer* r, Draw_List* list)
{
	list->sprites_count = 0;
	list->clip = {0, 0, 0, 0};
}

void render_start(Renderer* r, isize list_index)
{
	render_draw_list_start(r, r->draw_lists + list_index);
}

static inline bool render_draw_list_has_clip_rect(Draw_List* list)
{
	return 0 != (list->clip.w * list->clip.h);
}

void render_draw_list_add(Draw_List* list, Sprite* sprite)
{
	Sprite sp = *sprite;

	if(render_draw_list_has_clip_rect(list)) {
		Rect2 c = list->clip;
		Rect2 r;
		r.position = sp.position;
		r.size = sp.size;
		r.x -= r.w * (0.5f + SpriteAnchorX[sp.anchor]);
		r.y -= r.h * (0.5f + SpriteAnchorY[sp.anchor]);

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

	list->sprites[list->sprites_count++] = sp;
}

void render_add(Renderer* r, Sprite* sprite, isize list_index)
{
	Draw_List* list = r->draw_lists + list_index;
	render_draw_list_add(list, sprite);
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


void render_draw_list(Renderer* r, Draw_List* list, Vec2 size, real scale)
{
	glUseProgram(r->shader_program);
	list->offset.x = roundf(list->offset.x);
	list->offset.y = roundf(list->offset.y);
	glUniform2f(r->u_texture_size
		r->texture_width,
		r->texture_height);
	Vec4 screen = v4(
		list->offset.x, list->offset.y, 
		size.x + list->offset.x,
		size.y + list->offset.y);
	render_calculate_ortho_matrix(list->ortho, screen, 1, -1);
	glUniformMatrix4fv(Renderer->u_orthomat, 
		1, 
		GL_FALSE,
		list->ortho);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Renderer->texture);
	glBindVertexArray(r->vao);
	glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
	glBufferData(GL_ARRAY_BUFFER, list->sprites_count * sizeof(Sprite), list->sprites, GL_STREAM_DRAW);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, list->sprites_count);
	glBindVertexArray(0);
}

void render_draw(Renderer* r, isize list_index)
{
	Draw_List* list = r->draw_lists + list_index;
	render_draw_list(r, list);
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
	




