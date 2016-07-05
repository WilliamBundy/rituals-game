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

void deserialize_tilemap(Tilemap* map, FILE* file, Memory_Arena* arena)
{
	fread(&map->w, sizeof(isize), 1, file);
	fread(&map->h, sizeof(isize), 1, file);
	init_tilemap(map, map->w, map->h, arena);
	isize size = map->w * map->h;
	fread(map->tiles, sizeof(Tile), size, file);
	for(isize i = 0; i < size; ++i) {
		deserialize_tile_state(map->states + i, file);
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
	fwrite(&sim->bodies_count, sizeof(isize), 1, file);
	fwrite(&sim->bodies_capacity, sizeof(isize), 1, file);
	fwrite(&sim->next_body_id, sizeof(isize), 1, file);
	fwrite(&sim->sort_axis, sizeof(isize), 1, file);
	sim->bodies = arena_push_array(arena, Sim_Body, sim->bodies_capacity);
	for(isize i = 0; i < sim->bodies_count; ++i) {
		deserialize_sim_body(sim->bodies + i, file);
	}
}


void serialize_sprite(Sprite* s, FILE* file)
{
	//TODO(will) maybe serialize each field separately?
	fwrite(&s, sizeof(Sprite), 1, file);
}

void deserialize_sprite(Sprite* s, FILE* file)
{
	//TODO(will) maybe serialize each field separately?
	fread(&s, sizeof(Sprite), 1, file);
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
}

void deserialize_entity(Entity* entity, FILE* file)
{
	fread(&entity->id, sizeof(isize), 1, file);
	fread(&entity->body_id, sizeof(isize), 1, file);
	serialize_sprite(&entity->sprite, file);
	fread(&entity->counter, sizeof(int32), 1, file);
	fread(&entity->facing, sizeof(int32), 1, file);
	//TODO(will) standardize size of enum?
	fread(&entity->direction, sizeof(Direction), 1, file);
}


void serialize_area(World_Area* area, char* path)
{
	char file_name[FilePathMaxLength];
	snprintf(file_name, FilePathMaxLength, "%s/area_%d.dat", path, area->id);
	FILE* area_file = fopen(file_name, "w");
	if(area_file != NULL) {
		fwrite(&area->id, sizeof(isize), 1, area_file);
		serialize_tilemap(&area->map, area_file);
		serialize_simulator(&area->sim, area_file);
		fwrite(area->offset.e, sizeof(real), 2, area_file);
		fwrite(&area->entities_count, sizeof(isize), 1, area_file);
		fwrite(&area->entities_capacity, sizeof(isize), 1, area_file);
		fwrite(&area->next_entity_id, sizeof(isize), 1, area_file);
		for(isize i = 0; i < area->entities_count; ++i) {
			serialize_entity(area->entities + i, area_file);
		}
		fclose(area_file);
	}
}

void deserialize_area(World_Area* area, FILE* area_file, Memory_Arena* arena)
{
	fread(&area->id, sizeof(isize), 1, area_file);
	deserialize_tilemap(&area->map, area_file, arena);
	dserialize_simulator(&area->sim, area_file, arena);
	fread(area->offset.e, sizeof(real), 2, area_file);
	fread(&area->entities_count, sizeof(isize), 1, area_file);
	fread(&area->entities_capacity, sizeof(isize), 1, area_file);
	fread(&area->next_entity_id, sizeof(isize), 1, area_file);

	area->entities = arena_push_array(arena, Entity, area->entities_capacity);
	for(isize i = 0; i < area->entities_count; ++i) {
		deserialize_entity(area->entities + i, area_file);
	}
}


void serialize_world(World* world)
{
	char save_dir[FilePathMaxLength];
	snprintf(save_dir, FilePathMaxLength, "%ssave/", Game->base_path);
	check_dir(save_dir);
	snprintf(save_dir, FilePathMaxLength, "%ssave/%s", Game->base_path, world->name);
	check_dir(save_dir);
	
	snprintf(save_dir, FilePathMaxLength, "%ssave/%s/world.dat", Game->base_path, world->name);
	FILE* world_file = fopen(save_dir, "w");
	if(world_file != NULL) {
		isize namelen = strlen(world->name);
		fwrite(&namelen, sizeof(isize), 1, world_file);
		fwrite(world->name, sizeof(char), strlen(world->name), world_file);
		fwrite(&world->areas_count, sizeof(isize), 1, world_file);
		fwrite(&world->areas_capacity, sizeof(isize), 1, world_file);
		fwrite(&world->areas_width, sizeof(isize), 1, world_file);
		fwrite(&world->areas_height, sizeof(isize), 1, world_file);
		fwrite(&world->current_area->id, sizeof(isize), 1, world_file);
		//TODO(will) write world area stubs here
		fclose(world_file);
	}

	snprintf(save_dir, FilePathMaxLength, "%ssave/%s/areas", Game->base_path, world->name);
	check_dir(save_dir);
	serialize_area(world->current_area, save_dir);

}



