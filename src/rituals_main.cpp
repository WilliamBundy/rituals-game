#define $(...)
#define VariadicArgs ...
#define WirmphtEnabled

$(exclude)
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

struct Entity;
struct World;
struct WorldAreaStub;
#include "rituals_types.cpp"
$(end)

World* globalWorld;

#include "rituals_math.cpp"
#include "rituals_shaders.h"
#include "render.cpp"
#include "rituals_game_info.cpp"
#include "rituals_utilities.cpp"
#include "rituals_input.cpp"
#include "rituals_game.cpp"
#include "rituals_animations.cpp"
#include "rituals_tilemap.cpp"
//#include "rituals_simulation.cpp"
//#include "rituals_particles.cpp"
#include "sim.cpp"

SimWorld* sim;
SimBody* box;
SimBody* ground;
void testInit()
{
	sim = simNew(4096);
	box = simCreateBox(sim, v2(100, 100), v2(50, 50));
	for(isize i = 0; i < 20; ++i) {
		SimBody* b = simCreateBox(sim, v2(i * 50, 100), v2(50, 60));
		b = simCreateBox(sim, v2(i * 55, 50), v2(56, 50));
	}

	/*
	ground = simCreateBox(sim, v2(-1000, 700), v2(4280, 50));
	simAttachBox(sim, ground, v2(0, 0), v2(1280, 50));
	simAttachBox(sim, ground, v2(0, 0), v2(50, 720));
	simAttachBox(sim, ground, v2(1230, 0), v2(50, 720));
	ground->invMass = 0;
	*/
}


SimBody* mouseBody = NULL;
Vec2 mouseDiff;
void testUpdate()
{
	SimBody* b;
	Vec2 mouse = v2(Input->mouse_x, Input->mouse_y);
	if(!mouseBody && Input->mouse[1]) {
		for(isize j = 0; j < sim->shapeCount; ++j) {
			SimShape* s = sim->shapes[j];
			if(s->body->invMass == 0) continue;

			if(vaabbContains(s->pos + s->body->pos, s->size, mouse)) {
				mouseBody = s->body;
				mouseDiff = mouse - s->body->pos;
			}
		}
	} else if (!Input->mouse[1]) {
		mouseBody = NULL;
		mouseDiff = v2(0, 0);
	}

	if(mouseBody) {
		mouseBody->vel = ((mouse - mouseDiff) - mouseBody->pos) * 4; 
	}

	for(isize j = 0; j < sim->bodyCount; ++j) {
		b = sim->bodies[j];
		for(isize i = 0; i < b->shapeCount; ++i) {
			SimShape* shape = b->shapes[i];
			u32 c = 0xFFFFFF77;
			if(b == box) c = 0x33FF33FF;
			Sprite* s = renderAdd(Sprite_NoTexture, c, 
					b->pos + shape->pos, shape->size * 2, 
					rect2i(0, 0, 0, 0));
		}
	}

	{
		f32 move = 40;
		Vec2 v = v2(0, 0);
		if(Input->scancodes[SDL_SCANCODE_A] >= Button_Down) {
			v.x -= move;
		}
		if(Input->scancodes[SDL_SCANCODE_D] >= Button_Down) {
			v.x += move;
		}
		if(Input->scancodes[SDL_SCANCODE_W] >= Button_Down) {
			v.y -= move;
		}
		if(Input->scancodes[SDL_SCANCODE_S] >= Button_Down) {
			v.y += move;
		}

		box->vel += v;
		Game->render->offsetX = box->pos.x - Game->size.x / 2;
		Game->render->offsetY = box->pos.y - Game->size.y / 2;
	}

	simUpdate(sim, 1.0f/60.0f);
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

