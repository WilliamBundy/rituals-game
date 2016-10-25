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
#define Log_Error(e) printf("There was an error: %s \n", e);

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

enum Game_State
{
	Game_State_None,
	Game_State_Menu,
	Game_State_Play
};



// Game struct
//typedef struct Debug_Log Debug_Log;
typedef struct Spritefont Spritefont;
typedef struct OpenGL_Renderer OpenGL_Renderer;
typedef struct Render_Group Renderer_Group;
typedef struct Game_Registry Game_Registry;

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

	//Debug_Log* log;
	Game_Input* input;

	uint64 last_frame_time;
};

Game_Main* Game;
OpenGL_Renderer* Renderer;
Renderer_Group* CurrentGroup;
Game_Input* Input;
Spritefont* Body_Font;
Spritefont* Title_Font;

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

#define Backspace ('\b')
#define BackspaceStr ("\b")

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

