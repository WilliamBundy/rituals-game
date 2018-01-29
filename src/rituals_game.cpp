#define Log_Error(e) fprintf(stderr, "There was an error: %s \n", e);

#ifndef WirmphtEnabled
struct Game_Main
{
	SDL_Window* window;
	i32 state;
	f32 scale;
	Vec2 size;
	Vec2i window_size;

	MemoryArena* baseArena;
	MemoryArena* gameArena;
	MemoryArena* tempArena;

	const char* base_path;
	isize base_path_length;

	Random r;

	Game_Registry* registry;
	
	wRenderGroup* render;

	Game_Input* input; 
	isize last_frame_time;
};

struct Platform
{
	const char* window_title;
	Vec2i window_size;
	f32 game_scale;
	i32 display_index;

	bool check_gl_attributes;

	const char* vertex_shader;
	const char* frag_shader;

	SDL_Window* window;
};
#endif

Game_Main* Game;
Game_Input* Input;

i32 platform_init(Platform* platform)
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
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
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
	wbgl_load(NULL);

	return 0;
}


i32 game_init(Platform* platform)
{
	Game = Allocate(Game_Main, 1);
	Game->last_frame_time = 1;
	Game->window = platform->window;
	Game->state = 0;

	MemoryInfo mi = getMemoryInfo();
	Game->baseArena = arenaBootstrap(mi, 0);
	Game->gameArena = arenaBootstrap(mi, 0);
	Game->tempArena = arenaBootstrap(mi, 0);

	Game->base_path = SDL_GetBasePath();
	Game->base_path_length = strlen(Game->base_path);

	Game->input = arenaPush<Game_Input, 1>(Game->baseArena);
	input_init(Game->input, Game->baseArena);
	Input = Game->input;

	init_random(&Game->r, time(NULL));
	//TODO(will) load window settings from file
	Game->window_size = platform->window_size;
	Game->scale = platform->game_scale;


	Game->registry = (Game_Registry*)arenaPush(Game->baseArena, sizeof(Game_Registry));
	Game_Registry* registry = Game->registry;
	registryInit(registry, 256, Game->baseArena);
	register_everything_in_rituals(registry);
	
	Game->render = (wRenderGroup*)arenaPush(Game->baseArena, sizeof(wRenderGroup));
	Game->render->shader = (wShader*)arenaPush(Game->baseArena, sizeof(wShader));
	wInitDefaultShader(GLES2_vert, GLES2_frag, Game->render->shader);
	
	Game->render->texture = wLoadTexture(Game->base_path, "data/graphics.png", Game->baseArena);

	wGroupInit(Game->render, 8192, Game->render->shader, Game->render->texture);
	

	return 0;
}

void game_update_screen()
{
	SDL_GetWindowSize(Game->window, &Game->window_size.x, &Game->window_size.y);
	glViewport(0, 0, Game->window_size.x, Game->window_size.y);
	Game->size = v2(Game->window_size) * Game->scale;
}

