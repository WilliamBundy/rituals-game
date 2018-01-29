typedef union Vec2 Vec2;
typedef union Vec3 Vec3;
typedef union Vec4 Vec4;
typedef union Vec2i Vec2i;
typedef union Vec3i Vec3i;
typedef union Vec4i Vec4i;
typedef union Rect2 Rect2;
typedef struct Rect2_Clip_Info Rect2_Clip_Info;
typedef struct Rect2i Rect2i;
typedef union AABB AABB;
typedef struct Sprite Sprite;
typedef struct wVertex wVertex;
typedef struct wShader wShader;
typedef struct wTexture wTexture;
typedef struct SpriteList SpriteList;
typedef struct wRenderGroup wRenderGroup;
typedef struct Tile_Info Tile_Info;
typedef struct Tile_State Tile_State;
typedef struct Game_Registry Game_Registry;
typedef struct Rituals_Bat_Userdata Rituals_Bat_Userdata;
typedef struct Rituals_Snake_Userdata Rituals_Snake_Userdata;
typedef struct Rituals_Goblin_Knight_Userdata Rituals_Goblin_Knight_Userdata;
typedef struct Rituals_Enemy_Userdata Rituals_Enemy_Userdata;
typedef struct Rituals_Player_Userdata Rituals_Player_Userdata;
typedef struct Rituals_Prop_Userdata Rituals_Prop_Userdata;
typedef struct Rituals_Item_Pickup_Userdata Rituals_Item_Pickup_Userdata;
typedef struct Rituals_Health_Pickup_Userdata Rituals_Health_Pickup_Userdata;
typedef struct Rituals_Pickup_Userdata Rituals_Pickup_Userdata;
typedef struct Unknown_Userdata Unknown_Userdata;
typedef union Rituals_Entity_Userdata Rituals_Entity_Userdata;
typedef struct Random Random;
typedef struct Game_Input Game_Input;
typedef struct Game_Main Game_Main;
typedef struct Platform Platform;
typedef struct Animation_Frame Animation_Frame;
typedef struct Animated_Sprite Animated_Sprite;
typedef struct Animation Animation;
typedef struct Tilemap Tilemap;
typedef struct Sim_Body Sim_Body;
typedef struct Sim_Contact Sim_Contact;
typedef struct Sim_Grid_Cell Sim_Grid_Cell;
typedef struct Sim_Static_Grid Sim_Static_Grid;
typedef struct Simulator Simulator;
typedef struct Particle Particle;
typedef struct Particle_Style Particle_Style;
typedef struct Emitter Emitter;
typedef struct Hitbox_Contact Hitbox_Contact;
typedef struct Hitbox Hitbox;
typedef struct Entity Entity;
typedef struct Area_Link Area_Link;
typedef struct WorldAreaStub WorldAreaStub;
typedef struct WorldArea WorldArea;
typedef struct World World;
typedef enum SpriteFlags SpriteFlags;
typedef enum Rituals_Entity_Kinds Rituals_Entity_Kinds;
typedef enum Rituals_Enemy_Kind Rituals_Enemy_Kind;
typedef enum Rituals_Pickup_Kind Rituals_Pickup_Kind;
typedef enum Button_State Button_State;
typedef enum Direction Direction;
typedef enum Entity_Flags Entity_Flags;
typedef enum WorldArea_Biome WorldArea_Biome;
typedef void* Entity_On_Activate;
typedef i32 Tile;

union Vec2
{
	struct {
		f32 x;
		f32 y;
	};
	f32 e[2];
};

union Vec3
{
	struct {
		f32 x;
		f32 y;
		f32 z;
	};
	f32 e[3];
};

union Vec4
{
	struct {
		f32 x;
		f32 y;
		f32 z;
		f32 w;
	};
	f32 e[4];
};

union Vec2i
{
	struct {
		i32 x;
		i32 y;
	};
	i32 e[2];
};

union Vec3i
{
	struct {
		i32 x;
		i32 y;
		i32 z;
	};
	i32 e[3];
};

union Vec4i
{
	struct {
		i32 x;
		i32 y;
		i32 z;
		i32 w;
	};
	i32 e[4];
};

