enum Meta_Type
{
	Meta_Type_Vec2,
	Meta_Type_Vec2_struct0,
	Meta_Type_Vec2i,
	Meta_Type_Vec2i_struct0,
	Meta_Type_Vec3,
	Meta_Type_Vec3_struct0,
	Meta_Type_Vec4,
	Meta_Type_Vec4_struct0,
	Meta_Type_AABB,
	Meta_Type_AABB_struct0,
	Meta_Type_AABB_struct0_union0,
	Meta_Type_AABB_struct0_union0_struct0,
	Meta_Type_AABB_struct0_union0_struct1,
	Meta_Type_Rect2,
	Meta_Type_Rect2_struct0,
	Meta_Type_Rect2_struct1,
	Meta_Type_Rect2_Clip_Info,
	Meta_Type_Rect2i,
	Meta_Type_Memory_Arena,
	Meta_Type_Random,
	Meta_Type_Game_Input,
	Meta_Type_Game_Main,
	Meta_Type_Sprite,
	Meta_Type_Sprite4,
	Meta_Type_Render_Group,
	Meta_Type_OpenGL_Renderer,
	Meta_Type_Animation_Frame,
	Meta_Type_Animation,
	Meta_Type_Animated_Sprite,
	Meta_Type_Spritefont,
	Meta_Type_Gui_Window_Handle,
	Meta_Type_Gui_Text_Input_Handle,
	Meta_Type_Gui_Context,
	Meta_Type_Play_State,
	Meta_Type_Menu_State,
	Meta_Type_Tile_Info,
	Meta_Type_Tile_State,
	Meta_Type_Item_Info,
	Meta_Type_Rituals_Entity_Userdata,
	Meta_Type_Rituals_Entity_Userdata_prop,
	Meta_Type_Rituals_Entity_Userdata_player,
	Meta_Type_Rituals_Entity_Userdata_enemy,
	Meta_Type_Rituals_Entity_Userdata_enemy_union0,
	Meta_Type_Rituals_Entity_Userdata_enemy_union0_bat,
	Meta_Type_Rituals_Entity_Userdata_enemy_union0_snake,
	Meta_Type_Rituals_Entity_Userdata_enemy_union0_goblin_knight,
	Meta_Type_Rituals_Entity_Userdata_pickup,
	Meta_Type_Rituals_Entity_Userdata_pickup_union0,
	Meta_Type_Rituals_Entity_Userdata_pickup_union0_item,
	Meta_Type_Rituals_Entity_Userdata_pickup_union0_health,
	Meta_Type_Rituals_Entity_Userdata_unknown,
	Meta_Type_MyUnion,
	Meta_Type_MyUnion_struct0,
	Meta_Type_MyUnion_struct1,
	Meta_Type_MyUnion_struct2,
	Meta_Type_Game_Registry,
	Meta_Type__game_registry__Tile_Info_pair,
	Meta_Type__game_registry__Item_Info_pair,
	Meta_Type_Item_Stack,
	Meta_Type_Inventory,
	Meta_Type_Tilemap,
	Meta_Type_Sim_Body,
	Meta_Type_Sim_Contact,
	Meta_Type_Sim_Grid_Cell,
	Meta_Type_Sim_Static_Grid,
	Meta_Type_Simulator,
	Meta_Type_Particle,
	Meta_Type_Particle_Style,
	Meta_Type_Emitter,
	Meta_Type_Hitbox_Contact,
	Meta_Type_Hitbox,
	Meta_Type_Entity,
	Meta_Type_Area_Link,
	Meta_Type_World_Area_Stub,
	Meta_Type_World_Area,
	Meta_Type_World,
	Meta_Type_real,
	Meta_Type_int32,
	Meta_Type_uint8,
	Meta_Type_isize,
	Meta_Type_uint64,
	Meta_Type_int8,
	Meta_Type_bool,
	Meta_Type_char,
	Meta_Type_SDL_Window,
	Meta_Type_Game_State,
	Meta_Type_uint32,
	Meta_Type_GLuint,
	Meta_Type_real32,
	Meta_Type_usize,
	Meta_Type_tinydir_dir,
	Meta_Type_Rituals_Entity_Kinds,
	Meta_Type_void,
	Meta_Type_Tile,
	Meta_Type_Direction,
	Meta_Type_World_Area_Biome,
};
const char* Meta_Type_Names[] = {
	"Vec2",
	"Vec2_struct0",
	"Vec2i",
	"Vec2i_struct0",
	"Vec3",
	"Vec3_struct0",
	"Vec4",
	"Vec4_struct0",
	"AABB",
	"AABB_struct0",
	"AABB_struct0_union0",
	"AABB_struct0_union0_struct0",
	"AABB_struct0_union0_struct1",
	"Rect2",
	"Rect2_struct0",
	"Rect2_struct1",
	"Rect2_Clip_Info",
	"Rect2i",
	"Memory_Arena",
	"Random",
	"Game_Input",
	"Game_Main",
	"Sprite",
	"Sprite4",
	"Render_Group",
	"OpenGL_Renderer",
	"Animation_Frame",
	"Animation",
	"Animated_Sprite",
	"Spritefont",
	"Gui_Window_Handle",
	"Gui_Text_Input_Handle",
	"Gui_Context",
	"Play_State",
	"Menu_State",
	"Tile_Info",
	"Tile_State",
	"Item_Info",
	"Rituals_Entity_Userdata",
	"Rituals_Entity_Userdata_prop",
	"Rituals_Entity_Userdata_player",
	"Rituals_Entity_Userdata_enemy",
	"Rituals_Entity_Userdata_enemy_union0",
	"Rituals_Entity_Userdata_enemy_union0_bat",
	"Rituals_Entity_Userdata_enemy_union0_snake",
	"Rituals_Entity_Userdata_enemy_union0_goblin_knight",
	"Rituals_Entity_Userdata_pickup",
	"Rituals_Entity_Userdata_pickup_union0",
	"Rituals_Entity_Userdata_pickup_union0_item",
	"Rituals_Entity_Userdata_pickup_union0_health",
	"Rituals_Entity_Userdata_unknown",
	"MyUnion",
	"MyUnion_struct0",
	"MyUnion_struct1",
	"MyUnion_struct2",
	"Game_Registry",
	"_game_registry__Tile_Info_pair",
	"_game_registry__Item_Info_pair",
	"Item_Stack",
	"Inventory",
	"Tilemap",
	"Sim_Body",
	"Sim_Contact",
	"Sim_Grid_Cell",
	"Sim_Static_Grid",
	"Simulator",
	"Particle",
	"Particle_Style",
	"Emitter",
	"Hitbox_Contact",
	"Hitbox",
	"Entity",
	"Area_Link",
	"World_Area_Stub",
	"World_Area",
	"World",
	"real",
	"int32",
	"uint8",
	"isize",
	"uint64",
	"int8",
	"bool",
	"char",
	"SDL_Window",
	"Game_State",
	"uint32",
	"GLuint",
	"real32",
	"usize",
	"tinydir_dir",
	"Rituals_Entity_Kinds",
	"void",
	"Tile",
	"Direction",
	"World_Area_Biome",
};

struct Meta_Member
{
	uint64 flags;
	Meta_Type type;
	int32 pointer_depth;
	char* name;
	uint64 offset;
};

struct Meta_Struct_Info
{
	char* name;
	char* meta_name;
	isize index;
	const Meta_Member* members;
	isize count;
};

const Meta_Member Meta_Type_Vec2_Members[] = {
	Meta_Member{0, Meta_Type_Meta_Type_Vec2_struct0, 0, "", (uint64)&((Vec2*)NULL)->},
	Meta_Member{16, Meta_Type_real, 1, "e", (uint64)&((Vec2*)NULL)->e},
};

const Meta_Member Meta_Type_Vec2_struct0_Members[] = {
	Meta_Member{0, Meta_Type_real, 0, "x", (uint64)&((*)NULL)->x},
	Meta_Member{0, Meta_Type_real, 0, "y", (uint64)&((*)NULL)->y},
};

const Meta_Member Meta_Type_Vec2i_Members[] = {
	Meta_Member{0, Meta_Type_Meta_Type_Vec2i_struct0, 0, "", (uint64)&((Vec2i*)NULL)->},
	Meta_Member{16, Meta_Type_int32, 1, "e", (uint64)&((Vec2i*)NULL)->e},
};

const Meta_Member Meta_Type_Vec2i_struct0_Members[] = {
	Meta_Member{0, Meta_Type_int32, 0, "x", (uint64)&((*)NULL)->x},
	Meta_Member{0, Meta_Type_int32, 0, "y", (uint64)&((*)NULL)->y},
};

const Meta_Member Meta_Type_Vec3_Members[] = {
	Meta_Member{0, Meta_Type_Meta_Type_Vec3_struct0, 0, "", (uint64)&((Vec3*)NULL)->},
	Meta_Member{16, Meta_Type_real, 1, "e", (uint64)&((Vec3*)NULL)->e},
};

const Meta_Member Meta_Type_Vec3_struct0_Members[] = {
	Meta_Member{0, Meta_Type_real, 0, "x", (uint64)&((*)NULL)->x},
	Meta_Member{0, Meta_Type_real, 0, "y", (uint64)&((*)NULL)->y},
	Meta_Member{0, Meta_Type_real, 0, "z", (uint64)&((*)NULL)->z},
};

const Meta_Member Meta_Type_Vec4_Members[] = {
	Meta_Member{0, Meta_Type_Meta_Type_Vec4_struct0, 0, "", (uint64)&((Vec4*)NULL)->},
	Meta_Member{16, Meta_Type_real, 1, "e", (uint64)&((Vec4*)NULL)->e},
};

const Meta_Member Meta_Type_Vec4_struct0_Members[] = {
	Meta_Member{0, Meta_Type_real, 0, "x", (uint64)&((*)NULL)->x},
	Meta_Member{0, Meta_Type_real, 0, "y", (uint64)&((*)NULL)->y},
	Meta_Member{0, Meta_Type_real, 0, "z", (uint64)&((*)NULL)->z},
	Meta_Member{0, Meta_Type_real, 0, "w", (uint64)&((*)NULL)->w},
};

const Meta_Member Meta_Type_AABB_Members[] = {
	Meta_Member{0, Meta_Type_Meta_Type_AABB_struct0, 0, "", (uint64)&((AABB*)NULL)->},
	Meta_Member{16, Meta_Type_real, 1, "e", (uint64)&((AABB*)NULL)->e},
};

const Meta_Member Meta_Type_AABB_struct0_Members[] = {
	Meta_Member{0, Meta_Type_Vec2, 0, "center", (uint64)&((*)NULL)->center},
	Meta_Member{0, Meta_Type_Meta_Type_AABB_struct0_union0, 0, "", (uint64)&((*)NULL)->},
};

