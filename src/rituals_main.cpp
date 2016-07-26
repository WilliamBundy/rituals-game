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
 *  - Physics
 *  	- Static and dynamic friction
 *  	- Look at dynamic tree broadphase
 *  - Renderer
 *  	- Viewport/scrolling
 *  - Play
 *  	- Entity/physics inspector?
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
typedef struct World World;
struct Play_State
{
	usize current_time = 0, prev_time = 0;
	real accumulator = 0;
	World* world;

	Vec2i world_xy;
};
Play_State* play_state;


#include "rituals_renderer.cpp"
#include "rituals_gui.cpp"

#include "rituals_game_info.cpp"
#include "rituals_game_registry.cpp"

#include "rituals_inventory.cpp"

#include "rituals_tilemap.cpp"

#include "rituals_simulation.cpp"

#include "rituals_world_area.cpp"
#include "rituals_world.cpp"

#include "rituals_play_state.cpp"

#include "rituals_serialization.cpp"

struct Menu_State
{
	char save_dir[FilePathMaxLength];
	isize save_dir_len;
	tinydir_dir saves;
	Gui_Text_Input_Handle handle;
	isize delete_index;
};
Menu_State* menu_state;

void init_menu_state()
{
	menu_state = arena_push_struct(Game->game_arena, Menu_State);
	init_text_input_handle(&menu_state->handle, 256, Game->game_arena);
	menu_state->delete_index = -1;

	menu_state->save_dir_len = snprintf(menu_state->save_dir, FilePathMaxLength, "%ssave", Game->base_path);
	check_dir(menu_state->save_dir);
	tinydir_open_sorted(&menu_state->saves, menu_state->save_dir);
}


void main_menu_update()
{
	game_set_scale(2.0f);
	Renderer->offset = v2(0, 0);
	renderer_start();
	
	//draw_box_outline(v2(140, 140), v2(200, 200), v4(0.5f, 1.0f, 0.5f, 1), 2);
	//renderer_set_clip_rect(0, 0, 16, 200);
	//renderer_set_clip_rect(Input->mouse_x / Game->scale, Input->mouse_y / Game->scale, 200, 200);
	//render_clip_rect();
	real lasty = 32;
	Body_Font->color = v4(1, 1, 1, 1);
	render_body_text("Rituals", v2(32, 32), false, 4.0f);
	lasty += 16;
	lasty += Body_Font->glyph_height * 4;
	gui_add_text_input(&menu_state->handle, v2(32, lasty), v2(256, Body_Font->glyph_height + 8));

	bool saves_dirty = false;
	if(gui_add_button(v2(256 + 32 + 16 , lasty), "Create", v2(64, 0))) {
		saves_dirty = true;
	}
	if(Input->scancodes[SDL_SCANCODE_RETURN] == State_Just_Pressed) {
		saves_dirty = true;
	}

	if(saves_dirty) {
		tinydir_close(&menu_state->saves);
		char buf[FilePathMaxLength];
	 	snprintf(buf, FilePathMaxLength, "%ssave/%.*s", Game->base_path, menu_state->handle.buffer_length, menu_state->handle.buffer);
		check_dir(buf);
		
		tinydir_open_sorted(&menu_state->saves, menu_state->save_dir);
		menu_state->handle.buffer_length = 0;
	}


	lasty += 32 + 16;


	for(usize i = 0; i < menu_state->saves.n_files; ++i) {
		tinydir_file file;
		tinydir_readfile_n(&menu_state->saves, &file, i);
		if(file.is_dir && (file.name[0] != '.')) {
			if(gui_add_button(v2(32, lasty), file.name, v2(144, 0))) {
				start_play_state(file.name);
				Game->state = Game_State_Play;
			}
			if(menu_state->delete_index != i) {
				if(gui_add_button(v2(32 + 144 + 32, lasty), "Delete", v2(64, 0))) {
					menu_state->delete_index = i;
				}
			} 
			lasty += 32;
		}
	}

	
	


	renderer_draw();
}


Sprite* boxes;


void test_init()
{
	boxes = arena_push_array(Game->play_arena, Sprite, 100);
	for(isize i = 0; i < 100; ++i) {
		//init_sprite(boxes + i);
#if 1
		boxes[i] = get_box_sprite(v2(
			rand_range(&Game->r, 0,1000),
			rand_range(&Game->r, 0,1000)),
			v2(32, 16),
		v4(rand_range(&Game->r, 0.5f, 1.0), .8, .8, .9));
#endif

	}
}

void test_update()
{
	game_set_scale(2.0);
	renderer_start();
	renderer_set_clip_rect(Input->mouse_x / Game->scale, Input->mouse_y / Game->scale, 200, 200);
	for(isize i = 0; i < 100; ++i) {
		renderer_push_sprite(boxes + i);
	}	
	renderer_draw();
}

void update()
{
	switch(Game->state) {
		case Game_State_None:
			test_update();
			break;
		case Game_State_Menu:
			SDL_StartTextInput();
			main_menu_update();
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
			//TODO(will) write stop function
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

	Game->body_font = arena_push_struct(Game->asset_arena, Spritefont);
	init_spritefont(Game->body_font);
	Game->body_font->glyphs = parse_spritefont_rectangles(
#include "font.glyphs"
			,
			Game->asset_arena,
			2048 - 1142, 0, 
			&Game->body_font->glyph_width,
			&Game->body_font->glyph_height);
	Body_Font = Game->body_font;

	init_game_registry(Registry, Game->registry_arena); 
	register_everything_in_rituals();
	finalize_game_registry();

	Game->state = Game_State_Menu;
	play_state_init();
	init_menu_state();
//	Game->state = Game_State_None;
//test_init();
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

	int ret = SDL_GL_SetSwapInterval(-1);

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
	Game = Allocate(Game_Main, 1);
	{
		Game->window = window;
		Game->state = Game_State_None;
		Game->meta_arena = Allocate(Memory_Arena, 1);
		init_memory_arena(Game->meta_arena, isz(Memory_Arena) * 20);
		Game->game_arena = new_memory_arena(Megabytes(64), Game->meta_arena);
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
		Input->mouse_pos = 	v2(0, 0);
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
	}

	load_assets();

	bool running = true;
	SDL_Event event;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//glClearColor(1, 1, 1, 1);

	play_state->current_time = SDL_GetTicks();
	play_state->prev_time = play_state->current_time;
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
		Input->screen_mouse_pos = v2i(mx, my);
		Input->mouse_pos = v2(mx / Game->scale, my / Game->scale) + Renderer->offset;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		update();

		SDL_GL_SwapWindow(window);
		uint64 frame_ticks = SDL_GetTicks() - start_ticks;
		//if(frame_ticks > 18) printf("Slow frame! %d\n", frame_ticks);
	}

	SDL_Quit();
	return 0;
}