union Rect2
{
	struct {
		f32 x;
		f32 y;
		f32 w;
		f32 h;
	};
	struct {
		Vec2 position;
		Vec2 size;
	};
	f32 e[4];
};

struct Rect2_Clip_Info
{
	Vec2 rp1;
	Vec2 rp2;
	Vec2 diff1;
	Vec2 diff2;
};

struct Rect2i
{
	i16 x;
	i16 y;
	i16 w;
	i16 h;
};

union AABB
{
	struct {
		Vec2 center;
		union {
			struct {
				f32 hw;
				f32 hh;
			};
			struct {
				Vec2 hext;
			};
		};
	};
	f32 e[4];
};

struct Sprite
{
	i32 flags;
	u32 color;
	union {
		Vec2 pos;
		struct {
			f32 x;
			f32 y;
		};
	};
	union {
		Vec2 size;
		struct {
			f32 w;
			f32 h;
		};
	};
	union {
		Vec2 center;
		struct {
			f32 cx;
			f32 cy;
		};
	};
	union {
		struct {
			i16 tx;
			i16 ty;
			i16 tw;
			i16 th;
		};
		Rect2i texture;
	};
	f32 angle;
	f32 sdf;
	f32 sort_offset;
};

struct wVertex
{
	f32 kind;
	f32 x;
	f32 y;
	f32 u;
	f32 v;
	f32 sx;
	f32 sy;
	u32 color;
};

struct wShader
{
	u32 vert;
	u32 frag;
	u32 program;
	i32 uTint;
	i32 uInvTextureSize;
	i32 uPxRange;
};

struct wTexture
{
	i32 w;
	i32 h;
	u32 *pixels;
	u32 glIndex;
};

struct SpriteList
{
	i32 start;
	i32 count;
	f32 l;
	f32 b;
	f32 r;
	f32 t;
};

struct wRenderGroup
{
	wTexture *texture;
	wShader *shader;
	u32 vao;
	u32 vbo;
	i32 blank;
	i32 clearOnDraw;
	f32 dpi;
	f32 scale;
	f32 offsetX;
	f32 offsetY;
	u32 tint;
	float sdfPxRange;
	Sprite *sprites;
	wVertex *verts;
	u16 *indices;
	i32 count;
	i32 capacity;
};

struct Tile_Info
{
	isize id;
	usize hash;
	Rect2i texture;
	Rect2i bottom_texture;
	Rect2i top_texture;
	f32 movement_modifier;
	f32 friction;
	const char *name;
	i32 max_damage;
	bool immune_to_damage;
	char *break_to_name;
	isize break_to_id;
	bool solid;
	u64 body_mask;
	u64 body_group;
	bool has_top_texture;
	bool has_bottom_texture;
};

struct Tile_State
{
	isize id;
	i32 damage;
};

struct Game_Registry
{
	Tile_Info *tiles;
	isize tiles_count;
	isize tiles_capacity;
};

struct Rituals_Bat_Userdata
{
	Vec2 perch;
};

struct Rituals_Snake_Userdata
{
	f32 chase_speed_modifier;
};

struct Rituals_Goblin_Knight_Userdata
{
	Vec2 patrol_start;
	Vec2 patrol_end;
};

struct Rituals_Enemy_Userdata
{
	isize kind;
	isize mode;
	f32 speed;
	f32 alert_dist;
	f32 follow_dist;
	union {
		Rituals_Bat_Userdata bat;
		Rituals_Snake_Userdata snake;
		Rituals_Goblin_Knight_Userdata goblin_knight;
	};
};

struct Rituals_Player_Userdata
{
	isize held_entity_id;
	f32 heal_cooldown;
	f32 heal_timer;
	i32 heal_rate;
	i32 heal_to_interval;
};

struct Rituals_Prop_Userdata
{
	Rituals_Entity_Kinds contains;
	isize subtype;
	isize amount;
	isize quality;
};

struct Rituals_Item_Pickup_Userdata
{
	isize id;
	isize count;
};

struct Rituals_Health_Pickup_Userdata
{
	isize amount;
};

struct Rituals_Pickup_Userdata
{
	isize kind;
	Rituals_Item_Pickup_Userdata item;
	Rituals_Health_Pickup_Userdata health;
};

