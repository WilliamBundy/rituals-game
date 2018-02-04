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
typedef struct TileInfo TileInfo;
typedef struct TileState TileState;
typedef struct Game_Registry Game_Registry;
typedef struct Random Random;
typedef struct Game_Input Game_Input;
typedef struct Game_Main Game_Main;
typedef struct Platform Platform;
typedef struct Animation_Frame Animation_Frame;
typedef struct Animated_Sprite Animated_Sprite;
typedef struct Animation Animation;
typedef struct Tilemap Tilemap;
typedef struct SimShape SimShape;
typedef struct SimContact SimContact;
typedef struct SimBody SimBody;
typedef struct SimWorld SimWorld;
typedef enum SpriteFlags SpriteFlags;
typedef enum Button_State Button_State;
typedef enum Direction Direction;
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

struct TileInfo
{
	isize id;
	usize hash;
	Rect2i texture;
	Rect2i bottomTexture;
	Rect2i topTexture;
	f32 movementModifier;
	f32 friction;
	const char *name;
	i32 maxDamage;
	bool immuneToDamage;
	isize breakToId;
	bool solid;
	u64 bodyMask;
	u64 bodyGroup;
	bool hasTopTexture;
	bool hasBottomTexture;
};

struct TileState
{
	isize id;
	i32 damage;
};

struct Game_Registry
{
	TileInfo *tiles;
	isize tileCount;
	isize tileCapacity;
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
	TileState *states;
};

struct SimShape
{
	i32 flags;
	i32 index;
	Vec2 pos;
	Vec2 size;
	f32 staticFriction;
	f32 dynamicFriction;
	f32 restitution;
	SimBody *body;
};

struct SimContact
{
	Vec2 overlap;
	SimShape *a;
	SimShape *b;
};

struct SimBody
{
	i32 flags;
	i32 shapeCount;
	Vec2 pos;
	Vec2 vel;
	Vec2 acl;
	f32 invMass;
	i32 index;
	SimShape *shapes[4];
};

struct SimWorld
{
	MemoryArena *arena;
	MemoryPool *bodyPool;
	MemoryPool *shapePool;
	SimShape **shapes;
	SimBody **bodies;
	isize bodyCount;
	isize bodyCapacity;
	isize shapeCount;
	isize shapeCapacity;
	f32 testCount;
	f32 colCount;
	i32 iterations;
	i32 sortAxis;
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
void initTileState(TileState* state, isize id);
void registryInit(Game_Registry* reg, isize capacity, MemoryArena* arena);
TileInfo* add_tile_info(Game_Registry* Registry, isize index, const char* name, f32 movement_modifier, f32 friction, Rect2i texture, bool solid, isize break_to_index);
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
TileState* tilemap_get_state_at(Tilemap* tilemap, isize x, isize y);
TileState* tilemap_get_state_at(Tilemap* tilemap, Vec2 pos);
bool tilemap_set_at(Tilemap* tilemap, isize x, isize y, Tile value);
bool tilemap_set_at(Tilemap* tilemap, Vec2 pos, Tile value);
void update_tile_state_at(Game_Registry* Registry, Tilemap* map, isize x, isize y);
void update_tile_state_at(Game_Registry* Registry, Tilemap* map, Vec2 pos);
void render_tilemap(Game_Registry* Registry, Tilemap* tilemap, Vec2 pos);
isize render_tilemap(Game_Registry* Registry, Tilemap* tilemap, Vec2 pos, Rect2 area);
void simInitBody(SimBody* body);
void simInitShape(SimShape* shape);
SimBody* simGetBody(SimWorld* sim);
void simAddBody(SimWorld* sim, SimBody* body);
void simRemoveBody(SimWorld* sim, SimBody* body);
SimShape* simGetShape(SimWorld* sim);
void simAttachShape(SimBody* body, SimShape* shape);
void simDetachShape(SimShape* shape);
void simAddShape(SimWorld* sim, SimShape* shape);
void simRemoveShape(SimWorld* sim, SimShape* shape);
SimBody* simCreateBox(SimWorld* sim, Vec2 pos, Vec2 size);
SimBody* simAttachBox(SimWorld* sim, SimBody* body, Vec2 pos, Vec2 size);
void simShapeSort(SimShape** array, isize count, i32 axis);
i32 simIntersectShapes(SimWorld* sim, SimShape* a, SimShape* b, SimContact* c);
void simResolveContact(SimWorld* sim, SimContact* c);
SimWorld* simNew(isize capacity);
void simUpdate(SimWorld* sim, f32 dt);
void simDelete(SimWorld* sim);
void testInit();
void testUpdate();
int main(int argc, char** argv);
