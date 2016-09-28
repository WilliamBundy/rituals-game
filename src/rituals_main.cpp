/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 * rituals_main.cpp
 */ 

/* TODO(will) features
 * 	- Big Projects
 * 		- Massive refactoring
 * 		- Programmable ini replacement
 * 		- Memory dumps, logging
 * 		- Reflection/introspection metaprogramming system
 * 			- For use with serialization
 * 			- Can save versioned type layouts, then do its best to load from/to 
 * 		- Modding with packages
 * 			- requires a lot of sorting per package id.
 * 	- Graphics
 *		- Texture atlas stitching
 *			- able to combine pow2 squares into one big atlas
 *		- Release as lightweight spriting library
 *  - Physics
 *  	- Static and dynamic friction
 *  	- Multithreading/SIMD body processing?
 * 		- More sophisticated spacial partitioning?
 * 			- Hierarchical grids, quadtrees, k-d trees
 * 			- Current grid system works, but rather inflexible and memory-hungry
 * 	- UI
 * 		- Needs to be reworked
 *  - Play
 *  	- Entity inspector
 *  	- Another art pass
 * TODO(will) Refactoring
 * 	- rituals_animations.cpp
 * 		- To store animations in the registry?
 * 		- Have a way to pre-build animaition dictionaries
 * 		- Animated_Sprite isn't a sprite.
 * 	- rituals_entity_events.cpp
 * 		- Possibly a more sophisticated sorting method for entities? 
 * 		- Better typing support for entity userdata
 * 	- rituals_game.cpp
 * 		- Use generated structs/procedures for Game_States
 * 	- rituals_game_info.cpp & rituals_game_registry.cpp
 * 		- No meaningful distinction right now between these files?
 * 		- Supposedly the "types" of things go in info and the registry system 
 * 			belongs in registry, but due to C's declaration syntax, you can't mix them.
 * 		- All the game's definitions end up in registry when they should be in info
 * 	- rituals_gui.cpp
 * 		- Gui components need reworking.
 * 	- rituals_inventory.cpp
 * 		- This... just isn't done, and the project isn't quite ready for it.
 * 	- rituals_main.cpp
 * 		- Needs Main_Menu stuff taken out, possibly want to clean up includes/defines too.
 * 	- rituals_math.cpp
 * 		- Remove unused stuff
 * 		- Complete operator overloading for the whole type matrix
 * 	- rituals_memory.cpp
 * 		- Need more allocator types
 * 		- Free-list/Bucket Array?
 * 		- Pool allocator?
 * 		- I implement a lot of these inline, but it'd be nice to pull some out
 * 		- Maybe macro generators?
 * 	- rituals_particles.cpp
 * 		- Need to rework styles system, more types of effects? 
 * 		- Possibly needs to be included in the regsitry?
 * 		- Real-time particle editor?
 * 	- rituals_play_state.cpp
 * 		- Should be combined into a rituals_gamestates.cpp w/ generated code
 * 	- rituals_random.cpp is good!
 * 	- rituals_renderer.cpp is pretty good
 * 		- The "fancier" forms of rendering in the game rely on Sprite/render_add to work
 * 		- I'd like to standardize in-game objects vs. raw renderer Sprites
 * 		- A lot of the 3D stuff has been hardcoded (entity, particle, animation_frame)
 * 			but it's all very similar and could largely be combined.
 * 	- rituals_serialization.cpp
 * 		- Is totally broken; doesn't respect the current state of World, World_Area, Entity, etc
 * 	- rituals_simulation.cpp
 * 		- Is a bit of a mess overall
 * 		- Sim_Body is a pain to work with
 * 			- using shape.center for position sucks
 * 			- Shouldn't really have a entity pointer, right?
 * 			- I'm sure there's a slightly better way to hook up with Entity/other things?
 * 		- Question: sort array of pointers v. sort raw structs for sweep and prune?
 * 			- Ties into larger questions about cache coherency
 * 		- Need to investigate multithreading/taskpool implementation for update
 * 			- Also consider way to do some things in SIMD
 * 		- Overall, the static grid works well; maybe want grid for dynamic objects too?
 * 			- This is maybe where we look at quadtrees?
 * 	- rituals_sort_macros.cpp
 * 		- Maybe want to do speed testing vs. built in sort methods?
 *	- rituals_tilemap.cpp
 *		- Tilemap rendering needs tweaking; some tiles need more than a simple sprite
 *		- Maybe we need better representation of tiles w/ height?
 *	- rituals_world.cpp & rituals_world_area.cpp
 *		- Another mixed-up situation: world_area_update relies on world, so it ends up in world.cpp, 
 *		rather than closer to struct World_Area
 *		- Forward declarations and all the world_area_<handle_event> procedures clutter World.cpp
 *		- Entity/in-game object rendering needs to be reworked/standardized
 * 		
 *
 *
 * TODO(will) bugs???
 *	- All of serialization is broken
 *	- Current animation implementation is not serializable
 *		
 ***/

