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
	play_state = arena_push_struct(Game->play_arena, Play_State);
	play_state->world = arena_push_struct(Game->play_arena, World);
}

void play_state_start()
{
	World* world = play_state->world;
	init_world(world, 4, 4, Game->play_arena);
	generate_world(world, Registry->tiles, Registry->tiles_count, 1, Game->play_arena);

	world->current_area = world->areas;

	World_Area* area = world->current_area;
	world_area_init_player(world->current_area, 
			v2i(WorldAreaTilemapWidth / 2, WorldAreaTilemapHeight / 2));
}

void play_state_update()
{
	world_area_update(play_state->world->current_area);
}