const Meta_Member Meta_Type_AABB_struct0_union0_Members[] = {
	Meta_Member{0, Meta_Type_Meta_Type_AABB_struct0_union0_struct0, 0, "", (uint64)&((*)NULL)->},
	Meta_Member{0, Meta_Type_Meta_Type_AABB_struct0_union0_struct1, 0, "", (uint64)&((*)NULL)->},
};

const Meta_Member Meta_Type_AABB_struct0_union0_struct0_Members[] = {
	Meta_Member{0, Meta_Type_real, 0, "hw", (uint64)&((*)NULL)->hw},
	Meta_Member{0, Meta_Type_real, 0, "hh", (uint64)&((*)NULL)->hh},
};

const Meta_Member Meta_Type_AABB_struct0_union0_struct1_Members[] = {
	Meta_Member{0, Meta_Type_Vec2, 0, "hext", (uint64)&((*)NULL)->hext},
};

const Meta_Member Meta_Type_Rect2_Members[] = {
	Meta_Member{0, Meta_Type_Meta_Type_Rect2_struct0, 0, "", (uint64)&((Rect2*)NULL)->},
	Meta_Member{0, Meta_Type_Meta_Type_Rect2_struct1, 0, "", (uint64)&((Rect2*)NULL)->},
	Meta_Member{16, Meta_Type_real, 1, "e", (uint64)&((Rect2*)NULL)->e},
};

const Meta_Member Meta_Type_Rect2_struct0_Members[] = {
	Meta_Member{0, Meta_Type_real, 0, "x", (uint64)&((*)NULL)->x},
	Meta_Member{0, Meta_Type_real, 0, "y", (uint64)&((*)NULL)->y},
	Meta_Member{0, Meta_Type_real, 0, "w", (uint64)&((*)NULL)->w},
	Meta_Member{0, Meta_Type_real, 0, "h", (uint64)&((*)NULL)->h},
};

const Meta_Member Meta_Type_Rect2_struct1_Members[] = {
	Meta_Member{0, Meta_Type_Vec2, 0, "position", (uint64)&((*)NULL)->position},
	Meta_Member{0, Meta_Type_Vec2, 0, "size", (uint64)&((*)NULL)->size},
};

const Meta_Member Meta_Type_Rect2_Clip_Info_Members[] = {
	Meta_Member{0, Meta_Type_Vec2, 0, "rp1", (uint64)&((Rect2_Clip_Info*)NULL)->rp1},
	Meta_Member{0, Meta_Type_Vec2, 0, "rp2", (uint64)&((Rect2_Clip_Info*)NULL)->rp2},
	Meta_Member{0, Meta_Type_Vec2, 0, "diff1", (uint64)&((Rect2_Clip_Info*)NULL)->diff1},
	Meta_Member{0, Meta_Type_Vec2, 0, "diff2", (uint64)&((Rect2_Clip_Info*)NULL)->diff2},
};

const Meta_Member Meta_Type_Rect2i_Members[] = {
	Meta_Member{0, Meta_Type_int32, 0, "x", (uint64)&((Rect2i*)NULL)->x},
	Meta_Member{0, Meta_Type_int32, 0, "y", (uint64)&((Rect2i*)NULL)->y},
	Meta_Member{0, Meta_Type_int32, 0, "w", (uint64)&((Rect2i*)NULL)->w},
	Meta_Member{0, Meta_Type_int32, 0, "h", (uint64)&((Rect2i*)NULL)->h},
};

const Meta_Member Meta_Type_Memory_Arena_Members[] = {
	Meta_Member{1, Meta_Type_uint8, 1, "data", (uint64)&((Memory_Arena*)NULL)->data},
	Meta_Member{0, Meta_Type_isize, 0, "capacity", (uint64)&((Memory_Arena*)NULL)->capacity},
	Meta_Member{0, Meta_Type_isize, 0, "head", (uint64)&((Memory_Arena*)NULL)->head},
	Meta_Member{0, Meta_Type_isize, 0, "temp_head", (uint64)&((Memory_Arena*)NULL)->temp_head},
	Meta_Member{1, Meta_Type_Memory_Arena, 1, "next", (uint64)&((Memory_Arena*)NULL)->next},
};

const Meta_Member Meta_Type_Random_Members[] = {
	Meta_Member{0, Meta_Type_uint64, 0, "x", (uint64)&((Random*)NULL)->x},
	Meta_Member{0, Meta_Type_uint64, 0, "y", (uint64)&((Random*)NULL)->y},
};

const Meta_Member Meta_Type_Game_Input_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "num_keys_down", (uint64)&((Game_Input*)NULL)->num_keys_down},
	Meta_Member{0, Meta_Type_isize, 0, "num_mouse_down", (uint64)&((Game_Input*)NULL)->num_mouse_down},
	Meta_Member{1, Meta_Type_int8, 1, "scancodes", (uint64)&((Game_Input*)NULL)->scancodes},
	Meta_Member{1, Meta_Type_int8, 1, "keycodes", (uint64)&((Game_Input*)NULL)->keycodes},
	Meta_Member{1, Meta_Type_int8, 1, "mouse", (uint64)&((Game_Input*)NULL)->mouse},
	Meta_Member{0, Meta_Type_int32, 0, "mouse_x", (uint64)&((Game_Input*)NULL)->mouse_x},
	Meta_Member{0, Meta_Type_int32, 0, "mouse_y", (uint64)&((Game_Input*)NULL)->mouse_y},
	Meta_Member{0, Meta_Type_Vec2i, 0, "screen_mouse_pos", (uint64)&((Game_Input*)NULL)->screen_mouse_pos},
	Meta_Member{0, Meta_Type_Vec2, 0, "mouse_pos", (uint64)&((Game_Input*)NULL)->mouse_pos},
	Meta_Member{0, Meta_Type_bool, 0, "capture_tabs", (uint64)&((Game_Input*)NULL)->capture_tabs},
	Meta_Member{0, Meta_Type_bool, 0, "capture_newlines", (uint64)&((Game_Input*)NULL)->capture_newlines},
	Meta_Member{1, Meta_Type_char, 1, "text", (uint64)&((Game_Input*)NULL)->text},
	Meta_Member{0, Meta_Type_isize, 0, "text_count", (uint64)&((Game_Input*)NULL)->text_count},
};

const Meta_Member Meta_Type_Game_Main_Members[] = {
	Meta_Member{1, Meta_Type_SDL_Window, 1, "window", (uint64)&((Game_Main*)NULL)->window},
	Meta_Member{1, Meta_Type_OpenGL_Renderer, 1, "renderer", (uint64)&((Game_Main*)NULL)->renderer},
	Meta_Member{0, Meta_Type_Game_State, 0, "state", (uint64)&((Game_Main*)NULL)->state},
	Meta_Member{0, Meta_Type_real, 0, "scale", (uint64)&((Game_Main*)NULL)->scale},
	Meta_Member{0, Meta_Type_Vec2, 0, "size", (uint64)&((Game_Main*)NULL)->size},
	Meta_Member{0, Meta_Type_Vec2i, 0, "window_size", (uint64)&((Game_Main*)NULL)->window_size},
	Meta_Member{1, Meta_Type_Memory_Arena, 1, "meta_arena", (uint64)&((Game_Main*)NULL)->meta_arena},
	Meta_Member{1, Meta_Type_Memory_Arena, 1, "asset_arena", (uint64)&((Game_Main*)NULL)->asset_arena},
	Meta_Member{1, Meta_Type_Memory_Arena, 1, "game_arena", (uint64)&((Game_Main*)NULL)->game_arena},
	Meta_Member{1, Meta_Type_Memory_Arena, 1, "temp_arena", (uint64)&((Game_Main*)NULL)->temp_arena},
	Meta_Member{1, Meta_Type_Memory_Arena, 1, "renderer_arena", (uint64)&((Game_Main*)NULL)->renderer_arena},
	Meta_Member{1, Meta_Type_Memory_Arena, 1, "registry_arena", (uint64)&((Game_Main*)NULL)->registry_arena},
	Meta_Member{1, Meta_Type_Memory_Arena, 1, "world_arena", (uint64)&((Game_Main*)NULL)->world_arena},
	Meta_Member{1, Meta_Type_Memory_Arena, 1, "play_arena", (uint64)&((Game_Main*)NULL)->play_arena},
	Meta_Member{9, Meta_Type_char, 1, "base_path", (uint64)&((Game_Main*)NULL)->base_path},
	Meta_Member{0, Meta_Type_isize, 0, "base_path_length", (uint64)&((Game_Main*)NULL)->base_path_length},
	Meta_Member{1, Meta_Type_Spritefont, 1, "body_font", (uint64)&((Game_Main*)NULL)->body_font},
	Meta_Member{1, Meta_Type_Spritefont, 1, "title_font", (uint64)&((Game_Main*)NULL)->title_font},
	Meta_Member{0, Meta_Type_Random, 0, "r", (uint64)&((Game_Main*)NULL)->r},
	Meta_Member{1, Meta_Type_Game_Registry, 1, "registry", (uint64)&((Game_Main*)NULL)->registry},
	Meta_Member{1, Meta_Type_Game_Input, 1, "input", (uint64)&((Game_Main*)NULL)->input},
	Meta_Member{0, Meta_Type_uint64, 0, "last_frame_time", (uint64)&((Game_Main*)NULL)->last_frame_time},
};

const Meta_Member Meta_Type_Sprite_Members[] = {
	Meta_Member{0, Meta_Type_Vec2, 0, "position", (uint64)&((Sprite*)NULL)->position},
	Meta_Member{0, Meta_Type_Vec2, 0, "center", (uint64)&((Sprite*)NULL)->center},
	Meta_Member{0, Meta_Type_real, 0, "angle", (uint64)&((Sprite*)NULL)->angle},
	Meta_Member{0, Meta_Type_Vec2, 0, "size", (uint64)&((Sprite*)NULL)->size},
	Meta_Member{0, Meta_Type_Rect2, 0, "texture", (uint64)&((Sprite*)NULL)->texture},
	Meta_Member{0, Meta_Type_Vec4, 0, "color", (uint64)&((Sprite*)NULL)->color},
	Meta_Member{0, Meta_Type_uint32, 0, "flags", (uint64)&((Sprite*)NULL)->flags},
	Meta_Member{0, Meta_Type_real, 0, "sort_offset", (uint64)&((Sprite*)NULL)->sort_offset},
};

const Meta_Member Meta_Type_Sprite4_Members[] = {
	Meta_Member{16, Meta_Type_Sprite, 1, "e", (uint64)&((Sprite4*)NULL)->e},
};

