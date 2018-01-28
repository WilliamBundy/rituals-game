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
typedef struct Memory_Arena Memory_Arena;
typedef struct Random Random;
typedef struct Game_Input Game_Input;
typedef struct Game_Main Game_Main;
typedef struct Platform Platform;
typedef struct Sprite Sprite;
typedef struct Sprite4 Sprite4;
typedef struct Render_Group Render_Group;
typedef struct OpenGL_Renderer OpenGL_Renderer;
typedef struct Animation_Frame Animation_Frame;
typedef struct Animation Animation;
typedef struct Animated_Sprite Animated_Sprite;
typedef struct Spritefont Spritefont;
typedef struct Gui_Window_Handle Gui_Window_Handle;
typedef struct Gui_Text_Input_Handle Gui_Text_Input_Handle;
typedef struct Gui_Context Gui_Context;
typedef struct None_State None_State;
typedef struct Menu_State Menu_State;
typedef struct Play_State Play_State;
typedef struct Tile_Info Tile_Info;
typedef struct Tile_State Tile_State;
typedef struct Item_Info Item_Info;
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
typedef struct Game_Registry Game_Registry;
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
typedef struct World_Area_Stub World_Area_Stub;
typedef struct World_Area World_Area;
typedef struct World World;
typedef enum Button_State Button_State;
typedef enum Game_State Game_State;
typedef enum Direction Direction;
typedef enum Sprite_Anchor Sprite_Anchor;
typedef enum Sprite_Flags Sprite_Flags;
typedef enum Item_Info_Types Item_Info_Types;
typedef enum Entity_Event_Type Entity_Event_Type;
typedef enum Rituals_Entity_Kinds Rituals_Entity_Kinds;
typedef enum Rituals_Enemy_Kind Rituals_Enemy_Kind;
typedef enum Rituals_Pickup_Kind Rituals_Pickup_Kind;
typedef enum Sim_Body_Flags Sim_Body_Flags;
typedef enum Entity_Flags Entity_Flags;
typedef enum World_Area_Biome World_Area_Biome;
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef float real32;
typedef double real64;
typedef real32 real;
typedef int32 integer;
typedef real32 real;
typedef ptrdiff_t isize;
typedef size_t usize;
typedef int32 Tile;

union Vec2
{
	struct {
		real x;
		real y;
	};
	real e[2];
};

union Vec3
{
	struct {
		real x;
		real y;
		real z;
	};
	real e[3];
};

union Vec4
{
	struct {
		real x;
		real y;
		real z;
		real w;
	};
	real e[4];
};

union Vec2i
{
	struct {
		integer x;
		integer y;
	};
	integer e[2];
};

union Vec3i
{
	struct {
		integer x;
		integer y;
		integer z;
	};
	integer e[3];
};

union Vec4i
{
	struct {
		integer x;
		integer y;
		integer z;
		integer w;
	};
	integer e[4];
};

union Rect2
{
	struct {
		real x;
		real y;
		real w;
		real h;
	};
	struct {
		Vec2 position;
		Vec2 size;
	};
	real e[4];
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
	integer x;
	integer y;
	integer w;
	integer h;
};

union AABB
{
	struct {
		Vec2 center;
		union {
			struct {
				real hw;
				real hh;
			};
			struct {
				Vec2 hext;
			};
		};
	};
	real e[4];
};

struct Memory_Arena
{
	uint8 *data;
	isize capacity;
	isize head;
	isize temp_head;
	Memory_Arena *next;
};

struct Random
{
	uint64 x;
	uint64 y;
};

