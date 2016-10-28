/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 * rituals_game.cpp
 * Contains a bunch of misc stuff and game struct.
 *  - Macros for sort/search functions: DIY hashtables
 *  - Memory allocators: linear (Memory_Arena), pool? free list?
 *  - Random functions: one of the xorshift style ones, splitmix too
 *  - ButtonState enum
 *  - Game_Input struct: stores all game input stuff
 *  - Game_Assets typedef: DECLARE THIS YOURSELF LATER
 *  - Game struct: where everything is kept
 */


// Generic error logging define
// TODO(will) implement better error logging
#define Log_Error(e) fprintf(stderr, "There was an error: %s \n", e);

$(exclude)
#include "rituals_sort_macros.cpp"
$(end)
#include "rituals_memory.cpp"
#include "rituals_random.cpp"
// Input stuff
enum Button_State
{
	State_Just_Released = -1,
	State_Released = 0,
	State_Pressed = 1,
	State_Just_Pressed = 2,

	Button_State_Count
};

#define InputTextCapacity (1024)
#ifndef REFLECTED
struct Game_Input
{
	isize num_keys_down;
	isize num_mouse_down;
	int8* scancodes;
	int8* keycodes;
	int8* mouse;
	int32 mouse_x;
	int32 mouse_y;
	Vec2i screen_mouse_pos;
	Vec2 mouse_pos;
	bool capture_tabs;
	bool capture_newlines;
	char* text;
	isize text_count;
};
#endif

#define _game_state(name, ...) Game_State_##name,
enum Game_State
{
	_game_state_list
};
#undef _game_state

#define _game_state(name, ...) name##_State* name;
#ifndef REFLECTED
struct Game_Main
{
	SDL_Window* window;
	OpenGL_Renderer* renderer;
	Game_State state;
	real scale;
	Vec2 size;
	Vec2i window_size;

	Memory_Arena* meta_arena;
	Memory_Arena* asset_arena;
	Memory_Arena* game_arena;
	Memory_Arena* temp_arena;
	Memory_Arena* renderer_arena;

	//TODO(will) possibly come up with another type of arena for storing
	// static game info: world areas, loaded tile types, etc
	Memory_Arena* registry_arena;
	Memory_Arena* world_arena;
	Memory_Arena* play_arena;

	const char* base_path;
	isize base_path_length;

	Spritefont* body_font;
	Spritefont* title_font;

	Random r;

	Game_Registry* registry;

	Game_Input* input; 
	uint64 last_frame_time;

	_game_state_list
};
#endif
#undef _game_state

Game_Main* Game;
OpenGL_Renderer* Renderer;
Render_Group* CurrentGroup;
Game_Input* Input;
Spritefont* Body_Font;
Spritefont* Title_Font;
Game_Registry* Registry;

#ifndef REFLECTED
struct Platform
{
	const char* window_title;
	Vec2i window_size;
	real game_scale;
	int32 display_index;

	bool check_gl_attributes;

	const char* vertex_shader;
	const char* frag_shader;

	SDL_Window* window;
};
#endif