const Meta_Member Meta_Type_Render_Group_Members[] = {
	Meta_Member{0, Meta_Type_GLuint, 0, "texture", (uint64)&((Render_Group*)NULL)->texture},
	Meta_Member{0, Meta_Type_Vec2i, 0, "texture_size", (uint64)&((Render_Group*)NULL)->texture_size},
	Meta_Member{0, Meta_Type_Vec2, 0, "offset", (uint64)&((Render_Group*)NULL)->offset},
	Meta_Member{0, Meta_Type_Rect2, 0, "clip", (uint64)&((Render_Group*)NULL)->clip},
	Meta_Member{16, Meta_Type_real32, 1, "ortho", (uint64)&((Render_Group*)NULL)->ortho},
	Meta_Member{0, Meta_Type_real, 0, "night_amount", (uint64)&((Render_Group*)NULL)->night_amount},
	Meta_Member{0, Meta_Type_real, 0, "night_cutoff", (uint64)&((Render_Group*)NULL)->night_cutoff},
	Meta_Member{1, Meta_Type_Sprite, 1, "sprites", (uint64)&((Render_Group*)NULL)->sprites},
	Meta_Member{0, Meta_Type_isize, 0, "sprites_count", (uint64)&((Render_Group*)NULL)->sprites_count},
	Meta_Member{0, Meta_Type_isize, 0, "sprites_capacity", (uint64)&((Render_Group*)NULL)->sprites_capacity},
};

const Meta_Member Meta_Type_OpenGL_Renderer_Members[] = {
	Meta_Member{0, Meta_Type_GLuint, 0, "shader_program", (uint64)&((OpenGL_Renderer*)NULL)->shader_program},
	Meta_Member{0, Meta_Type_GLuint, 0, "vbo", (uint64)&((OpenGL_Renderer*)NULL)->vbo},
	Meta_Member{0, Meta_Type_GLuint, 0, "vao", (uint64)&((OpenGL_Renderer*)NULL)->vao},
	Meta_Member{0, Meta_Type_isize, 0, "u_texturesize", (uint64)&((OpenGL_Renderer*)NULL)->u_texturesize},
	Meta_Member{0, Meta_Type_isize, 0, "u_orthomat", (uint64)&((OpenGL_Renderer*)NULL)->u_orthomat},
	Meta_Member{0, Meta_Type_isize, 0, "u_night_amount", (uint64)&((OpenGL_Renderer*)NULL)->u_night_amount},
	Meta_Member{0, Meta_Type_isize, 0, "u_night_cutoff", (uint64)&((OpenGL_Renderer*)NULL)->u_night_cutoff},
	Meta_Member{1, Meta_Type_Render_Group, 1, "groups", (uint64)&((OpenGL_Renderer*)NULL)->groups},
	Meta_Member{0, Meta_Type_isize, 0, "groups_count", (uint64)&((OpenGL_Renderer*)NULL)->groups_count},
};

const Meta_Member Meta_Type_Animation_Frame_Members[] = {
	Meta_Member{0, Meta_Type_Vec3, 0, "position", (uint64)&((Animation_Frame*)NULL)->position},
	Meta_Member{0, Meta_Type_real, 0, "angle", (uint64)&((Animation_Frame*)NULL)->angle},
	Meta_Member{0, Meta_Type_Vec4, 0, "color", (uint64)&((Animation_Frame*)NULL)->color},
	Meta_Member{0, Meta_Type_Vec2, 0, "size", (uint64)&((Animation_Frame*)NULL)->size},
	Meta_Member{0, Meta_Type_Rect2, 0, "texture", (uint64)&((Animation_Frame*)NULL)->texture},
	Meta_Member{0, Meta_Type_real, 0, "sort_offset", (uint64)&((Animation_Frame*)NULL)->sort_offset},
};

const Meta_Member Meta_Type_Animation_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "id", (uint64)&((Animation*)NULL)->id},
	Meta_Member{1, Meta_Type_Animation_Frame, 1, "frames", (uint64)&((Animation*)NULL)->frames},
	Meta_Member{0, Meta_Type_isize, 0, "frames_count", (uint64)&((Animation*)NULL)->frames_count},
	Meta_Member{0, Meta_Type_isize, 0, "frames_capacity", (uint64)&((Animation*)NULL)->frames_capacity},
	Meta_Member{0, Meta_Type_real, 0, "fps", (uint64)&((Animation*)NULL)->fps},
	Meta_Member{0, Meta_Type_real, 0, "inv_fps", (uint64)&((Animation*)NULL)->inv_fps},
	Meta_Member{0, Meta_Type_bool, 0, "looping", (uint64)&((Animation*)NULL)->looping},
};

const Meta_Member Meta_Type_Animated_Sprite_Members[] = {
	Meta_Member{1, Meta_Type_Animation, 2, "animations", (uint64)&((Animated_Sprite*)NULL)->animations},
	Meta_Member{0, Meta_Type_isize, 0, "animations_count", (uint64)&((Animated_Sprite*)NULL)->animations_count},
	Meta_Member{0, Meta_Type_isize, 0, "animations_capacity", (uint64)&((Animated_Sprite*)NULL)->animations_capacity},
	Meta_Member{0, Meta_Type_isize, 0, "current_animation", (uint64)&((Animated_Sprite*)NULL)->current_animation},
	Meta_Member{0, Meta_Type_isize, 0, "current_frame", (uint64)&((Animated_Sprite*)NULL)->current_frame},
	Meta_Member{0, Meta_Type_bool, 0, "running", (uint64)&((Animated_Sprite*)NULL)->running},
	Meta_Member{0, Meta_Type_real, 0, "timer", (uint64)&((Animated_Sprite*)NULL)->timer},
};

const Meta_Member Meta_Type_Spritefont_Members[] = {
	Meta_Member{0, Meta_Type_int32, 0, "line_padding", (uint64)&((Spritefont*)NULL)->line_padding},
	Meta_Member{0, Meta_Type_int32, 0, "character_padding", (uint64)&((Spritefont*)NULL)->character_padding},
	Meta_Member{0, Meta_Type_int32, 0, "tab_size", (uint64)&((Spritefont*)NULL)->tab_size},
	Meta_Member{0, Meta_Type_int32, 0, "glyph_width", (uint64)&((Spritefont*)NULL)->glyph_width},
	Meta_Member{0, Meta_Type_int32, 0, "glyph_height", (uint64)&((Spritefont*)NULL)->glyph_height},
	Meta_Member{9, Meta_Type_Rect2, 1, "glyphs", (uint64)&((Spritefont*)NULL)->glyphs},
	Meta_Member{0, Meta_Type_Vec4, 0, "color", (uint64)&((Spritefont*)NULL)->color},
};

const Meta_Member Meta_Type_Gui_Window_Handle_Members[] = {
	Meta_Member{0, Meta_Type_Vec2, 0, "position", (uint64)&((Gui_Window_Handle*)NULL)->position},
	Meta_Member{1, Meta_Type_char, 1, "title", (uint64)&((Gui_Window_Handle*)NULL)->title},
	Meta_Member{0, Meta_Type_int32, 0, "z", (uint64)&((Gui_Window_Handle*)NULL)->z},
};

const Meta_Member Meta_Type_Gui_Text_Input_Handle_Members[] = {
	Meta_Member{1, Meta_Type_char, 1, "buffer", (uint64)&((Gui_Text_Input_Handle*)NULL)->buffer},
	Meta_Member{0, Meta_Type_isize, 0, "buffer_length", (uint64)&((Gui_Text_Input_Handle*)NULL)->buffer_length},
	Meta_Member{0, Meta_Type_isize, 0, "buffer_capacity", (uint64)&((Gui_Text_Input_Handle*)NULL)->buffer_capacity},
	Meta_Member{0, Meta_Type_isize, 0, "max_chars_by_width", (uint64)&((Gui_Text_Input_Handle*)NULL)->max_chars_by_width},
	Meta_Member{0, Meta_Type_isize, 0, "cursor", (uint64)&((Gui_Text_Input_Handle*)NULL)->cursor},
	Meta_Member{0, Meta_Type_isize, 0, "selection_start", (uint64)&((Gui_Text_Input_Handle*)NULL)->selection_start},
	Meta_Member{0, Meta_Type_bool, 0, "active", (uint64)&((Gui_Text_Input_Handle*)NULL)->active},
	Meta_Member{0, Meta_Type_bool, 0, "accept_newlines", (uint64)&((Gui_Text_Input_Handle*)NULL)->accept_newlines},
	Meta_Member{0, Meta_Type_bool, 0, "accept_tabs", (uint64)&((Gui_Text_Input_Handle*)NULL)->accept_tabs},
};

const Meta_Member Meta_Type_Gui_Context_Members[] = {
	Meta_Member{0, Meta_Type_Vec2, 0, "last_pos", (uint64)&((Gui_Context*)NULL)->last_pos},
	Meta_Member{0, Meta_Type_Vec2, 0, "next_pos", (uint64)&((Gui_Context*)NULL)->next_pos},
	Meta_Member{0, Meta_Type_isize, 0, "axis", (uint64)&((Gui_Context*)NULL)->axis},
};

const Meta_Member Meta_Type_Play_State_Members[] = {
	Meta_Member{0, Meta_Type_usize, 0, "current_time", (uint64)&((Play_State*)NULL)->current_time},
	Meta_Member{0, Meta_Type_usize, 0, "prev_time", (uint64)&((Play_State*)NULL)->prev_time},
	Meta_Member{0, Meta_Type_real, 0, "accumulator", (uint64)&((Play_State*)NULL)->accumulator},
	Meta_Member{1, Meta_Type_World, 1, "world", (uint64)&((Play_State*)NULL)->world},
	Meta_Member{0, Meta_Type_bool, 0, "running", (uint64)&((Play_State*)NULL)->running},
	Meta_Member{0, Meta_Type_Vec2i, 0, "world_xy", (uint64)&((Play_State*)NULL)->world_xy},
	Meta_Member{0, Meta_Type_bool, 0, "delete_world_on_stop", (uint64)&((Play_State*)NULL)->delete_world_on_stop},
	Meta_Member{0, Meta_Type_bool, 0, "save_world_on_stop", (uint64)&((Play_State*)NULL)->save_world_on_stop},
};

const Meta_Member Meta_Type_Menu_State_Members[] = {
	Meta_Member{16, Meta_Type_char, 1, "save_dir", (uint64)&((Menu_State*)NULL)->save_dir},
	Meta_Member{0, Meta_Type_isize, 0, "save_dir_len", (uint64)&((Menu_State*)NULL)->save_dir_len},
	Meta_Member{0, Meta_Type_tinydir_dir, 0, "saves", (uint64)&((Menu_State*)NULL)->saves},
	Meta_Member{0, Meta_Type_bool, 0, "saves_dirty", (uint64)&((Menu_State*)NULL)->saves_dirty},
	Meta_Member{0, Meta_Type_Gui_Text_Input_Handle, 0, "handle", (uint64)&((Menu_State*)NULL)->handle},
	Meta_Member{0, Meta_Type_isize, 0, "delete_index", (uint64)&((Menu_State*)NULL)->delete_index},
};