struct Game_Input
{
	isize num_keys_down;
	isize num_mouse_down;
	int8 *scancodes;
	int8 *keycodes;
	int8 *mouse;
	int32 mouse_x;
	int32 mouse_y;
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
	OpenGL_Renderer *renderer;
	Game_State state;
	real scale;
	Vec2 size;
	Vec2i window_size;
	Memory_Arena *meta_arena;
	Memory_Arena *asset_arena;
	Memory_Arena *game_arena;
	Memory_Arena *temp_arena;
	Memory_Arena *renderer_arena;
	Memory_Arena *registry_arena;
	Memory_Arena *world_arena;
	Memory_Arena *play_arena;
	const char *base_path;
	isize base_path_length;
	Spritefont *body_font;
	Spritefont *title_font;
	Random r;
	Game_Registry *registry;
	Game_Input *input;
	uint64 last_frame_time;
	_game_state_list;
(null);
(null);
	Game_Main *Game;
	OpenGL_Renderer *Renderer;
	Render_Group *CurrentGroup;
	Game_Input *Input;
	Spritefont *Body_Font;
	Spritefont *Title_Font;
	Game_Registry *Registry;
(null);
	struct {
		const char *window_title;
		Vec2i window_size;
		real game_scale;
		int32 display_index;
		bool check_gl_attributes;
		const char *vertex_shader;
		const char *frag_shader;
		SDL_Window *window;
	};
(null);
	int32 platform_init Platform platform if SDL_Init SDL_INIT_EVERYTHING *Log_Error;
	Log_Error SDL_GetError;
	return;
};

struct Platform
{
	const char *window_title;
	Vec2i window_size;
	real game_scale;
	int32 display_index;
	bool check_gl_attributes;
	const char *vertex_shader;
	const char *frag_shader;
	SDL_Window *window;
};

struct Sprite
{
	Vec2 position;
	Vec2 center;
	real angle;
	Vec2 size;
	Rect2 texture;
	Vec4 color;
	uint32 flags;
	real sort_offset;
};

struct Sprite4
{
	Sprite e[4];
};

struct Render_Group
{
	GLuint texture;
	Vec2i texture_size;
	Vec2 offset;
	Rect2 clip;
	real32 ortho[16];
	real night_amount;
	real night_cutoff;
	Sprite *sprites;
	isize sprites_count;
	isize sprites_capacity;
};

struct OpenGL_Renderer
{
	GLuint shader_program;
	GLuint vbo;
	GLuint vao;
	isize u_texturesize;
	isize u_orthomat;
	isize u_night_amount;
	isize u_night_cutoff;
	Render_Group *groups;
	isize groups_count;
};

struct Animation_Frame
{
	Vec3 position;
	real angle;
	Vec4 color;
	Vec2 size;
	Rect2 texture;
	real sort_offset;
};

struct Animation
{
	isize id;
	Animation_Frame *frames;
	isize frames_count;
	isize frames_capacity;
	real fps;
	real inv_fps;
	bool looping;
};

struct Animated_Sprite
{
	Animation **animations;
	isize animations_count;
	isize animations_capacity;
	isize current_animation;
	isize current_frame;
	bool running;
	real timer;
};

struct Spritefont
{
	int32 line_padding;
	int32 character_padding;
	int32 tab_size;
	int32 glyph_width;
	int32 glyph_height;
	const Rect2 *glyphs;
	Vec4 color;
};

struct Gui_Window_Handle
{
	Vec2 position;
	char *title;
	int32 z;
};

struct Gui_Text_Input_Handle
{
	char *buffer;
	isize buffer_length;
	isize buffer_capacity;
	isize max_chars_by_width;
	isize cursor;
	isize selection_start;
	bool active;
	bool accept_newlines;
	bool accept_tabs;
};

struct Gui_Context
{
	Vec2 last_pos;
	Vec2 next_pos;
	isize axis;
};

struct None_State
{
	int32 error;
};

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

struct Play_State
{
	int32 error;
	usize current_time;
	usize prev_time;
	real accumulator;
	World *world;
	bool running;
	Vec2i world_xy;
	bool delete_world_on_stop;
	bool save_world_on_stop;
	const char *world_name_in;
};

struct Tile_Info
{
	isize id;
	usize hash;
	Rect2 texture;
	Rect2 bottom_texture;
	Rect2 top_texture;
	real movement_modifier;
	real friction;
	const char *name;
	int32 max_damage;
	bool immune_to_damage;
	char *break_to_name;
	isize break_to_id;
	bool solid;
	uint64 body_mask;
	uint64 body_group;
	bool has_top_texture;
	bool has_bottom_texture;
};