int32 platform_init(Platform* platform)
{
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
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
			SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_Window* window = SDL_CreateWindow(platform->window_title, 
			SDL_WINDOWPOS_CENTERED_DISPLAY(platform->display_index), 
			SDL_WINDOWPOS_CENTERED_DISPLAY(platform->display_index),
			platform->window_size.x, platform->window_size.y,
			SDL_WINDOW_OPENGL | 
			SDL_WINDOW_RESIZABLE |
			SDL_WINDOW_MOUSE_FOCUS |
			SDL_WINDOW_INPUT_FOCUS);
	if(window == NULL) {
		Log_Error("Could not create window");
		Log_Error(SDL_GetError());
		return 1;
	}
	platform->window = window;
	SDL_GLContext glctx = SDL_GL_CreateContext(window);
	if(ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		Log_Error("Could not load OpenGL 3.3 functions...");
		return 1;
	}

	if(platform->check_gl_attributes) {
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
	return 0;
}


int32 game_init(Platform* platform)
{
	Game = Allocate(Game_Main, 1);
	Game->last_frame_time = 1;
	Game->window = platform->window;
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
	Input->mouse_pos = v2(0, 0);
	Input->text = arena_push_array(Game->game_arena, char, InputTextCapacity);
	Input->text_count = 0;
	Input->capture_newlines = false;
	Input->capture_tabs = false;

	init_random(&Game->r, time(NULL));
	//TODO(will) load window settings from file
	Game->window_size = platform->window_size;
	Game->scale = platform->game_scale;

	Game->renderer = arena_push_struct(Game->game_arena, OpenGL_Renderer);
	init_renderer(Game->renderer, 
			4, 
			Megabytes(32), 
			platform->vertex_shader,
			platform->frag_shader, 
			Game->renderer_arena);

	Game->registry = arena_push_struct(Game->game_arena, Game_Registry);

	Registry = Game->registry;
	Renderer = Game->renderer;
	CurrentGroup = Renderer->groups;
	game_allocate_states();
	return 0;
}

void game_update_screen()
{
	SDL_GetWindowSize(Game->window, &Game->window_size.x, &Game->window_size.y);
	glViewport(0, 0, Game->window_size.x, Game->window_size.y);
	Game->size = v2(Game->window_size) * Game->scale;
}

int32 game_start(Platform* platform)
{
	game_init_states();
	game_start_state();
	bool running = true;
	SDL_Event event;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//glClearColor(1, 1, 1, 1);

		printf("running...");
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
					game_stop_state();
					running = false;
					return 0;
					break;
				case SDL_WINDOWEVENT:
					game_update_screen();
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

		game_update_state();

		uint64 frame_ticks = SDL_GetTicks() - start_ticks;
#if 1
		//60hz lock?
		if(frame_ticks < 16) {
			SDL_Delay(16 - frame_ticks);
		}
#endif
		SDL_GL_SwapWindow(Game->window);
		Game->last_frame_time = SDL_GetTicks() - start_ticks;
	}
	return 0;
}

void platform_quit()
{
	SDL_Quit();
}



void game_switch_state(Game_State state)
{
	game_stop_state();
	Game->state = state;
	game_start_state();
}

#define start_state(state) game_start_##state##_state()
#define update_state(state) game_update_##state##_state()
#define init_state(state) game_init_##state##_state()
#define stop_state(state) game_stop_##state##_state()
#define free_state(state) game_free_##state##_state()

#define _game_state(name, ...) Game->name = arena_push_struct(Game->game_arena, name##_State);
void game_allocate_states()
{
	_game_state_list
}
#undef _game_state

#define _game_state(name, ...) game_init_##name##_state();
void game_init_states()
{
	_game_state_list
}
#undef _game_state

#define _game_state(name, ...) case Game_State_##name: game_start_##name##_state(); break;
void game_start_state()
{
	switch(Game->state) {
		_game_state_list
	}
}
#undef _game_state

#define _game_state(name, ...) case Game_State_##name: game_update_##name##_state(); break;
void game_update_state()
{
	switch(Game->state) {
		_game_state_list
	}
}
#undef _game_state

#define _game_state(name, ...) case Game_State_##name: game_stop_##name##_state(); break;
void game_stop_state()
{
	switch(Game->state) {
		_game_state_list
	}
}
#undef _game_state

real64 next_random_double()
{
	uint64 x = next_random_uint64(&Game->r);
	return (real64)x / UINT64_MAX;
}

real32 next_random_float()
{
	return (real32)(next_random_double(&Game->r));
}

real next_random() 
{
	return (real)(next_random_double(&Game->r));
}

real rand_range(real min, real max)
{
	return next_random_double(&Game->r) * (max - min) + min;
}

int32 rand_range_int(real min, real max)
{
	return (int32)(next_random_double(&Game->r) * (max - min + 1) + min);
}

