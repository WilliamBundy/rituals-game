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

struct Sprite
{
	Vec2 position;
	Vec2 center;
	real angle;
	Vec2 size;
	Rect2 texture;
	Vec4 color;
	uint32 anchor;
};

struct Renderer
{
	GLuint shader_program, vbo, vao, texture;
	AABB screen;
	real texture_width, texture_height;
	Vec2 offset;

	isize screen_loc, texture_size_loc;
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
}


#define _gl_offset(a) ((GLvoid*)(a*sizeof(real)))
int32 t = 0;
#define _glerror printf("OpenGL Error at #%d: %0x\n", t++, glGetError());
void renderer_init(Renderer* renderer, Memory_Arena* arena)
{
	renderer->offset = v2(0, 0);
	renderer->last_sprite_id = 0;
	renderer->sprite_data = Arena_Push_Array(arena, Sprite, Megabytes(32) / sizeof(Sprite)); 
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
	renderer->texture_size_loc = glGetUniformLocation(renderer->shader_program, "texture_size");
}

GLuint ogl_add_texture(uint8* data, isize w, isize h) 
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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
	char file[File_Path_Max_Length];
	const char* base_path = SDL_GetBasePath();
	isize len = snprintf(file, File_Path_Max_Length, "%s%s", base_path, filename);
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

#define Get_Texture_Coordinates(x, y, w, h) rect2((x) / renderer->texture_width, (y) / renderer->texture_height, (w) / renderer->texture_width, (h) / renderer->texture_height)

void renderer_start()
{
	renderer->data_index = 0;
	renderer->sprite_count = 0;
	renderer->screen.center = renderer->offset;
	renderer->screen.hext = game->size * 0.5;
	renderer->screen.center += v2(renderer->screen.hw, renderer->screen.hh);

	glUseProgram(renderer->shader_program);
	glUniform4f(renderer->screen_loc, 
			renderer->offset.x, renderer->offset.y, 
			game->size.x + renderer->offset.x,
			game->size.y + renderer->offset.y);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderer->texture);
}

void renderer_push_sprite(Sprite* s)
{
	renderer->sprite_data[renderer->sprite_count++] = *s;
}

#define _get_sprite_y_base(s) (s.position.y + s.size.y / 2 - s.center.y)
Generate_Quicksort_For_Type(sort_sprites_on_y_base, Sprite, _get_sprite_y_base)

void renderer_sort(isize offset)
{
	sort_sprites_on_y_base(renderer->sprite_data + offset,
			renderer->sprite_count - offset);
}

void renderer_draw()
{
	glBindVertexArray(renderer->vao);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
	glBufferData(GL_ARRAY_BUFFER, renderer->sprite_count * sizeof(Sprite),
			renderer->sprite_data, GL_STREAM_DRAW);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, renderer->sprite_count);
	glBindVertexArray(0);
}