struct Tile_State
{
	isize id;
	int32 damage;
};

struct Item_Info
{
	isize id;
	usize hash;
	const char *name;
	int32 max_stack;
	Rect2 texture;
};

struct Rituals_Bat_Userdata
{
	Vec2 perch;
};

struct Rituals_Snake_Userdata
{
	real chase_speed_modifier;
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
	real speed;
	real alert_dist;
	real follow_dist;
	union {
		Rituals_Bat_Userdata bat;
		Rituals_Snake_Userdata snake;
		Rituals_Goblin_Knight_Userdata goblin_knight;
	};
};

struct Rituals_Player_Userdata
{
	isize held_entity_id;
	real heal_cooldown;
	real heal_timer;
	int32 heal_rate;
	int32 heal_to_interval;
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

struct Game_Registry
{
	Tile_Info *tiles;
	usize *tiles_hash;
	isize tiles_count;
	Item_Info *items;
	usize *items_hash;
	isize items_count;
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
	real inv_mass;
	real restitution;
	real damping;
	uint64 flags;
	uint64 group;
	uint64 mask;
	isize entity_id;
	Entity *entity;
};

struct Sim_Contact
{
	isize a_id;
	isize b_id;
	Sim_Body *static_b;
	Vec2 overlap;
	real mag;
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
	real scale;
	real angle;
	real angular_vel;
	int32 frame;
	int32 time;
	int32 total_time;
	int32 style_index;
};

struct Particle_Style
{
	int32 id;
	Rect2 texture;
	Vec2 size;
	Vec4 color;
	Vec4 shadow_color;
	Vec3 acceleration;
	real impulse_min;
	real impulse_max;
	real angle_min;
	real angle_max;
	real angle_vel_min;
	real angle_vel_max;
	real scale_min;
	real scale_max;
	int32 frame_min;
	int32 frame_max;
	int32 max_frames;
	int32 framerate;
	int32 time_min;
	int32 time_max;
	bool time_scaling;
	bool time_alpha;
	real ground_restitution;
	real skid_on_bounce_min;
	real skid_on_bounce_max;
	real jitter_on_bounce_mag;
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
	uint64 mask;
	uint64 group;
	AABB box;
};

struct Entity
{
	isize id;
	isize body_id;
	Sim_Body *body;
	Vec2 walk_impulse;
	Hitbox hitbox;
	int32 health;
	int32 attack;
	real attack_interval;
	real attack_timer;
	real knockback;
	Animated_Sprite *anim;
	Sprite sprite;
	real z;
	real shadow_scale;
	isize kind;
	int32 counter;
	int32 facing;
	Direction direction;
	uint64 events;
	uint64 flags;
	Rituals_Entity_Userdata userdata;
};

struct Area_Link
{
	Vec2i position;
	World_Area_Stub *link;
};

struct World_Area_Stub
{
	isize id;
	usize seed;
	Area_Link north;
	Area_Link south;
	Area_Link west;
	Area_Link east;
	World_Area_Biome biome;
};

struct World_Area
{
	isize id;
	World_Area_Stub *stub;
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
	int32 hitbox_sort_axis;
	Hitbox_Contact *hitbox_contacts;
	isize hitbox_contacts_count;
	isize hitbox_contacts_capacity;
	Entity *player;
};

struct World
{
	char *name;
	World_Area_Stub *area_stubs;
	isize next_area_id;
	isize areas_count;
	isize areas_capacity;
	isize areas_width;
	isize areas_height;
	World_Area *current_area;
	usize seed;
	Entity global_player_entity;
	Sim_Body global_player_body;
	usize slowtick_timer;
	usize slowtick_timer_interval;
	Particle_Style base_style;
	Emitter emitter;
};

real clamp_01(real x);
real lerp(real a, real b, real t);
void init_memory_arena(Memory_Arena* arena, usize size);
uint8* arena_push(Memory_Arena* arena, isize size);
void start_temp_arena(Memory_Arena* arena);
void end_temp_arena(Memory_Arena* arena);
void clear_arena(Memory_Arena* arena);
Memory_Arena* new_memory_arena(usize size, Memory_Arena* src);
uint64 next_random_uint64(Random* r);
void init_random(Random* r, uint64 seed);
real64 next_random_double(Random* r);
real32 next_random_float(Random* r);
real next_random(Random* r);
real rand_range(Random* r, real min, real max);
int32 rand_range_int(Random* r, real min, real max);
int32 platform_init(Platform* platform);
int32 game_init(Platform* platform);
void game_update_screen();
int32 game_start(Platform* platform);
void platform_quit();
void game_switch_state(Game_State state);
void game_allocate_states();
void game_init_states();
void game_start_state();
void game_update_state();
void game_stop_state();
real64 next_random_double();
real32 next_random_float();
real next_random();
real rand_range(real min, real max);
int32 rand_range_int(real min, real max);
void game_set_scale(real scale);
void game_calc_mouse_pos(Vec2 offset);
void input_text_append_char(char c);
bool is_valid_filename_char(char c);
char replace_filename_char(char c, char replace);
isize replace_invalid_filename_chars(char* str, isize len, char replace);
isize append_input_text(char* str, isize str_cap, isize str_len, isize insert_from_end);
void init_group(Render_Group* group, isize sprites_capacity, Memory_Arena* arena);
void init_renderer(OpenGL_Renderer* r, isize group_count, isize group_size, const char* vertex_source, const char* frag_source, Memory_Arena* arena);
Render_Group* render_group(OpenGL_Renderer* r, isize index);
void render_start(Render_Group* group);
bool render_has_clip_rect(Render_Group* group);
void render_set_clip_rect(Render_Group* group, real x, real y, real w, real h);
void render_sort(Render_Group* group, isize offset);
void render_add(Render_Group* group, Sprite* sprite);
void render_add_unclipped(Render_Group* group, Sprite* sprite);
void render_add(Render_Group* group, Sprite4* s4);
void render_calculate_ortho_matrix(real32* ortho, Vec4 screen, real nearplane, real farplane);
void render_draw(OpenGL_Renderer* r, Render_Group* group, Vec2 size, real scale);
void render_set_current_group(isize index);
void render_start();
bool render_has_clip_rect();
void render_set_clip_rect(real x, real y, real w, real h);
void render_sort(isize offset);
void render_add(Sprite* sprite);
void render_add_unclipped(Sprite* sprite);
void render_add(Sprite4* s4);
void render_draw(Vec2 size, real scale);
GLuint ogl_add_texture(uint8* data, isize w, isize h);
GLuint ogl_load_texture(char* filename, isize* w_o, isize* h_o);
Sprite create_box_primitive(Vec2 pos, Vec2 size, Vec4 color);
void render_box_primitive(Render_Group* group, Vec2 pos, Vec2 size, Vec4 color);
Sprite create_line_primitive(Vec2 start, Vec2 end, Vec4 color, int32 thickness);
void render_line_primitive(Render_Group* group, Vec2 start, Vec2 end, Vec4 color, int32 thickness);
Sprite4 create_box_outline_primitive(Vec2 center, Vec2 size, Vec4 color, int32 thickness);
void render_box_outline_primitive(Render_Group* group, Vec2 center, Vec2 size, Vec4 color, int32 thickness);
Sprite4 create_box_outline_primitive(Vec2 center, Vec2 size, Vec4 color, int32 thickness);
void render_box_outline_primitive(Render_Group* group, Vec2 center, Vec2 size, Vec4 color, int32 thickness);
void render_box_primitive(Vec2 pos, Vec2 size, Vec4 color);
void render_line_primitive(Vec2 start, Vec2 end, Vec4 color, int32 thickness);
void render_box_outline_primitive(Vec2 center, Vec2 size, Vec4 color, int32 thickness);
void render_box_outline_primitive(Vec2 center, Vec2 size, Vec4 color, int32 thickness);
void init_animation_frame(Animation_Frame* fr);
void init_animation(Animation* a, real fps, isize capacity, Memory_Arena* arena);
Animation* make_animaiton_from_strip(Memory_Arena* arena, real fps, Rect2 frame, isize count);
void init_animated_sprite(Animated_Sprite* s, isize capacity, Memory_Arena* arena);
void animated_sprite_update(Animated_Sprite* s, real timestep);
isize add_animation(Animated_Sprite* s, Animation* a);
isize play_animation(Animated_Sprite* s, isize anim_index);
void init_spritefont(Spritefont* font, const Rect2* glyphs);
Vec2 spritefont_size_text(Spritefont* font, char* text, isize len);
Vec2 spritefont_size_text(Spritefont* font, char* text);
void spritefont_render_text(Spritefont* font, char* text, isize len, Vec2 position, int32 max_width, Sprite_Anchor anchor, real scale, Vec2* region);
void spritefont_render_text_ex(Spritefont* font, char* text, isize len, Vec2 position, int32 max_width, Sprite_Anchor anchor, real scale, Vec2* region);
void spritefont_render_text(Spritefont* font, char* text, Vec2 position, Sprite_Anchor anchor, real scale);
void spritefont_render_text_background(Spritefont* font, char* text, Vec2 position, real scale, Vec4 background);
void render_body_text(char* text, Vec2 position, bool background, real scale);
void render_title_text(char* text, Vec2 position);
bool gui_query_mouse(Rect2 region, Vec2 parent);
bool gui_add_button(Vec2 position, char* text, Vec2 minimum_size);
bool gui_add_button(Vec2 position, char* text);
bool gui_add_checkbox(Vec2 position, char* text, bool* value);
void gui_add_slider(Vec2 position, Vec2 size, char* label, real min, real max, int precision, real* in_value, bool* active, bool show_bounds);
void init_text_input_handle(Gui_Text_Input_Handle* handle, char* buf, isize capacity, isize length);
void init_text_input_handle(Gui_Text_Input_Handle* handle, real width, Memory_Arena* arena);
void gui_add_text_input(Gui_Text_Input_Handle* handle, Vec2 position, Vec2 size, char* default_text);
void game_init_None_state();
void game_start_None_state();
void game_update_None_state();
void game_stop_None_state();
void game_free_None_state();
void game_init_Menu_state();
void game_start_Menu_state();
void game_update_Menu_state();
void game_stop_Menu_state();
void game_free_Menu_state();
void game_init_Play_state();
void game_start_Play_state();
void game_update_Play_state();
void game_stop_Play_state();
void game_free_Play_state();
void init_tile_state(Tile_State* state, isize id);
usize hash_str(const char* str);
void init_game_registry(Game_Registry* registry, Memory_Arena* arena);
void finalize_game_registry();
Tile_Info* add_tile_info(const char* name, real movement_modifier, real friction, Rect2 texture, bool solid, char* break_to_name);
void register_all_rituals_tile_info();
Item_Info* add_item_type(const char* name, int32 max_stack, Rect2 texture);
void register_all_rituals_item_info();
void register_everything_in_rituals();
void init_tilemap(Tilemap* tilemap, isize w, isize h, Memory_Arena* arena);
void generate_tilemap(Tilemap* tilemap, uint64 seed);
Tile tilemap_get_at(Tilemap* tilemap, isize x, isize y);
Tile tilemap_get_at(Tilemap* tilemap, Vec2 pos);
Tile_State* tilemap_get_state_at(Tilemap* tilemap, isize x, isize y);
Tile_State* tilemap_get_state_at(Tilemap* tilemap, Vec2 pos);
bool tilemap_set_at(Tilemap* tilemap, isize x, isize y, Tile value);
bool tilemap_set_at(Tilemap* tilemap, Vec2 pos, Tile value);
void update_tile_state_at(Tilemap* map, isize x, isize y);
void update_tile_state_at(Tilemap* map, Vec2 pos);
void render_tilemap(Tilemap* tilemap, Vec2 pos);
isize render_tilemap(Tilemap* tilemap, Vec2 pos, Rect2 area);
void body_sort_on_x(Sim_Body* bodies, isize count);
void body_sort_on_y(Sim_Body* bodies, isize count);
void init_static_grid(Sim_Static_Grid* grid, Vec2i size, isize capacity, Memory_Arena* arena);
Sim_Grid_Cell* get_next_grid_cell(Sim_Static_Grid* grid);
void build_static_grid(Sim_Static_Grid* grid, Sim_Body* bodies, isize count);
Sim_Body* sim_get_next_static_body(Simulator* sim);
Sim_Body* sim_get_next_body(Simulator* sim);
void init_simulator(Simulator* sim, isize cap, Memory_Arena* arena);
Sim_Body* sim_find_body(Simulator* sim, isize id);
void sim_sort_bodies_on_id(Simulator* sim);
void sim_remove_body(Simulator* sim, isize body);
void sim_remove_body(Simulator* sim, Sim_Body* body);
Sim_Body* sim_query_aabb(Simulator* sim, AABB query);
int32 _do_collide_bodies_sweep(Sim_Body* a, Sim_Body* b, int32 sort_axis);
int32 _do_collide_bodies(Sim_Body* a, Sim_Body* b, int32 sort_axis);
void _separate_bodies(Sim_Body* a, Sim_Body* b, bool capture_contacts, int32 times, Simulator* sim);
void sim_update(Simulator* sim, Tilemap* map, real dt, bool capture_contacts);
Tile_Info* _get_at(isize x, isize y);
void generate_statics_for_tilemap(Simulator* sim, Tilemap* tilemap);
Particle_Style copy_particle_style(Particle_Style s, Vec2 impulse_range, Vec2i time_range);
Particle_Style make_particle_style(Rect2 texture, Vec2 size, Vec4 color, Vec4 shadow_color, Vec3 acl, Vec2 impulse_range, Vec2 angle_range, Vec2 angle_vel_range, Vec2 scale_range, Vec2i frame_range, int32 max_frames, int32 framerate, Vec2i time_range, bool time_scaling, real ground_restitution, Vec2 skid_on_bounce_range, real jitter_on_bounce_mag);
void init_particle(Particle* p, Vec3 pos, Vec3 vel, real scale, real angle, real anglev, int32 frame, int32 time, int32 style_index);
void init_emitter(Emitter* e, isize max_particles, Memory_Arena* arena);
void emitter_spawn(Emitter* e, Vec3 pos, Vec2 angle_range, isize count, Particle_Style style);
void emitter_render(Emitter* e, Simulator* sim, real dt);
void init_world_area(World_Area* area, Memory_Arena* arena);
void init_entity(Entity* entity);
Entity* world_area_get_next_entity(World_Area* area);
Entity* world_area_find_entity(World_Area* area, isize id);
int32 entity_id_cmp(const void* a, const void* b);
void world_area_sort_entities_on_id(World_Area* area);
void world_area_synchronize_entities_and_bodies(World_Area* area);
bool world_area_remove_entity(World_Area* area, Entity* entity);
void world_area_remove_entity_internal(World_Area* area, isize id);
void world_area_process_removed_entities(World_Area* area);
void world_area_build_hitboxes(World_Area* area);
void world_area_process_hitboxes(World_Area* area);
void init_world(World* world, isize width, isize height, usize seed, Memory_Arena* arena);
void world_area_init_player(World_Area* area, Vec2i tile_pos, bool move_player);
void world_area_deinit_player(World_Area* area, bool move_player);
World_Area* world_load_area(World* world, isize id, Memory_Arena* arena);
void generate_world_area(World* world, World_Area* area, World_Area_Stub* stub);
void world_switch_current_area(World* world, Area_Link link, Memory_Arena* arena);
void world_start_in_area(World* world, World_Area_Stub* area, Memory_Arena* arena);
void world_delete_self(World* world);
void generate_world(char* name, World* world);
void world_area_walk_entities(World_Area* area, World* world);
void world_area_animate_entities(World_Area* area, World* world);
bool world_area_frametick_entities(World_Area* area, World* world);
void world_area_slowtick_entities(World_Area* area, World* world);
void world_area_on_destroy_entity(Entity* e, World_Area* area, World* world);
void world_area_on_activate_entity(Entity* e, World_Area* area, World* world);
void world_area_hit_entities(World_Area* area, World* world);
void world_area_contact_entities(World_Area* area, World* world);
void world_area_interact(World_Area* area, World* world);
void world_area_render(World_Area* area, World* world);
void world_area_update(World_Area* area, World* world);
void rituals_prop_drop_on_break(World_Area* area, Entity* a);
Entity* rituals_spawn_enemy(World_Area* area, isize enemykind, Vec2 position);
void rituals_on_activate_entity(Entity* entity, World_Area* area, World* world);
void rituals_on_destroy_entity(Entity* e, World_Area* area, World* world);
bool rituals_frametick_entities(Entity* entities, isize count, World_Area* area, World* world);
void rituals_slowtick_entities(Entity* entities, isize count, World_Area* area, World* world);
void rituals_walk_entities(Entity* entities, isize count, World_Area* area, World* world);
void rituals_animate_entities(Entity* entities, isize count, World_Area* area, World* world);
void rituals_interact_entities(Entity* entities, isize count, World_Area* area, World* world);
void rituals_hit_entities(Hitbox_Contact* contacts, isize count, World_Area* area, World* world);
void rituals_entity_on_contact_terrain(Entity* e, World_Area* area, World* world);
void rituals_contact_entities(Sim_Contact* contacts, isize count, World_Area* area, World* world);
int delete_file(char* path, isize path_length, char* file);
int delete_folder(char* path, isize path_length);
int _recursive_delete(const char* last_path, isize last_path_len, char* path);
int recursively_delete_folder(char* path, bool append_base_path);
int check_path(char* path);
void check_dir(char* dir);
void serialize_tile_state(Tile_State* state, FILE* file);
void deserialize_tile_state(Tile_State* state, FILE* file);
void deserialize_tilemap(Tilemap* map, FILE* file, Memory_Arena* arena);
void serialize_tilemap(Tilemap* map, FILE* file);
void serialize_sim_body(Sim_Body* body, FILE* file);
void deserialize_sim_body(Sim_Body* body, FILE* file);
void serialize_simulator(Simulator* sim, FILE* file);
void deserialize_simulator(Simulator* sim, FILE* file, Memory_Arena* arena);
void serialize_sprite(Sprite* s, FILE* file);
void deserialize_sprite(Sprite* s, FILE* file);
void deserialize_rituals_entity_userdata(Entity* e, FILE* file);
void deserialize_hitbox(Hitbox* hb, FILE* file);
void deserialize_entity(Entity* entity, FILE* file);
void deserialize_area(World_Area* area, FILE* area_file, Memory_Arena* arena);
void serialize_rituals_entity_userdata(Entity* e, FILE* file);
void serialize_hitbox(Hitbox* hb, FILE* file);
void serialize_entity(Entity* entity, FILE* file);
void serialize_area(World_Area* area, FILE* area_file);
void serialize_area_link(Area_Link* link, FILE* fp);
void deserialize_area_link(Area_Link* link, World* world, FILE* fp);
void serialize_world_area_stub(World_Area_Stub* stub, FILE* fp);
void deserialize_world_area_stub(World_Area_Stub* stub, World* world, FILE* fp);
FILE* get_world_file(const char* name, const char* mode);
FILE* get_area_file(const char* name, isize id, const char* mode);
void serialize_world(World* world);
void deserialize_world(World* world, FILE* world_file);
void load_assets();
int main(int argc, char** argv);