Sprite* renderAdd(i32 flags, u32 color, Vec2 pos, Vec2 size, Rect2i t);
i32 game_start(Platform* platform, Game_Main* game)
{
	bool running = true;
	SDL_Event event;
	//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClearColor(0, 0, 0, 1);
	SDL_GL_SetSwapInterval(-1);

	while(running) {
		usize start_ticks = SDL_GetTicks();

		for(isize i = 0; i < SDL_NUM_SCANCODES; ++i) {
			i8* t = game->input->scancodes + i;
			if(*t == Button_JustUp) {
				*t = Button_Up;
			} else if(*t == Button_JustDown) {
				*t = Button_Down;
			}
			t = game->input->keycodes + i;
			if(*t == Button_JustUp) {
				*t = Button_Up;
			} else if(*t == Button_JustDown) {
				*t = Button_Down;
			}
		}
		for(isize i = 0; i < 16; ++i) {
			i8* t = game->input->mouse + i;
			if(*t == Button_JustUp) {
				*t = Button_Up;
			} else if(*t == Button_JustDown) {
				*t = Button_Down;
			}
		}
		while(SDL_PollEvent(&event)) {
			//TODO(will) handle text input
			Game_Input* input = game->input;
			isize textlen = 0;
			switch(event.type) {
				case SDL_QUIT:
					//game_stop_state();
					running = false;
					return 0;
					break;
				case SDL_WINDOWEVENT:
					game_update_screen();
					break;
				case SDL_TEXTINPUT:
					textlen = strlen(event.text.text);
					if((textlen + input->text_count) < InputTextCapacity) {
						memcpy(input->text + input->text_count, 
								event.text.text, 
								textlen);
						input->text_count += textlen;
						textlen = 0;
					}
					break;
				case SDL_KEYDOWN:
					input->num_keys_down++;
					if((event.key.keysym.sym == SDLK_BACKSPACE)) {
						input_text_append_char(input, Backspace);
						if(event.key.keysym.mod & KMOD_CTRL) {
							input_text_append_char(input, '\7');
						}
					} else if(event.key.keysym.sym == SDLK_RETURN || 
							event.key.keysym.sym == SDLK_RETURN2) {
						if(input->capture_newlines) {
							input_text_append_char(input, '\n');
						}
					} else if(event.key.keysym.sym == SDLK_TAB) {
						if(input->capture_tabs) {
							input_text_append_char(input, '\t');
						}
					}

					if(!event.key.repeat) {
						auto keysym = event.key.keysym;
						input->scancodes[keysym.scancode] = Button_JustDown;
						if(event.key.keysym.sym < SDL_NUM_SCANCODES) {
							input->keycodes[keysym.sym] = Button_JustDown;
						}
					}
					break;
				case SDL_KEYUP:
					input->num_keys_down--;
					if(!event.key.repeat) {
						auto keysym = event.key.keysym;
						input->scancodes[keysym.scancode] = Button_JustUp;
						if(event.key.keysym.sym < SDL_NUM_SCANCODES) {
							input->keycodes[keysym.sym] = Button_JustUp;
						}
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					input->num_mouse_down++;
					input->mouse[event.button.button] = Button_JustDown;
					break;
				case SDL_MOUSEBUTTONUP:
					input->num_mouse_down--;
					input->mouse[event.button.button] = Button_JustUp;
					break;
			}
		}
	
		int mx, my;
		SDL_GetMouseState(&mx, &my);
		game->input->mouse_x = mx;
		game->input->mouse_y = my;
		game->input->screen_mouse_pos = v2i(mx, my);

		glClear(GL_COLOR_BUFFER_BIT |
				GL_DEPTH_BUFFER_BIT |
				GL_STENCIL_BUFFER_BIT);

		testUpdate();

		wGroupDraw(game->size.x, game->size.y, game->render);
		usize frame_ticks = SDL_GetTicks() - start_ticks;

		/*
		if(frame_ticks < 16) {
			SDL_Delay((u32)(16 - frame_ticks));
		}
		*/
		SDL_GL_SwapWindow(game->window);
		game->last_frame_time = SDL_GetTicks() - start_ticks;
	}
	return 0;
}

void platform_quit()
{
	SDL_Quit();
}



void game_switch_state(i32 state)
{
}

f64 next_random_double()
{
	usize x = next_random_u64(&Game->r);
	return (f64)x / (f64)UINT64_MAX;
}

f32 next_random_float()
{
	return (f32)(next_random_double(&Game->r));
}

f32 next_random() 
{
	return (f32)(next_random_double(&Game->r));
}

f32 rand_range(f32 min, f32 max)
{
	return next_random_double(&Game->r) * (max - min) + min;
}

i32 rand_range_int(f32 min, f32 max)
{
	return (i32)(next_random_double(&Game->r) * (max - min + 1) + min);
}

void game_set_scale(f32 scale)
{
	Game->scale = scale;
	Game->size = v2(Game->window_size) * (1.0f / Game->scale);
}

void game_calc_mouse_pos(Vec2 offset)
{
	Input->mouse_pos = v2(Input->mouse_x / Game->scale, Input->mouse_y / Game->scale) + offset;
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

enum Direction 
{
	Direction_North,
	Direction_South,
	Direction_East,
	Direction_West
};

void render_add(Sprite* sprite)
{
	Sprite* ws = wGetSprite(Game->render);
	*ws = *sprite;
}

Sprite* renderAdd(
		i32 flags,
		u32 color,
		Vec2 pos, Vec2 size, Rect2i t)
{
	return wGroupAddRaw(Game->render,
			flags, color,
			pos.x, pos.y, 
			size.x, size.y,
			t.x, t.y, t.w, t.h);
}

