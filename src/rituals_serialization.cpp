/*
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* 
 * rituals_serialization.cpp
 * TODO(will) Convert all isize to int64 for serializaiton!!!
 * ptrdiff_t may not equal int64
 */ 
int delete_file(char* path, isize path_length, char* file)
{
	char buf[FilePathMaxLength];
	snprintf(buf, FilePathMaxLength, "%.*s/%s", path_length, path, file);
	return DeleteFile(buf);
}

int delete_folder(char* path, isize path_length)
{
	path[path_length] = '\0';
	return RemoveDirectory(path);
}

int _recursive_delete(const char* last_path, isize last_path_len, char* path)
{
	isize buf_size = last_path_len + 1 + strlen(path);
	char* buf = arena_push_array(Game->temp_arena, char, buf_size);
	isize len = snprintf(buf, FilePathMaxLength, "%.*s/%s", last_path_len, last_path, path);
	tinydir_dir dir;
	tinydir_open_sorted(&dir, buf);
	for(usize i = 0; i < dir.n_files; ++i) {
		tinydir_file f;
		tinydir_readfile_n(&dir, &f, i);
		if(f.name[0] != '.') {
			if(f.is_dir) {
				_recursive_delete(buf, len, f.name);
			} else {
				delete_file(buf, len, f.name);
			}
		}
	}
	tinydir_close(&dir);
	delete_folder(buf, len);
		
	
	return 1;
}


int recursively_delete_folder(char* path, bool append_base_path = false)
{
	start_temp_arena(Game->temp_arena);
	if(append_base_path) {

		_recursive_delete(Game->base_path, Game->base_path_length-1, path); 
	} else {
		_recursive_delete(path, strlen(path), "");
	}
	end_temp_arena(Game->temp_arena);
	return 1;
}


int check_path(char* path)
{
	return PathFileExists(path);
}

void check_dir(char* dir)
{
	if(!PathFileExists(dir)) {
		CreateDirectory(dir, NULL);
	}
}

void serialize_tile_state(Tile_State* state, FILE* file)
{
	fwrite(&state->id, sizeof(isize), 1, file);
	fwrite(&state->damage, sizeof(int32), 1, file);
}

void deserialize_tile_state(Tile_State* state, FILE* file)
{
	fread(&state->id, sizeof(isize), 1, file);
	fread(&state->damage, sizeof(int32), 1, file);
}



void deserialize_tilemap(Tilemap* map, FILE* file, Memory_Arena* arena)
{
	fread(&map->w, sizeof(isize), 1, file);
	fread(&map->h, sizeof(isize), 1, file);
	isize size = map->w * map->h;
	map->tiles = arena_push_array(arena, Tile, size);
	map->states = arena_push_array(arena, Tile_State, size);
	fread(map->tiles, sizeof(Tile), size, file);
	for(isize i = 0; i < size; ++i) {
		deserialize_tile_state(map->states + i, file);
	}
}

void serialize_tilemap(Tilemap* map, FILE* file)
{
	fwrite(&map->w, sizeof(isize), 1, file);
	fwrite(&map->h, sizeof(isize), 1, file);
	isize size = map->w * map->h;
	fwrite(map->tiles, sizeof(Tile), size, file);
	for(isize i = 0; i < size; ++i) {
		serialize_tile_state(map->states + i, file);
	}
}
void serialize_sim_body(Sim_Body* body, FILE* file)
{
	fwrite(&body->id, sizeof(isize), 1, file);
	fwrite(&body->shape.e, sizeof(real), 4, file);
	fwrite(&body->velocity.e, sizeof(real), 2, file);
	fwrite(&body->force.e, sizeof(real), 2, file);
	fwrite(&body->collision_vel.e, sizeof(real), 2, file);
	fwrite(&body->inv_mass, sizeof(real), 1, file);
	fwrite(&body->restitution, sizeof(real), 1, file);
	fwrite(&body->damping, sizeof(real), 1, file);
	fwrite(&body->flags, sizeof(uint64), 1, file);
	fwrite(&body->entity_id, sizeof(isize), 1, file);
}

void deserialize_sim_body(Sim_Body* body, FILE* file)
{
	fread(&body->id, sizeof(isize), 1, file);
	fread(&body->shape.e, sizeof(real), 4, file);
	fread(&body->velocity.e, sizeof(real), 2, file);
	fread(&body->force.e, sizeof(real), 2, file);
	fread(&body->collision_vel.e, sizeof(real), 2, file);
	fread(&body->inv_mass, sizeof(real), 1, file);
	fread(&body->restitution, sizeof(real), 1, file);
	fread(&body->damping, sizeof(real), 1, file);
	fread(&body->flags, sizeof(uint64), 1, file);
	fread(&body->entity_id, sizeof(isize), 1, file);
}