struct Unknown_Userdata
{
	void *ptr;
	isize size;
};

union Rituals_Entity_Userdata
{
	Rituals_Prop_Userdata prop;
	Rituals_Player_Userdata player;
	Rituals_Enemy_Userdata enemy;
	Rituals_Pickup_Userdata pickup;
	Unknown_Userdata unknown;
};

struct Random
{
	u64 x;
	u64 y;
};

struct Game_Input
{
	isize num_keys_down;
	isize num_mouse_down;
	i8 *scancodes;
	i8 *keycodes;
	i8 *mouse;
	i32 mouse_x;
	i32 mouse_y;
	Vec2i screen_mouse_pos;
	Vec2 mouse_pos;
	bool capture_tabs;
	bool capture_newlines;
	char *text;
	isize text_count;
};

struct Game_Main
{
	SDL_Window *window;
	i32 state;
	f32 scale;
	Vec2 size;
	Vec2i window_size;
	MemoryArena *baseArena;
	MemoryArena *gameArena;
	MemoryArena *tempArena;
	const char *base_path;
	isize base_path_length;
	Random r;
	Game_Registry *registry;
	wRenderGroup *render;
	Game_Input *input;
	isize last_frame_time;
};

struct Platform
{
	const char *window_title;
	Vec2i window_size;
	f32 game_scale;
	i32 display_index;
	bool check_gl_attributes;
	const char *vertex_shader;
	const char *frag_shader;
	SDL_Window *window;
};

struct Animation_Frame
{
	Vec3 position;
	f32 angle;
	Vec4 color;
	Vec2 size;
	Rect2i texture;
	f32 sort_offset;
};

struct Animated_Sprite
{
	Animation **animations;
	isize animations_count;
	isize animations_capacity;
	isize current_animation;
	isize current_frame;
	bool running;
	f32 timer;
};

struct Animation
{
	isize id;
	Animation_Frame *frames;
	isize frames_count;
	isize frames_capacity;
	f32 fps;
	f32 inv_fps;
	bool looping;
};

struct Tilemap
{
	isize w;
	isize h;
	Tile *tiles;
	Tile_State *states;
};

struct Sim_Body
{
	isize id;
	AABB shape;
	Vec2 velocity;
	Vec2 force;
	Vec2 collision_vel;
	f32 inv_mass;
	f32 restitution;
	f32 damping;
	u64 flags;
	u64 group;
	u64 mask;
	isize entity_id;
	Entity *entity;
};

struct Sim_Contact
{
	isize a_id;
	isize b_id;
	Sim_Body *static_b;
	Vec2 overlap;
	f32 mag;
	Vec2 normal;
};

struct Sim_Grid_Cell
{
	Sim_Body *body;
	Sim_Grid_Cell *next;
};

struct Sim_Static_Grid
{
	Sim_Grid_Cell *cell_storage;
	isize cell_storage_count;
	isize cell_storage_capacity;
	Sim_Grid_Cell **cells;
	isize cells_length;
	Vec2i size;
};

struct Simulator
{
	Sim_Body *static_bodies;
	isize static_bodies_count;
	isize static_bodies_capacity;
	Sim_Static_Grid *grid;
	Sim_Body *bodies;
	isize bodies_count;
	isize bodies_capacity;
	isize next_body_id;
	Sim_Contact *contacts;
	isize contacts_count;
	isize contacts_capacity;
	isize sort_axis;
};

struct Particle
{
	Vec3 position;
	Vec3 velocity;
	f32 scale;
	f32 angle;
	f32 angular_vel;
	i32 frame;
	i32 time;
	i32 total_time;
	i32 style_index;
};

struct Particle_Style
{
	i32 id;
	u32 color;
	u32 shadow_color;
	Rect2i texture;
	Vec2 size;
	Vec3 acceleration;
	f32 impulse_min;
	f32 impulse_max;
	f32 angle_min;
	f32 angle_max;
	f32 angle_vel_min;
	f32 angle_vel_max;
	f32 scale_min;
	f32 scale_max;
	i32 frame_min;
	i32 frame_max;
	i32 max_frames;
	i32 framerate;
	i32 time_min;
	i32 time_max;
	bool time_scaling;
	bool time_alpha;
	f32 ground_restitution;
	f32 skid_on_bounce_min;
	f32 skid_on_bounce_max;
	f32 jitter_on_bounce_mag;
};

