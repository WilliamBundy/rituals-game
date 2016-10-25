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
#define Enable_Flag(x, y) (x |= y)
#define Disable_Flag(x, y) (x &= ~y)

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
	FILE* fp = fopen(filename, "rb");
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
	//flags
	// -p --prototypes
	// -t --typedefs
	// -s --structs
	// -m --metadata
	bool pflag, tflag, sflag, mflag;
	pflag = tflag = sflag = false;
	char* file = NULL;
	for(isize i = 0; i < argc; ++i) {
		if(argv[i][0] == '-') {
			switch(argv[i][1]) {
				case 'p':
					pflag = true;
					break;
				case 't':
					tflag = true;
					break;
				case 's':
					sflag = true;
					break;
				case 'm':
					mflag = true;
					break;
				default:
					fprintf(stderr, "Unknown flag: %s \n", argv[i]);
			}
		} else {
			file = argv[i];
		}
	}
	
	bool istherework = pflag || tflag || sflag || mflag;
	if(file == NULL) {
		fprintf(stderr, "No input files.\n");
		return 1;
	} else if(!istherework) {
		fprintf(stderr, R"foo(Error: No work defined. Use flags:
	-p		Generate function prototypes
	-t 		Generate struct/union/enum typedefs
	-s		Copy struct definitions from source.
	-m		Generate reflection/introspection data
	)foo");
		return 1;
	}

	if(file != NULL && istherework) {
		//char* str = load_file("src/rituals_game.cpp", NULL, Work_Arena);
		//char* file = load_file(filename, NULL, Work_Arena);
		Lexer lex;
		init_lexer(&lex, 1024, Work_Arena);
		init_lexer_file(get_next_file(&lex), file, NULL, 0, Work_Arena);
		if(lex.files[0].head == NULL) {
			fprintf(stderr, "Error: File %s was null\n", lex.files[0].filename);
		}
		//init_lexer_file(get_next_file(&lex), "src\\rituals_math.cpp", NULL, 0, Work_Arena);

		Token* head = arena_push_struct(Work_Arena, Token);
		Token* start = head;
		Token* last = NULL;
		Token t;
		while(lexer_get_token(&lex, lex.main_file, &t)) {
			*head = t;
			if(head->kind == Token_EndOfFile) {
				head->next = NULL;
				break;
			}
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

		Struct_Def* s_head = structdef;
		Meta_Type* type_start = arena_push_struct(Work_Arena, Meta_Type);
		Meta_Type* type_head = NULL;
		do {
			if(s_head->name == NULL) continue;

			if(type_head == NULL) {
				type_head = get_types_in_struct(s_head, type_start, Work_Arena);
			} else {
				type_head = get_types_in_struct(s_head, type_head, Work_Arena); 
			}
		} while(s_head = s_head->next);
		type_head = type_start;
		s_head = structdef;

		if(tflag) {
			//Print struct typedefs
			do {
				if(s_head->name == NULL) continue;
				printf("typedef %s %s %s;\n", s_head->kind == StructKind_Struct ? "struct" : "union", s_head->name, s_head->name);
			} while(s_head = s_head->next);

			//Print Enum defs
			head = start; Hash enumhash = hash_literal("enum");
			do {
				if(head->hash == enumhash) {
					if(head->next->kind == Token_Identifier) {
						printf("typedef enum %.*s %.*s;\n", head->next->len, head->next->start,
								head->next->len, head->next->start);
					}
				}

			} while(head = head->next);

			//copy typedefs from program
			head = start;
			Hash typedefhash = hash_literal("typedef");
			Hash structhash = hash_literal("struct");
			Hash unionhash = hash_literal("union");
			do {
				head = parse_dollarsign_instructions(head);
				if(head->hash == typedefhash) {
					Token* tdef = head;
					Token* end = NULL;
					do {
						if(head->kind == Token_Semicolon) {
							end = head;
							break;
						}
					} while(head = head->next);
					if(end != NULL) {
						Token* subhead = tdef;
						bool doprint = true;
						do {
							if(subhead->hash == structhash || 
								subhead->hash == unionhash ||
								subhead->kind == Token_OpenParen ||
								subhead->kind == Token_CloseParen) {

								doprint = false;
								break;	
							}

							subhead = subhead->next;
						} while(subhead != end);
						if(doprint) {
							subhead = tdef;
							do {
								printf("%.*s", subhead->len, subhead->start);
								if(subhead->next->kind == Token_Identifier) {
									printf(" ");
								}
								if(subhead->next->kind == Token_Semicolon) {
									printf(";\n");
								}
								subhead = subhead->next;
							} while(subhead != end);

						}
					}
				}


			} while(head = head->next);
		}


		// find uniquetypes
		Meta_Type* unique_type_start = arena_push_struct(Work_Arena, Meta_Type);
		*unique_type_start = *type_start;
		unique_type_start->next = NULL;
		Meta_Type* unique_type_head = unique_type_start;
		s_head = structdef;
		do {
			bool eq = true;
			unique_type_head = unique_type_start;
			do {
				if(meta_type_equals(type_head, unique_type_head)) {
					eq = false;
					break;
				}
				if(unique_type_head->next == NULL) break;
				unique_type_head = unique_type_head->next;
			} while(1);
			if(eq) {
				s_head = structdef;
				do {
					if(type_head->hash == s_head->namehash) {
						//fprintf(stderr, "%s %s \n", type_head->name, s_head->name);
						eq = false; 
						break;
					}
				} while(s_head = s_head->next);
				if(!eq) continue;
				unique_type_head->next = arena_push_struct(Work_Arena, Meta_Type);
				unique_type_head = unique_type_head->next;
				*unique_type_head = *type_head;
				unique_type_head->next = NULL;
			}
		} while(type_head = type_head->next);

		isize meta_index_counter = 0;


		Struct_Def** all_structs = arena_push_array(Work_Arena, 
				Struct_Def*, lex.structs_count + meta_index_counter + 16);
		isize num_structs = meta_index_counter;
		fprintf(stderr, "Meta flag set: [%d] \n", mflag);
		if(mflag) {
			//Print Meta_Type enum
			s_head = structdef;
			printf("enum Meta_Type\n{\n");

			do {
				if(s_head->name == NULL) continue;
				start_temp_arena(Temp_Arena);
				print_struct_names(s_head, -1, 
						"Meta_Type_", strlen("Meta_Type_"), ",\n",
						all_structs, &meta_index_counter, 
						Temp_Arena);
				end_temp_arena(Temp_Arena);
			} while(s_head = s_head->next);

			num_structs = meta_index_counter;

			//TODO(will) This will change the index of system types
			// when new types are added. This makes the whole meta_type
			// system less robust as code changes with regard to serializiation
			type_start = unique_type_start;
			type_head = type_start;
			do {
				if(type_head->name == NULL) continue;
				printf("\tMeta_Type_%s,\n", type_head->name);
				type_head->type_index = meta_index_counter;
				meta_index_counter++;
			} while(type_head = type_head->next);

			printf("};\n");

			// Print names
			s_head = structdef;
			printf("const char* Meta_Type_Names[] = {\n");


			do {
				if(s_head->name == NULL) continue;
				start_temp_arena(Temp_Arena);
				print_struct_names(s_head, -1, 
						"\"", strlen("\""), "\",\n",
						NULL, NULL, 
						Temp_Arena);
				end_temp_arena(Temp_Arena);
			} while(s_head = s_head->next);
			type_start = unique_type_start;
			type_head = type_start;
			do {
				if(type_head->name == NULL) continue;
				printf("\t\"%s\",\n", type_head->name);
			} while(type_head = type_head->next);
			printf("};\n");



			// Print type_of overloads
#if 1
			s_head = structdef;
			type_start = unique_type_start;
			type_head = type_start;
			do {
				if(type_head->name == NULL) continue;
				printf("static inline Meta_Type type_of(const %s* object) { return Meta_Type_%s; }\n", type_head->name, type_head->name);
			} while(type_head = type_head->next);

			do {

				if(s_head == NULL) break;
				if(s_head->name == NULL) continue;
				printf("static inline Meta_Type type_of(const %s* object) { return Meta_Type_%s; }\n", s_head->name, s_head->name);

			} while(s_head = s_head->next);
#endif 
		}


		if(sflag) {
			// Print structs
			s_head = structdef;
			do {
				if(s_head->name == NULL) continue;
				print_struct(s_head);
			} while(s_head = s_head->next);
		}


		if(mflag) {
			// Print metadata
			print_metaprogram_types();
			for(isize i = 0; i < num_structs; ++i) {
				print_reflection_data(all_structs[i]);
			}
			printf("const Meta_Struct_Info All_Meta_Struct_Info[] = {\n");
			for(isize i = 0; i < num_structs; ++i) {
				print_struct_info(all_structs[i], "\t", ",\n");
			}
			printf("};\n\n");

			print_metaprogram_get_struct_info_proc();


			for(isize i = 0; i < num_structs; ++i) {
				Struct_Def* def = all_structs[i];
				if(def->is_anon_member) continue;
				printf("static inline const Meta_Struct_Info* get_struct_info(%s* object) { return get_struct_info(%s); } \n",
						def->name, def->meta_type_name);
			}
		}
		


		if(pflag) {
			Hash statichash = hash_literal("static");
			Hash inlinehash = hash_literal("inline");
			do {
				bool quit = false;
				for(isize i = 0; i < p->decorators_count; ++i) {
					Hash termhash = hash_string(p->decorators[i], strlen(p->decorators[i]));
					if(termhash == statichash || termhash == inlinehash) {
						quit = true;
						break;
					}
				}
				if(quit) continue;
				print_proc_prototype(p);
			} while (p = p->next);
		}
	}

	return 0;
}
