#define $(...)


#include <windows.h>
#include <Shlwapi.h>
#include <intrin.h>
#include <malloc.h>

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

#include <SDL.h>
#include "thirdparty.h"

#include "rituals_defines.h"

#define ALLOC_CUSTOM_INTEGER_TYPES
#define ALLOC_IMPLEMENTATION
#define ALLOC_CPLUSPLUS_FEATURES
#include "wb_alloc.h"

#define WBGL_GL_VERSION_3_3
#define WBGL_SDL
#define WBGL_IMPLEMENTATION
#include "wb_gl.h"

void testUpdate();
#include "rituals_math.cpp"
//#include "rituals_renderer.cpp"
#include "render.cpp"
#include "rituals_game_info.cpp"
#include "rituals_game_registry.cpp"
#include "rituals_game.cpp"
#include "rituals_animations.cpp"
//#include "rituals_gui.cpp"
//#include "rituals_game_states.cpp"
#include "rituals_tilemap.cpp"
struct Entity;
#include "rituals_simulation.cpp"
#include "rituals_particles.cpp"
struct World;
struct World_Area_Stub;
World* globalWorld;
#include "rituals_world_area.cpp"
#include "rituals_world.cpp"
#include "rituals_entity_events.cpp"
//#include "rituals_serialization.cpp"

Tilemap tm;

void testInit()
{
	init_tilemap(&tm, 32, 32, Game->baseArena);
	generate_tilemap(&tm, 1000);
	globalWorld = (World*)arenaPush(Game->gameArena, sizeof(World));
	init_world(globalWorld, 8, 8, 100, Game->gameArena);
	world_start_in_area(globalWorld, globalWorld->area_stubs, Game->gameArena);
}

void testUpdate()
{
	renderAdd(0, ColorWhite, v2(100, 100), v2(32, 32), rect2i(0, 0, 32, 32));
	render_tilemap(Game->registry, &tm, v2(0, 0));
	world_area_update(globalWorld->current_area, globalWorld);
}

int main(int argc, char** argv)
{
	Platform platform = {0};
	platform.window_title = "Rituals";
	platform.window_size = v2i(1280, 720);
	platform.game_scale = 1.0f;
	platform.display_index = 1;

	if(platform_init(&platform)) {
		return 1;
	}

	if(game_init(&platform)) {
		return 1;
	}
	
	testInit();
	game_start(&platform, Game);

	platform_quit();

	return 0;
}

