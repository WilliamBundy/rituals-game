/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 * rituals_play_state.cpp
 */

void play_state_init()
{
	clear_arena(Game->play_arena);
	play_state = arena_push_struct(Game->game_arena, Play_State);
	play_state->world = arena_push_struct(Game->world_arena, World);
}

void deserialize_world(World* world, FILE* world_file);
void play_state_start()
{
	World* world = play_state->world;

	FILE* fp = get_world_file("World_0", "rb");
	if(fp != NULL) {
		deserialize_world(world, fp);
	} else {
		init_world(world, 4, 4, 1, Game->world_arena);
		generate_world("World_0", world);
		world_start_in_area(
				world,
				world->area_stubs, 
				Game->play_arena);
	}
}

void play_state_update()
{
	world_area_update(play_state->world->current_area);
}
