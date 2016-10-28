/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 * rituals_main.cpp
 */ 

#ifdef PREPROCESSOR
#define $(...) $(__VA_ARGS__)
#else
#define $(...)
#define REFLECTED
#endif


#ifndef PREPROCESSOR

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

#endif

#include "rituals_defines.h"

#ifdef REFLECTED
#include "rituals_types.cpp"
#include "rituals_reflection.cpp"
#endif

#define _game_state_list \
	_game_state(None) \
	_game_state(Menu) \
	_game_state(Play) \

#include "rituals_math.cpp"
#include "rituals_game.cpp"

#include "rituals_renderer.cpp"
#include "rituals_animations.cpp"
#include "rituals_gui.cpp"

#include "rituals_game_states.cpp"

#include "rituals_game_info.cpp"
#include "rituals_game_registry.cpp"

#include "rituals_tilemap.cpp"

#include "rituals_simulation.cpp"

#include "rituals_particles.cpp"

#include "rituals_world_area.cpp"
#include "rituals_world.cpp"
#include "rituals_entity_events.cpp"

#include "rituals_serialization.cpp"

void load_assets()
{
	isize w, h;
	uint32 texture = ogl_load_texture("data/graphics.png", &w, &h);
	for(isize i = 0; i < Renderer->groups_count; ++i) {
		Renderer->groups[i].texture = texture;
		Renderer->groups[i].texture_size.x = w;
		Renderer->groups[i].texture_size.y = h;
	}

	Game->body_font = arena_push_struct(Game->asset_arena, Spritefont);
	init_spritefont(Game->body_font, Gohufont_Glyphs);
	Body_Font = Game->body_font;

	init_game_registry(Registry, Game->registry_arena); 
	register_everything_in_rituals();
	finalize_game_registry();
}


int main(int argc, char** argv)
{
	Platform platform = {0};
	platform.window_title = "Rituals";
	platform.window_size = v2i(1280, 720);
	platform.game_scale = 1.0f;
	platform.display_index = 1;
	platform.check_gl_attributes = false;
	platform.vertex_shader = 
#include "vert.glsl"
		;
	platform.frag_shader = 
#include "frag.glsl"
		;

	if(platform_init(&platform)) {
		return 1;
	}

	if(game_init(&platform)) {
		return 1;
	}
	
	load_assets();

	game_start(Game_State_Menu, &platform);

	platform_quit();

	return 0;
}

