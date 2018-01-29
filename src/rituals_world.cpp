#ifndef WirmphtEnabled
struct World
{
	char* name;

	WorldAreaStub* area_stubs;
	isize next_area_id;
	isize areas_count, areas_capacity;
	isize areas_width, areas_height;

	WorldArea* current_area;
	usize seed;

	Entity global_player_entity;
	Sim_Body global_player_body;

	usize slowtick_timer;
	usize slowtick_timer_interval;

	Particle_Style base_style;
	Emitter emitter;
};
#endif
isize Anim_Standing;
isize Anim_Walking;

void init_world(World* world, isize width, isize height, usize seed, MemoryArena* arena)
{
	world->seed = seed;
	world->areas_capacity = width * height * 2;
	world->area_stubs = (WorldAreaStub*)arenaPush(arena, 
			sizeof(WorldAreaStub) * world->areas_capacity);
	world->areas_count = 0;
	world->areas_width = width;
	world->areas_height = height;
	world->next_area_id = 0;
	world->current_area = NULL;
	world->slowtick_timer = 0;
	world->slowtick_timer_interval = 60;

	init_entity(&world->global_player_entity);
	init_body(&world->global_player_body);

	Entity* e = &world->global_player_entity;
	Sim_Body* b = &world->global_player_body;
	e->sprite.texture = rect2i(0, 0, 32, 32);
	e->anim = (Animated_Sprite*)arenaPush(arena, sizeof(Animated_Sprite)); 
	init_animated_sprite(e->anim, 64, arena);

	Anim_Standing = add_animation(e->anim, 
			make_animaiton_from_strip(arena, 12, rect2i(0, 0, 32, 32), 1));
	Anim_Walking = add_animation(e->anim, 
			make_animaiton_from_strip(arena, 12, rect2i(0, 11 * 32, 32, 32), 4));
	
	b->shape.hext = v2(5, 5);
	e->hitbox.box.hext = b->shape.hext + v2(1, 1);
	e->hitbox.box.center.y -= b->shape.hh;
	e->sprite.size = v2(32, 32);
	b->group = 1;
	e->sprite.flags = Anchor_BottomCenter;
	b->damping = 0.5f;
	b->restitution = 0;
	b->flags = Body_Flag_No_Friction;
	e->kind = EntityKind_Player;
	auto p = &e->userdata.player;
	p->heal_cooldown = 5.0f;
	p->heal_rate = 1;
	p->heal_to_interval = 25;
	p->heal_timer = 0;

}

//Make a player struct?
void world_area_init_player(WorldArea* area, Vec2i tile_pos, bool move_player=true)
{
	Entity* player_entity = world_area_find_entity(area, 0);
	Sim_Body* player = sim_find_body(&area->sim, player_entity->body_id);
	*player_entity = area->world->global_player_entity;
	*player = area->world->global_player_body;

	if(move_player) {
		player->shape.center = v2(tile_pos.x * TiSz, tile_pos.y * TiSz);
	}

	area->offset = player->shape.center;
	area->target = player->shape.center;
}

void world_area_deinit_player(WorldArea* area, bool move_player=true)
{
	Entity* player_entity = world_area_find_entity(area, 0);
	Sim_Body* player = sim_find_body(&area->sim, player_entity->body_id);
	area->world->global_player_entity = *player_entity;
	area->world->global_player_body = *player;
}

//void deserialize_area(WorldArea* area, FILE* file, MemoryArena* arena);

