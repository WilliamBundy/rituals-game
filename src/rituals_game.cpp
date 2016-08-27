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


// Sort/Search macros
#define GenerateQuicksortForType(func_name, T, Member_Macro) \
void func_name(T* array, isize count) \
{ \
	if(count > 1) { \
		T tmp = array[0]; \
		array[0] = array[count / 2]; \
		array[count / 2] = tmp; \
		isize pivot = 0; \
		for(isize i = 1; i < count; ++i) { \
			if(Member_Macro(array[i]) < Member_Macro(array[0])) { \
				tmp = array[++pivot]; \
				array[pivot] = array[i]; \
				array[i] = tmp; \
			} \
		} \
		tmp = array[0]; \
		array[0] = array[pivot]; \
		array[pivot] = tmp; \
		func_name(array, pivot); \
		func_name(array + pivot + 1, count - (pivot + 1)); \
	} \
}

#define GenerateInsertionSortForType(func_name, T, Member_Macro) \
void func_name(T* array, isize count) \
{ \
	for(isize i = 1; i < count; ++i) { \
		T x = array[i]; \
		isize j = i - 1; \
		while((j >= 0) && (Member_Macro(array[j]) > Member_Macro(x))) { \
			array[j + 1] = array[j]; \
			j--; \
		} \
		array[j+1] = x; \
	} \
}

#define GenerateIntrosortForType(func_name, T, Cutoff, Member_Macro) \
void func_name(T* array, isize count) \
{ \
	if(count > 1) \
	if(count > Cutoff) { \
		T tmp = array[0]; \
		array[0] = array[count / 2]; \
		array[count / 2] = tmp; \
		isize pivot = 0; \
		for(isize i = 1; i < count; ++i) { \
			if(Member_Macro(array[i]) < Member_Macro(array[0])) { \
				tmp = array[++pivot]; \
				array[pivot] = array[i]; \
				array[i] = tmp; \
			} \
		} \
		tmp = array[0]; \
		array[0] = array[pivot]; \
		array[pivot] = tmp; \
		func_name(array, pivot); \
		func_name(array + pivot + 1, count - (pivot + 1)); \
	} else for(isize i = 1; i < count; ++i) { \
		T x = array[i]; \
		isize j = i - 1; \
		while((j >= 0) && (Member_Macro(array[j]) > Member_Macro(x))) { \
			array[j + 1] = array[j]; \
			j--; \
		} \
		array[j+1] = x; \
	} \
}





// Returns -1 on fail to find.
#define GenerateBinarySearchForType(func_name, T, K, Member_Key_Macro) \
isize func_name(K key, T* array, isize count) \
{ \
	if(count == 0) return -1; \
	isize min = 0, max = count - 1, mid = 0; \
	K localkey; \
	while(min <= max) { \
		mid = (min + max) / 2; \
		localkey = Member_Key_Macro(array[mid]); \
		if(localkey == key) { \
			return mid; \
		} else if(localkey < key) { \
			min = mid + 1; \
		} else { \
			max = mid - 1; \
		} \
	} \
	return -1; \
} 
 

