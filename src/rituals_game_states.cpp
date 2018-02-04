/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef REFLECTED
struct None_State
{
	int32 error;
};
#endif 

void game_init_None_state()
{

}

void game_start_None_state()
{

}

void game_update_None_state()
{

}

void game_stop_None_state()
{

}


void game_free_None_state()
{

}

#ifndef REFLECTED
struct Menu_State
{
	int32 error;
	char save_dir[FilePathMaxLength];
	isize save_dir_len;
	tinydir_dir saves;
	bool saves_dirty;
	Gui_Text_Input_Handle handle;
	isize delete_index;
};
#endif 

void game_init_Menu_state()
{
	Menu_State* menu_state = Game->Menu;
	init_text_input_handle(&menu_state->handle, 256, Game->game_arena);
	menu_state->delete_index = -1;

	menu_state->save_dir_len = snprintf(menu_state->save_dir, FilePathMaxLength, "%ssave", Game->base_path);
	check_dir(menu_state->save_dir);
	tinydir_open_sorted(&menu_state->saves, menu_state->save_dir);

}

void game_start_Menu_state()
{

}

void game_update_Menu_state()
{
	Menu_State* menu_state = Game->Menu;
	game_set_scale(2.0f);
	Renderer->groups[0].offset = v2(0, 0);
	render_start();
	
	real lasty = 32;
	Body_Font->color = v4(1, 1, 1, 1);
	render_body_text("Rituals", v2(32, 32), false, 4.0f);
	lasty += 16;
	lasty += Body_Font->glyph_height * 4;
	gui_add_text_input(&menu_state->handle, 
			v2(32, lasty), 
			v2(256, Body_Font->glyph_height + 8), 
			"Enter new world name");

	if(gui_add_button(v2(256 + 32 + 16 , lasty), "Create", v2(64, 0))) {
		menu_state->saves_dirty = true;
	}
	if(Input->scancodes[SDL_SCANCODE_RETURN] == State_Just_Pressed) {
		menu_state->saves_dirty = true;
	}

	if(menu_state->saves_dirty) {
		tinydir_close(&menu_state->saves);
		char buf[FilePathMaxLength];
	 	snprintf(buf, FilePathMaxLength, "%ssave/%.*s",
				Game->base_path, 
				menu_state->handle.buffer_length, 
				menu_state->handle.buffer);
		check_dir(buf);
		
		tinydir_open_sorted(&menu_state->saves, menu_state->save_dir);
		menu_state->handle.buffer_length = 0;
		menu_state->saves_dirty = false;
	}
	lasty += 32 + 16;

	for(usize i = 0; i < menu_state->saves.n_files; ++i) {
		tinydir_file file;
		tinydir_readfile_n(&menu_state->saves, &file, i);
		if(file.is_dir && (file.name[0] != '.')) {
			if(gui_add_button(v2(32, lasty), file.name, v2(144, 0))) {
				recursively_delete_folder(file.path, false );
				Game->Play->world_name_in = file.path;
				game_switch_state(Game_State_Play);
			}
			if(menu_state->delete_index != i) {
				if(gui_add_button(v2(32 + 144 + 32, lasty), "Delete", v2(64, 0))) {
					menu_state->delete_index = i;
				}
			} else {

				if(gui_add_button(v2(32 + 144 + 32, lasty), "Cancel", v2(64, 0))) {
					menu_state->delete_index = -1;
				}
				if(gui_add_button(v2(32 + 144 + 32 + 64 + 32, lasty), "Confirm", v2(64, 0))) {
					printf("[%s] was deleted \n", file.path);
					recursively_delete_folder(file.path, false);
					menu_state->delete_index = -1;
					menu_state->saves_dirty = true;
				}
			}
			lasty += 32;
		}
	}

	if(menu_state->saves_dirty) {
		tinydir_close(&menu_state->saves);
		tinydir_open_sorted(&menu_state->saves, menu_state->save_dir);
	}

	render_draw(Game->size, Game->scale);
}

void game_stop_Menu_state()
{

}


void game_free_Menu_state()
{

}


#ifndef REFLECTED
struct Play_State
{
	int32 error;
	usize current_time = 0, prev_time = 0;
	real accumulator = 0;
	World* world;

	bool running;

	Vec2i world_xy;
	bool delete_world_on_stop;
	bool save_world_on_stop;

	const char* world_name_in;
};
#endif 

void game_init_Play_state()
{
	clear_arena(Game->play_arena);
	clear_arena(Game->world_arena);

	Game->Play = arena_push_struct(Game->game_arena, Play_State);
	Play_State* play_state = Game->Play;
	play_state->delete_world_on_stop = false;
	play_state->save_world_on_stop = true;
	play_state->world = arena_push_struct(Game->world_arena, World);
	play_state->running = true;

}

void game_start_Play_state()
{
	Play_State* play_state = Game->Play;

	World* world = play_state->world;
	isize wnl = strlen(play_state->world_name_in);
	char* world_name = arena_push_array(Game->world_arena, char, wnl + 1);
	memcpy(world_name, play_state->world_name_in, wnl+1);
	FILE* fp = get_world_file(world_name, "rb");
#if 0
	if(fp != NULL) {
		deserialize_world(world, fp);
		world->name = world_name;
	} else
#endif
	{
	
		init_world(world, 4, 4, next_random_uint64(&Game->r), Game->world_arena);
		generate_world(world_name, world);
		world_start_in_area(
				world,
				world->area_stubs, 
				Game->play_arena);
	}

}

void game_update_Play_state()
{
	Play_State* play_state = Game->Play;

	if(Game->state != Game_State_Play) return;
	if(_scancode(ESCAPE) == State_Just_Pressed) {
		play_state->running = !play_state->running;
	}
	world_area_update(play_state->world->current_area, play_state->world);


	if(!play_state->running) {
		Renderer->groups[0].offset = Vec2{};
		render_start(CurrentGroup);
		
		Sprite s = create_box_primitive(v2(-100, -100), Game->size + v2(100, 100), v4(0, 0, 0, 0.75f));
		s.flags = Anchor_Top_Left;
		render_add(&s);

		Body_Font->color = v4(1, 1, 1, 1);
		render_body_text("Paused", v2(32, 32), false, 4.0f);
		if(gui_add_button(v2(32, Game->size.y / 2), "Exit to main menu", v2(144, 0))) {
			serialize_world(play_state->world);
			init_state(Play);
			Game->state = Game_State_Menu;
		}
		render_draw(Game->size, Game->scale);
	}

}

void game_stop_Play_state()
{
	Play_State* play_state = Game->Play;

	if(play_state->save_world_on_stop) {
		serialize_world(play_state->world);
	}
	if(play_state->delete_world_on_stop) {
		world_delete_self(play_state->world);
	}
	init_state(Play);
}


void game_free_Play_state()
{

}