//TODO(will) call init_world_area on area before calling
Entity* rituals_spawn_enemy(WorldArea* area, isize enemykind, Vec2 position);
void generate_world_area(Game_Registry* Registry, World* world, WorldArea* area, WorldAreaStub* stub)
{
	area->stub = stub;
	area->world = world;
	Random rand;
	Random* r = &rand;
	init_random(r, stub->seed);
	generate_tilemap(&area->map, stub->seed);
	area->id = stub->id;
	//Spawn boxes
	for(isize i = 0; i < WorldAreaTilemapWidth; ++i) {
		Entity* e = world_area_get_next_entity(area);
		Sim_Body* b = sim_find_body(&area->sim, e->body_id);
		e->sprite.texture = rect2i(8*32, 16, 32, 48);
		b->shape.hw = 15;
		b->shape.hh = 11;
		b->inv_mass = 1.0f;
		e->sprite.size = v2(32, 48);
		e->sprite.flags = Anchor_BottomCenter;
		e->shadow_scale = 1/0.75f;  
		e->kind = EntityKind_Prop;
		auto p = &e->userdata.prop;
		p->contains = EntityKind_Pickup;
		p->subtype = PickupKind_Health;
		p->amount = rand_range_int(r, 1, 2);
		p->quality = 25;

		do {
			b->shape.center = v2(
					rand_range(r, 0, area->map.w * 32),
					rand_range(r, 0, area->map.h * 32));
		}
		while (Registry->tiles[tilemap_get_at(&area->map, b->shape.center)].solid);
	}

	//Spawn trees
	for(isize i = 0; i < WorldAreaTilemapWidth/4; ++i) {
		Entity* e = world_area_get_next_entity(area);
		Sim_Body* b = sim_find_body(&area->sim, e->body_id);
		e->sprite.texture = rect2i(0, 5*32, 96, 144);
		b->shape.hw = 16;
		b->shape.hh = 15;
		b->inv_mass = 1.0f;
		b->flags = Body_Flag_Static;
		e->sprite.size = v2(96, 144) * 2;
		e->sprite.flags = Anchor_BottomCenter;
		e->sprite.sort_offset = -60;
		e->sprite.center = v2(2, -b->shape.hh * 2);
		e->kind = EntityKind_Static;
		//e->kind = 1000;
		
		do {
			b->shape.center = v2(
					rand_range(r, 0, area->map.w * 32),
					rand_range(r, 0, area->map.h * 32));
		}
		while (Registry->tiles[tilemap_get_at(&area->map, b->shape.center)].solid);
	}

	for(isize i = 0; i < WorldAreaTilemapWidth ; ++i) {
		Vec2 pos;
		do {
			pos = v2(
					rand_range(r, 0, area->map.w * 32),
					rand_range(r, 0, area->map.h * 32));
		}
		while (Registry->tiles[tilemap_get_at(&area->map, pos)].solid);
		rituals_spawn_enemy(area, rand_range_int(r, 0, 4), pos);
	}

	generate_statics_for_tilemap(Registry, &area->sim, &area->map, Game->tempArena);
}

void world_switch_current_area(World* world, Area_Link link, MemoryArena* arena)
{
	if(link.link == NULL) return;
	world_area_deinit_player(world->current_area);
	WorldArea* new_area = NULL;
	if(new_area == NULL) {
		new_area = (WorldArea*)arenaPush(arena, sizeof(WorldArea));
		init_world_area(new_area, arena);
		generate_world_area(Game->registry, world, new_area, link.link);
	}
	world_area_init_player(new_area, link.position);
	world->current_area = new_area;
}

void world_start_in_area(World* world, WorldAreaStub* area, MemoryArena* arena)
{
	WorldArea* new_area = NULL;
	if(new_area == NULL) {
		new_area = (WorldArea*)arenaPush(arena, sizeof(WorldArea));
		init_world_area(new_area, arena);
		generate_world_area(Game->registry,world, new_area, area);
		world_area_init_player(new_area, 
				v2i(WorldAreaTilemapWidth / 2,
					WorldAreaTilemapHeight /2));
	} else {
		world_area_init_player(new_area, v2i(0, 0),  false);
	}
	world_area_synchronize_entities_and_bodies(new_area);
	world->current_area = new_area;
}