struct Emitter
{
	Particle *particles;
	isize particles_count;
	isize particles_capacity;
	Particle_Style *styles;
	isize styles_count;
	isize styles_capacity;
};

struct Hitbox_Contact
{
	isize a_id;
	isize b_id;
	Entity *a;
	Entity *b;
};

struct Hitbox
{
	isize id;
	Entity *ref;
	u64 mask;
	u64 group;
	AABB box;
};

struct Entity
{
	isize id;
	isize body_id;
	Sim_Body *body;
	Vec2 walk_impulse;
	Hitbox hitbox;
	i32 health;
	i32 attack;
	f32 attack_interval;
	f32 attack_timer;
	f32 knockback;
	Animated_Sprite *anim;
	Sprite sprite;
	f32 z;
	f32 shadow_scale;
	isize kind;
	i32 counter;
	i32 facing;
	Direction direction;
	u64 events;
	u64 flags;
	Rituals_Entity_Userdata userdata;
};

struct Area_Link
{
	Vec2i position;
	WorldAreaStub *link;
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
	WorldAreaStub *stub;
	World *world;
	Simulator sim;
	Tilemap map;
	Vec2 offset;
	Vec2 target;
	Entity *entities;
	bool entities_dirty;
	isize entities_count;
	isize entities_capacity;
	isize next_entity_id;
	isize *removed_entities;
	isize removed_entities_count;
	isize removed_entities_capacity;
	Hitbox *hitboxes;
	isize hitboxes_count;
	isize hitboxes_capacity;
	i32 hitbox_sort_axis;
	Hitbox_Contact *hitbox_contacts;
	isize hitbox_contacts_count;
	isize hitbox_contacts_capacity;
	Entity *player;
};

struct World
{
	char *name;
	WorldAreaStub *area_stubs;
	isize next_area_id;
	isize areas_count;
	isize areas_capacity;
	isize areas_width;
	isize areas_height;
	WorldArea *current_area;
	usize seed;
	Entity global_player_entity;
	Sim_Body global_player_body;
	usize slowtick_timer;
	usize slowtick_timer_interval;
	Particle_Style base_style;
	Emitter emitter;
};

