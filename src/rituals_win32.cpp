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
 * 		- Just_ circles and AABBs
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
#include "rituals_renderer.cpp"

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

Sprite* sp;
isize sprite_count;
Renderer* renderer;

void update(Game* game)
{
	sp->tr.position = v2(
			game->input->mouse_x,
			game->input->mouse_y);

	renderer_start(renderer, game);

	for(isize i = 0; i < sprite_count; ++i) {
		(sp+i)->tr.angle += 0.05f;
		renderer_push_sprite(renderer, sp + i);
	}

	renderer_draw(renderer);
	//b = 1;
	//renderer_draw_sprite(renderer, NULL);
}

void load_assets(Game* game)
{
	renderer = Arena_Push_Struct(game->play_arena, Renderer);
	renderer_init(renderer);
	renderer->texture = ogl_load_texture("data/graphics.png");
	sp = Arena_Push_Array(game->play_arena, Sprite, 1000);
	for(isize i = 0; i < 1000; ++i) {
		Sprite* s = sp + sprite_count++;
		s->tr.position = v2(
				rand_range(&game->r, 0, game->width),
				rand_range(&game->r, 0, game->height));
		s->tr.angle = i;
		s->tr.scale_x = rand_range(&game->r, 50, 100);
		s->tr.scale_y = s->tr.scale_x;
	}
}


void update_screen(Game* game, Renderer* renderer)
{
	SDL_GetWindowSize(game->window, &game->width, &game->height);
	glViewport(0, 0, game->width, game->height);
	renderer->ortho = v4(
		2 / (game->width - 0),  -1 * (0 + game->width) / (game->width - 0),
		2 / (0-game->height), -1 * (0 + game->height) / (0 - game->height)
	);
}

int main(int argc, char** argv)
{
	printf("Size of sprites? %d ", Megabytes(1) / 64);
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

	printf("%s \n", SDL_GetError());
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
		game->window = window;
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

		init_random(&game->r, time(NULL));
		//TODO(will) load window settings from file
		game->width = 1280;
		game->height = 720;

	}

	load_assets(game);
	update_screen(game, renderer);

	bool running = true;
	SDL_Event event;
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
		while(SDL_PollEvent(&event)) {
			//TODO(will) handle text input
			switch(event.type) {
				case SDL_QUIT:
					running = false;
					break;
				case SDL_WINDOWEVENT:
					update_screen(game, renderer);
					break;
				case SDL_KEYDOWN:
					if(!event.key.repeat) {
						game->input->scancodes[event.key.keysym.scancode] = State_Just_Pressed;
						if(event.key.keysym.sym < SDL_NUM_SCANCODES) {
							game->input->keycodes[event.key.keysym.sym] = State_Just_Pressed;
						}
					}
					break;
				case SDL_KEYUP:
					if(!event.key.repeat) {
						game->input->scancodes[event.key.keysym.scancode] = State_Just_Released;
						if(event.key.keysym.sym < SDL_NUM_SCANCODES) {
							game->input->keycodes[event.key.keysym.sym] = State_Just_Released;
						}
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					game->input->mouse[event.button.button] = State_Just_Pressed;
					break;
				case SDL_MOUSEBUTTONUP:
					game->input->mouse[event.button.button] = State_Just_Released;
					break;
			}
		}
		
		int mx, my;
		SDL_GetMouseState(&mx, &my);
		game->input->mouse_x = mx;
		game->input->mouse_y = my;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		update(game);
		SDL_GL_SwapWindow(window);
		uint64 frame_ticks = SDL_GetTicks() - start_ticks;
		if(frame_ticks > 17) printf("Slow frame! %d\n", frame_ticks);
	}

	SDL_Quit();
	return 0;
}