void generate_world(char* name, World* world)
{
	world->name = name;

	for(isize i = 0; i < world->areas_height; ++i) {
		for(isize j = 0; j < world->areas_width; ++j) {
			isize index = i * world->areas_width + j;
			WorldAreaStub* stub = world->area_stubs + world->areas_count++; 
			stub->id = world->next_area_id++;
			stub->seed = world->seed + stub->id;
			isize north_link = modulus(i - 1, world->areas_height) * 
				world->areas_width + j;
			isize south_link = modulus(i + 1, world->areas_height) * 
				world->areas_width + j;
			isize west_link = i * world->areas_width + 
				modulus(j - 1, world->areas_width);
			isize east_link = i * world->areas_width + 
				modulus(j + 1, world->areas_width);

			stub->north = Area_Link {
				v2i(WorldAreaTilemapWidth / 2,  WorldAreaTilemapHeight - 1), 
					world->area_stubs + north_link
			};
			stub->south = Area_Link {
				v2i(WorldAreaTilemapWidth / 2, 1),
					world->area_stubs + south_link
			};
			stub->west = Area_Link {
				v2i(WorldAreaTilemapWidth - 1, WorldAreaTilemapHeight / 2),
					world->area_stubs + west_link
			};
			stub->east = Area_Link {
				v2i(1, WorldAreaTilemapHeight / 2),
					world->area_stubs + east_link
			};

		}
	}
}

void world_area_walk_entities(WorldArea* area, World* world)
{
	rituals_walk_entities(area->entities, area->entities_count, 
			area, world);
}

void world_area_animate_entities(WorldArea* area, World* world)
{
	rituals_animate_entities(area->entities, area->entities_count,
			area, world);
}

bool world_area_frametick_entities(WorldArea* area, World* world)
{
	return rituals_frametick_entities(area->entities, area->entities_count,
			area, world);
}

void world_area_slowtick_entities(WorldArea* area, World* world)
{
	rituals_frametick_entities(area->entities, area->entities_count, 
			area, world);
}

void world_area_on_destroy_entity(Entity* e, WorldArea* area, World* world)
{
	rituals_on_destroy_entity(e, area, world);	
}

void world_area_on_activate_entity(Entity* e, WorldArea* area, World* world)
{
	rituals_on_activate_entity(e, area, world);
}

void world_area_hit_entities(WorldArea* area, World* world)
{
	rituals_hit_entities(area->hitbox_contacts, area->hitbox_contacts_count,
			area, world);
}

void world_area_contact_entities(WorldArea* area, World* world)
{
	rituals_contact_entities(area->sim.contacts, area->sim.contacts_count, 
			area, world);
}

void world_area_interact(WorldArea* area, World* world)
{
	rituals_interact_entities(area->entities, area->entities_count,
			area, world);
}

void world_area_render(WorldArea* area, World* world)
{	
	//render_set_current_group(0);
	Vec2 target = area->target;
	Game->render->scale = 2;
	area->offset += (target - area->offset) * 0.1f;
	area->offset -= (Game->size * 0.5f) / Game->render->scale;
	
	if(area->offset.x < 0) 
		area->offset.x = 0;
	else if((area->offset.x + Game->size.x) > area->map.w * TiSz)
		area->offset.x = area->map.w * TiSz - Game->size.x;

	if(area->offset.y < 0) 
		area->offset.y = 0;
	else if((area->offset.y + Game->size.y) > area->map.h * TiSz)
		area->offset.y = area->map.h * TiSz - Game->size.y;
	Game->render->offsetX = area->offset.x * 2;
	Game->render->offsetY = area->offset.y * 2;
	area->offset += (Game->size * 0.5f) / Game->render->scale;

	Rect2 screen = rect2(
			area->offset.x - Game->size.x / (2 * Game->render->scale),
			area->offset.y - Game->size.y / (2 * Game->render->scale),  
			Game->size.x, Game->size.y);

	isize sprite_count_offset = render_tilemap(Game->registry, &area->map, v2(0,0), screen);

	world_area_animate_entities(area, world);
	wGroupSort(Game->render, sprite_count_offset, Game->render->count);
}


