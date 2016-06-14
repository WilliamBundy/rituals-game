/*
 * rituals_win32.cpp
 * main file for windows platform
 *
 */ 

/* PROJECT TODO
 * 
 * 1. Get stuff on screen with OpenGL
 *		- Rendering quads as triangle strips
 *		- Rendering primitives with magic
 *		- Possibly some sort of particle system too?
 * 2. Write 2D physics sim code
 * 		- Just circles and AABBs
 * 3. Write a console thing?
 * 4. Learn to multithread the SHIT out of this
 *
 */




//platform imports
#include <windows.h>

//CRT imports
#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stddef.h>
#include <errno.h>
#include <malloc.h>

//3rd party imports
#include <SDL.h>
#include "thirdparty.h"


//Some defines
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float real32;
typedef double real64;

//TODO(will): use this as all floating point sizes
typedef real32 real;

typedef ptrdiff_t isize;
typedef size_t usize;

#define Math_Sqrt2    1.414213f
#define Math_InvSqrt2 0.7071067f
#define Math_Pi       3.141592f
#define Math_Tau      6.283185f
#define Math_Rad2Deg  57.29577f
#define Math_Deg2Rad  0.01745329f

#define Allocate(type, count) ((type*)calloc(sizeof(type), count))
#define StackAlloc(type, count) ((type*)_alloca(sizeof(type) * count))

#define isizeof(t) ((isize)sizeof(t))
#define isz(t) (isizeof(t))

#define Mixer_Number_Of_Channels (64)

#define File_Path_Max_Length (4096)

#define Path_Separator ("\\")
#define Path_Separator_Char ('\\')

#define Kilobytes(b) (b * UINT64_C(1024))
#define Megabytes(b) (Kilobytes(b) * UINT64_C(1024))
#define Gigabytes(b) (Megabytes(b) * UINT64_C(1024))

//local imports
#include "rituals_math.cpp"
#include "rituals_game.cpp"

// game assets struct
struct Game_Assets
{

};
//actual game code


//
// OpenGL things to do:
// Textures
// Instancing
// Understanding when to use VAOs v. Uniforms when instancing
// Camera, turning screen points (mouse clicks) into camera points
// Turning all this into a SpriteBatch style thing.
//
#define _ti 2
#define _tcx(px, sz) ((px + 32*_ti) / (real)sz)
#define _tcy(px, sz) ((sz-px) / (real)sz)
real vertices[] = {
     0.5f,  0.5f,   1.0f, 1.0f, // _tcx(32, 512),  _tcy(0, 512),  // Top Right
     0.5f, -0.5f,   1.0f, 0.0f, // _tcx(32, 512),  _tcy(32, 512),  // Bottom Right
    -0.5f, -0.5f,   0.0f, 0.0f, // _tcx(0, 512),  _tcy(32, 512),  // Bottom Left
    -0.5f,  0.5f,   0.0f, 1.0f, // _tcx(0 , 512),  _tcy(0, 512)   // Top Left 
};  

GLuint indices[] = {  // Note that we start from 0!
    0, 1, 3,   // First Triangle
    1, 2, 3   // Second Triangle
		
};  

GLuint shader_program, vbo, vao, texture, ebo;
isize color_loc, texture_loc;


