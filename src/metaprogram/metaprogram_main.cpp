/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* TODO(will) Things for metaprogramming tool
 *	- Finish up main features:
 *		- Reflection data generator
 *		- Serializer generator
 *		- These rely on structs
 *		- Header to be included in project being introspected
 *	- Code cleanup pass
 *		- Merge duplicated code
 *		- Possibly split into a few files
 *	- Optimization/Bug fixing pass
 *		- Already pretty fast
 *		- Figure out why it crashes on stb_image.c
 *		- Improve the robustness of system
 */ 


#if RITUALS_WINDOWS == 1
#define _CRT_SECURE_NO_WARNINGS 1
#include <windows.h>
#include <Shlwapi.h>
#include <malloc.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stddef.h>
#include <errno.h>

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

typedef real32 real;

typedef ptrdiff_t isize;
typedef size_t usize;

#define FilePathMaxLength (4096)

#define PathSeparator ("\\")
#define PathSeparatorChar ('\\')

#define Min(x,y) ((x<y)?x:y)
#define Max(x,y) ((x>y)?x:y)

#define Flag(x) (1 << x)
#define Has_Flag(x, y) (x & y)

#define Kilobytes(b) (b * UINT64_C(1024))
#define Megabytes(b) (Kilobytes(b) * UINT64_C(1024))
#define Gigabytes(b) (Megabytes(b) * UINT64_C(1024))

#define Log_Error(ii) printf("%s \n", (ii));

#include "../rituals_memory.cpp"
#include "../rituals_sort_macros.cpp"

struct Metaprogram_Core
{
	Memory_Arena base_arena;
	Memory_Arena work_arena;
	Memory_Arena temp_arena;
};
Metaprogram_Core* Metaprogram;
Memory_Arena* Base_Arena;
Memory_Arena* Work_Arena;
Memory_Arena* Temp_Arena;


char* load_file(char* filename, isize* size_out, Memory_Arena* arena)
{
	char* str = NULL;
	FILE* fp = fopen(filename, "r");
	if(fp != NULL) {
		fseek(fp, 0L, SEEK_END);
		isize size = ftell(fp);
		rewind(fp);
		str = arena_push_array(arena, char, size + 1);
		fread(str, sizeof(char), size, fp);
		str[size] = '\0';
		fclose(fp);
		if(size_out != NULL) {
			*size_out = size;
		}
	} else {
		printf("Could not open file %s\n", filename);
	}
	return str;
}


#include "metaprogram_parser.cpp"


int main(int argc, char** argv)
{
	//Core + arena initialization
	Memory_Arena base;
	init_memory_arena(&base, Megabytes(1));
	Metaprogram = arena_push_struct(&base, Metaprogram_Core);
	Metaprogram->base_arena = base;
	init_memory_arena(&Metaprogram->work_arena, Gigabytes(1));
	init_memory_arena(&Metaprogram->temp_arena, Megabytes(256));
	Base_Arena = &Metaprogram->base_arena;
	Work_Arena = &Metaprogram->work_arena;
	Temp_Arena = &Metaprogram->temp_arena;
	
	//getchar();

	//argc = 2;
	if(argc >= 2) {
		//char* str = load_file("src/rituals_game.cpp", NULL, Work_Arena);
		//char* file = load_file(filename, NULL, Work_Arena);
		Lexer lex;
		init_lexer(&lex, 1024, Work_Arena);
		init_lexer_file(get_next_file(&lex), argv[1], NULL, 0, Work_Arena);
		//init_lexer_file(get_next_file(&lex), "src\\rituals_math.cpp", NULL, 0, Work_Arena);

		Token* head = arena_push_struct(Work_Arena, Token);
		Token* start = head;
		Token* last = NULL;
		Token t;
		while(lexer_get_token(&lex, lex.main_file, &t)) {
			*head = t;
			head->next = arena_push_struct(Work_Arena, Token);
			head->prev = last;
			last = head;
			head = head->next;
		}
		head->prev = last;

		parse_tokens(&lex, start);
		Proc_Prototype* p = find_proc_prototypes(&lex, start, Work_Arena);
		Struct_Def* structdef = find_struct_defs(&lex, start, Work_Arena);

		fprintf(stderr, "Found %d procedures, %d structs \n",
				lex.procedures_count, lex.structs_count);

		//Print struct typedefs
		Struct_Def* s_head = structdef;
		do {
			if(s_head->name == NULL) continue;
			printf("typedef struct %s %s;\n", s_head->name, s_head->name);



		} while(s_head = s_head->next);

		//Print Meta_Types for structs
		Struct_Def** all_structs = arena_push_array(Work_Arena, 
				Struct_Def*, lex.structs_count + 16);

		s_head = structdef;
		printf("enum Meta_Type\n {\n");
		isize meta_index_counter = 0;
		do {
			if(s_head->name == NULL) continue;
			//start_temp_arena(Temp_Arena);
			print_struct_names(s_head, -1, 
					"Meta_Type", strlen("Meta_Type"), 
					all_structs, &meta_index_counter, 
					Temp_Arena);
			//end_temp_arena(Temp_Arena);
		} while(s_head = s_head->next);
		printf("};\n");

		// Print structs
		s_head = structdef;
		do {
			if(s_head->name == NULL) continue;
			print_struct(s_head);
		} while(s_head = s_head->next);

		// Print metadata
		
		
#if 1
		do {
			print_proc_prototype(p);
		} while (p = p->next);

#endif 


#if 0
		head = start;
		do {
			printf("%.*s ", head->len, head->start);
			if(head->next && head->next->kind == Token_CloseBrace) {
				printf("\n");
			}
			switch(head->kind) {
				case Token_Semicolon:
				case Token_OpenBrace:
				case Token_CloseBrace:
				case Token_CompilerDirective:
					printf("\n");
			}

				
		} while(head = head->next);
#endif
	}


	return 0;
}