void serialize_simulator(Simulator* sim, FILE* file)
{
	fwrite(&sim->bodies_count, sizeof(isize), 1, file);
	fwrite(&sim->bodies_capacity, sizeof(isize), 1, file);
	fwrite(&sim->next_body_id, sizeof(isize), 1, file);
	fwrite(&sim->sort_axis, sizeof(isize), 1, file);
	for(isize i = 0; i < sim->bodies_count; ++i) {
		serialize_sim_body(sim->bodies + i, file);
	}
}
void deserialize_simulator(Simulator* sim, FILE* file, Memory_Arena* arena)
{
	fread(&sim->bodies_count, sizeof(isize), 1, file);
	fread(&sim->bodies_capacity, sizeof(isize), 1, file);
	fread(&sim->next_body_id, sizeof(isize), 1, file);
	fread(&sim->sort_axis, sizeof(isize), 1, file);
	sim->bodies = arena_push_array(arena, Sim_Body, sim->bodies_capacity);
	for(isize i = 0; i < sim->bodies_count; ++i) {
		deserialize_sim_body(sim->bodies + i, file);
	}
}


void serialize_sprite(Sprite* s, FILE* file)
{
	fwrite(&s->position.e, sizeof(real), 2, file);
	fwrite(&s->center.e, sizeof(real), 2, file);
	fwrite(&s->angle, sizeof(real), 1, file);
	fwrite(&s->size.e, sizeof(real), 2, file);
	fwrite(&s->texture.e, sizeof(real), 4, file);
	fwrite(&s->color.e, sizeof(real), 4, file);
	fwrite(&s->anchor, sizeof(uint32), 1, file);
	fwrite(&s->sort_point_offset.e, sizeof(real), 2, file);
}

void deserialize_sprite(Sprite* s, FILE* file)
{
	fread(&s->position.e, sizeof(real), 2, file);
	fread(&s->center.e, sizeof(real), 2, file);
	fread(&s->angle, sizeof(real), 1, file);
	fread(&s->size.e, sizeof(real), 2, file);
	fread(&s->texture.e, sizeof(real), 4, file);
	fread(&s->color.e, sizeof(real), 4, file);
	fread(&s->anchor, sizeof(uint32), 1, file);
	fread(&s->sort_point_offset.e, sizeof(real), 2, file);
}

void deserialize_entity(Entity* entity, FILE* file)
{
	fread(&entity->id, sizeof(isize), 1, file);
	fread(&entity->body_id, sizeof(isize), 1, file);
	deserialize_sprite(&entity->sprite, file);
	fread(&entity->counter, sizeof(int32), 1, file);
	fread(&entity->facing, sizeof(int32), 1, file);
	fread(&entity->direction, sizeof(Direction), 1, file);
	fread(&entity->kind, sizeof(isize), 1, file);
	fread(&entity->events, sizeof(uint64), 1, file);
}

void deserialize_area(World_Area* area, FILE* area_file, Memory_Arena* arena)
{
	fread(&area->id, sizeof(isize), 1, area_file);
	fread(&area->entities_count, sizeof(isize), 1, area_file);
	fread(&area->entities_capacity, sizeof(isize), 1, area_file);
	fread(&area->next_entity_id, sizeof(isize), 1, area_file);
	fread(area->offset.e, sizeof(real), 2, area_file);
	area->entities = arena_push_array(arena, Entity, area->entities_capacity);
	for(isize i = 0; i < area->entities_count; ++i) {
		deserialize_entity(area->entities + i, area_file);
	}
	deserialize_tilemap(&area->map, area_file, arena);
	deserialize_simulator(&area->sim, area_file, arena);
}

void serialize_entity(Entity* entity, FILE* file)
{
	fwrite(&entity->id, sizeof(isize), 1, file);
	fwrite(&entity->body_id, sizeof(isize), 1, file);
	serialize_sprite(&entity->sprite, file);
	fwrite(&entity->counter, sizeof(int32), 1, file);
	fwrite(&entity->facing, sizeof(int32), 1, file);
	//TODO(will) standardize size of enum?
	fwrite(&entity->direction, sizeof(Direction), 1, file);
	fwrite(&entity->kind, sizeof(isize), 1, file);
	fwrite(&entity->events, sizeof(uint64), 1, file);
	//TODO(will) serialize userdata
}

void serialize_area(World_Area* area, FILE* area_file)
{
	fwrite(&area->id, sizeof(isize), 1, area_file);
	fwrite(&area->entities_count, sizeof(isize), 1, area_file);
	fwrite(&area->entities_capacity, sizeof(isize), 1, area_file);
	fwrite(&area->next_entity_id, sizeof(isize), 1, area_file);
	fwrite(area->offset.e, sizeof(real), 2, area_file);
	for(isize i = 0; i < area->entities_count; ++i) {
		serialize_entity(area->entities + i, area_file);
	}
	serialize_tilemap(&area->map, area_file);
	serialize_simulator(&area->sim, area_file);
	fclose(area_file);
}

void serialize_area_link(Area_Link* link, FILE* fp)
{
	fwrite(&link->position.x, sizeof(int32), 1, fp);
	fwrite(&link->position.y, sizeof(int32), 1, fp);
	fwrite(&link->link->id, sizeof(isize), 1, fp);
}