const Meta_Member Meta_Type_Tile_Info_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "id", (uint64)&((Tile_Info*)NULL)->id},
	Meta_Member{0, Meta_Type_usize, 0, "hash", (uint64)&((Tile_Info*)NULL)->hash},
	Meta_Member{0, Meta_Type_Rect2, 0, "texture", (uint64)&((Tile_Info*)NULL)->texture},
	Meta_Member{0, Meta_Type_Rect2, 0, "bottom_texture", (uint64)&((Tile_Info*)NULL)->bottom_texture},
	Meta_Member{0, Meta_Type_Rect2, 0, "top_texture", (uint64)&((Tile_Info*)NULL)->top_texture},
	Meta_Member{0, Meta_Type_real, 0, "movement_modifier", (uint64)&((Tile_Info*)NULL)->movement_modifier},
	Meta_Member{0, Meta_Type_real, 0, "friction", (uint64)&((Tile_Info*)NULL)->friction},
	Meta_Member{9, Meta_Type_char, 1, "name", (uint64)&((Tile_Info*)NULL)->name},
	Meta_Member{0, Meta_Type_int32, 0, "max_damage", (uint64)&((Tile_Info*)NULL)->max_damage},
	Meta_Member{0, Meta_Type_bool, 0, "immune_to_damage", (uint64)&((Tile_Info*)NULL)->immune_to_damage},
	Meta_Member{1, Meta_Type_char, 1, "break_to_name", (uint64)&((Tile_Info*)NULL)->break_to_name},
	Meta_Member{0, Meta_Type_isize, 0, "break_to_id", (uint64)&((Tile_Info*)NULL)->break_to_id},
	Meta_Member{0, Meta_Type_bool, 0, "solid", (uint64)&((Tile_Info*)NULL)->solid},
	Meta_Member{0, Meta_Type_uint64, 0, "body_mask", (uint64)&((Tile_Info*)NULL)->body_mask},
	Meta_Member{0, Meta_Type_uint64, 0, "body_group", (uint64)&((Tile_Info*)NULL)->body_group},
	Meta_Member{0, Meta_Type_bool, 0, "has_top_texture", (uint64)&((Tile_Info*)NULL)->has_top_texture},
	Meta_Member{0, Meta_Type_bool, 0, "has_bottom_texture", (uint64)&((Tile_Info*)NULL)->has_bottom_texture},
};

const Meta_Member Meta_Type_Tile_State_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "id", (uint64)&((Tile_State*)NULL)->id},
	Meta_Member{0, Meta_Type_int32, 0, "damage", (uint64)&((Tile_State*)NULL)->damage},
};

const Meta_Member Meta_Type_Item_Info_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "id", (uint64)&((Item_Info*)NULL)->id},
	Meta_Member{0, Meta_Type_usize, 0, "hash", (uint64)&((Item_Info*)NULL)->hash},
	Meta_Member{9, Meta_Type_char, 1, "name", (uint64)&((Item_Info*)NULL)->name},
	Meta_Member{0, Meta_Type_int32, 0, "max_stack", (uint64)&((Item_Info*)NULL)->max_stack},
	Meta_Member{0, Meta_Type_Rect2, 0, "texture", (uint64)&((Item_Info*)NULL)->texture},
};

const Meta_Member Meta_Type_Rituals_Entity_Userdata_Members[] = {
	Meta_Member{0, Meta_Type_Meta_Type_Rituals_Entity_Userdata_prop, 0, "prop", (uint64)&((Rituals_Entity_Userdata*)NULL)->prop},
	Meta_Member{0, Meta_Type_Meta_Type_Rituals_Entity_Userdata_player, 0, "player", (uint64)&((Rituals_Entity_Userdata*)NULL)->player},
	Meta_Member{0, Meta_Type_Meta_Type_Rituals_Entity_Userdata_enemy, 0, "enemy", (uint64)&((Rituals_Entity_Userdata*)NULL)->enemy},
	Meta_Member{0, Meta_Type_Meta_Type_Rituals_Entity_Userdata_pickup, 0, "pickup", (uint64)&((Rituals_Entity_Userdata*)NULL)->pickup},
	Meta_Member{0, Meta_Type_Meta_Type_Rituals_Entity_Userdata_unknown, 0, "unknown", (uint64)&((Rituals_Entity_Userdata*)NULL)->unknown},
};

const Meta_Member Meta_Type_Rituals_Entity_Userdata_prop_Members[] = {
	Meta_Member{0, Meta_Type_Rituals_Entity_Kinds, 0, "contains", (uint64)&((prop*)NULL)->contains},
	Meta_Member{0, Meta_Type_isize, 0, "subtype", (uint64)&((prop*)NULL)->subtype},
	Meta_Member{0, Meta_Type_isize, 0, "amount", (uint64)&((prop*)NULL)->amount},
	Meta_Member{0, Meta_Type_isize, 0, "quality", (uint64)&((prop*)NULL)->quality},
};

const Meta_Member Meta_Type_Rituals_Entity_Userdata_player_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "held_entity_id", (uint64)&((player*)NULL)->held_entity_id},
	Meta_Member{0, Meta_Type_real, 0, "heal_cooldown", (uint64)&((player*)NULL)->heal_cooldown},
	Meta_Member{0, Meta_Type_real, 0, "heal_timer", (uint64)&((player*)NULL)->heal_timer},
	Meta_Member{0, Meta_Type_int32, 0, "heal_rate", (uint64)&((player*)NULL)->heal_rate},
	Meta_Member{0, Meta_Type_int32, 0, "heal_to_interval", (uint64)&((player*)NULL)->heal_to_interval},
};

const Meta_Member Meta_Type_Rituals_Entity_Userdata_enemy_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "kind", (uint64)&((enemy*)NULL)->kind},
	Meta_Member{0, Meta_Type_isize, 0, "mode", (uint64)&((enemy*)NULL)->mode},
	Meta_Member{0, Meta_Type_real, 0, "speed", (uint64)&((enemy*)NULL)->speed},
	Meta_Member{0, Meta_Type_real, 0, "alert_dist", (uint64)&((enemy*)NULL)->alert_dist},
	Meta_Member{0, Meta_Type_real, 0, "follow_dist", (uint64)&((enemy*)NULL)->follow_dist},
	Meta_Member{0, Meta_Type_Meta_Type_Rituals_Entity_Userdata_enemy_union0, 0, "", (uint64)&((enemy*)NULL)->},
};

const Meta_Member Meta_Type_Rituals_Entity_Userdata_enemy_union0_Members[] = {
	Meta_Member{0, Meta_Type_Meta_Type_Rituals_Entity_Userdata_enemy_union0_bat, 0, "bat", (uint64)&((*)NULL)->bat},
	Meta_Member{0, Meta_Type_Meta_Type_Rituals_Entity_Userdata_enemy_union0_snake, 0, "snake", (uint64)&((*)NULL)->snake},
	Meta_Member{0, Meta_Type_Meta_Type_Rituals_Entity_Userdata_enemy_union0_goblin_knight, 0, "goblin_knight", (uint64)&((*)NULL)->goblin_knight},
};

const Meta_Member Meta_Type_Rituals_Entity_Userdata_enemy_union0_bat_Members[] = {
	Meta_Member{0, Meta_Type_Vec2, 0, "perch", (uint64)&((bat*)NULL)->perch},
};

const Meta_Member Meta_Type_Rituals_Entity_Userdata_enemy_union0_snake_Members[] = {
	Meta_Member{0, Meta_Type_real, 0, "chase_speed_modifier", (uint64)&((snake*)NULL)->chase_speed_modifier},
};

const Meta_Member Meta_Type_Rituals_Entity_Userdata_enemy_union0_goblin_knight_Members[] = {
	Meta_Member{0, Meta_Type_Vec2, 0, "patrol_start", (uint64)&((goblin_knight*)NULL)->patrol_start},
	Meta_Member{0, Meta_Type_Vec2, 0, "patrol_end", (uint64)&((goblin_knight*)NULL)->patrol_end},
};

const Meta_Member Meta_Type_Rituals_Entity_Userdata_pickup_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "kind", (uint64)&((pickup*)NULL)->kind},
	Meta_Member{0, Meta_Type_Meta_Type_Rituals_Entity_Userdata_pickup_union0, 0, "", (uint64)&((pickup*)NULL)->},
};

const Meta_Member Meta_Type_Rituals_Entity_Userdata_pickup_union0_Members[] = {
	Meta_Member{0, Meta_Type_Meta_Type_Rituals_Entity_Userdata_pickup_union0_item, 0, "item", (uint64)&((*)NULL)->item},
	Meta_Member{0, Meta_Type_Meta_Type_Rituals_Entity_Userdata_pickup_union0_health, 0, "health", (uint64)&((*)NULL)->health},
};

const Meta_Member Meta_Type_Rituals_Entity_Userdata_pickup_union0_item_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "id", (uint64)&((item*)NULL)->id},
	Meta_Member{0, Meta_Type_isize, 0, "count", (uint64)&((item*)NULL)->count},
};

const Meta_Member Meta_Type_Rituals_Entity_Userdata_pickup_union0_health_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "amount", (uint64)&((health*)NULL)->amount},
};

const Meta_Member Meta_Type_Rituals_Entity_Userdata_unknown_Members[] = {
	Meta_Member{1, Meta_Type_void, 1, "ptr", (uint64)&((unknown*)NULL)->ptr},
	Meta_Member{0, Meta_Type_isize, 0, "size", (uint64)&((unknown*)NULL)->size},
};

const Meta_Member Meta_Type_MyUnion_Members[] = {
	Meta_Member{0, Meta_Type_Meta_Type_MyUnion_struct0, 0, "", (uint64)&((MyUnion*)NULL)->},
	Meta_Member{0, Meta_Type_Meta_Type_MyUnion_struct1, 0, "", (uint64)&((MyUnion*)NULL)->},
	Meta_Member{0, Meta_Type_Meta_Type_MyUnion_struct2, 0, "", (uint64)&((MyUnion*)NULL)->},
};

const Meta_Member Meta_Type_MyUnion_struct0_Members[] = {
	Meta_Member{0, Meta_Type_real, 0, "x", (uint64)&((*)NULL)->x},
	Meta_Member{0, Meta_Type_real, 0, "y", (uint64)&((*)NULL)->y},
	Meta_Member{0, Meta_Type_real, 0, "z", (uint64)&((*)NULL)->z},
	Meta_Member{0, Meta_Type_real, 0, "w", (uint64)&((*)NULL)->w},
};