f32 clamp_01(f32 x);
f32 lerp(f32 a, f32 b, f32 t);
wTexture* wLoadTexture(const char* basePath, const char* filename, MemoryArena* arena);
void wUploadTexture(wTexture* texture);
void wInitSprite(Sprite* s);
Sprite* wGroupAddRaw(wRenderGroup* group, i32 flags, u32 color, f32 x, f32 y, f32 w, f32 h, i16 tx, i16 ty, i16 tw, i16 th);
Sprite spriteMake(i32 flags, u32 color, Vec2 pos, f32 w, f32 h, i16 tx, i16 ty, i16 tw, i16 th);
Sprite* wGetSprite(wRenderGroup* group);
void wGroupInit(wRenderGroup* group, i32 cap, wShader* shader, wTexture* texture);
void wGroupSort(wRenderGroup* group, isize start, isize end);
void wGroupDraw(i32 width, i32 height, wRenderGroup* group);
void init_tile_state(Tile_State* state, isize id);
void registryInit(Game_Registry* reg, isize capacity, MemoryArena* arena);
Tile_Info* add_tile_info(Game_Registry* Registry, isize index, const char* name, f32 movement_modifier, f32 friction, Rect2i texture, bool solid, isize break_to_index);
void register_all_rituals_tile_info(Game_Registry* Registry);
void register_everything_in_rituals(Game_Registry* Registry);
u64 next_random_u64(Random* r);
void init_random(Random* r, u64 seed);
f64 next_random_double(Random* r);
f32 next_random_float(Random* r);
f32 next_random(Random* r);
f32 rand_range(Random* r, f32 min, f32 max);
i32 rand_range_int(Random* r, f32 min, f32 max);
void input_init(Game_Input* input, MemoryArena* arena);
void input_text_append_char(Game_Input* input, char c);
isize append_input_text(Game_Input* input, char* str, isize str_cap, isize str_len, isize insert_from_end, MemoryArena* tempArena);
i32 platform_init(Platform* platform);
i32 game_init(Platform* platform);
void game_update_screen();
i32 game_start(Platform* platform, Game_Main* game);
void platform_quit();
void game_switch_state(i32 state);
f64 next_random_double();
f32 next_random_float();
f32 next_random();
f32 rand_range(f32 min, f32 max);
i32 rand_range_int(f32 min, f32 max);
void game_set_scale(f32 scale);
void game_calc_mouse_pos(Vec2 offset);
bool is_valid_filename_char(char c);
char replace_filename_char(char c, char replace);
isize replace_invalid_filename_chars(char* str, isize len, char replace);
void render_add(Sprite* sprite);
Sprite* renderAdd(i32 flags, u32 color, Vec2 pos, Vec2 size, Rect2i t);
void init_animation_frame(Animation_Frame* fr);
void init_animation(Animation* a, f32 fps, isize capacity, MemoryArena* arena);
Animation* make_animaiton_from_strip(MemoryArena* arena, f32 fps, Rect2i frame, isize count);
void init_animated_sprite(Animated_Sprite* s, isize capacity, MemoryArena* arena);
void animated_sprite_update(Animated_Sprite* s, f32 timestep);
isize add_animation(Animated_Sprite* s, Animation* a);
isize play_animation(Animated_Sprite* s, isize anim_index);
void init_tilemap(Tilemap* tilemap, isize w, isize h, MemoryArena* arena);
void generate_tilemap(Tilemap* tilemap, u64 seed);
Tile tilemap_get_at(Tilemap* tilemap, isize x, isize y);
Tile tilemap_get_at(Tilemap* tilemap, Vec2 pos);
Tile_State* tilemap_get_state_at(Tilemap* tilemap, isize x, isize y);
Tile_State* tilemap_get_state_at(Tilemap* tilemap, Vec2 pos);
bool tilemap_set_at(Tilemap* tilemap, isize x, isize y, Tile value);
bool tilemap_set_at(Tilemap* tilemap, Vec2 pos, Tile value);
void update_tile_state_at(Game_Registry* Registry, Tilemap* map, isize x, isize y);
void update_tile_state_at(Game_Registry* Registry, Tilemap* map, Vec2 pos);
void render_tilemap(Game_Registry* Registry, Tilemap* tilemap, Vec2 pos);
isize render_tilemap(Game_Registry* Registry, Tilemap* tilemap, Vec2 pos, Rect2 area);
void body_sort_on_y(Sim_Body* bodies, isize count);
void init_static_grid(Sim_Static_Grid* grid, Vec2i size, isize capacity, MemoryArena* arena);
Sim_Grid_Cell* get_next_grid_cell(Sim_Static_Grid* grid);
void build_static_grid(Sim_Static_Grid* grid, Sim_Body* bodies, isize count);
Sim_Body* sim_get_next_static_body(Simulator* sim);
Sim_Body* sim_get_next_body(Simulator* sim);
void init_simulator(Simulator* sim, isize cap, MemoryArena* arena);
Sim_Body* sim_find_body(Simulator* sim, isize id);
void sim_sort_bodies_on_id(Simulator* sim);
void sim_remove_body(Simulator* sim, isize body);
void sim_remove_body(Simulator* sim, Sim_Body* body);
Sim_Body* sim_query_aabb(Simulator* sim, AABB query);
i32 _do_collide_bodies_sweep(Sim_Body* a, Sim_Body* b, i32 sort_axis);
i32 _do_collide_bodies(Sim_Body* a, Sim_Body* b, i32 sort_axis);
void _separate_bodies(Sim_Body* a, Sim_Body* b, bool capture_contacts, i32 times, Simulator* sim);
void sim_update(Game_Registry* Registry, Simulator* sim, Tilemap* map, f32 dt);
Tile_Info* _get_at(Game_Registry* Registry, isize x, isize y);
void generate_statics_for_tilemap(Game_Registry* Registry, Simulator* sim, Tilemap* tilemap, MemoryArena* tempArena);
Particle_Style copy_particle_style(Particle_Style s, Vec2 impulse_range, Vec2i time_range);
Particle_Style make_particle_style(Rect2i texture, Vec2 size, u32 color, u32 shadow_color, Vec3 acl, Vec2 impulse_range, Vec2 angle_range, Vec2 angle_vel_range, Vec2 scale_range, Vec2i frame_range, i32 max_frames, i32 framerate, Vec2i time_range, bool time_scaling, f32 ground_restitution, Vec2 skid_on_bounce_range, f32 jitter_on_bounce_mag);
void init_particle(Particle* p, Vec3 pos, Vec3 vel, f32 scale, f32 angle, f32 anglev, i32 frame, i32 time, i32 style_index);
void init_emitter(Emitter* e, isize max_particles, MemoryArena* arena);
void emitter_spawn(Emitter* e, Vec3 pos, Vec2 angle_range, isize count, Particle_Style style);
void emitter_render(Emitter* e, Simulator* sim, f32 dt);
void init_entity(Entity* entity);
Entity* world_area_get_next_entity(WorldArea* area);
Entity* world_area_find_entity(WorldArea* area, isize id);
i32 entity_id_cmp(const void* a, const void* b);
void world_area_sort_entities_on_id(WorldArea* area);
void world_area_synchronize_entities_and_bodies(WorldArea* area);
bool world_area_remove_entity(WorldArea* area, Entity* entity);
void world_area_remove_entity_internal(WorldArea* area, isize id);
void world_area_process_removed_entities(WorldArea* area);
void world_area_build_hitboxes(WorldArea* area);
void world_area_process_hitboxes(WorldArea* area);
void init_world(World* world, isize width, isize height, usize seed, MemoryArena* arena);
void world_area_init_player(WorldArea* area, Vec2i tile_pos, bool move_player);
void world_area_deinit_player(WorldArea* area, bool move_player);
void generate_world_area(Game_Registry* Registry, World* world, WorldArea* area, WorldAreaStub* stub);
void world_switch_current_area(World* world, Area_Link link, MemoryArena* arena);
void world_start_in_area(World* world, WorldAreaStub* area, MemoryArena* arena);
void generate_world(char* name, World* world);
void world_area_walk_entities(WorldArea* area, World* world);
void world_area_animate_entities(WorldArea* area, World* world);
bool world_area_frametick_entities(WorldArea* area, World* world);
void world_area_slowtick_entities(WorldArea* area, World* world);
void world_area_on_destroy_entity(Entity* e, WorldArea* area, World* world);
void world_area_on_activate_entity(Entity* e, WorldArea* area, World* world);
void world_area_hit_entities(WorldArea* area, World* world);
void world_area_contact_entities(WorldArea* area, World* world);
void world_area_interact(WorldArea* area, World* world);
void world_area_render(WorldArea* area, World* world);
void world_area_update(WorldArea* area, World* world);
void rituals_prop_drop_on_break(WorldArea* area, Entity* a);
Entity* rituals_spawn_enemy(WorldArea* area, isize enemykind, Vec2 position);
void rituals_on_activate_entity(Entity* entity, WorldArea* area, World* world);
void rituals_on_destroy_entity(Entity* e, WorldArea* area, World* world);
bool rituals_frametick_entities(Entity* entities, isize count, WorldArea* area, World* world);
void rituals_slowtick_entities(Entity* entities, isize count, WorldArea* area, World* world);
void rituals_walk_entities(Entity* entities, isize count, WorldArea* area, World* world);
void rituals_animate_entities(Entity* entities, isize count, WorldArea* area, World* world);
void rituals_interact_entities(Entity* entities, isize count, WorldArea* area, World* world);
void rituals_hit_entities(Hitbox_Contact* contacts, isize count, WorldArea* area, World* world);
void rituals_entity_on_contact_terrain(Entity* e, WorldArea* area, World* world);
void rituals_contact_entities(Sim_Contact* contacts, isize count, WorldArea* area, World* world);
void testInit();
void testUpdate();
int main(int argc, char** argv);