void game_set_scale(real scale)
{
	Game->scale = scale;
	Game->size = v2(Game->window_size) * (1.0f / Game->scale);
}

void game_calc_mouse_pos(Vec2 offset)
{
	Input->mouse_pos = v2(Input->mouse_x / Game->scale, Input->mouse_y / Game->scale) + offset;
}


void input_text_append_char(char c)
{
	if((Input->text_count + 1) < InputTextCapacity) {
		Input->text[Input->text_count++] = c;
	}
}


bool is_valid_filename_char(char c)
{
	return !((c == '?') || 
			(c == '/') || 
			(c == '\\') ||
			(c == '<') ||
			(c == '>') ||
			(c == '|') || 
			(c == '*') ||
			(c == ':') || 
			(c == '"') ||
			(c <= 31));
}

char replace_filename_char(char c, char replace)
{
	if(!is_valid_filename_char(c)) return replace;
	return c;
}

isize replace_invalid_filename_chars(char* str, isize len, char replace)
{
	for(isize i = 0; i < len; ++i) {
		if(!is_valid_filename_char(str[i])) {
			str[i] = replace;
		}
	}
	while(isspace(str[len]) || (str[len] == '.')) len--;
	return len;
}

isize append_input_text(char* str, isize str_cap, isize str_len, isize insert_from_end=0)
{
	if(Input->text_count <= 0) return str_len;
	isize index = str_len;
	start_temp_arena(Game->temp_arena);
	char* extra_chars = NULL;
	if((insert_from_end <= str_len) && (insert_from_end != 0)) {
		index -= insert_from_end;
		if(index >= 0) {
			extra_chars = arena_push_array(Game->temp_arena, char, insert_from_end + 32);
			memset(extra_chars, 0, insert_from_end + 32);
			memcpy(extra_chars, str + index, insert_from_end);
			extra_chars[insert_from_end] = '\0';
		} else {
			index = 0;
		}
	}

	if((str_len + Input->text_count) > str_cap) {
		if(Input->text[0] != '\b')
			Input->text_count = str_cap - str_len;
	}
	isize bufsize = Input->text_count + str_len;
	char* buffer = arena_push_array(Game->temp_arena, char, bufsize + 32);
	memset(buffer, 0, bufsize);
	memcpy(buffer, str, index);
	memcpy(buffer + index, Input->text, Input->text_count);
	if(extra_chars != NULL) {
		memcpy(buffer + index + Input->text_count, extra_chars, insert_from_end);
	}

	isize idx = 0;
	bool contains_bs = false;
	char* buffer2 = NULL; 
	for(isize i = 0; i < Input->text_count; ++i) {
		if(Input->text[i] == Backspace) {
			contains_bs = true;
			break;
		}
	}
	contains_bs = true;
	if(contains_bs) {
		buffer2 = arena_push_array(Game->temp_arena, char, bufsize + 32);
		memset(buffer2, 0, bufsize);
		for(isize i = 0; i < bufsize; ++i) {
			if(buffer[i] != Backspace) {
				buffer2[idx] = buffer[i];
				idx++;
			} else if(idx != 0) {
				if(buffer[i+1] == '\7') {
					while(idx >= 0 && isspace(buffer[idx])) idx--;
					while(idx >= 0 && !isspace(buffer[idx])) idx--;
					while(idx >= 0 && isspace(buffer[idx])) idx--;
					idx++;
					i++;
				} else {
					idx--;
				}
			} else {
				if(buffer[i] == Backspace && buffer[i+1] == '\7') {
					i++;
				}
			}
		}
	} else {
		buffer2 = buffer;
		idx = str_len + Input->text_count;
	}
	memcpy(str, buffer2, idx);
	
	Input->text_count = 0;
	if(idx > str_cap) {
		idx = str_cap;
	}
	end_temp_arena(Game->temp_arena);
	return idx;
}

enum Direction 
{
	Direction_North,
	Direction_South,
	Direction_East,
	Direction_West
};