const Meta_Member Meta_Type_MyUnion_struct1_Members[] = {
	Meta_Member{0, Meta_Type_uint32, 0, "aa", (uint64)&((*)NULL)->aa},
	Meta_Member{0, Meta_Type_uint32, 0, "bb", (uint64)&((*)NULL)->bb},
	Meta_Member{0, Meta_Type_uint32, 0, "cc", (uint64)&((*)NULL)->cc},
	Meta_Member{0, Meta_Type_uint32, 0, "dd", (uint64)&((*)NULL)->dd},
};

const Meta_Member Meta_Type_MyUnion_struct2_Members[] = {
	Meta_Member{1, Meta_Type_char, 1, "p", (uint64)&((*)NULL)->p},
	Meta_Member{1, Meta_Type_char, 1, "q", (uint64)&((*)NULL)->q},
	Meta_Member{1, Meta_Type_char, 1, "r", (uint64)&((*)NULL)->r},
	Meta_Member{1, Meta_Type_char, 1, "s", (uint64)&((*)NULL)->s},
};

const Meta_Member Meta_Type_Game_Registry_Members[] = {
	Meta_Member{1, Meta_Type_Tile_Info, 1, "tiles", (uint64)&((Game_Registry*)NULL)->tiles},
	Meta_Member{1, Meta_Type_usize, 1, "tiles_hash", (uint64)&((Game_Registry*)NULL)->tiles_hash},
	Meta_Member{0, Meta_Type_isize, 0, "tiles_count", (uint64)&((Game_Registry*)NULL)->tiles_count},
	Meta_Member{1, Meta_Type_Item_Info, 1, "items", (uint64)&((Game_Registry*)NULL)->items},
	Meta_Member{1, Meta_Type_usize, 1, "items_hash", (uint64)&((Game_Registry*)NULL)->items_hash},
	Meta_Member{0, Meta_Type_isize, 0, "items_count", (uint64)&((Game_Registry*)NULL)->items_count},
};

const Meta_Member Meta_Type__game_registry__Tile_Info_pair_Members[] = {
	Meta_Member{0, Meta_Type_Tile_Info, 0, "v", (uint64)&((_game_registry__Tile_Info_pair*)NULL)->v},
	Meta_Member{0, Meta_Type_usize, 0, "k", (uint64)&((_game_registry__Tile_Info_pair*)NULL)->k},
};

const Meta_Member Meta_Type__game_registry__Item_Info_pair_Members[] = {
	Meta_Member{0, Meta_Type_Item_Info, 0, "v", (uint64)&((_game_registry__Item_Info_pair*)NULL)->v},
	Meta_Member{0, Meta_Type_usize, 0, "k", (uint64)&((_game_registry__Item_Info_pair*)NULL)->k},
};

const Meta_Member Meta_Type_Item_Stack_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "id", (uint64)&((Item_Stack*)NULL)->id},
	Meta_Member{1, Meta_Type_Item_Info, 1, "info", (uint64)&((Item_Stack*)NULL)->info},
	Meta_Member{0, Meta_Type_int32, 0, "number", (uint64)&((Item_Stack*)NULL)->number},
};

const Meta_Member Meta_Type_Inventory_Members[] = {
	Meta_Member{1, Meta_Type_Item_Stack, 2, "contents", (uint64)&((Inventory*)NULL)->contents},
	Meta_Member{0, Meta_Type_isize, 0, "w", (uint64)&((Inventory*)NULL)->w},
	Meta_Member{0, Meta_Type_isize, 0, "h", (uint64)&((Inventory*)NULL)->h},
	Meta_Member{0, Meta_Type_isize, 0, "size", (uint64)&((Inventory*)NULL)->size},
};

const Meta_Member Meta_Type_Tilemap_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "w", (uint64)&((Tilemap*)NULL)->w},
	Meta_Member{0, Meta_Type_isize, 0, "h", (uint64)&((Tilemap*)NULL)->h},
	Meta_Member{1, Meta_Type_Tile, 1, "tiles", (uint64)&((Tilemap*)NULL)->tiles},
	Meta_Member{1, Meta_Type_Tile_State, 1, "states", (uint64)&((Tilemap*)NULL)->states},
};

const Meta_Member Meta_Type_Sim_Body_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "id", (uint64)&((Sim_Body*)NULL)->id},
	Meta_Member{0, Meta_Type_AABB, 0, "shape", (uint64)&((Sim_Body*)NULL)->shape},
	Meta_Member{0, Meta_Type_Vec2, 0, "velocity", (uint64)&((Sim_Body*)NULL)->velocity},
	Meta_Member{0, Meta_Type_Vec2, 0, "force", (uint64)&((Sim_Body*)NULL)->force},
	Meta_Member{0, Meta_Type_Vec2, 0, "collision_vel", (uint64)&((Sim_Body*)NULL)->collision_vel},
	Meta_Member{0, Meta_Type_real, 0, "inv_mass", (uint64)&((Sim_Body*)NULL)->inv_mass},
	Meta_Member{0, Meta_Type_real, 0, "restitution", (uint64)&((Sim_Body*)NULL)->restitution},
	Meta_Member{0, Meta_Type_real, 0, "damping", (uint64)&((Sim_Body*)NULL)->damping},
	Meta_Member{0, Meta_Type_uint64, 0, "flags", (uint64)&((Sim_Body*)NULL)->flags},
	Meta_Member{0, Meta_Type_uint64, 0, "group", (uint64)&((Sim_Body*)NULL)->group},
	Meta_Member{0, Meta_Type_uint64, 0, "mask", (uint64)&((Sim_Body*)NULL)->mask},
	Meta_Member{0, Meta_Type_isize, 0, "entity_id", (uint64)&((Sim_Body*)NULL)->entity_id},
	Meta_Member{1, Meta_Type_Entity, 1, "entity", (uint64)&((Sim_Body*)NULL)->entity},
};

const Meta_Member Meta_Type_Sim_Contact_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "a_id", (uint64)&((Sim_Contact*)NULL)->a_id},
	Meta_Member{0, Meta_Type_isize, 0, "b_id", (uint64)&((Sim_Contact*)NULL)->b_id},
	Meta_Member{1, Meta_Type_Sim_Body, 1, "static_b", (uint64)&((Sim_Contact*)NULL)->static_b},
	Meta_Member{0, Meta_Type_Vec2, 0, "overlap", (uint64)&((Sim_Contact*)NULL)->overlap},
	Meta_Member{0, Meta_Type_real, 0, "mag", (uint64)&((Sim_Contact*)NULL)->mag},
	Meta_Member{0, Meta_Type_Vec2, 0, "normal", (uint64)&((Sim_Contact*)NULL)->normal},
};

const Meta_Member Meta_Type_Sim_Grid_Cell_Members[] = {
	Meta_Member{1, Meta_Type_Sim_Body, 1, "body", (uint64)&((Sim_Grid_Cell*)NULL)->body},
	Meta_Member{1, Meta_Type_Sim_Grid_Cell, 1, "next", (uint64)&((Sim_Grid_Cell*)NULL)->next},
};

const Meta_Member Meta_Type_Sim_Static_Grid_Members[] = {
	Meta_Member{1, Meta_Type_Sim_Grid_Cell, 1, "cell_storage", (uint64)&((Sim_Static_Grid*)NULL)->cell_storage},
	Meta_Member{0, Meta_Type_isize, 0, "cell_storage_count", (uint64)&((Sim_Static_Grid*)NULL)->cell_storage_count},
	Meta_Member{0, Meta_Type_isize, 0, "cell_storage_capacity", (uint64)&((Sim_Static_Grid*)NULL)->cell_storage_capacity},
	Meta_Member{1, Meta_Type_Sim_Grid_Cell, 2, "cells", (uint64)&((Sim_Static_Grid*)NULL)->cells},
	Meta_Member{0, Meta_Type_isize, 0, "cells_length", (uint64)&((Sim_Static_Grid*)NULL)->cells_length},
	Meta_Member{0, Meta_Type_Vec2i, 0, "size", (uint64)&((Sim_Static_Grid*)NULL)->size},
};

const Meta_Member Meta_Type_Simulator_Members[] = {
	Meta_Member{1, Meta_Type_Sim_Body, 1, "static_bodies", (uint64)&((Simulator*)NULL)->static_bodies},
	Meta_Member{0, Meta_Type_isize, 0, "static_bodies_count", (uint64)&((Simulator*)NULL)->static_bodies_count},
	Meta_Member{0, Meta_Type_isize, 0, "static_bodies_capacity", (uint64)&((Simulator*)NULL)->static_bodies_capacity},
	Meta_Member{1, Meta_Type_Sim_Static_Grid, 1, "grid", (uint64)&((Simulator*)NULL)->grid},
	Meta_Member{1, Meta_Type_Sim_Body, 1, "bodies", (uint64)&((Simulator*)NULL)->bodies},
	Meta_Member{0, Meta_Type_isize, 0, "bodies_count", (uint64)&((Simulator*)NULL)->bodies_count},
	Meta_Member{0, Meta_Type_isize, 0, "bodies_capacity", (uint64)&((Simulator*)NULL)->bodies_capacity},
	Meta_Member{0, Meta_Type_isize, 0, "next_body_id", (uint64)&((Simulator*)NULL)->next_body_id},
	Meta_Member{1, Meta_Type_Sim_Contact, 1, "contacts", (uint64)&((Simulator*)NULL)->contacts},
	Meta_Member{0, Meta_Type_isize, 0, "contacts_count", (uint64)&((Simulator*)NULL)->contacts_count},
	Meta_Member{0, Meta_Type_isize, 0, "contacts_capacity", (uint64)&((Simulator*)NULL)->contacts_capacity},
	Meta_Member{0, Meta_Type_isize, 0, "sort_axis", (uint64)&((Simulator*)NULL)->sort_axis},
};

const Meta_Member Meta_Type_Particle_Members[] = {
	Meta_Member{0, Meta_Type_Vec3, 0, "position", (uint64)&((Particle*)NULL)->position},
	Meta_Member{0, Meta_Type_Vec3, 0, "velocity", (uint64)&((Particle*)NULL)->velocity},
	Meta_Member{0, Meta_Type_real, 0, "scale", (uint64)&((Particle*)NULL)->scale},
	Meta_Member{0, Meta_Type_real, 0, "angle", (uint64)&((Particle*)NULL)->angle},
	Meta_Member{0, Meta_Type_real, 0, "angular_vel", (uint64)&((Particle*)NULL)->angular_vel},
	Meta_Member{0, Meta_Type_int32, 0, "frame", (uint64)&((Particle*)NULL)->frame},
	Meta_Member{0, Meta_Type_int32, 0, "time", (uint64)&((Particle*)NULL)->time},
	Meta_Member{0, Meta_Type_int32, 0, "total_time", (uint64)&((Particle*)NULL)->total_time},
	Meta_Member{0, Meta_Type_int32, 0, "style_index", (uint64)&((Particle*)NULL)->style_index},
};