#define _passthru_macro(x) (x) 
#define _generate_sort_and_search_for_numeric_type(t) \
	GenerateQuicksortForType(t##_sort, t, _passthru_macro) \
	GenerateBinarySearchForType(t##_search, t, t, _passthru_macro) 
_generate_sort_and_search_for_numeric_type(real);
_generate_sort_and_search_for_numeric_type(real32);
_generate_sort_and_search_for_numeric_type(real64);
_generate_sort_and_search_for_numeric_type(uint8);
_generate_sort_and_search_for_numeric_type(uint16);
_generate_sort_and_search_for_numeric_type(uint32);
_generate_sort_and_search_for_numeric_type(uint64);
_generate_sort_and_search_for_numeric_type(int8);
_generate_sort_and_search_for_numeric_type(int16);
_generate_sort_and_search_for_numeric_type(int32);
_generate_sort_and_search_for_numeric_type(int64);
_generate_sort_and_search_for_numeric_type(usize);
_generate_sort_and_search_for_numeric_type(isize);


// Generic error logging define
// TODO(will) implement better error logging
#define Log_Error(e) printf("There was an error: %s \n", e);


// Linear allocator
// TODO(will) implement a pool allocator

#define arena_push_struct(arena, type) ((type*)arena_push(arena, sizeof(type)))
#define arena_push_array(arena, type, count) ((type*)arena_push(arena, sizeof(type) * count))

struct Memory_Arena
{
	uint8* data;
	isize capacity, head;
	isize temp_head;
	Memory_Arena* next;
};

static inline isize mem_align_4(isize p)
{
	usize mod = p & 3;
	return (mod ? p + 4 - mod : p);
}

void init_memory_arena(Memory_Arena* arena, usize size)
{
#if RITUALS_WINDOWS
	arena->data = (uint8*)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#elif RITUALS_LINUX
	arena->data = (uint8*)calloc(sizeof(uint8), size);
#endif
	arena->capacity = (isize)size;
	arena->head = (isize)arena->data;
	arena->temp_head = -1;
	arena->next = NULL;
}

uint8* arena_push(Memory_Arena* arena, isize size)
{
	isize old_head = arena->head;
	isize new_head = mem_align_4(size + arena->head);
	if((new_head - (isize)arena->data) > arena->capacity) {
		Log_Error("An arena was filled");
		return NULL;
	}
	arena->head = new_head;
	return (uint8*) old_head;

}

void start_temp_arena(Memory_Arena* arena)
{
	arena->temp_head = arena->head;
}

void end_temp_arena(Memory_Arena* arena)
{
#if RITUALS_WINDOWS
	VirtualAlloc((char*)arena->temp_head, 
			arena->head - arena->temp_head, 
			MEM_RESET, PAGE_EXECUTE_READWRITE);
#elif RITUALS_LINUX == 1
	memset((void*)arena->data, 0, arena->head - arena->temp_head);
#endif
	arena->head = arena->temp_head;
	arena->temp_head = -1;
}

void clear_arena(Memory_Arena* arena)
{
	//memset(arena->data, 0, arena->head - (isize)arena->data);
#if RITUALS_WINDOWS == 1
	VirtualFree(arena->data, arena->capacity, MEM_DECOMMIT);
	arena->data = (uint8*)VirtualAlloc(arena->data, arena->capacity, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#elif RITUALS_LINUX == 1
	free(arena->data);
	arena->data = (uint8*)calloc(sizeof(uint8), arena->capacity);
#endif

	if(arena->data == NULL) {
		Log_Error("There was an error recommitting memory");
	}
	
	arena->head = (isize)arena->data;
	arena->temp_head = -1;
}


Memory_Arena* new_memory_arena(usize size, Memory_Arena* src)
{
	Memory_Arena* arena = (Memory_Arena*)arena_push(src, sizeof(Memory_Arena));
	init_memory_arena(arena, size);
	return arena;
}

// Random number generators
static inline uint64 _splitmix64(uint64* x)
{
	*x += UINT64_C(0x9E3779B97F4A7C15);
	uint64 z = *x;
	z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
	z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
	return z ^ (z >> 31);	
}

static inline uint64 _rotate_left(const uint64 t, int64 k)
{
	return (t << k) | (t >> (64 - k));
}

struct Random
{
	uint64 x, y;
};

uint64 next_random_uint64(Random* r)
{
	uint64 a = r->x;
	uint64 b = r->y;
	uint64 result = a + b;

	b ^= a;
	r->x = _rotate_left(a, 55) ^ b ^ (b << 14);
	r->y = _rotate_left(b, 36);
	return result;
}

void init_random(Random* r, uint64 seed) 
{
	_splitmix64(&seed);
	r->x = _splitmix64(&seed);
	r->y = _splitmix64(&seed);
	next_random_uint64(r);
}

real64 next_random_double(Random* r)
{
	uint64 x = next_random_uint64(r);
	return (real64)x / UINT64_MAX;
}

real32 next_random_float(Random* r)
{
	return (real32)(next_random_double(r));
}

real next_random(Random* r) 
{
	return (real)(next_random_double(r));
}

real rand_range(Random* r, real min, real max)
{
	return next_random_double(r) * (max - min) + min;
}

int32 rand_range_int(Random* r, real min, real max)
{
	return (int32)(next_random_double(r) * (max - min) + min);
}

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
typedef struct Renderer_Group Renderer_Group;
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
};

Game_Main* Game;
OpenGL_Renderer* Renderer;
Renderer_Group* Default_Group;
Game_Input* Input;
Spritefont* Body_Font;
Spritefont* Title_Font;

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