#define $(...)
 //platform imports
#if RITUALS_WINDOWS == 1
#include <windows.h>
#include <Shlwapi.h>
#include <malloc.h>
#endif

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

#if RITUALS_LINUX == 1
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif 

//3rd party imports
#if RITUALS_WINDOWS == 1
#include <SDL.h>
#elif RITUALS_LINUX == 1
#include <SDL2/SDL.h>
#endif
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
#define RITUALS_USE_FLOAT32
#ifdef RITUALS_USE_FLOAT32
typedef real32 real;
#else
typedef real64 real;
#endif

typedef ptrdiff_t isize;
typedef size_t usize;


#define Math_Sqrt2 1.414213f
#define Math_InvSqrt2 0.7071067f
#define Math_Pi 3.141592f
#define Math_Tau 6.283185f
#define Math_RadToDeg 57.29577f
#define Math_DegToRad 0.01745329f

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

#define Flag(x) (1 << x)
#define Has_Flag(x, y) (x & y)
#define Enable_Flag(x, y) (x |= y)
#define Disable_Flag(x, y) (x &= ~y)

#define Kilobytes(b) (b * UINT64_C(1024))
#define Megabytes(b) (Kilobytes(b) * UINT64_C(1024))
#define Gigabytes(b) (Megabytes(b) * UINT64_C(1024))

//local imports
#include "rituals_math.cpp"
#include "rituals_game.cpp"
void switch_state(Game_State newstate);

#include "rituals_renderer.cpp"
#include "rituals_animations.cpp"
#include "rituals_gui.cpp"

typedef struct World World;
struct Play_State
{
	usize current_time = 0, prev_time = 0;
	real accumulator = 0;
	World* world;

	bool running;

	Vec2i world_xy;
	bool delete_world_on_stop;
	bool save_world_on_stop;
};
Play_State* play_state;

//TODO(will) add world creation/deletion functions to menu_state area
struct Menu_State
{
	char save_dir[FilePathMaxLength];
	isize save_dir_len;
	tinydir_dir saves;
	bool saves_dirty;
	Gui_Text_Input_Handle handle;
	isize delete_index;
};
Menu_State* menu_state;

#include "rituals_game_info.cpp"
#include "rituals_game_registry.cpp"

#include "rituals_inventory.cpp"

#include "rituals_tilemap.cpp"

#include "rituals_simulation.cpp"

#include "rituals_particles.cpp"

#include "rituals_world_area.cpp"
#include "rituals_world.cpp"
#include "rituals_entity_events.cpp"

#include "rituals_play_state.cpp"

#include "rituals_serialization.cpp"

void init_menu_state()
{	
	menu_state = arena_push_struct(Game->game_arena, Menu_State);
	init_text_input_handle(&menu_state->handle, 256, Game->game_arena);
	menu_state->delete_index = -1;

	menu_state->save_dir_len = snprintf(menu_state->save_dir, FilePathMaxLength, "%ssave", Game->base_path);
	check_dir(menu_state->save_dir);
	tinydir_open_sorted(&menu_state->saves, menu_state->save_dir);
}

void stop_state()
{
	switch(Game->state) {
		case Game_State_None:
			break;
		case Game_State_Menu:
			break;
		case Game_State_Play:
			//TODO(will) write stop function
			play_state_stop();
			break;
		default:
			break;
	}
	Game->state = Game_State_None;
}

void start_state()
{

}

void switch_state(Game_State newstate)
{
	stop_state();
	Game->state = newstate;
	start_state();
}