const Meta_Member Meta_Type_Particle_Style_Members[] = {
	Meta_Member{0, Meta_Type_int32, 0, "id", (uint64)&((Particle_Style*)NULL)->id},
	Meta_Member{0, Meta_Type_Rect2, 0, "texture", (uint64)&((Particle_Style*)NULL)->texture},
	Meta_Member{0, Meta_Type_Vec2, 0, "size", (uint64)&((Particle_Style*)NULL)->size},
	Meta_Member{0, Meta_Type_Vec4, 0, "color", (uint64)&((Particle_Style*)NULL)->color},
	Meta_Member{0, Meta_Type_Vec4, 0, "shadow_color", (uint64)&((Particle_Style*)NULL)->shadow_color},
	Meta_Member{0, Meta_Type_Vec3, 0, "acceleration", (uint64)&((Particle_Style*)NULL)->acceleration},
	Meta_Member{0, Meta_Type_real, 0, "impulse_min", (uint64)&((Particle_Style*)NULL)->impulse_min},
	Meta_Member{0, Meta_Type_real, 0, "impulse_max", (uint64)&((Particle_Style*)NULL)->impulse_max},
	Meta_Member{0, Meta_Type_real, 0, "angle_min", (uint64)&((Particle_Style*)NULL)->angle_min},
	Meta_Member{0, Meta_Type_real, 0, "angle_max", (uint64)&((Particle_Style*)NULL)->angle_max},
	Meta_Member{0, Meta_Type_real, 0, "angle_vel_min", (uint64)&((Particle_Style*)NULL)->angle_vel_min},
	Meta_Member{0, Meta_Type_real, 0, "angle_vel_max", (uint64)&((Particle_Style*)NULL)->angle_vel_max},
	Meta_Member{0, Meta_Type_real, 0, "scale_min", (uint64)&((Particle_Style*)NULL)->scale_min},
	Meta_Member{0, Meta_Type_real, 0, "scale_max", (uint64)&((Particle_Style*)NULL)->scale_max},
	Meta_Member{0, Meta_Type_int32, 0, "frame_min", (uint64)&((Particle_Style*)NULL)->frame_min},
	Meta_Member{0, Meta_Type_int32, 0, "frame_max", (uint64)&((Particle_Style*)NULL)->frame_max},
	Meta_Member{0, Meta_Type_int32, 0, "max_frames", (uint64)&((Particle_Style*)NULL)->max_frames},
	Meta_Member{0, Meta_Type_int32, 0, "framerate", (uint64)&((Particle_Style*)NULL)->framerate},
	Meta_Member{0, Meta_Type_int32, 0, "time_min", (uint64)&((Particle_Style*)NULL)->time_min},
	Meta_Member{0, Meta_Type_int32, 0, "time_max", (uint64)&((Particle_Style*)NULL)->time_max},
	Meta_Member{0, Meta_Type_bool, 0, "time_scaling", (uint64)&((Particle_Style*)NULL)->time_scaling},
	Meta_Member{0, Meta_Type_bool, 0, "time_alpha", (uint64)&((Particle_Style*)NULL)->time_alpha},
	Meta_Member{0, Meta_Type_real, 0, "ground_restitution", (uint64)&((Particle_Style*)NULL)->ground_restitution},
	Meta_Member{0, Meta_Type_real, 0, "skid_on_bounce_min", (uint64)&((Particle_Style*)NULL)->skid_on_bounce_min},
	Meta_Member{0, Meta_Type_real, 0, "skid_on_bounce_max", (uint64)&((Particle_Style*)NULL)->skid_on_bounce_max},
	Meta_Member{0, Meta_Type_real, 0, "jitter_on_bounce_mag", (uint64)&((Particle_Style*)NULL)->jitter_on_bounce_mag},
};

const Meta_Member Meta_Type_Emitter_Members[] = {
	Meta_Member{1, Meta_Type_Particle, 1, "particles", (uint64)&((Emitter*)NULL)->particles},
	Meta_Member{0, Meta_Type_isize, 0, "particles_count", (uint64)&((Emitter*)NULL)->particles_count},
	Meta_Member{0, Meta_Type_isize, 0, "particles_capacity", (uint64)&((Emitter*)NULL)->particles_capacity},
	Meta_Member{1, Meta_Type_Particle_Style, 1, "styles", (uint64)&((Emitter*)NULL)->styles},
	Meta_Member{0, Meta_Type_isize, 0, "styles_count", (uint64)&((Emitter*)NULL)->styles_count},
	Meta_Member{0, Meta_Type_isize, 0, "styles_capacity", (uint64)&((Emitter*)NULL)->styles_capacity},
};

const Meta_Member Meta_Type_Hitbox_Contact_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "a_id", (uint64)&((Hitbox_Contact*)NULL)->a_id},
	Meta_Member{0, Meta_Type_isize, 0, "b_id", (uint64)&((Hitbox_Contact*)NULL)->b_id},
	Meta_Member{1, Meta_Type_Entity, 1, "a", (uint64)&((Hitbox_Contact*)NULL)->a},
	Meta_Member{1, Meta_Type_Entity, 1, "b", (uint64)&((Hitbox_Contact*)NULL)->b},
};

const Meta_Member Meta_Type_Hitbox_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "id", (uint64)&((Hitbox*)NULL)->id},
	Meta_Member{1, Meta_Type_Entity, 1, "ref", (uint64)&((Hitbox*)NULL)->ref},
	Meta_Member{0, Meta_Type_uint64, 0, "mask", (uint64)&((Hitbox*)NULL)->mask},
	Meta_Member{0, Meta_Type_uint64, 0, "group", (uint64)&((Hitbox*)NULL)->group},
	Meta_Member{0, Meta_Type_AABB, 0, "box", (uint64)&((Hitbox*)NULL)->box},
};

const Meta_Member Meta_Type_Entity_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "id", (uint64)&((Entity*)NULL)->id},
	Meta_Member{0, Meta_Type_isize, 0, "body_id", (uint64)&((Entity*)NULL)->body_id},
	Meta_Member{1, Meta_Type_Sim_Body, 1, "body", (uint64)&((Entity*)NULL)->body},
	Meta_Member{0, Meta_Type_Vec2, 0, "walk_impulse", (uint64)&((Entity*)NULL)->walk_impulse},
	Meta_Member{0, Meta_Type_Hitbox, 0, "hitbox", (uint64)&((Entity*)NULL)->hitbox},
	Meta_Member{0, Meta_Type_int32, 0, "health", (uint64)&((Entity*)NULL)->health},
	Meta_Member{0, Meta_Type_int32, 0, "attack", (uint64)&((Entity*)NULL)->attack},
	Meta_Member{0, Meta_Type_real, 0, "attack_interval", (uint64)&((Entity*)NULL)->attack_interval},
	Meta_Member{0, Meta_Type_real, 0, "attack_timer", (uint64)&((Entity*)NULL)->attack_timer},
	Meta_Member{0, Meta_Type_real, 0, "knockback", (uint64)&((Entity*)NULL)->knockback},
	Meta_Member{1, Meta_Type_Animated_Sprite, 1, "anim", (uint64)&((Entity*)NULL)->anim},
	Meta_Member{0, Meta_Type_Sprite, 0, "sprite", (uint64)&((Entity*)NULL)->sprite},
	Meta_Member{0, Meta_Type_real, 0, "z", (uint64)&((Entity*)NULL)->z},
	Meta_Member{0, Meta_Type_real, 0, "shadow_scale", (uint64)&((Entity*)NULL)->shadow_scale},
	Meta_Member{0, Meta_Type_isize, 0, "kind", (uint64)&((Entity*)NULL)->kind},
	Meta_Member{0, Meta_Type_int32, 0, "counter", (uint64)&((Entity*)NULL)->counter},
	Meta_Member{0, Meta_Type_int32, 0, "facing", (uint64)&((Entity*)NULL)->facing},
	Meta_Member{0, Meta_Type_Direction, 0, "direction", (uint64)&((Entity*)NULL)->direction},
	Meta_Member{0, Meta_Type_uint64, 0, "events", (uint64)&((Entity*)NULL)->events},
	Meta_Member{0, Meta_Type_uint64, 0, "flags", (uint64)&((Entity*)NULL)->flags},
	Meta_Member{0, Meta_Type_Rituals_Entity_Userdata, 0, "userdata", (uint64)&((Entity*)NULL)->userdata},
};

const Meta_Member Meta_Type_Area_Link_Members[] = {
	Meta_Member{0, Meta_Type_Vec2i, 0, "position", (uint64)&((Area_Link*)NULL)->position},
	Meta_Member{1, Meta_Type_World_Area_Stub, 1, "link", (uint64)&((Area_Link*)NULL)->link},
};

const Meta_Member Meta_Type_World_Area_Stub_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "id", (uint64)&((World_Area_Stub*)NULL)->id},
	Meta_Member{0, Meta_Type_usize, 0, "seed", (uint64)&((World_Area_Stub*)NULL)->seed},
	Meta_Member{0, Meta_Type_Area_Link, 0, "north", (uint64)&((World_Area_Stub*)NULL)->north},
	Meta_Member{0, Meta_Type_Area_Link, 0, "south", (uint64)&((World_Area_Stub*)NULL)->south},
	Meta_Member{0, Meta_Type_Area_Link, 0, "west", (uint64)&((World_Area_Stub*)NULL)->west},
	Meta_Member{0, Meta_Type_Area_Link, 0, "east", (uint64)&((World_Area_Stub*)NULL)->east},
	Meta_Member{0, Meta_Type_World_Area_Biome, 0, "biome", (uint64)&((World_Area_Stub*)NULL)->biome},
};

