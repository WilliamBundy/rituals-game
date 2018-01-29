enum Entity_Flags
{
	EntityFlag_None,
	EntityFlag_Tail = Flag(1),
	EntityFlag_SameShadow = Flag(2)
};
 
#ifndef WirmphtEnabled
struct Hitbox_Contact
{
	isize a_id;
	isize b_id;

	Entity* a;
	Entity* b;
};

struct Hitbox
{
	isize id;
	Entity* ref;
	u64 mask;
	u64 group;
	AABB box;
};



struct Entity
{
	isize id;
	isize body_id;
	Sim_Body* body;
	Vec2 walk_impulse;

	Hitbox hitbox;
	i32 health;
	i32 attack;
	f32 attack_interval;
	f32 attack_timer;
	f32 knockback;

	Animated_Sprite* anim;
	Sprite sprite;
	f32 z;
	f32 shadow_scale;
	isize kind;

	i32 counter;

	i32 facing;
	Direction direction;

	u64 events;
	u64 flags;
	//void* userdata;
	Rituals_Entity_Userdata userdata;
};

struct Area_Link
{
	Vec2i position;
	WorldAreaStub* link;
};

enum WorldArea_Biome
{
	AreaBiome_Grassland,
	AreaBiome_Desert
};

struct WorldAreaStub
{
	isize id;
	usize seed;
	Area_Link north;
	Area_Link south;
	Area_Link west;
	Area_Link east;
	WorldArea_Biome biome;
};

struct WorldArea
{
	isize id;
	WorldAreaStub* stub;
	World* world;
	Simulator sim;
	Tilemap map;
	Vec2 offset;
	Vec2 target;

	Entity* entities;
	bool entities_dirty;
	isize entities_count, entities_capacity, next_entity_id;

	isize* removed_entities;
	isize removed_entities_count, removed_entities_capacity;

	Hitbox* hitboxes;
	isize hitboxes_count, hitboxes_capacity;
	i32 hitbox_sort_axis;
	Hitbox_Contact* hitbox_contacts;
	isize hitbox_contacts_count, hitbox_contacts_capacity;

	Entity* player;
};
#endif


#define _hitbox_get_x1(h) (AABB_x1(h.box))
GenerateIntrosortForType(_hitbox_sort_on_x_axis, Hitbox, 12, _hitbox_get_x1)
#define _hitbox_get_y1(h) (AABB_y1(h.box))
GenerateIntrosortForType(_hitbox_sort_on_y_axis, Hitbox, 12, _hitbox_get_y1)
#define _entity_get_id(e) (e.id)
GenerateIntrosortForType(entity_sort_on_id, Entity, 12,  _entity_get_id)
GenerateBinarySearchForType(entity_search_for_id, Entity, isize, _entity_get_id)

//#define WorldAreaTilemapWidth (64)
//#define WorldAreaTilemapHeight (64)
#define WorldAreaEntityCapacity (16384)

void init_world_area(WorldArea* area, MemoryArena* arena)
{
	init_simulator(&area->sim, WorldAreaEntityCapacity, arena);
	init_tilemap(&area->map, 
			WorldAreaTilemapWidth,
			WorldAreaTilemapHeight,
			arena);

	area->id = -1;
	area->entities = (Entity*)arenaPush(arena, sizeof(Entity) *  WorldAreaEntityCapacity);
	area->entities_count = 0;
	area->entities_capacity = WorldAreaEntityCapacity;
	area->removed_entities_capacity = 256;
	area->removed_entities = (isize*)arenaPush(arena, sizeof(isize) *  WorldAreaEntityCapacity);
	area->removed_entities_count = 0;
	area->next_entity_id = 0;
	area->entities_dirty = false;
	area->hitboxes = (Hitbox*)arenaPush(arena, sizeof(Hitbox) *  WorldAreaEntityCapacity);
	area->hitboxes_count = 0;
	area->hitboxes_capacity = WorldAreaEntityCapacity;
	area->hitbox_contacts = (Hitbox_Contact*)arenaPush(arena, sizeof(Hitbox_Contact) * WorldAreaEntityCapacity);
	area->hitbox_contacts_count = 0;
	area->hitbox_contacts_capacity = WorldAreaEntityCapacity;
}

void init_entity(Entity* entity)
{
	entity->id = 0;
	entity->body_id = 0;
	wInitSprite(&entity->sprite);
	entity->counter = 0;
	entity->health = 100;
	entity->attack = 0;
	entity->attack_interval = 0.5f;
	entity->attack_timer = 0;
	entity->z = 0;
	entity->shadow_scale = 1.0f;
	entity->anim = NULL;
}

Entity* world_area_get_next_entity(WorldArea* area)
{
	if(area->entities_count + 1 >= area->entities_capacity) {
		Log_Error("Ran out of entities");
		return NULL;
	}

	Entity* e = area->entities + area->entities_count++;
	init_entity(e);
	e->body = sim_get_next_body(&area->sim);
	e->body_id = e->body->id;
	e->id = area->next_entity_id++;
	//e->body->entity = e;
	e->body->entity_id = e->id;
	return e;
}

Entity* world_area_find_entity(WorldArea* area, isize id)
{
	if(area->entities_dirty) {
		entity_sort_on_id(area->entities, area->entities_count);
	}
	isize index = entity_search_for_id(id, area->entities, area->entities_count);
	return index == -1 ? NULL : area->entities + index;
}


