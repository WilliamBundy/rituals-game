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
	clear_arena(Game->world_arena);
	play_state = arena_push_struct(Game->game_arena, Play_State);
	play_state->world = arena_push_struct(Game->world_arena, World);
	play_state->running = true;
}

void deserialize_world(World* world, FILE* world_file);
void start_play_state(char* world_name_in)
{
	World* world = play_state->world;
	isize wnl = strlen(world_name_in);
	char* world_name = arena_push_array(Game->world_arena, char, wnl + 1);
	memcpy(world_name, world_name_in, wnl+1);
	FILE* fp = get_world_file(world_name, "rb");
	if(fp != NULL) {
		deserialize_world(world, fp);
		world->name = world_name;
	} else {
	
		init_world(world, 4, 4, next_random_uint64(&Game->r), Game->world_arena);
		generate_world(world_name, world);
		world_start_in_area(
				world,
				world->area_stubs, 
				Game->play_arena);
	}
}

void play_state_update()
{
	if(_scancode(ESCAPE) == State_Just_Pressed) {
		play_state->running = !play_state->running;
	}
	world_area_update(play_state->world->current_area, play_state->world);


	if(!play_state->running) {
		Renderer->offset = Vec2{};
		renderer_start();
		
		Sprite s = get_box_sprite(v2(-100, -100), Game->size + v2(100, 100), v4(0, 0, 0, 0.9f));
		s.anchor = Anchor_Top_Left;
		renderer_push_sprite(&s);

		Body_Font->color = v4(1, 1, 1, 1);
		render_body_text("Paused", v2(32, 32), false, 4.0f);
		if(gui_add_button(v2(32, Game->size.y / 2), "Exit to main menu", v2(144, 0))) {
			serialize_world(world);
			Game->state = Game_State_Menu;
		}
		renderer_draw();
	}
}

void play_state_stop()
{
	serialize_world(play_state->world);
}