const Meta_Member Meta_Type_World_Area_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "id", (uint64)&((World_Area*)NULL)->id},
	Meta_Member{1, Meta_Type_World_Area_Stub, 1, "stub", (uint64)&((World_Area*)NULL)->stub},
	Meta_Member{1, Meta_Type_World, 1, "world", (uint64)&((World_Area*)NULL)->world},
	Meta_Member{0, Meta_Type_Simulator, 0, "sim", (uint64)&((World_Area*)NULL)->sim},
	Meta_Member{0, Meta_Type_Tilemap, 0, "map", (uint64)&((World_Area*)NULL)->map},
	Meta_Member{0, Meta_Type_Vec2, 0, "offset", (uint64)&((World_Area*)NULL)->offset},
	Meta_Member{0, Meta_Type_Vec2, 0, "target", (uint64)&((World_Area*)NULL)->target},
	Meta_Member{1, Meta_Type_Entity, 1, "entities", (uint64)&((World_Area*)NULL)->entities},
	Meta_Member{0, Meta_Type_bool, 0, "entities_dirty", (uint64)&((World_Area*)NULL)->entities_dirty},
	Meta_Member{0, Meta_Type_isize, 0, "entities_count", (uint64)&((World_Area*)NULL)->entities_count},
	Meta_Member{0, Meta_Type_isize, 0, "entities_capacity", (uint64)&((World_Area*)NULL)->entities_capacity},
	Meta_Member{0, Meta_Type_isize, 0, "next_entity_id", (uint64)&((World_Area*)NULL)->next_entity_id},
	Meta_Member{1, Meta_Type_isize, 1, "removed_entities", (uint64)&((World_Area*)NULL)->removed_entities},
	Meta_Member{0, Meta_Type_isize, 0, "removed_entities_count", (uint64)&((World_Area*)NULL)->removed_entities_count},
	Meta_Member{0, Meta_Type_isize, 0, "removed_entities_capacity", (uint64)&((World_Area*)NULL)->removed_entities_capacity},
	Meta_Member{1, Meta_Type_Hitbox, 1, "hitboxes", (uint64)&((World_Area*)NULL)->hitboxes},
	Meta_Member{0, Meta_Type_isize, 0, "hitboxes_count", (uint64)&((World_Area*)NULL)->hitboxes_count},
	Meta_Member{0, Meta_Type_isize, 0, "hitboxes_capacity", (uint64)&((World_Area*)NULL)->hitboxes_capacity},
	Meta_Member{0, Meta_Type_int32, 0, "hitbox_sort_axis", (uint64)&((World_Area*)NULL)->hitbox_sort_axis},
	Meta_Member{1, Meta_Type_Hitbox_Contact, 1, "hitbox_contacts", (uint64)&((World_Area*)NULL)->hitbox_contacts},
	Meta_Member{0, Meta_Type_isize, 0, "hitbox_contacts_count", (uint64)&((World_Area*)NULL)->hitbox_contacts_count},
	Meta_Member{0, Meta_Type_isize, 0, "hitbox_contacts_capacity", (uint64)&((World_Area*)NULL)->hitbox_contacts_capacity},
	Meta_Member{1, Meta_Type_Entity, 1, "player", (uint64)&((World_Area*)NULL)->player},
};

const Meta_Member Meta_Type_World_Members[] = {
	Meta_Member{1, Meta_Type_char, 1, "name", (uint64)&((World*)NULL)->name},
	Meta_Member{1, Meta_Type_World_Area_Stub, 1, "area_stubs", (uint64)&((World*)NULL)->area_stubs},
	Meta_Member{0, Meta_Type_isize, 0, "next_area_id", (uint64)&((World*)NULL)->next_area_id},
	Meta_Member{0, Meta_Type_isize, 0, "areas_count", (uint64)&((World*)NULL)->areas_count},
	Meta_Member{0, Meta_Type_isize, 0, "areas_capacity", (uint64)&((World*)NULL)->areas_capacity},
	Meta_Member{0, Meta_Type_isize, 0, "areas_width", (uint64)&((World*)NULL)->areas_width},
	Meta_Member{0, Meta_Type_isize, 0, "areas_height", (uint64)&((World*)NULL)->areas_height},
	Meta_Member{1, Meta_Type_World_Area, 1, "current_area", (uint64)&((World*)NULL)->current_area},
	Meta_Member{0, Meta_Type_usize, 0, "seed", (uint64)&((World*)NULL)->seed},
	Meta_Member{0, Meta_Type_Entity, 0, "global_player_entity", (uint64)&((World*)NULL)->global_player_entity},
	Meta_Member{0, Meta_Type_Sim_Body, 0, "global_player_body", (uint64)&((World*)NULL)->global_player_body},
	Meta_Member{0, Meta_Type_usize, 0, "slowtick_timer", (uint64)&((World*)NULL)->slowtick_timer},
	Meta_Member{0, Meta_Type_usize, 0, "slowtick_timer_interval", (uint64)&((World*)NULL)->slowtick_timer_interval},
	Meta_Member{0, Meta_Type_Particle_Style, 0, "base_style", (uint64)&((World*)NULL)->base_style},
	Meta_Member{0, Meta_Type_Emitter, 0, "emitter", (uint64)&((World*)NULL)->emitter},
};