i32 entity_id_cmp(const void* a, const void* b)
{
	Entity* ea = (Entity*)a;
	Entity* eb = (Entity*)b;
	return ea->id - eb->id;
}


void world_area_sort_entities_on_id(WorldArea* area)
{
	//qsort(area->entities, area->entities_count, sizeof(Entity),  &entity_id_cmp);
	//printf("%d\n", area->entities[0].id);
	entity_sort_on_id(area->entities, area->entities_count);
}

void world_area_synchronize_entities_and_bodies(WorldArea* area)
{
	world_area_sort_entities_on_id(area);
	sim_sort_bodies_on_id(&area->sim);
	for(isize i = 0; i < area->entities_count; ++i) {
		Entity* e = area->entities + i;
		if(e->body_id == -1) continue;
		Sim_Body* b = sim_find_body(&area->sim, e->body_id);
		if(b == NULL) continue;
		//b->entity = e;
		b->entity_id = e->id;
		e->body = b;
	}
}



//Returns 1 if failed
bool world_area_remove_entity(WorldArea* area, Entity* entity)
{
	if(area->removed_entities_count >= area->removed_entities_capacity) {
		printf("Ran out of room for removing entities\n");
		return true;
	}
	area->removed_entities[area->removed_entities_count++] = entity->id;
	return false;
}

void world_area_on_destroy_entity(Entity* e, WorldArea* area, World* world);
void world_area_remove_entity_internal(WorldArea* area, isize id)
{
	isize index = entity_search_for_id(id, area->entities, area->entities_count);
	if(index == -1) {
		return;
	}
	Entity* entity = area->entities + index;
	world_area_on_destroy_entity(entity, area, area->world);
	sim_remove_body(&area->sim, entity->body_id);
	area->entities[index] = area->entities[--area->entities_count];
	world_area_synchronize_entities_and_bodies(area);
}

void world_area_process_removed_entities(WorldArea* area)
{
	for(isize i = 0; i < area->removed_entities_count; ++i) {
		isize e = area->removed_entities[i];
		world_area_remove_entity_internal(area, e);
	}
	area->removed_entities_count = 0;
}
void world_area_build_hitboxes(WorldArea* area)
{
	area->hitboxes_count = 0;
	for(isize i = 0; i < area->entities_count; ++i) {
		Entity* e = area->entities + i;
		if(e->body == NULL) continue;
		Hitbox* h = area->hitboxes + area->hitboxes_count++;
		h->id = e->id;
		h->ref = e;
		AABB box = e->hitbox.box;
		h->box.center = e->sprite.pos + box.center;
		//if the entity's box is set
		if(v2_dot(box.hext, box.hext) > 1) {
			h->box.hext = box.hext;
		} else {
			h->box.hext = e->body->shape.hext;
			e->hitbox.box.hext = h->box.hext;
			e->hitbox.box.center.y -= e->body->shape.hh;
		}
	}
}

void world_area_process_hitboxes(WorldArea* area)
{
	if(area->hitboxes_count == 0) return;

	area->hitbox_contacts_count = 0;

	if(area->hitbox_sort_axis == 0) {
		_hitbox_sort_on_x_axis(area->hitboxes, area->hitboxes_count);
	} else {
		_hitbox_sort_on_y_axis(area->hitboxes, area->hitboxes_count);
	}
	Vec2 center_sum1 = v2(0, 0);
	Vec2 center_sum2 = v2(0, 0);
	Vec2 variance = v2(0, 0);
	for(isize i = 0; i < area->hitboxes_count; ++i) {
		Hitbox* a = area->hitboxes + i;
		center_sum1 += a->box.center;
		for(isize q = 0; q < 2; ++q) {
			center_sum2.e[q] += a->box.center.e[q] * a->box.center.e[q];
		}

		for(isize j = i + 1; j < area->hitboxes_count; ++j) {
			Hitbox* b = area->hitboxes + j;
			//if(!(a->mask == 0 && b->mask == 0)) {
				//if(!(a->mask & b->mask)) break;
			//}
			u64 ma = a->mask & b->group;
			u64 mb = a->group & b->mask;
			if(ma != 0 || mb != 0) {
				continue;		
			}

			if(area->hitbox_sort_axis == 0) {
				if(AABB_x1(b->box) > AABB_x2(a->box)) {
					break;
				}
			} else if(area->hitbox_sort_axis == 1) {
				if(AABB_y1(b->box) > AABB_y2(a->box)) {
					break;
				}
			}

			if(aabb_intersect(a->box, b->box)) {
				Hitbox_Contact* c = area->hitbox_contacts + area->hitbox_contacts_count++;
				c->a = a->ref;
				c->b = b->ref;
				c->a_id = a->id;
				c->b_id = b->id;
			}
		}
	}
	for(isize i = 0; i < 2; ++i) {
		variance.e[i] = center_sum2.e[i] - center_sum1.e[i] * center_sum1.e[i] / 
			area->hitboxes_count;
	}

	if(variance.x > variance.y) {
		area->hitbox_sort_axis = 0;
	} else {
		area->hitbox_sort_axis = 1;
	}
}