void init_play_state();
int recursively_delete_folder(char* path, bool append_base_path);
void world_area_update(WorldArea* area, World* world)
{
	world_area_synchronize_entities_and_bodies(area);
	area->player = world_area_find_entity(area, 0);

	f32 dt = 1.0 / 60.0f;
	world_area_synchronize_entities_and_bodies(area);
	area->player = world_area_find_entity(area, 0);

	bool exit = world_area_frametick_entities(area, world);
	if(exit) return;
	world_area_slowtick_entities(area, world);

	world_area_walk_entities(area, world);
	//TODO(will) use same sorted array as world_area_walk_entities
	for(isize i = 0; i < area->entities_count; ++i) {
		Entity* e = area->entities + i;
		e->body->velocity += e->walk_impulse;
		if(e->attack_timer > 0) {
			e->attack_timer -= TimeStep;
		}
	}

	sim_update(Game->registry, &area->sim, &area->map, dt);
	
	Vec2 target = area->player->body->shape.center;
	area->target = target;
	if(target.x < 0) {
		world_switch_current_area(globalWorld, area->stub->west, Game->gameArena);
	} else if(target.x > area->map.w * TiSz) {
		world_switch_current_area(globalWorld, area->stub->east, Game->gameArena);
	} else if(target.y < 0) {
		world_switch_current_area(globalWorld, area->stub->north, Game->gameArena);
	} else if(target.y > area->map.h * TiSz) {
		world_switch_current_area(globalWorld, area->stub->south, Game->gameArena);
	}
	world_area_interact(area, world);
	world_area_render(area, world);

	Game_Input* input = Game->input;
	if(input->mouse[SDL_BUTTON_LEFT] == Button_JustDown) {
		Vec2 pv = area->player->walk_impulse * TimeStep;
		f32 player_mag = v2_dot(pv, pv);
		player_mag = sqrtf(player_mag);
		for(isize i = 0; i < 2; ++i) {
			Entity* e = world_area_get_next_entity(area);
			e->kind = EntityKind_Bullet;
			e->sprite = spriteMake(Sprite_NoTexture, ColorWhite, 
					area->player->sprite.pos, 2, 2, 0, 0, 0, 0);
			e->sprite.pos += v2(
					rand_range(&Game->r, -3, 3),
					rand_range(&Game->r, -3, 3));
			e->body->shape.center = e->sprite.pos;
			e->sprite.flags = Anchor_BottomCenter;
			e->body->shape.hext = v2(1, 1);
			e->body->flags = Body_Flag_No_Friction | Body_Flag_Always_Contact;
			e->body->damping = 1.0f;
			e->body->mask = 3;
			e->body->group = 1;
			e->attack = 6;
			e->attack += (i32)roundf(rand_range(&Game->r, -0.15, 0.15) * e->attack);
			e->hitbox.mask = Flag(2);
			e->hitbox.box.hext = e->body->shape.hext * 1.5f;
			e->hitbox.box.hh = 8;
			e->hitbox.box.center.y = -8;
			e->flags = EntityFlag_Tail;// | EntityFlag_SameShadow;
			e->z = 16;
			e->shadow_scale = 8.0f;

			Vec2 dmouse =  Input->mouse_pos - e->sprite.pos; 
			f32 a = v2_to_angle(dmouse);
					
			Vec2 angle_range = v2(a - 0.2f, a + 0.2f);
			a += rand_range(&Game->r, -5, 5) * Math_DegToRad;
			e->body->velocity = v2_from_angle(a) * (600 - rand_range(&Game->r, 0, 200));
			area->player->body->velocity -= e->body->velocity;
		}
	}

	world_area_build_hitboxes(area);
	world_area_process_hitboxes(area);
	world_area_hit_entities(area, world);
	world_area_contact_entities(area, world);
	world_area_process_removed_entities(area);
	
}




