typedef uint64 Hash;

Hash hash_string(char* c, int len)
{
	Hash hash = 0;
	for(isize i = 0; i < len; ++i) {
		hash = c[i] + hash * 65599;
	}
	return hash;
}

static inline int32 dec_str_to_int(char* str, isize len)
{
	int32 result = 0;
	for(isize i = 0; i < len; ++i) {
		result = result * 10 + str[i] - '0';
	}
	return result;
}


#define hash_literal(c) hash_string(c, sizeof(c) - 1)

enum Token_Kind
{
	Token_Unknown,

	Token_Semicolon,
	Token_Backslash,
	Token_OpenBrace,
	Token_CloseBrace,
	Token_OpenParen,
	Token_CloseParen,
	Token_OpenBracket,
	Token_CloseBracket,
	Token_Tilde,
	Token_ExclamationMark,
	Token_DollarSign,
	Token_Modulo,
	Token_Caret,
	Token_Ampersand,
	Token_Asterisk,
	Token_Pipe,
	Token_Dot,
	Token_Comma,
	Token_QuestionMark,
	Token_Colon,
	Token_Plus,
	Token_Minus,
	Token_Divide, 
	Token_Equals,
	Token_GreaterThan,
	Token_LessThan,
	
	Token_CompilerDirective,
	Token_MetaprogramDirective,
	Token_Char,
	Token_String,
	Token_Number,
	Token_Identifier,

	Token_Integer,
	Token_Float,

	Operator_LogicalAnd,
	Operator_LogicalOr,
	Operator_BooleanEquals,
	Operator_BooleanNotEquals,
	Operator_BooleanGreaterEquals,
	Operator_BooleanLessEquals,
	Operator_PtrMemberAccess,
	Operator_Decrement,
	Operator_Increment,

	Token_EndOfFile,
	Token_Kind_Count,
	
};

struct Lexer_Location
{
	isize file;
	isize line;
	isize offset;
};

struct Token
{
	Token_Kind kind;	
	char* start;
	isize len;
	Hash hash;

	Lexer_Location location;

	Token *prev, *next;
};

void print_token(Token* t, Token* start)
{
	isize sp = (isize) start;
	printf("%d<-[%d %d %.*s]->%d", ((isize)t->prev - sp) / sizeof(Token), ((isize)t - sp) / sizeof(Token), t->kind, t->len, t->start, ((isize)t->next - sp) / sizeof(Token));
}



struct Lexer_File
{
	char* filename;
	isize pathlen;
	isize index;
	isize length;
	Lexer_Location location;
	char* start;
	char* head;

};

void init_lexer_file(Lexer_File* file, char* filename, char* prev_path, isize prev_path_len, Memory_Arena* arena)
{
	isize len = strlen(filename);
	char* filename_copy = arena_push_array(arena, char, len + prev_path_len+1);
	memcpy(filename_copy, prev_path, prev_path_len);
	memcpy(filename_copy + prev_path_len, filename, len);
	filename_copy[len + prev_path_len] = '\0';
	//printf(">>>>%s \n", filename_copy);
	len = strlen(filename_copy);
	isize pathlen = 0;
	isize extlen = 0;
	for(isize i = len - 1; i >= 0; --i) {
		if(filename_copy[i] == '.') {
			extlen = i;
		}
		if(filename_copy[i] == '/' || filename_copy[i] == '\\') {
			pathlen = i + 1;
			break;
		}
	}
	Hash exthash = hash_string(filename_copy + extlen, len - extlen);
	if(exthash != hash_literal(".i") && exthash != hash_literal(".c") && exthash != hash_literal(".cpp") && exthash != hash_literal(".h")) {
		if(Metaprogram->verbose) fprintf(stderr, "Skipping %s\n", filename_copy);
		file->pathlen = pathlen;
		file->filename = filename_copy;
		file->start = NULL;
		file->head = NULL;
		return;
	}
	file->pathlen = pathlen;
	file->filename = filename_copy;
	file->start = load_file(filename_copy, &file->length, arena);
	file->head = file->start;
}


struct Lexer
{
	Lexer_File* main_file;

	Lexer_File* files;
	isize files_count, files_capacity;

	isize structs_count;
	isize procedures_count;
};

void init_lexer(Lexer* lex, isize file_capacity, Memory_Arena* arena)
{
	lex->files = arena_push_array(arena, Lexer_File, file_capacity);
	lex->files_count = 0;
	lex->files_capacity = file_capacity;
	lex->main_file = lex->files;
	lex->main_file->index = 0;

	lex->structs_count = 0;
	lex->procedures_count = 0;
}

Lexer_File* get_next_file(Lexer* lex)
{
	Lexer_File* file =  lex->files + lex->files_count++;
	file->index = lex->files_count - 1;
	return file;
}



bool is_space(char c)
{
	return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\f';
}

bool is_number(char c)
{
	return c >= '0' &&  c <= '9'; 
}