real t = 0;
void update()
{
	glViewport(0, 0, 1280, 720);
	glUseProgram(shader_program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//t += 1.0f;
	//real color = sinf(t * Math_Deg2Rad);
	//color = color * color; 
	//color = color / 2 + 0.5;
	glUniform4f(color_loc, 1.0f, 1.0f, 1.0f, 1.0f);

	//okay, let's write a naive renderer first
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void load_assets()
{

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Texture coords
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);  

	//Texture coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

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

	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glUseProgram(shader_program);


	color_loc = glGetUniformLocation(shader_program, "local_color");
	texture_loc = glGetUniformLocation(shader_program, "local_texture");

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);


	int w, h, n;
	char file[File_Path_Max_Length];
	const char* base_path = SDL_GetBasePath();
	isize len = snprintf(file, File_Path_Max_Length, "%s%s", base_path, "small.png");
	uint8* data = (uint8*)stbi_load(file, &w, &h, &n, STBI_rgb_alpha);


	//GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	STBI_FREE(data);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

int main(int argc, char** argv)
{
	stbi_set_flip_vertically_on_load(1);

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		Log_Error("Could not init SDL"); 
		Log_Error(SDL_GetError());
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	SDL_Window* window = SDL_CreateWindow("Rituals", 
			SDL_WINDOWPOS_CENTERED, 
			SDL_WINDOWPOS_CENTERED,
			1280, 720, 
			SDL_WINDOW_OPENGL | 
			SDL_WINDOW_RESIZABLE |
			SDL_WINDOW_MOUSE_FOCUS |
			SDL_WINDOW_INPUT_FOCUS);



	if(window == NULL) {
		Log_Error("Could not create window");
		Log_Error(SDL_GetError());
		return 1;
	}

	SDL_GLContext glctx = SDL_GL_CreateContext(window);

	if(ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		Log_Error("Could not load OpenGL 3.3 functions...");
		return 1;
	}

	int ret = SDL_GL_SetSwapInterval(-1);
	
//#ifdef DEBUG
	{
#define _check_gl_attribute(attr, val) int _##attr##_val; \
	int _##attr##_success = SDL_GL_GetAttribute(attr, &_##attr##_val); \
	gl_checks[gl_check_count++] = _##attr##_val == val; \
	gl_names[gl_check_count - 1] = #attr; \
	gl_vals[gl_check_count - 1] = _##attr##_val; \
	gl_exp_vals[gl_check_count - 1] = val; 
			 
		//check if we got everything
		bool gl_checks[64];
		char* gl_names[64];
		int gl_vals[64];
		int gl_exp_vals[64];
		isize gl_check_count = 0;

		_check_gl_attribute(SDL_GL_RED_SIZE, 8);
		_check_gl_attribute(SDL_GL_GREEN_SIZE, 8);
		_check_gl_attribute(SDL_GL_BLUE_SIZE, 8);
		_check_gl_attribute(SDL_GL_ALPHA_SIZE, 8);
		_check_gl_attribute(SDL_GL_DOUBLEBUFFER, 1);
		_check_gl_attribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		_check_gl_attribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		_check_gl_attribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1);
		_check_gl_attribute(SDL_GL_ACCELERATED_VISUAL, 1);
		_check_gl_attribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		for(isize i = 0; i < gl_check_count; ++i) {
			printf("%s %s: wanted %d, got %d \n", 
					gl_names[i], 
					gl_checks[i] ? "succeeeded" : "failed", 
					gl_exp_vals[i], 
					gl_vals[i]);
		}

	}	
//#endif 

	// Game initializiation
	Game* game = Allocate(Game, 1);
	{
		game->meta_arena = Allocate(Memory_Arena, 1);
		init_memory_arena(game->meta_arena, isz(Memory_Arena) * 10);
		game->game_arena = new_memory_arena(Kilobytes(64), game->meta_arena);
		game->asset_arena = new_memory_arena(Megabytes(512), game->meta_arena);
		game->temp_arena = new_memory_arena(Kilobytes(64), game->meta_arena);
		game->play_arena = new_memory_arena(Megabytes(512), game->meta_arena);

		game->base_path = SDL_GetBasePath();
		game->base_path_length = strlen(game->base_path);

		game->input = Arena_Push_Struct(game->game_arena, Game_Input);
		game->input->scancodes = Arena_Push_Array(game->game_arena, int8, SDL_NUM_SCANCODES);
		game->input->keycodes = Arena_Push_Array(game->game_arena, int8, SDL_NUM_SCANCODES);
		game->input->mouse = Arena_Push_Array(game->game_arena, int8, 16);

	}



	load_assets();

	bool running = true;
	SDL_Event evt;
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	while(running) {
		uint64 start_ticks = SDL_GetTicks();

		for(int64 i = 0; i < SDL_NUM_SCANCODES; ++i) {
			int8* t = game->input->scancodes + i;
			if(*t == State_Just_Released) {
				*t = State_Released;
			} else if(*t == State_Just_Pressed) {
				*t = State_Pressed;
			}
			t = game->input->keycodes + i;
			if(*t == State_Just_Released) {
				*t = State_Released;
			} else if(*t == State_Just_Pressed) {
				*t = State_Pressed;
			}
		}
		for(int64 i = 0; i < 16; ++i) {
			int8* t = game->input->mouse + i;
			if(*t == State_Just_Released) {
				*t = State_Released;
			} else if(*t == State_Just_Pressed) {
				*t = State_Pressed;
			}
		}
		while(SDL_PollEvent(&evt)) {
			if(evt.type == SDL_QUIT) {
				running = false;
			}	
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		update();
		SDL_GL_SwapWindow(window);
		uint64 frame_ticks = SDL_GetTicks() - start_ticks;
	}

	SDL_Quit();
	return 0;
}