bool b = false;
real r = 0;
void main_menu_update()
{
	game_set_scale(2.0f);
	Renderer->groups[0].offset = v2(0, 0);
	render_start();
	
	real lasty = 32;
	Body_Font->color = v4(1, 1, 1, 1);
	render_body_text("Rituals", v2(32, 32), false, 4.0f);
	lasty += 16;
	lasty += Body_Font->glyph_height * 4;
	gui_add_text_input(&menu_state->handle, 
			v2(32, lasty), 
			v2(256, Body_Font->glyph_height + 8), 
			"Enter new world name");

	if(gui_add_button(v2(256 + 32 + 16 , lasty), "Create", v2(64, 0))) {
		menu_state->saves_dirty = true;
	}
	if(Input->scancodes[SDL_SCANCODE_RETURN] == State_Just_Pressed) {
		menu_state->saves_dirty = true;
	}

	if(menu_state->saves_dirty) {
		tinydir_close(&menu_state->saves);
		char buf[FilePathMaxLength];
	 	snprintf(buf, FilePathMaxLength, "%ssave/%.*s",
				Game->base_path, 
				menu_state->handle.buffer_length, 
				menu_state->handle.buffer);
		check_dir(buf);
		
		tinydir_open_sorted(&menu_state->saves, menu_state->save_dir);
		menu_state->handle.buffer_length = 0;
		menu_state->saves_dirty = false;
	}
	lasty += 32 + 16;

	for(usize i = 0; i < menu_state->saves.n_files; ++i) {
		tinydir_file file;
		tinydir_readfile_n(&menu_state->saves, &file, i);
		if(file.is_dir && (file.name[0] != '.')) {
			if(gui_add_button(v2(32, lasty), file.name, v2(144, 0))) {
				recursively_delete_folder(file.path);
				start_play_state(file.name);
				Game->state = Game_State_Play;
			}
			if(menu_state->delete_index != i) {
				if(gui_add_button(v2(32 + 144 + 32, lasty), "Delete", v2(64, 0))) {
					menu_state->delete_index = i;
				}
			} else {

				if(gui_add_button(v2(32 + 144 + 32, lasty), "Cancel", v2(64, 0))) {
					menu_state->delete_index = -1;
				}
				if(gui_add_button(v2(32 + 144 + 32 + 64 + 32, lasty), "Confirm", v2(64, 0))) {
					printf("[%s] was deleted \n", file.path);
					recursively_delete_folder(file.path);
					menu_state->delete_index = -1;
					menu_state->saves_dirty = true;
				}
			}
			lasty += 32;
		}
	}

	if(menu_state->saves_dirty) {
		tinydir_close(&menu_state->saves);
		tinydir_open_sorted(&menu_state->saves, menu_state->save_dir);
	}

	render_draw(Game->size, Game->scale);
}


Sprite* boxes;


void test_init()
{
}

void test_update()
{
	game_set_scale(2.0);
	render_start();

	render_draw(Game->size, Game->scale);
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

void load_assets()
{
	isize w, h;
	Renderer->groups[0].texture = ogl_load_texture("data/graphics.png", &w, &h);
	Renderer->groups[0].texture_size.x = w;
	Renderer->groups[0].texture_size.y = h;
	Renderer->groups[1].texture = Renderer->groups[0].texture;
	Renderer->groups[1].texture_size = Renderer->groups[0].texture_size;

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
	init_play_state();
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

float tt = 0;
int main(int argc, char** argv)
{
	//stbi_set_flip_vertically_on_load(1);
	printf("%d \n", (usize)&(((Sprite*)(NULL))->center));
	
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		Log_Error("Could not init SDL"); 
		Log_Error(SDL_GetError());
		return 1;
	}

	//SDL_Version ver;
	//SDL_GetVersion(&ver);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//if(ver.minor >= 1) {
		//NOTE(will): "no matching glx visual" on some setups?
		SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1);
//	}
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

#if 1
	int ret = SDL_GL_SetSwapInterval(-1);
	if(ret == -1) {
		ret = SDL_GL_SetSwapInterval(1);
		if(ret == -1) {
			Log_Error("Could not get Vsync");
		}
	}
#endif 

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
		Game->temp_arena = new_memory_arena(Gigabytes(4), Game->meta_arena);
		Game->play_arena = new_memory_arena(Gigabytes(4), Game->meta_arena);
		Game->renderer_arena = new_memory_arena(Megabytes(256), Game->meta_arena);
		Game->world_arena = new_memory_arena(Megabytes(1024), Game->meta_arena);
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

		char* vert_src = 
#include "vert.glsl"
			;
		 char* frag_src = 
#include "frag.glsl"
			;
		init_renderer(Game->renderer, 4, Megabytes(32), vert_src, frag_src, Game->renderer_arena);
		
		Game->registry = arena_push_struct(Game->game_arena, Game_Registry);

		Registry = Game->registry;
		Renderer = Game->renderer;
		CurrentGroup = Renderer->groups;
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
					stop_state();
					running = false;
					return 0;
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
		game_calc_mouse_pos(Renderer->groups[0].offset);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		update();

		SDL_GL_SwapWindow(window);
		uint64 frame_ticks = SDL_GetTicks() - start_ticks;
		//60hz lock?
		if(frame_ticks < 16) {
			SDL_Delay(16 - frame_ticks);
		}
	}

	SDL_Quit();
	return 0;
}

