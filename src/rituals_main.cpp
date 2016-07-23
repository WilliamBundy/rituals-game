/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 * rituals_win32.cpp
 * main file for windows platform
 *
 */ 

/* TODO(will) features
 *	- Improve physics system
 *		- Investigate different types of broadphases
 *			- currently using sweep and prune
 *			- most big engines use the b2 style dynamic tree?
 *		- Weird behavior caused by oversimplified friction
 *			- need to simulate both static and dynamic friction
 *			- static friction: objects at rest experience higher friction
 *			- dynamic friction: once objects have a certain speed/momentum
 *								use a different friction value (lower)
 *	- Main menu
 *		- World select is pretty much done?
 *		- Create new world submenu
 *			- Need text input
 *			- Double check which characters are legal in filenames
 *			- Scrollable panels 
 *				- Probably requires additions to the renderer?
 *				- Gotta clip sprite size and texture, adjusting for anchor?
 *		- Create options submenu
 *			- Might need to wait until some stuff is finalized before we 
 *			  can have options for it, no?
 * - 
 *
 * TODO(will) logical fixes
 *  - current/last time/accumulator need to belong to simulation
 */



//platform imports
#include <windows.h>
#include <Shlwapi.h>

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

#define MixerNumberOfChannels (64)

#define FilePathMaxLength (4096)

#define PathSeparator ("\\")
#define PathSeparatorChar ('\\')

#define Min(x,y) ((x<y)?x:y)
#define Max(x,y) ((x>y)?x:y)


#define Kilobytes(b) (b * UINT64_C(1024))
#define Megabytes(b) (Kilobytes(b) * UINT64_C(1024))
#define Gigabytes(b) (Megabytes(b) * UINT64_C(1024))

//local imports
#include "rituals_math.cpp"
#include "rituals_game.cpp"

#include "rituals_renderer.cpp"
#include "rituals_gui.cpp"
void update()
{
	switch(Game->state) {
		case Game_State_None:
			test_update();
			break;
		case Game_State_Menu:
			main_menu_update();
			SDL_StartTextInput();
			break;
		case Game_State_Play:
			SDL_StopTextInput();
			play_state_update();
			break;
		default:
			break;
	}
}


void stop()
{
	switch(Game->state) {
		case Game_State_None:
			break;
		case Game_State_Menu:
			break;
		case Game_State_Play:
			play_state_stop();
			break;
		default:
			break;
	}
}


void load_assets()
{
	isize w, h;
	Renderer->texture = ogl_load_texture("data/graphics.png", &w, &h);
	Renderer->texture_width = w;
	Renderer->texture_height = h;

	Game->body_font = load_spritefont("data/gohufont-14.glyphs", 
			v2i(2048 - 1142, 0));
	Body_Font = Game->body_font;

	init_game_registry(Registry, Game->registry_arena); 
	register_everything_in_rituals();
	finalize_game_registry();

	init_menu_state();
	Game->state = Game_State_Menu;
	char buf[FilePathMaxLength];
	isize len = snprintf(buf, FilePathMaxLength, "%ssave", Game->base_path);

	test_string = arena_push_array(Game->temp_arena, char, 2048);
	test_string_length = 0;

	tinydir_open_sorted(&Menu->saves, buf);
	init_text_input_handle(&handle, 256, Game->temp_arena);
}


void update_screen()
{
	SDL_GetWindowSize(Game->window, &Game->window_size.x, &Game->window_size.y);
	glViewport(0, 0, Game->window_size.x, Game->window_size.y);
	Game->size = v2(Game->window_size) * Game->scale;
}

