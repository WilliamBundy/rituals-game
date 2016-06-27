/*
 * rituals_play_state.cpp
 */

void play_state_init()
{
	clear_arena(game->play_arena);
	play_state = Arena_Push_Struct(game->play_arena, Play_State);
	play_state->world = Arena_Push_Struct(game->play_arena, World);
}

void play_state_start()
{
	World* world = play_state->world;
	init_world(world, 4, 4, game->play_arena);
	Tile_Info* tile_info = Arena_Push_Array(
			game->play_arena, 
			Tile_Info, 
			Max_Tile_Info_Count);
	isize tile_count = 0;
	register_all_rituals_tile_info(tile_info, &tile_count);

	generate_world(world, tile_info, tile_count, 1, game->play_arena);

	world->current_area = world->areas;

	World_Area* area = world->current_area;
	world_area_init_player(world->current_area, 
			v2i(World_Area_Tilemap_Width / 2, World_Area_Tilemap_Height / 2));
}

void play_state_update()
{
	update_world_area(play_state->world->current_area);
}
