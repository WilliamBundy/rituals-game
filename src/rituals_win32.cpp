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
#include "rituals_tilemap.cpp"
#include "rituals_simulation.cpp"

Tilemap map;
Vec2 offset;

Simulator sim;

void update()
{

	sim_update(&sim, 1.0f / 60.0f);
	sim.entities->body.center = v2(input->mouse_x, input->mouse_y);

	renderer_start();
	Sprite s; 
	init_sprite(&s);
	s.texture = Get_Texure_Coordinates(0, 0, 32, 32);
	s.size = v2(30, 30);

	real movespeed = 10;
	if(input->scancodes[SDL_SCANCODE_LEFT] == State_Pressed) {
		offset.x -= movespeed;
	}
	if(input->scancodes[SDL_SCANCODE_RIGHT] == State_Pressed) {
		offset.x += movespeed;
	}
	if(input->scancodes[SDL_SCANCODE_UP] == State_Pressed) {
		offset.y -= movespeed;
	}
	if(input->scancodes[SDL_SCANCODE_DOWN] == State_Pressed) {
		offset.y += movespeed;
	}
	//render_tilemap(&map, -offset, 1.0f); // v2(input->mouse_x, input->mouse_y), 1.0f);
	for(isize i = 0; i < 10; ++i) {
		s.position = v2(i * 40 + 100, 100);
		renderer_push_sprite(&s);
	}

	for(isize i = 0; i < sim.entities_count; ++i) {
		Entity* e = sim.entities + i;
		e->sprite.position = e->body.center;
		e->sprite.size = v2(e->body.hw * 2, e->body.hh * 2);
		renderer_push_sprite(&e->sprite);
	}


	renderer_draw();

}

void load_assets()
{
	isize w, h;
	renderer->texture = ogl_load_texture("data/terrain.png", &w, &h);
	renderer->texture_width = w;
	renderer->texture_height = h;
	init_tilemap(&map, 32, 32, game->play_arena);

	add_tile_info(&map, Get_Texure_Coordinates(0, 0, 0, 0), true);
	add_tile_info(&map, Get_Texure_Coordinates(32 * 0, 32, 32, 32), false); 
	add_tile_info(&map, Get_Texure_Coordinates(32 * 1, 32, 32, 32), false); 
	add_tile_info(&map, Get_Texure_Coordinates(32 * 2, 32, 32, 32), false); 
	add_tile_info(&map, Get_Texure_Coordinates(32 * 3, 32, 32, 32), false); 
	add_tile_info(&map, Get_Texure_Coordinates(32 * 4, 32, 32, 32), false); 
	add_tile_info(&map, Get_Texure_Coordinates(32 * 0, 64, 32, 32), true); 
	add_tile_info(&map, Get_Texure_Coordinates(32 * 1, 64, 32, 32), true); 
	add_tile_info(&map, Get_Texure_Coordinates(32 * 2, 64, 32, 32), true); 
	add_tile_info(&map, Get_Texure_Coordinates(32 * 3, 64, 32, 32), true); 
	add_tile_info(&map, Get_Texure_Coordinates(32 * 4, 64, 32, 32), true); 
	generate_tilemap(&map);
	init_simulator(&sim, 512, game->play_arena);

	for(isize i = 0; i < 16; ++i) {
		Entity* e = sim.entities + sim.entities_count++;
		init_entity(e);
		e->sprite.texture = Get_Texure_Coordinates(32 * 3, 64, 32, 32);
		e->body.hw = e->body.hh = 16;
		e->body.center = v2(
				rand_range(&game->r, 0, 800),
				rand_range(&game->r, 0, 800));
	}
	sim_refresh_sorted(&sim);
}


void update_screen()
{
	SDL_GetWindowSize(game->window, &game->width, &game->height);
	glViewport(0, 0, game->width, game->height);
}

int main(int argc, char** argv)
{
	//stbi_set_flip_vertically_on_load(1);

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
	

	int32 window_display_index = 0;
#if 1
	window_display_index = 1;
#endif
	SDL_Window* window = SDL_CreateWindow("Rituals", 
			SDL_WINDOWPOS_CENTERED_DISPLAY(window_display_index), 
			SDL_WINDOWPOS_CENTERED_DISPLAY(window_display_index),
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

	int ret = SDL_GL_SetSwapInterval(1);
	
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

	// Game initializiation
	game = Allocate(Game, 1);
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
		

		game->renderer = Arena_Push_Struct(game->game_arena, Renderer);
		renderer_init(game->renderer, game->play_arena);

		renderer = game->renderer;


		input = game->input;

	}

	load_assets();
	update_screen();

	bool running = true;
	SDL_Event event;
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	//glClearColor(1, 1, 1, 1);


	while(running) {
		uint64 start_ticks = SDL_GetTicks();

		if(game->input->num_keys_down < 0) game->input->num_keys_down = 0;
		if(game->input->num_mouse_down < 0) game->input->num_mouse_down = 0;

		if(game->input->num_keys_down > 0)
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
		if(game->input->num_mouse_down > 0)
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
					update_screen();
					break;
				case SDL_KEYDOWN:
					game->input->num_keys_down++;
					if(!event.key.repeat) {
						game->input->scancodes[event.key.keysym.scancode] = State_Just_Pressed;
						if(event.key.keysym.sym < SDL_NUM_SCANCODES) {
							game->input->keycodes[event.key.keysym.sym] = State_Just_Pressed;
						}
					}
					break;
				case SDL_KEYUP:
					game->input->num_keys_down--;
					if(!event.key.repeat) {
						game->input->scancodes[event.key.keysym.scancode] = State_Just_Released;
						if(event.key.keysym.sym < SDL_NUM_SCANCODES) {
							game->input->keycodes[event.key.keysym.sym] = State_Just_Released;
						}
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					game->input->num_mouse_down++;
					game->input->mouse[event.button.button] = State_Just_Pressed;
					break;
				case SDL_MOUSEBUTTONUP:
					game->input->num_mouse_down--;
					game->input->mouse[event.button.button] = State_Just_Released;
					break;
			}
		}
	
		int mx, my;
		SDL_GetMouseState(&mx, &my);
		input->mouse_x = mx;
		input->mouse_y = my;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		update();

		SDL_GL_SwapWindow(window);
		uint64 frame_ticks = SDL_GetTicks() - start_ticks;
		//if(frame_ticks > 18) printf("Slow frame! %d\n", frame_ticks);
	}

	SDL_Quit();
	return 0;
}