int main(int argc, char** argv)
{
	//stbi_set_flip_vertically_on_load(1);
	printf("%d %d %d \n", sizeof(ptrdiff_t), sizeof(int64), sizeof(isize));

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
	SDL_Window* window = SDL_CreateWindow("Spaceship Draft", 
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

	int ret = SDL_GL_SetSwapInterval(-1);
	// Game initializiation
	Game = Allocate(Game_Main, 1);
	{
		Game->window = window;
		Game->state = Game_State_None;
		Game->meta_arena = Allocate(Memory_Arena, 1);
		init_memory_arena(Game->meta_arena, isz(Memory_Arena) * 20);
		Game->game_arena = new_memory_arena(Kilobytes(64), Game->meta_arena);
		Game->asset_arena = new_memory_arena(Megabytes(512), Game->meta_arena);
		Game->temp_arena = new_memory_arena(Megabytes(64), Game->meta_arena);
		Game->play_arena = new_memory_arena(Megabytes(512), Game->meta_arena);
		Game->renderer_arena = new_memory_arena(Megabytes(32), Game->meta_arena);
		Game->world_arena = new_memory_arena(Megabytes(2), Game->meta_arena);
		Game->registry_arena = new_memory_arena(Megabytes(2), Game->meta_arena);

		Game->base_path = SDL_GetBasePath();
		Game->base_path_length = strlen(Game->base_path);

		Game->input = arena_push_struct(Game->game_arena, Game_Input);
		Input = Game->input;
		Input->scancodes = arena_push_array(Game->game_arena, int8, SDL_NUM_SCANCODES);
		Input->keycodes = arena_push_array(Game->game_arena, int8, SDL_NUM_SCANCODES);
		Input->mouse = arena_push_array(Game->game_arena, int8, 16);
		Input->mouse_pos = v2i(0, 0);
		Input->text = arena_push_array(Game->game_arena, char, InputTextCapacity);
		Input->text_count = 0;
		Input->capture_newlines = false;
		Input->capture_tabs = false;


		init_random(&Game->r, time(NULL));
		//TODO(will) load window settings from file
		Game->window_size = v2i(1280, 720);
		Game->scale = 1.0f;

		Game->renderer = arena_push_struct(Game->game_arena, OpenGL_Renderer);
		renderer_init(Game->renderer, Game->renderer_arena);
		
		Game->registry = arena_push_struct(Game->game_arena, Game_Registry);

		Registry = Game->registry;
		Renderer = Game->renderer;
		//init_gui_colors();
	}

	load_assets();

	bool running = true;
	SDL_Event event;
	glClearColor(0, 0, 0, 1);
	//glClearColor(1, 1, 1, 1);

	isize input_text_length = 0;
	while(running) {
		uint64 start_ticks = SDL_GetTicks();

		for(int64 i = 0; i < SDL_NUM_SCANCODES; ++i) {
			int8* t = Game->input->scancodes + i;
			if(*t == State_Just_Released) {
				*t = State_Released;
			} else if(*t == State_Just_Pressed) {
				*t = State_Pressed;
			}
			t = Game->input->keycodes + i;
			if(*t == State_Just_Released) {
				*t = State_Released;
			} else if(*t == State_Just_Pressed) {
				*t = State_Pressed;
			}
		}
		for(int64 i = 0; i < 16; ++i) {
			int8* t = Game->input->mouse + i;
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
					stop();
					running = false;
					break;
				case SDL_WINDOWEVENT:
					update_screen();
					break;
				case SDL_TEXTINPUT:
					input_text_length = strlen(event.text.text);
					if((input_text_length + Input->text_count) < InputTextCapacity) {
						memcpy(Input->text + Input->text_count, 
								event.text.text, 
								input_text_length);
						Input->text_count += input_text_length;
						input_text_length = 0;
					}
					break;
				case SDL_KEYDOWN:
					Game->input->num_keys_down++;
					if((event.key.keysym.sym == SDLK_BACKSPACE)) {
						input_text_append_char(Backspace);
						if(event.key.keysym.mod & KMOD_CTRL) {
							input_text_append_char('\7');
						}
					} else if(event.key.keysym.sym == SDLK_RETURN || 
							event.key.keysym.sym == SDLK_RETURN2) {
						if(Input->capture_newlines) {
							input_text_append_char('\n');
						}
					} else if(event.key.keysym.sym == SDLK_TAB) {
						if(Input->capture_tabs) {
							input_text_append_char('\t');
						}
					}

					if(!event.key.repeat) {
						Game->input->scancodes[event.key.keysym.scancode] = State_Just_Pressed;
						if(event.key.keysym.sym < SDL_NUM_SCANCODES) {
							Game->input->keycodes[event.key.keysym.sym] = State_Just_Pressed;
						}
					}
					break;
				case SDL_KEYUP:
					Game->input->num_keys_down--;
					if(!event.key.repeat) {
						Game->input->scancodes[event.key.keysym.scancode] = State_Just_Released;
						if(event.key.keysym.sym < SDL_NUM_SCANCODES) {
							Game->input->keycodes[event.key.keysym.sym] = State_Just_Released;
						}
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					Game->input->num_mouse_down++;
					Game->input->mouse[event.button.button] = State_Just_Pressed;
					break;
				case SDL_MOUSEBUTTONUP:
					Game->input->num_mouse_down--;
					Game->input->mouse[event.button.button] = State_Just_Released;
					break;
			}
		}
	
		int mx, my;
		SDL_GetMouseState(&mx, &my);
		Input->mouse_x = mx;
		Input->mouse_y = my;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		update();

		SDL_GL_SwapWindow(window);
	}

	SDL_Quit();
	return 0;
}

