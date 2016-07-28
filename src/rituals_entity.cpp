/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* 
 * rituals_entity.cpp
 *
 */ 


enum Direction 
{
	Direction_North,
	Direction_South,
	Direction_East,
	Direction_West
};

struct Entity
{
	isize id;
	isize body_id;
	Sim_Body* body;
	Sprite sprite;

	int32 counter;

	int32 facing;
	Direction direction;
	
	//deprecated
	isize held_entity_id;

	//probably overkill?
	isize userdata_id_major;
	isize userdata_id_minor;
	void* userdata;
};

#define _entity_get_id(e) (e.id)
GenerateIntrosortForType(entity_sort_on_id, Entity, 12,  _entity_get_id)
GenerateBinarySearchForType(entity_search_for_id, Entity, isize, _entity_get_id)

typedef void (*UpdateEntitiesFunc)(Entity* entities, isize count, World_Area* area, World* world);
void rituals_update_entities(Entity* entities, isize count, World_Area* area, World* world)
{

}