const Meta_Struct_Info All_Meta_Struct_Info[] = {
	Meta_Struct_Info {
		"Vec2", 
		"Meta_Type_Vec2", 
		Meta_Type_Vec2, 
		Meta_Type_Vec2_Members, 
		2},
	Meta_Struct_Info {
		"", 
		"Meta_Type_Vec2_struct0", 
		Meta_Type_Vec2_struct0, 
		Meta_Type_Vec2_struct0_Members, 
		2},
	Meta_Struct_Info {
		"Vec2i", 
		"Meta_Type_Vec2i", 
		Meta_Type_Vec2i, 
		Meta_Type_Vec2i_Members, 
		2},
	Meta_Struct_Info {
		"", 
		"Meta_Type_Vec2i_struct0", 
		Meta_Type_Vec2i_struct0, 
		Meta_Type_Vec2i_struct0_Members, 
		2},
	Meta_Struct_Info {
		"Vec3", 
		"Meta_Type_Vec3", 
		Meta_Type_Vec3, 
		Meta_Type_Vec3_Members, 
		2},
	Meta_Struct_Info {
		"", 
		"Meta_Type_Vec3_struct0", 
		Meta_Type_Vec3_struct0, 
		Meta_Type_Vec3_struct0_Members, 
		3},
	Meta_Struct_Info {
		"Vec4", 
		"Meta_Type_Vec4", 
		Meta_Type_Vec4, 
		Meta_Type_Vec4_Members, 
		2},
	Meta_Struct_Info {
		"", 
		"Meta_Type_Vec4_struct0", 
		Meta_Type_Vec4_struct0, 
		Meta_Type_Vec4_struct0_Members, 
		4},
	Meta_Struct_Info {
		"AABB", 
		"Meta_Type_AABB", 
		Meta_Type_AABB, 
		Meta_Type_AABB_Members, 
		2},
	Meta_Struct_Info {
		"", 
		"Meta_Type_AABB_struct0", 
		Meta_Type_AABB_struct0, 
		Meta_Type_AABB_struct0_Members, 
		2},
	Meta_Struct_Info {
		"", 
		"Meta_Type_AABB_struct0_union0", 
		Meta_Type_AABB_struct0_union0, 
		Meta_Type_AABB_struct0_union0_Members, 
		2},
	Meta_Struct_Info {
		"", 
		"Meta_Type_AABB_struct0_union0_struct0", 
		Meta_Type_AABB_struct0_union0_struct0, 
		Meta_Type_AABB_struct0_union0_struct0_Members, 
		2},
	Meta_Struct_Info {
		"", 
		"Meta_Type_AABB_struct0_union0_struct1", 
		Meta_Type_AABB_struct0_union0_struct1, 
		Meta_Type_AABB_struct0_union0_struct1_Members, 
		1},
	Meta_Struct_Info {
		"Rect2", 
		"Meta_Type_Rect2", 
		Meta_Type_Rect2, 
		Meta_Type_Rect2_Members, 
		3},
	Meta_Struct_Info {
		"", 
		"Meta_Type_Rect2_struct0", 
		Meta_Type_Rect2_struct0, 
		Meta_Type_Rect2_struct0_Members, 
		4},
	Meta_Struct_Info {
		"", 
		"Meta_Type_Rect2_struct1", 
		Meta_Type_Rect2_struct1, 
		Meta_Type_Rect2_struct1_Members, 
		2},
	Meta_Struct_Info {
		"Rect2_Clip_Info", 
		"Meta_Type_Rect2_Clip_Info", 
		Meta_Type_Rect2_Clip_Info, 
		Meta_Type_Rect2_Clip_Info_Members, 
		4},
	Meta_Struct_Info {
		"Rect2i", 
		"Meta_Type_Rect2i", 
		Meta_Type_Rect2i, 
		Meta_Type_Rect2i_Members, 
		4},
	Meta_Struct_Info {
		"Memory_Arena", 
		"Meta_Type_Memory_Arena", 
		Meta_Type_Memory_Arena, 
		Meta_Type_Memory_Arena_Members, 
		5},
	Meta_Struct_Info {
		"Random", 
		"Meta_Type_Random", 
		Meta_Type_Random, 
		Meta_Type_Random_Members, 
		2},
	Meta_Struct_Info {
		"Game_Input", 
		"Meta_Type_Game_Input", 
		Meta_Type_Game_Input, 
		Meta_Type_Game_Input_Members, 
		13},
	Meta_Struct_Info {
		"Game_Main", 
		"Meta_Type_Game_Main", 
		Meta_Type_Game_Main, 
		Meta_Type_Game_Main_Members, 
		22},
	Meta_Struct_Info {
		"Sprite", 
		"Meta_Type_Sprite", 
		Meta_Type_Sprite, 
		Meta_Type_Sprite_Members, 
		8},
	Meta_Struct_Info {
		"Sprite4", 
		"Meta_Type_Sprite4", 
		Meta_Type_Sprite4, 
		Meta_Type_Sprite4_Members, 
		1},
	Meta_Struct_Info {
		"Render_Group", 
		"Meta_Type_Render_Group", 
		Meta_Type_Render_Group, 
		Meta_Type_Render_Group_Members, 
		10},
	Meta_Struct_Info {
		"OpenGL_Renderer", 
		"Meta_Type_OpenGL_Renderer", 
		Meta_Type_OpenGL_Renderer, 
		Meta_Type_OpenGL_Renderer_Members, 
		9},
	Meta_Struct_Info {
		"Animation_Frame", 
		"Meta_Type_Animation_Frame", 
		Meta_Type_Animation_Frame, 
		Meta_Type_Animation_Frame_Members, 
		6},
	Meta_Struct_Info {
		"Animation", 
		"Meta_Type_Animation", 
		Meta_Type_Animation, 
		Meta_Type_Animation_Members, 
		7},
	Meta_Struct_Info {
		"Animated_Sprite", 
		"Meta_Type_Animated_Sprite", 
		Meta_Type_Animated_Sprite, 
		Meta_Type_Animated_Sprite_Members, 
		7},
	Meta_Struct_Info {
		"Spritefont", 
		"Meta_Type_Spritefont", 
		Meta_Type_Spritefont, 
		Meta_Type_Spritefont_Members, 
		7},
	Meta_Struct_Info {
		"Gui_Window_Handle", 
		"Meta_Type_Gui_Window_Handle", 
		Meta_Type_Gui_Window_Handle, 
		Meta_Type_Gui_Window_Handle_Members, 
		3},
	Meta_Struct_Info {
		"Gui_Text_Input_Handle", 
		"Meta_Type_Gui_Text_Input_Handle", 
		Meta_Type_Gui_Text_Input_Handle, 
		Meta_Type_Gui_Text_Input_Handle_Members, 
		9},
	Meta_Struct_Info {
		"Gui_Context", 
		"Meta_Type_Gui_Context", 
		Meta_Type_Gui_Context, 
		Meta_Type_Gui_Context_Members, 
		3},
	Meta_Struct_Info {
		"Play_State", 
		"Meta_Type_Play_State", 
		Meta_Type_Play_State, 
		Meta_Type_Play_State_Members, 
		8},
	Meta_Struct_Info {
		"Menu_State", 
		"Meta_Type_Menu_State", 
		Meta_Type_Menu_State, 
		Meta_Type_Menu_State_Members, 
		6},
	Meta_Struct_Info {
		"Tile_Info", 
		"Meta_Type_Tile_Info", 
		Meta_Type_Tile_Info, 
		Meta_Type_Tile_Info_Members, 
		17},
	Meta_Struct_Info {
		"Tile_State", 
		"Meta_Type_Tile_State", 
		Meta_Type_Tile_State, 
		Meta_Type_Tile_State_Members, 
		2},
	Meta_Struct_Info {
		"Item_Info", 
		"Meta_Type_Item_Info", 
		Meta_Type_Item_Info, 
		Meta_Type_Item_Info_Members, 
		5},
	Meta_Struct_Info {
		"Rituals_Entity_Userdata", 
		"Meta_Type_Rituals_Entity_Userdata", 
		Meta_Type_Rituals_Entity_Userdata, 
		Meta_Type_Rituals_Entity_Userdata_Members, 
		5},
	Meta_Struct_Info {
		"prop", 
		"Meta_Type_Rituals_Entity_Userdata_prop", 
		Meta_Type_Rituals_Entity_Userdata_prop, 
		Meta_Type_Rituals_Entity_Userdata_prop_Members, 
		4},
	Meta_Struct_Info {
		"player", 
		"Meta_Type_Rituals_Entity_Userdata_player", 
		Meta_Type_Rituals_Entity_Userdata_player, 
		Meta_Type_Rituals_Entity_Userdata_player_Members, 
		5},
	Meta_Struct_Info {
		"enemy", 
		"Meta_Type_Rituals_Entity_Userdata_enemy", 
		Meta_Type_Rituals_Entity_Userdata_enemy, 
		Meta_Type_Rituals_Entity_Userdata_enemy_Members, 
		6},
	Meta_Struct_Info {
		"", 
		"Meta_Type_Rituals_Entity_Userdata_enemy_union0", 
		Meta_Type_Rituals_Entity_Userdata_enemy_union0, 
		Meta_Type_Rituals_Entity_Userdata_enemy_union0_Members, 
		3},
	Meta_Struct_Info {
		"bat", 
		"Meta_Type_Rituals_Entity_Userdata_enemy_union0_bat", 
		Meta_Type_Rituals_Entity_Userdata_enemy_union0_bat, 
		Meta_Type_Rituals_Entity_Userdata_enemy_union0_bat_Members, 
		1},
	Meta_Struct_Info {
		"snake", 
		"Meta_Type_Rituals_Entity_Userdata_enemy_union0_snake", 
		Meta_Type_Rituals_Entity_Userdata_enemy_union0_snake, 
		Meta_Type_Rituals_Entity_Userdata_enemy_union0_snake_Members, 
		1},
	Meta_Struct_Info {
		"goblin_knight", 
		"Meta_Type_Rituals_Entity_Userdata_enemy_union0_goblin_knight", 
		Meta_Type_Rituals_Entity_Userdata_enemy_union0_goblin_knight, 
		Meta_Type_Rituals_Entity_Userdata_enemy_union0_goblin_knight_Members, 
		2},
	Meta_Struct_Info {
		"pickup", 
		"Meta_Type_Rituals_Entity_Userdata_pickup", 
		Meta_Type_Rituals_Entity_Userdata_pickup, 
		Meta_Type_Rituals_Entity_Userdata_pickup_Members, 
		2},
	Meta_Struct_Info {
		"", 
		"Meta_Type_Rituals_Entity_Userdata_pickup_union0", 
		Meta_Type_Rituals_Entity_Userdata_pickup_union0, 
		Meta_Type_Rituals_Entity_Userdata_pickup_union0_Members, 
		2},
	Meta_Struct_Info {
		"item", 
		"Meta_Type_Rituals_Entity_Userdata_pickup_union0_item", 
		Meta_Type_Rituals_Entity_Userdata_pickup_union0_item, 
		Meta_Type_Rituals_Entity_Userdata_pickup_union0_item_Members, 
		2},
	Meta_Struct_Info {
		"health", 
		"Meta_Type_Rituals_Entity_Userdata_pickup_union0_health", 
		Meta_Type_Rituals_Entity_Userdata_pickup_union0_health, 
		Meta_Type_Rituals_Entity_Userdata_pickup_union0_health_Members, 
		1},
	Meta_Struct_Info {
		"unknown", 
		"Meta_Type_Rituals_Entity_Userdata_unknown", 
		Meta_Type_Rituals_Entity_Userdata_unknown, 
		Meta_Type_Rituals_Entity_Userdata_unknown_Members, 
		2},
	Meta_Struct_Info {
		"MyUnion", 
		"Meta_Type_MyUnion", 
		Meta_Type_MyUnion, 
		Meta_Type_MyUnion_Members, 
		3},
	Meta_Struct_Info {
		"", 
		"Meta_Type_MyUnion_struct0", 
		Meta_Type_MyUnion_struct0, 
		Meta_Type_MyUnion_struct0_Members, 
		4},
	Meta_Struct_Info {
		"", 
		"Meta_Type_MyUnion_struct1", 
		Meta_Type_MyUnion_struct1, 
		Meta_Type_MyUnion_struct1_Members, 
		4},
	Meta_Struct_Info {
		"", 
		"Meta_Type_MyUnion_struct2", 
		Meta_Type_MyUnion_struct2, 
		Meta_Type_MyUnion_struct2_Members, 
		4},
	Meta_Struct_Info {
		"Game_Registry", 
		"Meta_Type_Game_Registry", 
		Meta_Type_Game_Registry, 
		Meta_Type_Game_Registry_Members, 
		6},
	Meta_Struct_Info {
		"_game_registry__Tile_Info_pair", 
		"Meta_Type__game_registry__Tile_Info_pair", 
		Meta_Type__game_registry__Tile_Info_pair, 
		Meta_Type__game_registry__Tile_Info_pair_Members, 
		2},
	Meta_Struct_Info {
		"_game_registry__Item_Info_pair", 
		"Meta_Type__game_registry__Item_Info_pair", 
		Meta_Type__game_registry__Item_Info_pair, 
		Meta_Type__game_registry__Item_Info_pair_Members, 
		2},
	Meta_Struct_Info {
		"Item_Stack", 
		"Meta_Type_Item_Stack", 
		Meta_Type_Item_Stack, 
		Meta_Type_Item_Stack_Members, 
		3},
	Meta_Struct_Info {
		"Inventory", 
		"Meta_Type_Inventory", 
		Meta_Type_Inventory, 
		Meta_Type_Inventory_Members, 
		4},
	Meta_Struct_Info {
		"Tilemap", 
		"Meta_Type_Tilemap", 
		Meta_Type_Tilemap, 
		Meta_Type_Tilemap_Members, 
		4},
	Meta_Struct_Info {
		"Sim_Body", 
		"Meta_Type_Sim_Body", 
		Meta_Type_Sim_Body, 
		Meta_Type_Sim_Body_Members, 
		13},
	Meta_Struct_Info {
		"Sim_Contact", 
		"Meta_Type_Sim_Contact", 
		Meta_Type_Sim_Contact, 
		Meta_Type_Sim_Contact_Members, 
		6},
	Meta_Struct_Info {
		"Sim_Grid_Cell", 
		"Meta_Type_Sim_Grid_Cell", 
		Meta_Type_Sim_Grid_Cell, 
		Meta_Type_Sim_Grid_Cell_Members, 
		2},
	Meta_Struct_Info {
		"Sim_Static_Grid", 
		"Meta_Type_Sim_Static_Grid", 
		Meta_Type_Sim_Static_Grid, 
		Meta_Type_Sim_Static_Grid_Members, 
		6},
	Meta_Struct_Info {
		"Simulator", 
		"Meta_Type_Simulator", 
		Meta_Type_Simulator, 
		Meta_Type_Simulator_Members, 
		12},
	Meta_Struct_Info {
		"Particle", 
		"Meta_Type_Particle", 
		Meta_Type_Particle, 
		Meta_Type_Particle_Members, 
		9},
	Meta_Struct_Info {
		"Particle_Style", 
		"Meta_Type_Particle_Style", 
		Meta_Type_Particle_Style, 
		Meta_Type_Particle_Style_Members, 
		26},
	Meta_Struct_Info {
		"Emitter", 
		"Meta_Type_Emitter", 
		Meta_Type_Emitter, 
		Meta_Type_Emitter_Members, 
		6},
	Meta_Struct_Info {
		"Hitbox_Contact", 
		"Meta_Type_Hitbox_Contact", 
		Meta_Type_Hitbox_Contact, 
		Meta_Type_Hitbox_Contact_Members, 
		4},
	Meta_Struct_Info {
		"Hitbox", 
		"Meta_Type_Hitbox", 
		Meta_Type_Hitbox, 
		Meta_Type_Hitbox_Members, 
		5},
	Meta_Struct_Info {
		"Entity", 
		"Meta_Type_Entity", 
		Meta_Type_Entity, 
		Meta_Type_Entity_Members, 
		21},
	Meta_Struct_Info {
		"Area_Link", 
		"Meta_Type_Area_Link", 
		Meta_Type_Area_Link, 
		Meta_Type_Area_Link_Members, 
		2},
	Meta_Struct_Info {
		"World_Area_Stub", 
		"Meta_Type_World_Area_Stub", 
		Meta_Type_World_Area_Stub, 
		Meta_Type_World_Area_Stub_Members, 
		7},
	Meta_Struct_Info {
		"World_Area", 
		"Meta_Type_World_Area", 
		Meta_Type_World_Area, 
		Meta_Type_World_Area_Members, 
		23},
	Meta_Struct_Info {
		"World", 
		"Meta_Type_World", 
		Meta_Type_World, 
		Meta_Type_World_Members, 
		15},
};


static inline const Meta_Struct_Info* get_struct_info(Meta_Type t) 
{
	return All_Meta_Struct_Info + t;
}