void deserialize_area_link(Area_Link* link, World* world, FILE* fp)
{
	fread(&link->position.x, sizeof(int32), 1, fp);
	fread(&link->position.y, sizeof(int32), 1, fp);
	isize linkid = 0;
	fread(&linkid, sizeof(isize), 1, fp);
	link->link = world->area_stubs + linkid;
}

void serialize_world_area_stub(World_Area_Stub* stub, FILE* fp)
{
	fwrite(&stub->id, sizeof(isize), 1, fp);
	fwrite(&stub->seed, sizeof(usize), 1, fp);
	serialize_area_link(&stub->north, fp);
	serialize_area_link(&stub->south, fp);
	serialize_area_link(&stub->east, fp);
	serialize_area_link(&stub->west, fp);
	fwrite(&stub->biome, sizeof(World_Area_Biome), 1, fp);
}

void deserialize_world_area_stub(World_Area_Stub* stub, World* world, FILE* fp)
{
	fread(&stub->id, sizeof(isize), 1, fp);
	fread(&stub->seed, sizeof(usize), 1, fp);
	deserialize_area_link(&stub->north, world, fp);
	deserialize_area_link(&stub->south, world, fp);
	deserialize_area_link(&stub->east, world, fp);
	deserialize_area_link(&stub->west, world, fp);
	fread(&stub->biome, sizeof(World_Area_Biome), 1, fp);
}



FILE* get_world_file(const char* name, const char* mode)
{
	char save_dir[FilePathMaxLength];
	snprintf(save_dir, FilePathMaxLength, "%ssave/", Game->base_path);
	check_dir(save_dir);
	snprintf(save_dir, FilePathMaxLength, "%ssave/%s", Game->base_path, name);
	check_dir(save_dir);
	
	snprintf(save_dir, FilePathMaxLength, "%ssave/%s/world.dat", 
			Game->base_path,
			name);
	//printf("%s\n", save_dir);
	FILE* world_file = fopen(save_dir, mode);
	return world_file;
}

FILE* get_area_file(const char* name, isize id, const char* mode)
{
	char save_dir[FilePathMaxLength];
	snprintf(save_dir, FilePathMaxLength, "%ssave/", Game->base_path);
	check_dir(save_dir);
	snprintf(save_dir, FilePathMaxLength, "%ssave/%s", Game->base_path, name);
	check_dir(save_dir);
	
	snprintf(save_dir, FilePathMaxLength, "%ssave/%s/areas", Game->base_path, name);
	check_dir(save_dir);
	snprintf(save_dir, FilePathMaxLength, "%ssave/%s/areas/area_%d.dat",
			Game->base_path,
			name,
			id);

	FILE* area_file = fopen(save_dir, mode);
	return area_file;
}

void serialize_world(World* world)
{
	if(world->name[0] == '\0') {
		printf("Could not save world -- name was null\n");
		return;
	}
	FILE* world_file = get_world_file(world->name, "wb");
	if(world_file != NULL) {
		isize namelen = strlen(world->name);
		fwrite(&namelen, sizeof(isize), 1, world_file);
		fwrite(world->name, sizeof(char), strlen(world->name), world_file);
		fwrite(&world->areas_count, sizeof(isize), 1, world_file);
		fwrite(&world->areas_capacity, sizeof(isize), 1, world_file);
		fwrite(&world->areas_width, sizeof(isize), 1, world_file);
		fwrite(&world->areas_height, sizeof(isize), 1, world_file);
		fwrite(&world->next_area_id, sizeof(isize), 1, world_file);
		fwrite(&world->current_area->id, sizeof(isize), 1, world_file);
		for(isize i = 0; i < world->areas_count; ++i) {
			serialize_world_area_stub(world->area_stubs + i, world_file);
		}
		FILE* area_file = get_area_file(world->name, world->current_area->id, "wb");

		if(area_file != NULL) {
			serialize_area(world->current_area, area_file);
		} else {
			printf("Could not open area file\n");
		}
		fclose(world_file);
	} else {
		printf("Could not open world file \n");
	}
}

void deserialize_world(World* world, FILE* world_file)
{
	isize namelen = 0;
	fread(&namelen, sizeof(isize), 1, world_file);
	world->name = arena_push_array(Game->world_arena, char, namelen+1);
	fread((char*)world->name, sizeof(char), namelen, world_file);
	world->name[namelen] = '\0';
	fread(&world->areas_count, sizeof(isize), 1, world_file);
	fread(&world->areas_capacity, sizeof(isize), 1, world_file);
	fread(&world->areas_width, sizeof(isize), 1, world_file);
	fread(&world->areas_height, sizeof(isize), 1, world_file);
	fread(&world->next_area_id, sizeof(isize), 1, world_file);
	isize current_area_id = 0;
	fread(&current_area_id, sizeof(isize), 1, world_file);
	world->area_stubs = arena_push_array(Game->world_arena, World_Area_Stub, world->areas_count);
	for(isize i = 0; i < world->areas_count; ++i) {
		deserialize_world_area_stub(world->area_stubs + i, world, world_file);
	}
	world_start_in_area(world, world->area_stubs + current_area_id, Game->play_arena);
}



