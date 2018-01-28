#if 0
/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

// Linear allocator
// TODO(will) use mmap on linux to map virtual memory 

#define arena_push_struct(arena, type) ((type*)arena_push(arena, sizeof(type)))
#define arena_push_array(arena, type, count) ((type*)arena_push(arena, sizeof(type) * count))

#ifndef REFLECTED
struct Memory_Arena
{
	uint8* data;
	isize capacity, head;
	isize temp_head;
	Memory_Arena* next;
};
#endif

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
#endif
