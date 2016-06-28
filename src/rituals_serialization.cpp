/*
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* 
 * rituals_serialization.cpp
 *
 */ 

void check_dir(char* dir)
{
	if(!PathFileExists(dir)) {
		CreateDirectory(dir, NULL);
	}
}

void serialize_area(World_Area* area, char* path)
{
	char file_name[FilePathMaxLength];
	snprintf(file_name, FilePathMaxLength, "%s/area_%d.dat", path, area->id);
	FILE* area_file = fopen(file_name, "w");

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
		fwrite((void*)strlen(world->name), sizeof(isize), 1, world_file);
		fwrite(world->name, sizeof(char), strlen(world->name), world_file);
		fwrite(world->areas_count, sizeof(isize), 1, world_file);
		fwrite(world->areas_capacity, sizeof(isize), 1, world_file);
		fwrite(world->areas_width, sizeof(isize), 1, world_file);
		fwrite(world->areas_height, sizeof(isize), 1, world_file);
		fwrite(world->current_area->id, sizeof(isize), 1, world_file);
		fclose();
	}

	snprintf(save_dir, FilePathMaxLength, "%ssave/%s/areas", Game->base_path, world->name);
	check_dir(save_dir);

	for(isize i = 0; i < world->areas_count; ++i) {
		serialize_area(world->areas + i, save_dir);
	}
}