bool is_valid_identifier(char c)
{
	return is_number(c) || ((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')) || c == '_';
}


#define nextchar do { \
		if(f->head[0] == '\n') { \
			f->location.line++; \
			f->location.offset = 0; \
		} else { \
			f->location.offset++; \
		} \
		f->head++; \
} while(0) 

#define valid ((f->head[0] != '\0') && (f->head[0] != EOF))

bool lexer_get_token(Lexer* lexer, Lexer_File* f, Token* t) 
{ 
	while(valid) {
		if(is_space(f->head[0])) {
			nextchar;
		} else if(f->head[0] == '/') {
			if(f->head[1] == '*') {
				nextchar;
				nextchar;
				while(valid && !(f->head[0] == '*' &&
							f->head[1] == '/')) {
					nextchar;
				}
				nextchar;
				nextchar;
			} else if(f->head[1] == '/') {
				nextchar;
				nextchar;
				if(f->head[0] == '\\') {
					nextchar;
				}
				while(valid && (f->head[0] != '\n')) {
					nextchar;	
				}
				nextchar;
			} else {
				break;
			}
		} else {
			break;
		}
	}
	
	t->start = f->head;
	t->len = 1;
	t->kind = Token_Unknown;
	f->location.file = f->index;
	t->location = f->location;
	t->next = NULL;

	//printf("%d<%c> ", f->head[0], f->head[0]);
	bool hit = true;
	switch(f->head[0]) {
		case ';':
			t->kind = Token_Semicolon;
			break;
		case '\\':
			t->kind = Token_Backslash;
			break;
		case '{':
			t->kind = Token_OpenBrace;
			break;
		case '}':
			t->kind = Token_CloseBrace;
			break;
		case '(':
			t->kind = Token_OpenParen;
			break;
		case ')':
			t->kind = Token_CloseParen;
			break;
		case '[':
			t->kind = Token_OpenBracket;
			break;
		case ']':
			t->kind = Token_CloseBracket;
			break;
		case '~':
			t->kind = Token_Tilde;
			break;
		case '!':
			t->kind = Token_ExclamationMark;
			break;
		case '#':
			t->kind = Token_CompilerDirective;
			t->location.offset++;
			t->start++;
			nextchar;
			while(valid && (f->head[0] != '\n')) {
				if(f->head[0] == '/') {
					if(f->head[1] == '/') {
						break;
					} else if(f->head[1] == '*') {
						while(valid) {
							if(f->head[0] == '*' && f->head[1] == '/') {
								break;
							}
							nextchar;
						}
					}
				} else if(f->head[0] == '\\') {
					nextchar;
				}
				nextchar;
			}
			t->len = f->head - t->start;
			break;
		case '$': {
			char* oldhead = f->head;
			nextchar;
			while(valid) {
				if(is_space(f->head[0])) {
					nextchar;
				} else if(f->head[0] == '/') {
					if(f->head[1] == '*') {
						nextchar;
						nextchar;
						while(valid && !(f->head[0] == '*' &&
									f->head[1] == '/')) {
							nextchar;
						}
						nextchar;
						nextchar;
					} else if(f->head[1] == '/') {
						nextchar;
						nextchar;
						if(f->head[0] == '\\') {
							nextchar;
						}
						while(valid && (f->head[0] != '\n')) {
							nextchar;	
						}
						nextchar;
					} else {
						break;
					}
				} else {
					break;
				}
			}
			if(f->head[0] == '(') {
				t->kind = Token_MetaprogramDirective;
				nextchar;
				t->start = f->head;
				t->location = f->location;
				int brace_depth = 0;
				while(brace_depth >= 0) {
					nextchar;
					if(f->head[0] == '(') {
						brace_depth++;
					} else if (f->head[0] == ')') {
						brace_depth--;
					}
					if(!valid) {
						break;
					}
				}
				t->len = f->head - t->start;
			} else {
				t->kind = Token_DollarSign;
			}


		} break;
		case '%':
			t->kind = Token_Modulo;
			break;
		case '^':
			t->kind = Token_Caret;
			break;
		case '&':
			t->kind = Token_Ampersand;
			break;
		case '*':
			t->kind = Token_Asterisk;
			break;
		case '|':
			t->kind = Token_Pipe;
			break;
		case '.':
			t->kind = Token_Dot;
			break;
		case ',':
			t->kind = Token_Comma;
			break;
		case '?':
			t->kind = Token_QuestionMark;
			break;
		case ':':
			t->kind = Token_Colon;
			break;
		case '+':
			t->kind = Token_Plus;
			break;
		case '-':
			t->kind = Token_Minus;
			break;
		case '/':
			t->kind = Token_Divide;
			break;
		case '=':
			t->kind = Token_Equals;
			break;
		case '>':
			t->kind = Token_GreaterThan;
			break;
		case '<':
			t->kind = Token_LessThan;
			break;
		default:
			hit = false;
			break;
	}
	if(hit) {
		nextchar;
	} else switch(f->head[0]) {		
		case '\'':
			t->kind = Token_Char;
			t->location.offset++;
			t->start++;
			nextchar;
			while(valid) {
				nextchar;
				if(f->head[0] == '\'') {
					if(f->head[-1] != '\\') {
						break;
					} else {
						if(f->head[-2] == '\\') {
							break;
						}
					}
				}
			}
			t->len = f->head - t->start;
			nextchar;
			break;
		case '"':
			t->kind = Token_String;
			t->location.offset++;
			t->start++;
			nextchar;
			while(valid) {
				if(f->head[0] == '"') {
					char* last = f->head - 1;
					if(last[0] != '\\') {
						break;
					}
				}
				nextchar;

				/*if(f->head[0] == '\n') {
					
					fprintf(stderr, ">>> Error, encountered newline in string literal\n");
					fprintf(stderr, ">>> line %d, char %d\n", f->location.line, f->location.offset);
					fprintf(stderr, ">>> %.*s \n", f->head - t->start, t->start);
					break;
				}*/
			}
			t->len = f->head - t->start;
			nextchar;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			t->kind = Token_Number;
			while(valid && is_number(f->head[0])) {
				nextchar;
			}
			t->len = f->head - t->start;
			break;
		case '\0':
		case EOF:
			t->kind = Token_EndOfFile;
			return false;
		default:
			if(is_valid_identifier(f->head[0])) {
				t->kind = Token_Identifier;
				while(is_valid_identifier(f->head[0])) {
					nextchar;
				}
				t->len = f->head - t->start;
			} else {
				t->kind = Token_Unknown;
			}
			break;
	}
	t->hash = hash_string(t->start, t->len);

	return true;
}
#undef nextchar
#undef valid

void parse_number_tokens(Token* head)
{
	Token* next = head->next;
	if(head->kind == Token_Number) {
		if(next && next->kind == Token_Dot) {
			head->kind = Token_Float;
			head->len++;
			head->next = next->next;
			parse_number_tokens(head);
		} else {
			head->kind = Token_Integer;
		}
	} else if(head->kind == Token_Float) {
		if(next && next->kind == Token_Number) {
			head->len += next->len;
			head->next = next->next;
		}
	}
}

void parse_include_directive(Lexer* lex, Token* directive)
{
	start_temp_arena(Temp_Arena);
	char* buf = arena_push_array(Temp_Arena, char, directive->len + 1);
	memcpy(buf, directive->start, directive->len);
	buf[directive->len] = '\0';
	Token* head = arena_push_struct(Temp_Arena, Token);
	Token* start = head;

	{
		Token t;
		Lexer_File f;
		f.head = buf;
		f.start = buf;
		while(lexer_get_token(NULL, &f, &t)) {
			*head = t;
			head->next = arena_push_struct(Temp_Arena, Token);
			head = head->next;
		}
		head->next = NULL;
	}
	if(start->hash != hash_string("include", sizeof("include") - 1)) {
		end_temp_arena(Temp_Arena);
		return;
	}

	head = start;
	do {
		if(head->kind == Token_String) {
			char* filename = arena_push_array(Temp_Arena, char, head->len + 1);
			memcpy(filename, head->start, head->len);
			filename[head->len] = '\0';
			Lexer_File* file = get_next_file(lex);
			Lexer_File* including = lex->files + directive->location.file;
			init_lexer_file(file, filename, including->filename, including->pathlen, Work_Arena);
			if(file->start != NULL) {
				Token* new_file_head = arena_push_struct(Work_Arena, Token);
				Token* new_file_start = new_file_head;
				Token* last = directive;
				Token t;
				while(lexer_get_token(lex, file, &t)) {
					*new_file_head = t;
					new_file_head->next = arena_push_struct(Work_Arena, Token);
					new_file_head->prev = last;
					last = new_file_head;
					new_file_head = new_file_head->next;
				}
				Token* oldnext = directive->next;
				directive->next = new_file_start;
				new_file_start->prev = directive;
				new_file_head->next = oldnext;
				oldnext->prev = new_file_head;
			} 
			break;
		}
	} while(head = head->next);

	end_temp_arena(Temp_Arena);
}


Token* parse_metaprogram_directive(Token* t) 
{
	if(t->kind != Token_MetaprogramDirective) return t;
	Token* head = t;
	
	Hash excludehash = hash_literal("exclude");
	Hash endhash = hash_literal("end");

	if(t->hash == excludehash) {
		do {
			if(head->kind == Token_MetaprogramDirective) {
				if(head->hash == endhash) {
					head = head->next;
					break;
				}
			}
		} while(head = head->next);
	}


	

	/*
	if(t->kind != Token_DollarSign) return t;
	Token* next = t->next->next;
	Token* head = t;
	if(next->hash == hash_literal("exclude")) {
		do {
			if(next->kind == Token_DollarSign) {
				Token* tk = next->next->next;
				if(tk->kind == Token_Identifier) {
					if(tk->hash == hash_literal("end")) {
						head = tk->next->next;
						break;
					}
				}
			}
		} while(next = next->next);
	} 
	*/
	return head;
}

void parse_tokens(Lexer* lex, Token* start)
{
	Token* head = start;

	do {
		head = parse_metaprogram_directive(head);
		if(head->kind == Token_CompilerDirective && head->start[0] == 'i') {
			parse_include_directive(lex, head);
		}
	} while(head = head->next);

	head = start;

	do {
		Token* next;
		switch(head->kind) {
			case Token_DollarSign:
				head = parse_metaprogram_directive(head);
				break;
			case Token_Ampersand:
				next = head->next;
				if(next && next->kind == Token_Ampersand) {
					head->kind = Operator_LogicalAnd;
					head->len++;
					head->next = next->next;
				}
				break;
			case Token_Pipe:
				next = head->next;
				if(next && next->kind == Token_Pipe) {
					head->kind = Operator_LogicalOr;
					head->len++;
					head->next = next->next;
				}
				break;
			case Token_Equals:
				next = head->next;
				if(next && next->kind == Token_Equals) {
					head->kind = Operator_BooleanEquals;
					head->len++;
					head->next = next->next;
				}
				break;
			case Token_ExclamationMark:
				next = head->next;
				if(next && next->kind == Token_Equals) {
					head->kind = Operator_BooleanNotEquals;
					head->len++;
					head->next = next->next;
				}
				break;
			case Token_GreaterThan:
				next = head->next;
				if(next && next->kind == Token_Equals) {
					head->kind = Operator_BooleanGreaterEquals;
					head->len++;
					head->next = next->next;
				}
				break;
			case Token_LessThan:
				next = head->next;
				if(next && next->kind == Token_Equals) {
					head->kind = Operator_BooleanLessEquals;
					head->len++;
					head->next = next->next;
				}
				break;
			case Token_Number:
				parse_number_tokens(head);
				break;
			case Token_Minus:
				next = head->next;
				if(next && next->kind == Token_GreaterThan) {
					head->kind = Operator_PtrMemberAccess;
					head->len++;
					head->next = next->next;
				} else if(next && next->kind == Token_Minus) {
					head->kind = Operator_Decrement;
					head->len++;
					head->next = next->next;
				} else if(next && next->kind == Token_Number) {
					Token_Kind prevkind = Token_Unknown;
					if(head->prev != NULL) {
						prevkind = head->prev->kind;
					}
					if(prevkind != Token_Number &&
							prevkind != Token_Integer && 
							prevkind != Token_Float && 
							prevkind != Token_Identifier ) {
						head->kind = Token_Number;
						head->len += next->len;
						head->next = next->next;
						next = head->next;
						parse_number_tokens(head);
					}
				}
				break;
			case Token_Plus:
				next = head->next;
				if(next && next->kind == Token_Plus) {
					head->kind = Operator_Increment;
					head->len++;
					head->next = next->next;
				}
			default:
				break;
		}
	} while(head = head->next);

}

struct Proc_Arg
{
	char** terms;
	isize count;

	char* defaults;
};

void init_proc_arg(Proc_Arg* arg, isize count, Memory_Arena* arena)
{
	arg->terms = arena_push_array(arena, char*, count);
	arg->defaults = NULL;
	arg->count = 0;
}

struct Proc_Prototype
{
	Token *start, *end;
	char** decorators;
	isize decorators_count;
	char* name;
	Proc_Arg* args;
	isize args_count;

	Proc_Prototype* next;
};

void print_proc_prototype(Proc_Prototype* p)
{
	if(p->name == NULL) return;
	//TODO(will) figure out how to prototype generated functions, maybe?
	// Gotta parse #define stuff.
	if(p->decorators_count <= 0) return;


	for(isize i = 0; i < p->decorators_count; ++i) {
		printf("%s ", p->decorators[i]);
	}
	printf("%s(", p->name);

	for(isize i = 0; i < p->args_count; ++i) {
		Proc_Arg* a = p->args + i;
		for(isize j = 0; j < a->count; ++j) {
			printf("%s", a->terms[j]);
			if(j == a->count - 1) {
				//TODO(will): there's something dumb with the way C++ handles default arguments
				// so, we don't. 
				a->defaults = NULL;
				if(a->defaults == NULL) {
					if(i != p->args_count - 1) printf(", ");
				} else {
					printf(" %s", a->defaults);
					if(i != p->args_count - 1) printf(" ");
				}
			} else {
				printf(" ");
			}
		}
	} 

	if(p->args_count != 0) {
		if(p->args[p->args_count - 1].defaults == NULL) 
			printf(");");
		else 
			printf(";");
	}

	printf("\n");


}

Proc_Prototype* find_proc_prototypes(Lexer* lex, Token* start, Memory_Arena* arena)
{
	//NOTE(will) will not find C-style "struct Type function() {"
	Token* head = start;
	Token* next = NULL;
	Hash structhash = hash_literal("struct");
	Hash enumhash = hash_literal("enum");

	Proc_Prototype proc = {0};
	Proc_Prototype* proc_start = arena_push_struct(arena, Proc_Prototype);
	Proc_Prototype* proc_head = proc_start;

	do {
		head = parse_metaprogram_directive(head);
		if (head->kind == Token_Identifier) {
			if(head->hash != structhash && head->hash != enumhash) {
				//pattern: 
				//	mode 0 <identifiers...>
				//	mode 0 <open paren> 
				//	mode 1 <identifier, * identifier, comma...>
				//	mode 1 <close paren> 
				//	mode 2 <open brace>
				proc = {0};
				proc.decorators = arena_push_array(arena, char*, 256);
				proc.args = arena_push_array(arena, Proc_Arg, 256);
				proc.start = head;

				Token* sub_head = head;
				int32 mode = 0;
				Token* default_args_token = NULL;
				int32 paren_depth = 0;
				Proc_Arg* arg = proc.args;
				init_proc_arg(arg, 256, arena);
				bool quit = false;
				do {
					if(sub_head->kind == Token_Semicolon) {
						proc = {0};
						mode = -1;
						break;
					}
					sub_head = parse_metaprogram_directive(sub_head);
					switch(mode) {
						case 0:
							if (sub_head->kind == Token_Identifier) {
								char* buf = arena_push_array(arena, char, 256);
								memcpy(buf, sub_head->start, sub_head->len);
								int len = sub_head->len;
								next = sub_head->next;
								while(next->kind == Token_Asterisk) {
									buf[len++] = '*';
									next = next->next;
								}
								buf[len] = '\0';
								proc.decorators[proc.decorators_count++] = buf;
							} else if(sub_head->kind == Token_OpenParen) {
								arg = proc.args + proc.args_count++;
								init_proc_arg(arg, 256, arena);
								proc.name = proc.decorators[--proc.decorators_count];
								mode = 1;
							} else if(sub_head->kind == Token_Asterisk) {
								continue;
							}
							break;
						case 1:
							if(sub_head->kind == Token_CloseParen) {
								if(paren_depth == 0) {
									if(default_args_token != NULL) {
										char* start = default_args_token->start;
										isize len = sub_head->start - start + sub_head->len;
										char* buf = arena_push_array(arena, char, len+1);
										memcpy(buf, start, len);
										buf[len] = '\0';
										arg->defaults = buf;
										default_args_token = NULL;
									}
									mode = 2;
								} else {
									paren_depth--;
								}
							} else if(sub_head->kind == Token_Identifier) {
								if(default_args_token == NULL) {
									char* buf = arena_push_array(arena, char, 256);
									memcpy(buf, sub_head->start, sub_head->len);
									int len = sub_head->len;
									next = sub_head->next;
									while(next->kind == Token_Asterisk) {
										buf[len++] = '*';
										next = next->next;
									}
									buf[len] = '\0';
									arg->terms[arg->count++] = buf;
								}

							} else if (sub_head->kind == Token_Equals) {
								default_args_token = sub_head;
							} else if (sub_head->kind == Token_Comma) {
								if(paren_depth == 0) {
									if(default_args_token != NULL) {
										//TODO(will) code duplication
										char* start = default_args_token->start;
										isize len = sub_head->start - start + sub_head->len;
										char* buf = arena_push_array(arena, char, len+1);
										memcpy(buf, start, len);
										buf[len] = '\0';
										arg->defaults = buf;
										default_args_token = NULL;
									}
									arg = proc.args + proc.args_count++;
									init_proc_arg(arg, 256, arena);
								}
							} else if(sub_head->kind == Token_OpenParen) {
								paren_depth++;
							}
							break;
						case 2: 
							if(sub_head->kind == Token_OpenBrace) {
								int32 brace_depth = 0;
								sub_head = sub_head->next;
								do {
									if(sub_head->kind == Token_OpenBrace) {
										brace_depth++;
									} else if(sub_head->kind == Token_CloseBrace) {
										if(brace_depth == 0) {
									//		print_token(sub_head, start);
									//		printf("\n");
											quit = true;
											break;	
										}
										brace_depth--;
									}
								} while (sub_head = sub_head->next);
							}
							break;
					}
					if(quit) {
						head = sub_head;
						proc.end = head;
						*proc_head = proc;
						proc_head->next = arena_push_struct(arena, Proc_Prototype);
						lex->procedures_count++;
						proc_head = proc_head->next;
						break;
					}
					if (sub_head == NULL) break;
				} while(sub_head = sub_head->next);
			}
		}
		if (head == NULL) break;
	} while(head = head->next);
	return proc_start;
}


enum Struct_Kind
{
	StructKind_None,
	StructKind_Struct,
	StructKind_Union,
	StructKind_Member
};

typedef union Struct_Member Struct_Member;
#define StructMemberCapacity (256)
struct Struct_Def
{
	char* name;
	char* parentname;
	Struct_Kind kind;

	isize meta_index;
	char* meta_type_name;
	Hash namehash;

	bool is_anon_member;
	bool is_named_child;

	Struct_Member* members;
	Struct_Kind* member_kinds;
	isize member_count;

	Struct_Def* next;
}; 

typedef struct Meta_Type Meta_Type;
union Struct_Member
{
	struct {
		Struct_Def def;
		char** array_sizes;
		isize array_levels;
	} anon_struct;
	struct {
		Hash namehash;
		char* name;
		char** terms;
		isize count;

		Meta_Type* type;
		int32 asterisk_count;

		char** array_sizes;
		isize array_levels;
	} member_var;
};

void print_indent(int32 indent)
{
	for(isize i = 0; i < indent; ++i) {
		printf("\t");
	}
}

void print_struct_names(Struct_Def* def, isize index, char* prefix, isize prefix_len, char* suffix, Struct_Def** all_structs, isize* counter, Memory_Arena* arena)
{
	if(def->kind == StructKind_None) return;
	isize chars = 0;
	
	if(counter != NULL) {
		isize local_counter = *counter;
		Struct_Def** local_struct = all_structs + local_counter;

		local_struct[0] = def;
		def->meta_index = local_counter;
		*counter = local_counter + 1;
	}

	char* buf = arena_push_array(Work_Arena, char, 256);
	if(index == -1) {
		chars = snprintf(buf, 256, "%.*s%s", prefix_len, prefix, def->name);
	} else {
		if(def->name[0] == '\0') {
			//truly anonymous
			chars = snprintf(buf, 256, "%.*s%s%d", 
					prefix_len, prefix,
					def->kind == StructKind_Struct ?
					"struct" : "union",
					index);
			def->is_anon_member = true;
		} else {
			//has a variable name
			chars = snprintf(buf, 256, "%.*s%s",
					prefix_len, prefix,
					def->name);
			def->is_anon_member = true;
		}
	}
	chars++;
	chars++;
	if(all_structs != NULL) {
		def->meta_type_name = buf;
	}
	printf("\t%s%s", buf, suffix);
	char* new_prefix = arena_push_array(arena, char, chars + 256);
	prefix_len = chars;
	if(index == -1) {
		snprintf(new_prefix, chars, "%.*s%s_", prefix_len, prefix, def->name);
	} else {
		if(def->name[0] == '\0') {
			//truly anonymous
			snprintf(new_prefix, chars, "%.*s%s%d_", 
					prefix_len, prefix,
					def->kind == StructKind_Struct ?
					"struct" : "union",
					index);
		} else {
			//has a variable name
			snprintf(new_prefix, chars, "%.*s%s_",
					prefix_len, prefix,
					def->name);
		}
	}

	isize subcount = 0;
	for(isize i = 0; i < def->member_count; ++i) {
		if(def->member_kinds[i] != StructKind_Member) {
			auto var = &def->members[i].anon_struct;

			if(def->name[0] != '\0') {
				if(def->is_named_child) {
					var->def.parentname = def->parentname;
				} else {
					var->def.parentname = def->name;
				}
			} else {
				var->def.parentname = def->parentname;
			}
			
			print_struct_names(&var->def, subcount++, new_prefix, chars, suffix, all_structs, counter, arena);
		}
	}
}

void print_struct(Struct_Def* def, bool as_member_struct = false, int32 indent = 0)
{
	if(def->name == NULL) return;
	print_indent(indent);
	if(as_member_struct) {
		if(def->kind == StructKind_Struct) {
			printf("struct {\n");
		} else if(def->kind == StructKind_Union) {
			printf("union {\n");
		}
	} else {
		if(def->kind == StructKind_Struct) {
			printf("struct %s\n", def->name);
		} else if(def->kind == StructKind_Union) {
			printf("union %s\n", def->name);
		}
		print_indent(indent);
		printf("{\n");
	}
	indent++;
	for(isize i = 0; i < def->member_count; ++i) {
		if(def->member_kinds[i] == StructKind_Member) {
			auto var = &def->members[i].member_var;
			print_indent(indent);
			for(isize a = 0; a < var->count; ++a) {
				printf("%s ", var->terms[a]);
			}
			for(isize a = 0; a < var->asterisk_count; ++a) {
				printf("*");
			}
			printf("%s", def->members[i].member_var.name);
			for(isize a = 0; a < var->array_levels; ++a) {
				printf("[%s]", var->array_sizes[a]);
			}
			printf(";\n");
		} else {
			auto var = &def->members[i].anon_struct;
			print_struct(&var->def, true, indent);
			printf("%s", var->def.name);
			
			for(isize a = 0; a < var->array_levels; ++a) {
				printf("[%s]", var->array_sizes[a]);
			}

			printf(";\n");
		}	
	}
	indent--;
	print_indent(indent);
	if(as_member_struct) {
		if(def->name[0] == '\0')
			printf("}");
		else 
			printf("} ");
	} else {
		printf("};\n");
	}
	
}



Token* parse_struct_member(Lexer* lex, Struct_Def* parent, Token* start, Memory_Arena* arena)
{
	Hash structhash = hash_literal("struct");
	Hash unionhash = hash_literal("union");
	Token* head = start;
	Token* next = head->next;
	
	Struct_Member* member = parent->members + parent->member_count;
	Struct_Kind* kind = parent->member_kinds + parent->member_count;

	if(head->kind == Token_Identifier) {
		if(head->hash == structhash || head->hash == unionhash) {
			lex->structs_count++;
			if(head->hash == unionhash) {
				*kind = StructKind_Union;
			} else {
				*kind = StructKind_Struct;
			}

			do {
 				head = head->next;
			} while(head->kind != Token_OpenBrace);
			head = head->next;
			Struct_Def def = {0};
			def.members = arena_push_array(arena, Struct_Member, StructMemberCapacity);
			def.member_kinds = arena_push_array(arena, Struct_Kind, StructMemberCapacity);
			def.kind = *kind;
			do {
				head = parse_struct_member(lex, &def, head, arena);
				head = head->next;
			} while(head->kind != Token_CloseBrace);

 			head = head->next;
			auto var = &member->anon_struct;
			if(head->kind != Token_Identifier) {
				if(head->kind == Token_Semicolon) {
					def.name = "";
					def.is_anon_member = true;
					var->def = def;
					var->array_levels = 0;
					var->array_sizes = NULL;
					parent->member_count++;


					return head;
				} else {
					if(Metaprogram->verbose) fprintf(stderr, "ERROR: wanted identifier, got %d:[%.*s] \n", 
							head->kind, head->len, head->start);
				}
			} else {
				char* buf = arena_push_array(arena, char, 256);
				memcpy(buf, head->start, head->len);
				int len = head->len;
				next = head->next;
				buf[len] = '\0';
				def.name = buf;
				def.is_named_child = true;
				def.namehash = head->hash;
			}
			head = head->next;

			bool quit = false;
			do { 
				switch(head->kind) {
					case Token_Semicolon:
						quit = true;
						break;
					case Token_Comma:
						//TODO(will) Support commas like with regular variables.
						break;
					case Token_OpenBracket: {						
						head = head->next;
						if(var->array_levels == 0) {
							var->array_sizes = arena_push_array(arena, char*, 256);
						}
						char* buf = arena_push_array(arena, char, head->len + 1);
						memcpy(buf, head->start, head->len);
						int len = head->len;
						next = head->next;
						buf[len] = '\0';
						var->array_sizes[var->array_levels++] = buf;
					} break;
					default:
						break;
				}
				if(quit) break;
			} while(head = head->next);
			var->def = def;
		} else {
			*kind = StructKind_Member;
			auto var = &member->member_var;
			var->terms = arena_push_array(arena, char*, 256);
			var->count = 0;
			var->asterisk_count = 0;
			var->name = NULL;
			var->array_sizes = NULL;
			var->array_levels = 0;
			int32 mode = 0;
			bool quit = false;
			do {
				if(head->kind == Token_Comma) {
					if(mode == 0) {
						var->namehash = head->prev->hash;
						var->name = var->terms[--var->count];
					}
					parent->member_count++;
					Struct_Kind oldkind = *kind;
					member = parent->members + parent->member_count;
					kind = parent->member_kinds + parent->member_count;
					*kind = oldkind;
					member->member_var = *var;
					var = &member->member_var;
					var->asterisk_count = 0;
					var->array_levels = 0;
					var->array_sizes = NULL;
					var->name = NULL;
					mode = 1;
				} else if(head->kind == Token_Semicolon) {
					if(mode == 0) {
						var->name = var->terms[--var->count];
					}
					break;
				} else if(head->kind == Token_Identifier) {
					char* buf = arena_push_array(arena, char, head->len + 1);
					memcpy(buf, head->start, head->len);
					int len = head->len;
					next = head->next;
					buf[len] = '\0';
					if(mode == 0) {
						var->terms[var->count++] = buf;
					} else if(mode == 1) {
						var->namehash = head->hash;
						var->name = buf;
					}
				} else if(head->kind == Token_Asterisk) {
					var->asterisk_count++;
				} else if(head->kind == Token_OpenBracket) {
					head = head->next;
					if(var->array_levels == 0) {
						var->array_sizes = arena_push_array(arena, char*, 256);
					}
					char* buf = arena_push_array(arena, char, head->len + 1);
					memcpy(buf, head->start, head->len);
					int len = head->len;
					next = head->next;
					buf[len] = '\0';
					var->array_sizes[var->array_levels++] = buf;
				}
			} while(head = head->next);
		}
	}

	parent->member_count++;
	return head;
}


Struct_Def* find_struct_defs(Lexer* lex, Token* start, Memory_Arena* arena)
{
	Token* head = start;
	Token* next = NULL;

	Hash structhash = hash_literal("struct");
	Hash unionhash = hash_literal("union");
	Hash typedefhash = hash_literal("typedef");
	Struct_Def def = {0};

	Struct_Def* def_start = arena_push_struct(arena, Struct_Def);
	Struct_Def* def_head = def_start;

	int32 brace_depth = 0;
	Token* last_open, *last_closed;
	last_open = last_closed = NULL;
	do {
		head = parse_metaprogram_directive(head);
		if(head->kind == Token_OpenBrace) {
			brace_depth++;
			last_open = head;
		} else if(head->kind == Token_CloseBrace) {
			brace_depth--;
			last_closed = head;
		}

		if(brace_depth < 0) {
			if(Metaprogram->verbose) {
				fprintf(stderr, ">>> Brace Depth went negative at: %s line %d col %d \n", lex->files[head->location.file].filename, head->location.line, head->location.offset);
				if(last_open == NULL) {
					fprintf(stderr, ">>> No previous opening brace");
				} else {
					fprintf(stderr, ">>> Last Open Brace: %s line %d col %d \n", lex->files[last_open->location.file].filename, last_open->location.line, last_open->location.offset);
					fprintf(stderr, ">>> %.*s \n", last_open->len + 32, last_open->start);
				}

				if(last_closed == NULL) {
					fprintf(stderr, ">>> No previous closing brace");
				} else {
					fprintf(stderr, ">>> Last Closing Brace: %s line %d col %d \n", lex->files[last_closed->location.file].filename, last_closed->location.line, last_closed->location.offset);
					fprintf(stderr, ">>> %.*s \n", last_open->len + 32, last_open->start - 32);

				}
			}

			brace_depth = 0;
		}

		if(head->hash == typedefhash) {
			do {
				head = head->next;
			} while(head->kind != Token_Semicolon);
		}

		if(brace_depth > 0 || head->kind != Token_Identifier) continue;

		Struct_Kind kind = StructKind_None;
		if(head->hash == structhash) {
			kind = StructKind_Struct;
		} else if(head->hash == unionhash) {
			kind = StructKind_Union;
		}

		if(kind != StructKind_None) {
			def = {0};
			def.kind = kind;
			def.members = arena_push_array(arena, Struct_Member, StructMemberCapacity);
			def.member_kinds = arena_push_array(arena, Struct_Kind, StructMemberCapacity);
			Token* subhead = head->next;
			do {
				if(subhead->kind == Token_Identifier) {
					//this is the name
					char* buf = arena_push_array(arena, char, subhead->len + 1);
					memcpy(buf, subhead->start, subhead->len);
					buf[subhead->len] = '\0';
					def.name = buf;
					def.namehash = subhead->hash;
				} else if(subhead->kind == Token_OpenBrace) {
					break;
				}
			} while (subhead = subhead->next);
			subhead = subhead->next;

			do {
				subhead = parse_struct_member(lex, &def, subhead, arena);
				subhead = subhead->next;
			} while(subhead->kind != Token_CloseBrace);

			lex->structs_count++;

			*def_head = def;
			def_head->next = arena_push_struct(arena, Struct_Def); 
			def_head = def_head->next;
		}
	} while(head = head->next);
	if(Metaprogram->verbose)fprintf(stderr, "\n");

	return def_start;
}



/* Serialization "what is this thing" rules
 * 	- If something is a pointer
 * 		- if this is the only pointer in the 'class'
 * 			- assume any member named count, len, length, capacity, size 
 * 			 could be the number.
 * 		- when initializing, if something has the name
 * 			"pointer(s)_{capacity, size}
 * 			- Assume this is the size to allocate
 * 		- if something has the name
 * 			"pointer(s)_{count, length}" 
 * 			- Assume this is the number of things to (de)serialize
 * 		- if $(tag, pointername:length) is present, use that, ignore
 * 			heuristic rules
 * 	- If something is a union
 * 		- if something matches the name
 * 			"unionname_{kind,type}"
 * 			use this to index sub-structs in the union
 * 		- If a union has a base-level array that is the same size 
 * 			as the union, and no kind/type variable is present, 
 * 			(de)serialize that
 *
 */



struct Meta_Type
{
	Hash hash;
	char* name;
	bool ispointer;
	bool isunsigned;
	bool isvolatile;
	bool isconst;
	bool isarray;
	uint32 pointer_depth;

	isize type_index;

	Meta_Type* next;
};

bool meta_type_equals(Meta_Type* a, Meta_Type* b)
{
	if (a == NULL) return false;
	if (b == NULL) return false;
	return a->hash == b->hash;
}

void populate_meta_type(Struct_Member* member, Struct_Kind kind, Struct_Def* parent, Meta_Type* meta)
{
#define defhash(v) Hash v##hash = hash_literal(#v)
	defhash(const);
	defhash(volatile);
	defhash(unsigned);
	if(kind == StructKind_Member) {
		auto var = &member->member_var;
		for(isize i = 0; i < var->count; ++i) {
			char* term = var->terms[i];

			Hash termhash = hash_string(term, strlen(term));
			if(termhash == consthash) {
				meta->isconst = true;
			} else if(termhash == volatilehash) {
				meta->isvolatile = true;
			} else if(termhash == unsignedhash) {
				meta->isunsigned = true;
			} else {
				meta->name = term;
				meta->hash = termhash;
			}
			meta->ispointer = var->asterisk_count > 0;
			meta->pointer_depth = var->asterisk_count;
			meta->isarray = var->array_levels > 0;
			var->type = meta;
	 	}
	}
}

Meta_Type* get_types_in_struct(Struct_Def* def, Meta_Type* head, Memory_Arena* arena)
{
	for(isize i = 0; i < def->member_count; ++i) {
		if(def->member_kinds[i] == StructKind_Member) {
			populate_meta_type(def->members + i, def->member_kinds[i], def, head);
		} else {
			get_types_in_struct(&def->members[i].anon_struct.def, head, arena);
		}
		head->next = arena_push_struct(arena, Meta_Type); 
		head = head->next;
	}
	return head;
}

enum Meta_Flags
{
	MetaFlag_IsPointer = Flag(0),
	MetaFlag_IsUnsigned = Flag(1),
	MetaFlag_IsVolatile = Flag(2),
	MetaFlag_IsConst = Flag(3),
	MetaFlag_IsArray = Flag(4),
};

struct Meta_Member
{
	uint64 flags;
	char* type_name;
	uint32 pointer_depth;
	char* parentname;
	char* name;
};

void print_meta_member(Meta_Member* member, char* prefix, char* suffix)
{
	char* name;
	char* parentname;
	if(member->name[0] == '\0') {
		name = "null";
		parentname = "_empty";
	} else {
		name = member->name;
		parentname = member->parentname;
	}

	isize namelen = Min(strlen("Meta_Type_"), strlen(member->type_name));
	bool is_toplevel = false;

	for(isize i = 0; i < namelen; ++i) {
		if(member->type_name[i] != "Meta_Type_"[i]) {
			is_toplevel = true;
			break;
		}
	}

	printf("%sMeta_Member{%d, %s%s, %d, \"%s\", offsetof(%s, %s)}%s",
			prefix,
			member->flags, 
			is_toplevel ? "Meta_Type_" : "",
			member->type_name,
			member->pointer_depth,
			member->name,
			parentname,
			name,
			suffix);
}

void print_struct_info(Struct_Def* def, char* prefix, char* suffix) 
{
	printf("%sMeta_Struct_Info {\n\t\t\"%s\", \n\t\t\"%s\", \n\t\t%s, \n\t\t%s_Members, \n\t\t%d}%s", 
			prefix,
			def->name,
			def->meta_type_name,
			def->meta_type_name,
			def->meta_type_name,
			def->member_count,
			suffix);

}

void print_reflection_data(Struct_Def* def)
{
	printf("const Meta_Member %s_Members[] = {\n", def->meta_type_name);
	for(isize i = 0; i < def->member_count; ++i) {
		Meta_Member m = {
			0
		};
		if(def->name[0] != '\0')
			if(def->is_named_child)
				m.parentname = def->parentname;
			else
				m.parentname = def->name;
		else 
			m.parentname = def->parentname;
		if(def->member_kinds[i] == StructKind_Member) {
			auto var = &def->members[i].member_var;
			Meta_Type* meta = var->type;
			if(meta == NULL) {
				fprintf(stderr, "Meta type was null on %s::%s\n", def->name, var->name);
				continue;
			}
	 		m.name = var->name;
			m.type_name = meta->name;
			if(meta->isarray) {
				m.pointer_depth = var->array_levels;
			} else {
				m.pointer_depth = var->asterisk_count;
			}

			if(meta->ispointer) Enable_Flag(m.flags, MetaFlag_IsPointer);
			if(meta->isunsigned) Enable_Flag(m.flags, MetaFlag_IsUnsigned);
			if(meta->isvolatile) Enable_Flag(m.flags, MetaFlag_IsVolatile);
			if(meta->isconst) Enable_Flag(m.flags, MetaFlag_IsConst);
			if(meta->isarray) Enable_Flag(m.flags, MetaFlag_IsArray);
		} else {
			auto var = &def->members[i].anon_struct;
	 		m.name = var->def.name;
			m.type_name = var->def.meta_type_name;
			m.pointer_depth = var->array_levels;
			if(var->array_levels > 0) {
				m.flags = MetaFlag_IsArray;
			}
		}
		char* suffix = i == def->member_count - 1 ? "\n" : ",\n";
		print_meta_member(&m, "\t", suffix);
	}
	printf("};\n\n");
}

void print_metaprogram_types()
{
		printf(R"foo(
struct _empty
{
	int null;
};
struct Meta_Member
{
	uint64 flags;
	Meta_Type type;
	int32 pointer_depth;
	char* name;
	uint64 offset;
};

struct Meta_Struct_Info
{
	char* name;
	char* meta_name;
	isize index;
	const Meta_Member* members;
	isize count;
};

)foo");
}

void print_metaprogram_get_struct_info_proc()
{
		printf(R"foo(
static inline const Meta_Struct_Info* get_struct_info(Meta_Type t) 
{
	return All_Meta_Struct_Info + t;
}
)foo");

}
