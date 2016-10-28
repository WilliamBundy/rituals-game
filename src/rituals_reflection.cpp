enum Meta_Type
{
	Meta_Type_Vec2,
	Meta_Type_Vec2_struct0,
	Meta_Type_Vec3,
	Meta_Type_Vec3_struct0,
	Meta_Type_Vec4,
	Meta_Type_Vec4_struct0,
	Meta_Type_Vec2i,
	Meta_Type_Vec2i_struct0,
	Meta_Type_Vec3i,
	Meta_Type_Vec3i_struct0,
	Meta_Type_Vec4i,
	Meta_Type_Vec4i_struct0,
	Meta_Type_Rect2,
	Meta_Type_Rect2_struct0,
	Meta_Type_Rect2_struct1,
	Meta_Type_Rect2_Clip_Info,
	Meta_Type_Rect2i,
	Meta_Type_AABB,
	Meta_Type_AABB_struct0,
	Meta_Type_AABB_struct0_union0,
	Meta_Type_AABB_struct0_union0_struct0,
	Meta_Type_AABB_struct0_union0_struct1,
	Meta_Type_Memory_Arena,
	Meta_Type_Random,
	Meta_Type_Game_Input,
	Meta_Type_Game_Main,
	Meta_Type_Platform,
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
	Meta_Type_None_State,
	Meta_Type_Menu_State,
	Meta_Type_Play_State,
	Meta_Type_Tile_Info,
	Meta_Type_Tile_State,
	Meta_Type_Item_Info,
	Meta_Type_Rituals_Bat_Userdata,
	Meta_Type_Rituals_Snake_Userdata,
	Meta_Type_Rituals_Goblin_Knight_Userdata,
	Meta_Type_Rituals_Enemy_Userdata,
	Meta_Type_Rituals_Enemy_Userdata_union0,
	Meta_Type_Rituals_Player_Userdata,
	Meta_Type_Rituals_Prop_Userdata,
	Meta_Type_Rituals_Item_Pickup_Userdata,
	Meta_Type_Rituals_Health_Pickup_Userdata,
	Meta_Type_Rituals_Pickup_Userdata,
	Meta_Type_Unknown_Userdata,
	Meta_Type_Rituals_Entity_Userdata,
	Meta_Type_Game_Registry,
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
	Meta_Type_integer,
	Meta_Type_uint8,
	Meta_Type_isize,
	Meta_Type_uint64,
	Meta_Type_int8,
	Meta_Type_int32,
	Meta_Type_bool,
	Meta_Type_char,
	Meta_Type_SDL_Window,
	Meta_Type_Game_State,
	Meta_Type_uint32,
	Meta_Type_GLuint,
	Meta_Type_real32,
	Meta_Type_tinydir_dir,
	Meta_Type_usize,
	Meta_Type_Rituals_Entity_Kinds,
	Meta_Type_void,
	Meta_Type_Tile,
	Meta_Type_Direction,
	Meta_Type_World_Area_Biome,
};
const char* Meta_Type_Names[] = {
	"Vec2",
	"Vec2_struct0",
	"Vec3",
	"Vec3_struct0",
	"Vec4",
	"Vec4_struct0",
	"Vec2i",
	"Vec2i_struct0",
	"Vec3i",
	"Vec3i_struct0",
	"Vec4i",
	"Vec4i_struct0",
	"Rect2",
	"Rect2_struct0",
	"Rect2_struct1",
	"Rect2_Clip_Info",
	"Rect2i",
	"AABB",
	"AABB_struct0",
	"AABB_struct0_union0",
	"AABB_struct0_union0_struct0",
	"AABB_struct0_union0_struct1",
	"Memory_Arena",
	"Random",
	"Game_Input",
	"Game_Main",
	"Platform",
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
	"None_State",
	"Menu_State",
	"Play_State",
	"Tile_Info",
	"Tile_State",
	"Item_Info",
	"Rituals_Bat_Userdata",
	"Rituals_Snake_Userdata",
	"Rituals_Goblin_Knight_Userdata",
	"Rituals_Enemy_Userdata",
	"Rituals_Enemy_Userdata_union0",
	"Rituals_Player_Userdata",
	"Rituals_Prop_Userdata",
	"Rituals_Item_Pickup_Userdata",
	"Rituals_Health_Pickup_Userdata",
	"Rituals_Pickup_Userdata",
	"Unknown_Userdata",
	"Rituals_Entity_Userdata",
	"Game_Registry",
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
	"integer",
	"uint8",
	"isize",
	"uint64",
	"int8",
	"int32",
	"bool",
	"char",
	"SDL_Window",
	"Game_State",
	"uint32",
	"GLuint",
	"real32",
	"tinydir_dir",
	"usize",
	"Rituals_Entity_Kinds",
	"void",
	"Tile",
	"Direction",
	"World_Area_Biome",
};

struct _empty
{
	int null;
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
	Meta_Member{0, Meta_Type_Vec2_struct0, 0, "", offsetof(_empty, null)},
	Meta_Member{16, Meta_Type_real, 1, "e", offsetof(Vec2, e)}
};

const Meta_Member Meta_Type_Vec2_struct0_Members[] = {
	Meta_Member{0, Meta_Type_real, 0, "x", offsetof(Vec2, x)},
	Meta_Member{0, Meta_Type_real, 0, "y", offsetof(Vec2, y)}
};

const Meta_Member Meta_Type_Vec3_Members[] = {
	Meta_Member{0, Meta_Type_Vec3_struct0, 0, "", offsetof(_empty, null)},
	Meta_Member{16, Meta_Type_real, 1, "e", offsetof(Vec3, e)}
};

const Meta_Member Meta_Type_Vec3_struct0_Members[] = {
	Meta_Member{0, Meta_Type_real, 0, "x", offsetof(Vec3, x)},
	Meta_Member{0, Meta_Type_real, 0, "y", offsetof(Vec3, y)},
	Meta_Member{0, Meta_Type_real, 0, "z", offsetof(Vec3, z)}
};

const Meta_Member Meta_Type_Vec4_Members[] = {
	Meta_Member{0, Meta_Type_Vec4_struct0, 0, "", offsetof(_empty, null)},
	Meta_Member{16, Meta_Type_real, 1, "e", offsetof(Vec4, e)}
};

const Meta_Member Meta_Type_Vec4_struct0_Members[] = {
	Meta_Member{0, Meta_Type_real, 0, "x", offsetof(Vec4, x)},
	Meta_Member{0, Meta_Type_real, 0, "y", offsetof(Vec4, y)},
	Meta_Member{0, Meta_Type_real, 0, "z", offsetof(Vec4, z)},
	Meta_Member{0, Meta_Type_real, 0, "w", offsetof(Vec4, w)}
};

const Meta_Member Meta_Type_Vec2i_Members[] = {
	Meta_Member{0, Meta_Type_Vec2i_struct0, 0, "", offsetof(_empty, null)},
	Meta_Member{16, Meta_Type_integer, 1, "e", offsetof(Vec2i, e)}
};

const Meta_Member Meta_Type_Vec2i_struct0_Members[] = {
	Meta_Member{0, Meta_Type_integer, 0, "x", offsetof(Vec2i, x)},
	Meta_Member{0, Meta_Type_integer, 0, "y", offsetof(Vec2i, y)}
};

const Meta_Member Meta_Type_Vec3i_Members[] = {
	Meta_Member{0, Meta_Type_Vec3i_struct0, 0, "", offsetof(_empty, null)},
	Meta_Member{16, Meta_Type_integer, 1, "e", offsetof(Vec3i, e)}
};

const Meta_Member Meta_Type_Vec3i_struct0_Members[] = {
	Meta_Member{0, Meta_Type_integer, 0, "x", offsetof(Vec3i, x)},
	Meta_Member{0, Meta_Type_integer, 0, "y", offsetof(Vec3i, y)},
	Meta_Member{0, Meta_Type_integer, 0, "z", offsetof(Vec3i, z)}
};

const Meta_Member Meta_Type_Vec4i_Members[] = {
	Meta_Member{0, Meta_Type_Vec4i_struct0, 0, "", offsetof(_empty, null)},
	Meta_Member{16, Meta_Type_integer, 1, "e", offsetof(Vec4i, e)}
};

const Meta_Member Meta_Type_Vec4i_struct0_Members[] = {
	Meta_Member{0, Meta_Type_integer, 0, "x", offsetof(Vec4i, x)},
	Meta_Member{0, Meta_Type_integer, 0, "y", offsetof(Vec4i, y)},
	Meta_Member{0, Meta_Type_integer, 0, "z", offsetof(Vec4i, z)},
	Meta_Member{0, Meta_Type_integer, 0, "w", offsetof(Vec4i, w)}
};

const Meta_Member Meta_Type_Rect2_Members[] = {
	Meta_Member{0, Meta_Type_Rect2_struct0, 0, "", offsetof(_empty, null)},
	Meta_Member{0, Meta_Type_Rect2_struct1, 0, "", offsetof(_empty, null)},
	Meta_Member{16, Meta_Type_real, 1, "e", offsetof(Rect2, e)}
};

const Meta_Member Meta_Type_Rect2_struct0_Members[] = {
	Meta_Member{0, Meta_Type_real, 0, "x", offsetof(Rect2, x)},
	Meta_Member{0, Meta_Type_real, 0, "y", offsetof(Rect2, y)},
	Meta_Member{0, Meta_Type_real, 0, "w", offsetof(Rect2, w)},
	Meta_Member{0, Meta_Type_real, 0, "h", offsetof(Rect2, h)}
};

const Meta_Member Meta_Type_Rect2_struct1_Members[] = {
	Meta_Member{0, Meta_Type_Vec2, 0, "position", offsetof(Rect2, position)},
	Meta_Member{0, Meta_Type_Vec2, 0, "size", offsetof(Rect2, size)}
};

const Meta_Member Meta_Type_Rect2_Clip_Info_Members[] = {
	Meta_Member{0, Meta_Type_Vec2, 0, "rp1", offsetof(Rect2_Clip_Info, rp1)},
	Meta_Member{0, Meta_Type_Vec2, 0, "rp2", offsetof(Rect2_Clip_Info, rp2)},
	Meta_Member{0, Meta_Type_Vec2, 0, "diff1", offsetof(Rect2_Clip_Info, diff1)},
	Meta_Member{0, Meta_Type_Vec2, 0, "diff2", offsetof(Rect2_Clip_Info, diff2)}
};

const Meta_Member Meta_Type_Rect2i_Members[] = {
	Meta_Member{0, Meta_Type_integer, 0, "x", offsetof(Rect2i, x)},
	Meta_Member{0, Meta_Type_integer, 0, "y", offsetof(Rect2i, y)},
	Meta_Member{0, Meta_Type_integer, 0, "w", offsetof(Rect2i, w)},
	Meta_Member{0, Meta_Type_integer, 0, "h", offsetof(Rect2i, h)}
};

const Meta_Member Meta_Type_AABB_Members[] = {
	Meta_Member{0, Meta_Type_AABB_struct0, 0, "", offsetof(_empty, null)},
	Meta_Member{16, Meta_Type_real, 1, "e", offsetof(AABB, e)}
};

const Meta_Member Meta_Type_AABB_struct0_Members[] = {
	Meta_Member{0, Meta_Type_Vec2, 0, "center", offsetof(AABB, center)},
	Meta_Member{0, Meta_Type_AABB_struct0_union0, 0, "", offsetof(_empty, null)}
};

const Meta_Member Meta_Type_AABB_struct0_union0_Members[] = {
	Meta_Member{0, Meta_Type_AABB_struct0_union0_struct0, 0, "", offsetof(_empty, null)},
	Meta_Member{0, Meta_Type_AABB_struct0_union0_struct1, 0, "", offsetof(_empty, null)}
};

const Meta_Member Meta_Type_AABB_struct0_union0_struct0_Members[] = {
	Meta_Member{0, Meta_Type_real, 0, "hw", offsetof(AABB, hw)},
	Meta_Member{0, Meta_Type_real, 0, "hh", offsetof(AABB, hh)}
};

const Meta_Member Meta_Type_AABB_struct0_union0_struct1_Members[] = {
	Meta_Member{0, Meta_Type_Vec2, 0, "hext", offsetof(AABB, hext)}
};

const Meta_Member Meta_Type_Memory_Arena_Members[] = {
	Meta_Member{1, Meta_Type_uint8, 1, "data", offsetof(Memory_Arena, data)},
	Meta_Member{0, Meta_Type_isize, 0, "capacity", offsetof(Memory_Arena, capacity)},
	Meta_Member{0, Meta_Type_isize, 0, "head", offsetof(Memory_Arena, head)},
	Meta_Member{0, Meta_Type_isize, 0, "temp_head", offsetof(Memory_Arena, temp_head)},
	Meta_Member{1, Meta_Type_Memory_Arena, 1, "next", offsetof(Memory_Arena, next)}
};

const Meta_Member Meta_Type_Random_Members[] = {
	Meta_Member{0, Meta_Type_uint64, 0, "x", offsetof(Random, x)},
	Meta_Member{0, Meta_Type_uint64, 0, "y", offsetof(Random, y)}
};

const Meta_Member Meta_Type_Game_Input_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "num_keys_down", offsetof(Game_Input, num_keys_down)},
	Meta_Member{0, Meta_Type_isize, 0, "num_mouse_down", offsetof(Game_Input, num_mouse_down)},
	Meta_Member{1, Meta_Type_int8, 1, "scancodes", offsetof(Game_Input, scancodes)},
	Meta_Member{1, Meta_Type_int8, 1, "keycodes", offsetof(Game_Input, keycodes)},
	Meta_Member{1, Meta_Type_int8, 1, "mouse", offsetof(Game_Input, mouse)},
	Meta_Member{0, Meta_Type_int32, 0, "mouse_x", offsetof(Game_Input, mouse_x)},
	Meta_Member{0, Meta_Type_int32, 0, "mouse_y", offsetof(Game_Input, mouse_y)},
	Meta_Member{0, Meta_Type_Vec2i, 0, "screen_mouse_pos", offsetof(Game_Input, screen_mouse_pos)},
	Meta_Member{0, Meta_Type_Vec2, 0, "mouse_pos", offsetof(Game_Input, mouse_pos)},
	Meta_Member{0, Meta_Type_bool, 0, "capture_tabs", offsetof(Game_Input, capture_tabs)},
	Meta_Member{0, Meta_Type_bool, 0, "capture_newlines", offsetof(Game_Input, capture_newlines)},
	Meta_Member{1, Meta_Type_char, 1, "text", offsetof(Game_Input, text)},
	Meta_Member{0, Meta_Type_isize, 0, "text_count", offsetof(Game_Input, text_count)}
};

const Meta_Member Meta_Type_Game_Main_Members[] = {
	Meta_Member{1, Meta_Type_SDL_Window, 1, "window", offsetof(Game_Main, window)},
	Meta_Member{1, Meta_Type_OpenGL_Renderer, 1, "renderer", offsetof(Game_Main, renderer)},
	Meta_Member{0, Meta_Type_Game_State, 0, "state", offsetof(Game_Main, state)},
	Meta_Member{0, Meta_Type_real, 0, "scale", offsetof(Game_Main, scale)},
	Meta_Member{0, Meta_Type_Vec2, 0, "size", offsetof(Game_Main, size)},
	Meta_Member{0, Meta_Type_Vec2i, 0, "window_size", offsetof(Game_Main, window_size)},
	Meta_Member{1, Meta_Type_Memory_Arena, 1, "meta_arena", offsetof(Game_Main, meta_arena)},
	Meta_Member{1, Meta_Type_Memory_Arena, 1, "asset_arena", offsetof(Game_Main, asset_arena)},
	Meta_Member{1, Meta_Type_Memory_Arena, 1, "game_arena", offsetof(Game_Main, game_arena)},
	Meta_Member{1, Meta_Type_Memory_Arena, 1, "temp_arena", offsetof(Game_Main, temp_arena)},
	Meta_Member{1, Meta_Type_Memory_Arena, 1, "renderer_arena", offsetof(Game_Main, renderer_arena)},
	Meta_Member{1, Meta_Type_Memory_Arena, 1, "registry_arena", offsetof(Game_Main, registry_arena)},
	Meta_Member{1, Meta_Type_Memory_Arena, 1, "world_arena", offsetof(Game_Main, world_arena)},
	Meta_Member{1, Meta_Type_Memory_Arena, 1, "play_arena", offsetof(Game_Main, play_arena)},
	Meta_Member{9, Meta_Type_char, 1, "base_path", offsetof(Game_Main, base_path)},
	Meta_Member{0, Meta_Type_isize, 0, "base_path_length", offsetof(Game_Main, base_path_length)},
	Meta_Member{1, Meta_Type_Spritefont, 1, "body_font", offsetof(Game_Main, body_font)},
	Meta_Member{1, Meta_Type_Spritefont, 1, "title_font", offsetof(Game_Main, title_font)},
	Meta_Member{0, Meta_Type_Random, 0, "r", offsetof(Game_Main, r)},
	Meta_Member{1, Meta_Type_Game_Registry, 1, "registry", offsetof(Game_Main, registry)},
	Meta_Member{1, Meta_Type_Game_Input, 1, "input", offsetof(Game_Main, input)},
	Meta_Member{0, Meta_Type_uint64, 0, "last_frame_time", offsetof(Game_Main, last_frame_time)},
	Meta_Member{1, Meta_Type_None_State, 1, "None", offsetof(Game_Main, None)},
	Meta_Member{1, Meta_Type_Menu_State, 1, "Menu", offsetof(Game_Main, Menu)},
	Meta_Member{1, Meta_Type_Play_State, 1, "Play", offsetof(Game_Main, Play)}
};

const Meta_Member Meta_Type_Platform_Members[] = {
	Meta_Member{9, Meta_Type_char, 1, "window_title", offsetof(Platform, window_title)},
	Meta_Member{0, Meta_Type_Vec2i, 0, "window_size", offsetof(Platform, window_size)},
	Meta_Member{0, Meta_Type_real, 0, "game_scale", offsetof(Platform, game_scale)},
	Meta_Member{0, Meta_Type_int32, 0, "display_index", offsetof(Platform, display_index)},
	Meta_Member{0, Meta_Type_bool, 0, "check_gl_attributes", offsetof(Platform, check_gl_attributes)},
	Meta_Member{9, Meta_Type_char, 1, "vertex_shader", offsetof(Platform, vertex_shader)},
	Meta_Member{9, Meta_Type_char, 1, "frag_shader", offsetof(Platform, frag_shader)},
	Meta_Member{1, Meta_Type_SDL_Window, 1, "window", offsetof(Platform, window)}
};

const Meta_Member Meta_Type_Sprite_Members[] = {
	Meta_Member{0, Meta_Type_Vec2, 0, "position", offsetof(Sprite, position)},
	Meta_Member{0, Meta_Type_Vec2, 0, "center", offsetof(Sprite, center)},
	Meta_Member{0, Meta_Type_real, 0, "angle", offsetof(Sprite, angle)},
	Meta_Member{0, Meta_Type_Vec2, 0, "size", offsetof(Sprite, size)},
	Meta_Member{0, Meta_Type_Rect2, 0, "texture", offsetof(Sprite, texture)},
	Meta_Member{0, Meta_Type_Vec4, 0, "color", offsetof(Sprite, color)},
	Meta_Member{0, Meta_Type_uint32, 0, "flags", offsetof(Sprite, flags)},
	Meta_Member{0, Meta_Type_real, 0, "sort_offset", offsetof(Sprite, sort_offset)}
};

const Meta_Member Meta_Type_Sprite4_Members[] = {
	Meta_Member{16, Meta_Type_Sprite, 1, "e", offsetof(Sprite4, e)}
};

const Meta_Member Meta_Type_Render_Group_Members[] = {
	Meta_Member{0, Meta_Type_GLuint, 0, "texture", offsetof(Render_Group, texture)},
	Meta_Member{0, Meta_Type_Vec2i, 0, "texture_size", offsetof(Render_Group, texture_size)},
	Meta_Member{0, Meta_Type_Vec2, 0, "offset", offsetof(Render_Group, offset)},
	Meta_Member{0, Meta_Type_Rect2, 0, "clip", offsetof(Render_Group, clip)},
	Meta_Member{16, Meta_Type_real32, 1, "ortho", offsetof(Render_Group, ortho)},
	Meta_Member{0, Meta_Type_real, 0, "night_amount", offsetof(Render_Group, night_amount)},
	Meta_Member{0, Meta_Type_real, 0, "night_cutoff", offsetof(Render_Group, night_cutoff)},
	Meta_Member{1, Meta_Type_Sprite, 1, "sprites", offsetof(Render_Group, sprites)},
	Meta_Member{0, Meta_Type_isize, 0, "sprites_count", offsetof(Render_Group, sprites_count)},
	Meta_Member{0, Meta_Type_isize, 0, "sprites_capacity", offsetof(Render_Group, sprites_capacity)}
};

const Meta_Member Meta_Type_OpenGL_Renderer_Members[] = {
	Meta_Member{0, Meta_Type_GLuint, 0, "shader_program", offsetof(OpenGL_Renderer, shader_program)},
	Meta_Member{0, Meta_Type_GLuint, 0, "vbo", offsetof(OpenGL_Renderer, vbo)},
	Meta_Member{0, Meta_Type_GLuint, 0, "vao", offsetof(OpenGL_Renderer, vao)},
	Meta_Member{0, Meta_Type_isize, 0, "u_texturesize", offsetof(OpenGL_Renderer, u_texturesize)},
	Meta_Member{0, Meta_Type_isize, 0, "u_orthomat", offsetof(OpenGL_Renderer, u_orthomat)},
	Meta_Member{0, Meta_Type_isize, 0, "u_night_amount", offsetof(OpenGL_Renderer, u_night_amount)},
	Meta_Member{0, Meta_Type_isize, 0, "u_night_cutoff", offsetof(OpenGL_Renderer, u_night_cutoff)},
	Meta_Member{1, Meta_Type_Render_Group, 1, "groups", offsetof(OpenGL_Renderer, groups)},
	Meta_Member{0, Meta_Type_isize, 0, "groups_count", offsetof(OpenGL_Renderer, groups_count)}
};

const Meta_Member Meta_Type_Animation_Frame_Members[] = {
	Meta_Member{0, Meta_Type_Vec3, 0, "position", offsetof(Animation_Frame, position)},
	Meta_Member{0, Meta_Type_real, 0, "angle", offsetof(Animation_Frame, angle)},
	Meta_Member{0, Meta_Type_Vec4, 0, "color", offsetof(Animation_Frame, color)},
	Meta_Member{0, Meta_Type_Vec2, 0, "size", offsetof(Animation_Frame, size)},
	Meta_Member{0, Meta_Type_Rect2, 0, "texture", offsetof(Animation_Frame, texture)},
	Meta_Member{0, Meta_Type_real, 0, "sort_offset", offsetof(Animation_Frame, sort_offset)}
};

const Meta_Member Meta_Type_Animation_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "id", offsetof(Animation, id)},
	Meta_Member{1, Meta_Type_Animation_Frame, 1, "frames", offsetof(Animation, frames)},
	Meta_Member{0, Meta_Type_isize, 0, "frames_count", offsetof(Animation, frames_count)},
	Meta_Member{0, Meta_Type_isize, 0, "frames_capacity", offsetof(Animation, frames_capacity)},
	Meta_Member{0, Meta_Type_real, 0, "fps", offsetof(Animation, fps)},
	Meta_Member{0, Meta_Type_real, 0, "inv_fps", offsetof(Animation, inv_fps)},
	Meta_Member{0, Meta_Type_bool, 0, "looping", offsetof(Animation, looping)}
};

const Meta_Member Meta_Type_Animated_Sprite_Members[] = {
	Meta_Member{1, Meta_Type_Animation, 2, "animations", offsetof(Animated_Sprite, animations)},
	Meta_Member{0, Meta_Type_isize, 0, "animations_count", offsetof(Animated_Sprite, animations_count)},
	Meta_Member{0, Meta_Type_isize, 0, "animations_capacity", offsetof(Animated_Sprite, animations_capacity)},
	Meta_Member{0, Meta_Type_isize, 0, "current_animation", offsetof(Animated_Sprite, current_animation)},
	Meta_Member{0, Meta_Type_isize, 0, "current_frame", offsetof(Animated_Sprite, current_frame)},
	Meta_Member{0, Meta_Type_bool, 0, "running", offsetof(Animated_Sprite, running)},
	Meta_Member{0, Meta_Type_real, 0, "timer", offsetof(Animated_Sprite, timer)}
};

const Meta_Member Meta_Type_Spritefont_Members[] = {
	Meta_Member{0, Meta_Type_int32, 0, "line_padding", offsetof(Spritefont, line_padding)},
	Meta_Member{0, Meta_Type_int32, 0, "character_padding", offsetof(Spritefont, character_padding)},
	Meta_Member{0, Meta_Type_int32, 0, "tab_size", offsetof(Spritefont, tab_size)},
	Meta_Member{0, Meta_Type_int32, 0, "glyph_width", offsetof(Spritefont, glyph_width)},
	Meta_Member{0, Meta_Type_int32, 0, "glyph_height", offsetof(Spritefont, glyph_height)},
	Meta_Member{9, Meta_Type_Rect2, 1, "glyphs", offsetof(Spritefont, glyphs)},
	Meta_Member{0, Meta_Type_Vec4, 0, "color", offsetof(Spritefont, color)}
};

const Meta_Member Meta_Type_Gui_Window_Handle_Members[] = {
	Meta_Member{0, Meta_Type_Vec2, 0, "position", offsetof(Gui_Window_Handle, position)},
	Meta_Member{1, Meta_Type_char, 1, "title", offsetof(Gui_Window_Handle, title)},
	Meta_Member{0, Meta_Type_int32, 0, "z", offsetof(Gui_Window_Handle, z)}
};

const Meta_Member Meta_Type_Gui_Text_Input_Handle_Members[] = {
	Meta_Member{1, Meta_Type_char, 1, "buffer", offsetof(Gui_Text_Input_Handle, buffer)},
	Meta_Member{0, Meta_Type_isize, 0, "buffer_length", offsetof(Gui_Text_Input_Handle, buffer_length)},
	Meta_Member{0, Meta_Type_isize, 0, "buffer_capacity", offsetof(Gui_Text_Input_Handle, buffer_capacity)},
	Meta_Member{0, Meta_Type_isize, 0, "max_chars_by_width", offsetof(Gui_Text_Input_Handle, max_chars_by_width)},
	Meta_Member{0, Meta_Type_isize, 0, "cursor", offsetof(Gui_Text_Input_Handle, cursor)},
	Meta_Member{0, Meta_Type_isize, 0, "selection_start", offsetof(Gui_Text_Input_Handle, selection_start)},
	Meta_Member{0, Meta_Type_bool, 0, "active", offsetof(Gui_Text_Input_Handle, active)},
	Meta_Member{0, Meta_Type_bool, 0, "accept_newlines", offsetof(Gui_Text_Input_Handle, accept_newlines)},
	Meta_Member{0, Meta_Type_bool, 0, "accept_tabs", offsetof(Gui_Text_Input_Handle, accept_tabs)}
};

const Meta_Member Meta_Type_Gui_Context_Members[] = {
	Meta_Member{0, Meta_Type_Vec2, 0, "last_pos", offsetof(Gui_Context, last_pos)},
	Meta_Member{0, Meta_Type_Vec2, 0, "next_pos", offsetof(Gui_Context, next_pos)},
	Meta_Member{0, Meta_Type_isize, 0, "axis", offsetof(Gui_Context, axis)}
};

const Meta_Member Meta_Type_None_State_Members[] = {
	Meta_Member{0, Meta_Type_int32, 0, "error", offsetof(None_State, error)}
};

const Meta_Member Meta_Type_Menu_State_Members[] = {
	Meta_Member{0, Meta_Type_int32, 0, "error", offsetof(Menu_State, error)},
	Meta_Member{16, Meta_Type_char, 1, "save_dir", offsetof(Menu_State, save_dir)},
	Meta_Member{0, Meta_Type_isize, 0, "save_dir_len", offsetof(Menu_State, save_dir_len)},
	Meta_Member{0, Meta_Type_tinydir_dir, 0, "saves", offsetof(Menu_State, saves)},
	Meta_Member{0, Meta_Type_bool, 0, "saves_dirty", offsetof(Menu_State, saves_dirty)},
	Meta_Member{0, Meta_Type_Gui_Text_Input_Handle, 0, "handle", offsetof(Menu_State, handle)},
	Meta_Member{0, Meta_Type_isize, 0, "delete_index", offsetof(Menu_State, delete_index)}
};

const Meta_Member Meta_Type_Play_State_Members[] = {
	Meta_Member{0, Meta_Type_int32, 0, "error", offsetof(Play_State, error)},
	Meta_Member{0, Meta_Type_usize, 0, "current_time", offsetof(Play_State, current_time)},
	Meta_Member{0, Meta_Type_usize, 0, "prev_time", offsetof(Play_State, prev_time)},
	Meta_Member{0, Meta_Type_real, 0, "accumulator", offsetof(Play_State, accumulator)},
	Meta_Member{1, Meta_Type_World, 1, "world", offsetof(Play_State, world)},
	Meta_Member{0, Meta_Type_bool, 0, "running", offsetof(Play_State, running)},
	Meta_Member{0, Meta_Type_Vec2i, 0, "world_xy", offsetof(Play_State, world_xy)},
	Meta_Member{0, Meta_Type_bool, 0, "delete_world_on_stop", offsetof(Play_State, delete_world_on_stop)},
	Meta_Member{0, Meta_Type_bool, 0, "save_world_on_stop", offsetof(Play_State, save_world_on_stop)},
	Meta_Member{9, Meta_Type_char, 1, "world_name_in", offsetof(Play_State, world_name_in)}
};

const Meta_Member Meta_Type_Tile_Info_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "id", offsetof(Tile_Info, id)},
	Meta_Member{0, Meta_Type_usize, 0, "hash", offsetof(Tile_Info, hash)},
	Meta_Member{0, Meta_Type_Rect2, 0, "texture", offsetof(Tile_Info, texture)},
	Meta_Member{0, Meta_Type_Rect2, 0, "bottom_texture", offsetof(Tile_Info, bottom_texture)},
	Meta_Member{0, Meta_Type_Rect2, 0, "top_texture", offsetof(Tile_Info, top_texture)},
	Meta_Member{0, Meta_Type_real, 0, "movement_modifier", offsetof(Tile_Info, movement_modifier)},
	Meta_Member{0, Meta_Type_real, 0, "friction", offsetof(Tile_Info, friction)},
	Meta_Member{9, Meta_Type_char, 1, "name", offsetof(Tile_Info, name)},
	Meta_Member{0, Meta_Type_int32, 0, "max_damage", offsetof(Tile_Info, max_damage)},
	Meta_Member{0, Meta_Type_bool, 0, "immune_to_damage", offsetof(Tile_Info, immune_to_damage)},
	Meta_Member{1, Meta_Type_char, 1, "break_to_name", offsetof(Tile_Info, break_to_name)},
	Meta_Member{0, Meta_Type_isize, 0, "break_to_id", offsetof(Tile_Info, break_to_id)},
	Meta_Member{0, Meta_Type_bool, 0, "solid", offsetof(Tile_Info, solid)},
	Meta_Member{0, Meta_Type_uint64, 0, "body_mask", offsetof(Tile_Info, body_mask)},
	Meta_Member{0, Meta_Type_uint64, 0, "body_group", offsetof(Tile_Info, body_group)},
	Meta_Member{0, Meta_Type_bool, 0, "has_top_texture", offsetof(Tile_Info, has_top_texture)},
	Meta_Member{0, Meta_Type_bool, 0, "has_bottom_texture", offsetof(Tile_Info, has_bottom_texture)}
};

const Meta_Member Meta_Type_Tile_State_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "id", offsetof(Tile_State, id)},
	Meta_Member{0, Meta_Type_int32, 0, "damage", offsetof(Tile_State, damage)}
};

const Meta_Member Meta_Type_Item_Info_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "id", offsetof(Item_Info, id)},
	Meta_Member{0, Meta_Type_usize, 0, "hash", offsetof(Item_Info, hash)},
	Meta_Member{9, Meta_Type_char, 1, "name", offsetof(Item_Info, name)},
	Meta_Member{0, Meta_Type_int32, 0, "max_stack", offsetof(Item_Info, max_stack)},
	Meta_Member{0, Meta_Type_Rect2, 0, "texture", offsetof(Item_Info, texture)}
};

const Meta_Member Meta_Type_Rituals_Bat_Userdata_Members[] = {
	Meta_Member{0, Meta_Type_Vec2, 0, "perch", offsetof(Rituals_Bat_Userdata, perch)}
};

const Meta_Member Meta_Type_Rituals_Snake_Userdata_Members[] = {
	Meta_Member{0, Meta_Type_real, 0, "chase_speed_modifier", offsetof(Rituals_Snake_Userdata, chase_speed_modifier)}
};

const Meta_Member Meta_Type_Rituals_Goblin_Knight_Userdata_Members[] = {
	Meta_Member{0, Meta_Type_Vec2, 0, "patrol_start", offsetof(Rituals_Goblin_Knight_Userdata, patrol_start)},
	Meta_Member{0, Meta_Type_Vec2, 0, "patrol_end", offsetof(Rituals_Goblin_Knight_Userdata, patrol_end)}
};

const Meta_Member Meta_Type_Rituals_Enemy_Userdata_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "kind", offsetof(Rituals_Enemy_Userdata, kind)},
	Meta_Member{0, Meta_Type_isize, 0, "mode", offsetof(Rituals_Enemy_Userdata, mode)},
	Meta_Member{0, Meta_Type_real, 0, "speed", offsetof(Rituals_Enemy_Userdata, speed)},
	Meta_Member{0, Meta_Type_real, 0, "alert_dist", offsetof(Rituals_Enemy_Userdata, alert_dist)},
	Meta_Member{0, Meta_Type_real, 0, "follow_dist", offsetof(Rituals_Enemy_Userdata, follow_dist)},
	Meta_Member{0, Meta_Type_Rituals_Enemy_Userdata_union0, 0, "", offsetof(_empty, null)}
};

const Meta_Member Meta_Type_Rituals_Enemy_Userdata_union0_Members[] = {
	Meta_Member{0, Meta_Type_Rituals_Bat_Userdata, 0, "bat", offsetof(Rituals_Enemy_Userdata, bat)},
	Meta_Member{0, Meta_Type_Rituals_Snake_Userdata, 0, "snake", offsetof(Rituals_Enemy_Userdata, snake)},
	Meta_Member{0, Meta_Type_Rituals_Goblin_Knight_Userdata, 0, "goblin_knight", offsetof(Rituals_Enemy_Userdata, goblin_knight)}
};

const Meta_Member Meta_Type_Rituals_Player_Userdata_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "held_entity_id", offsetof(Rituals_Player_Userdata, held_entity_id)},
	Meta_Member{0, Meta_Type_real, 0, "heal_cooldown", offsetof(Rituals_Player_Userdata, heal_cooldown)},
	Meta_Member{0, Meta_Type_real, 0, "heal_timer", offsetof(Rituals_Player_Userdata, heal_timer)},
	Meta_Member{0, Meta_Type_int32, 0, "heal_rate", offsetof(Rituals_Player_Userdata, heal_rate)},
	Meta_Member{0, Meta_Type_int32, 0, "heal_to_interval", offsetof(Rituals_Player_Userdata, heal_to_interval)}
};

const Meta_Member Meta_Type_Rituals_Prop_Userdata_Members[] = {
	Meta_Member{0, Meta_Type_Rituals_Entity_Kinds, 0, "contains", offsetof(Rituals_Prop_Userdata, contains)},
	Meta_Member{0, Meta_Type_isize, 0, "subtype", offsetof(Rituals_Prop_Userdata, subtype)},
	Meta_Member{0, Meta_Type_isize, 0, "amount", offsetof(Rituals_Prop_Userdata, amount)},
	Meta_Member{0, Meta_Type_isize, 0, "quality", offsetof(Rituals_Prop_Userdata, quality)}
};

const Meta_Member Meta_Type_Rituals_Item_Pickup_Userdata_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "id", offsetof(Rituals_Item_Pickup_Userdata, id)},
	Meta_Member{0, Meta_Type_isize, 0, "count", offsetof(Rituals_Item_Pickup_Userdata, count)}
};

const Meta_Member Meta_Type_Rituals_Health_Pickup_Userdata_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "amount", offsetof(Rituals_Health_Pickup_Userdata, amount)}
};

const Meta_Member Meta_Type_Rituals_Pickup_Userdata_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "kind", offsetof(Rituals_Pickup_Userdata, kind)},
	Meta_Member{0, Meta_Type_Rituals_Item_Pickup_Userdata, 0, "item", offsetof(Rituals_Pickup_Userdata, item)},
	Meta_Member{0, Meta_Type_Rituals_Health_Pickup_Userdata, 0, "health", offsetof(Rituals_Pickup_Userdata, health)}
};

const Meta_Member Meta_Type_Unknown_Userdata_Members[] = {
	Meta_Member{1, Meta_Type_void, 1, "ptr", offsetof(Unknown_Userdata, ptr)},
	Meta_Member{0, Meta_Type_isize, 0, "size", offsetof(Unknown_Userdata, size)}
};

const Meta_Member Meta_Type_Rituals_Entity_Userdata_Members[] = {
	Meta_Member{0, Meta_Type_Rituals_Prop_Userdata, 0, "prop", offsetof(Rituals_Entity_Userdata, prop)},
	Meta_Member{0, Meta_Type_Rituals_Player_Userdata, 0, "player", offsetof(Rituals_Entity_Userdata, player)},
	Meta_Member{0, Meta_Type_Rituals_Enemy_Userdata, 0, "enemy", offsetof(Rituals_Entity_Userdata, enemy)},
	Meta_Member{0, Meta_Type_Rituals_Pickup_Userdata, 0, "pickup", offsetof(Rituals_Entity_Userdata, pickup)},
	Meta_Member{0, Meta_Type_Unknown_Userdata, 0, "unknown", offsetof(Rituals_Entity_Userdata, unknown)}
};

const Meta_Member Meta_Type_Game_Registry_Members[] = {
	Meta_Member{1, Meta_Type_Tile_Info, 1, "tiles", offsetof(Game_Registry, tiles)},
	Meta_Member{1, Meta_Type_usize, 1, "tiles_hash", offsetof(Game_Registry, tiles_hash)},
	Meta_Member{0, Meta_Type_isize, 0, "tiles_count", offsetof(Game_Registry, tiles_count)},
	Meta_Member{1, Meta_Type_Item_Info, 1, "items", offsetof(Game_Registry, items)},
	Meta_Member{1, Meta_Type_usize, 1, "items_hash", offsetof(Game_Registry, items_hash)},
	Meta_Member{0, Meta_Type_isize, 0, "items_count", offsetof(Game_Registry, items_count)}
};

const Meta_Member Meta_Type_Tilemap_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "w", offsetof(Tilemap, w)},
	Meta_Member{0, Meta_Type_isize, 0, "h", offsetof(Tilemap, h)},
	Meta_Member{1, Meta_Type_Tile, 1, "tiles", offsetof(Tilemap, tiles)},
	Meta_Member{1, Meta_Type_Tile_State, 1, "states", offsetof(Tilemap, states)}
};

const Meta_Member Meta_Type_Sim_Body_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "id", offsetof(Sim_Body, id)},
	Meta_Member{0, Meta_Type_AABB, 0, "shape", offsetof(Sim_Body, shape)},
	Meta_Member{0, Meta_Type_Vec2, 0, "velocity", offsetof(Sim_Body, velocity)},
	Meta_Member{0, Meta_Type_Vec2, 0, "force", offsetof(Sim_Body, force)},
	Meta_Member{0, Meta_Type_Vec2, 0, "collision_vel", offsetof(Sim_Body, collision_vel)},
	Meta_Member{0, Meta_Type_real, 0, "inv_mass", offsetof(Sim_Body, inv_mass)},
	Meta_Member{0, Meta_Type_real, 0, "restitution", offsetof(Sim_Body, restitution)},
	Meta_Member{0, Meta_Type_real, 0, "damping", offsetof(Sim_Body, damping)},
	Meta_Member{0, Meta_Type_uint64, 0, "flags", offsetof(Sim_Body, flags)},
	Meta_Member{0, Meta_Type_uint64, 0, "group", offsetof(Sim_Body, group)},
	Meta_Member{0, Meta_Type_uint64, 0, "mask", offsetof(Sim_Body, mask)},
	Meta_Member{0, Meta_Type_isize, 0, "entity_id", offsetof(Sim_Body, entity_id)},
	Meta_Member{1, Meta_Type_Entity, 1, "entity", offsetof(Sim_Body, entity)}
};

const Meta_Member Meta_Type_Sim_Contact_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "a_id", offsetof(Sim_Contact, a_id)},
	Meta_Member{0, Meta_Type_isize, 0, "b_id", offsetof(Sim_Contact, b_id)},
	Meta_Member{1, Meta_Type_Sim_Body, 1, "static_b", offsetof(Sim_Contact, static_b)},
	Meta_Member{0, Meta_Type_Vec2, 0, "overlap", offsetof(Sim_Contact, overlap)},
	Meta_Member{0, Meta_Type_real, 0, "mag", offsetof(Sim_Contact, mag)},
	Meta_Member{0, Meta_Type_Vec2, 0, "normal", offsetof(Sim_Contact, normal)}
};

const Meta_Member Meta_Type_Sim_Grid_Cell_Members[] = {
	Meta_Member{1, Meta_Type_Sim_Body, 1, "body", offsetof(Sim_Grid_Cell, body)},
	Meta_Member{1, Meta_Type_Sim_Grid_Cell, 1, "next", offsetof(Sim_Grid_Cell, next)}
};

const Meta_Member Meta_Type_Sim_Static_Grid_Members[] = {
	Meta_Member{1, Meta_Type_Sim_Grid_Cell, 1, "cell_storage", offsetof(Sim_Static_Grid, cell_storage)},
	Meta_Member{0, Meta_Type_isize, 0, "cell_storage_count", offsetof(Sim_Static_Grid, cell_storage_count)},
	Meta_Member{0, Meta_Type_isize, 0, "cell_storage_capacity", offsetof(Sim_Static_Grid, cell_storage_capacity)},
	Meta_Member{1, Meta_Type_Sim_Grid_Cell, 2, "cells", offsetof(Sim_Static_Grid, cells)},
	Meta_Member{0, Meta_Type_isize, 0, "cells_length", offsetof(Sim_Static_Grid, cells_length)},
	Meta_Member{0, Meta_Type_Vec2i, 0, "size", offsetof(Sim_Static_Grid, size)}
};

const Meta_Member Meta_Type_Simulator_Members[] = {
	Meta_Member{1, Meta_Type_Sim_Body, 1, "static_bodies", offsetof(Simulator, static_bodies)},
	Meta_Member{0, Meta_Type_isize, 0, "static_bodies_count", offsetof(Simulator, static_bodies_count)},
	Meta_Member{0, Meta_Type_isize, 0, "static_bodies_capacity", offsetof(Simulator, static_bodies_capacity)},
	Meta_Member{1, Meta_Type_Sim_Static_Grid, 1, "grid", offsetof(Simulator, grid)},
	Meta_Member{1, Meta_Type_Sim_Body, 1, "bodies", offsetof(Simulator, bodies)},
	Meta_Member{0, Meta_Type_isize, 0, "bodies_count", offsetof(Simulator, bodies_count)},
	Meta_Member{0, Meta_Type_isize, 0, "bodies_capacity", offsetof(Simulator, bodies_capacity)},
	Meta_Member{0, Meta_Type_isize, 0, "next_body_id", offsetof(Simulator, next_body_id)},
	Meta_Member{1, Meta_Type_Sim_Contact, 1, "contacts", offsetof(Simulator, contacts)},
	Meta_Member{0, Meta_Type_isize, 0, "contacts_count", offsetof(Simulator, contacts_count)},
	Meta_Member{0, Meta_Type_isize, 0, "contacts_capacity", offsetof(Simulator, contacts_capacity)},
	Meta_Member{0, Meta_Type_isize, 0, "sort_axis", offsetof(Simulator, sort_axis)}
};

const Meta_Member Meta_Type_Particle_Members[] = {
	Meta_Member{0, Meta_Type_Vec3, 0, "position", offsetof(Particle, position)},
	Meta_Member{0, Meta_Type_Vec3, 0, "velocity", offsetof(Particle, velocity)},
	Meta_Member{0, Meta_Type_real, 0, "scale", offsetof(Particle, scale)},
	Meta_Member{0, Meta_Type_real, 0, "angle", offsetof(Particle, angle)},
	Meta_Member{0, Meta_Type_real, 0, "angular_vel", offsetof(Particle, angular_vel)},
	Meta_Member{0, Meta_Type_int32, 0, "frame", offsetof(Particle, frame)},
	Meta_Member{0, Meta_Type_int32, 0, "time", offsetof(Particle, time)},
	Meta_Member{0, Meta_Type_int32, 0, "total_time", offsetof(Particle, total_time)},
	Meta_Member{0, Meta_Type_int32, 0, "style_index", offsetof(Particle, style_index)}
};

const Meta_Member Meta_Type_Particle_Style_Members[] = {
	Meta_Member{0, Meta_Type_int32, 0, "id", offsetof(Particle_Style, id)},
	Meta_Member{0, Meta_Type_Rect2, 0, "texture", offsetof(Particle_Style, texture)},
	Meta_Member{0, Meta_Type_Vec2, 0, "size", offsetof(Particle_Style, size)},
	Meta_Member{0, Meta_Type_Vec4, 0, "color", offsetof(Particle_Style, color)},
	Meta_Member{0, Meta_Type_Vec4, 0, "shadow_color", offsetof(Particle_Style, shadow_color)},
	Meta_Member{0, Meta_Type_Vec3, 0, "acceleration", offsetof(Particle_Style, acceleration)},
	Meta_Member{0, Meta_Type_real, 0, "impulse_min", offsetof(Particle_Style, impulse_min)},
	Meta_Member{0, Meta_Type_real, 0, "impulse_max", offsetof(Particle_Style, impulse_max)},
	Meta_Member{0, Meta_Type_real, 0, "angle_min", offsetof(Particle_Style, angle_min)},
	Meta_Member{0, Meta_Type_real, 0, "angle_max", offsetof(Particle_Style, angle_max)},
	Meta_Member{0, Meta_Type_real, 0, "angle_vel_min", offsetof(Particle_Style, angle_vel_min)},
	Meta_Member{0, Meta_Type_real, 0, "angle_vel_max", offsetof(Particle_Style, angle_vel_max)},
	Meta_Member{0, Meta_Type_real, 0, "scale_min", offsetof(Particle_Style, scale_min)},
	Meta_Member{0, Meta_Type_real, 0, "scale_max", offsetof(Particle_Style, scale_max)},
	Meta_Member{0, Meta_Type_int32, 0, "frame_min", offsetof(Particle_Style, frame_min)},
	Meta_Member{0, Meta_Type_int32, 0, "frame_max", offsetof(Particle_Style, frame_max)},
	Meta_Member{0, Meta_Type_int32, 0, "max_frames", offsetof(Particle_Style, max_frames)},
	Meta_Member{0, Meta_Type_int32, 0, "framerate", offsetof(Particle_Style, framerate)},
	Meta_Member{0, Meta_Type_int32, 0, "time_min", offsetof(Particle_Style, time_min)},
	Meta_Member{0, Meta_Type_int32, 0, "time_max", offsetof(Particle_Style, time_max)},
	Meta_Member{0, Meta_Type_bool, 0, "time_scaling", offsetof(Particle_Style, time_scaling)},
	Meta_Member{0, Meta_Type_bool, 0, "time_alpha", offsetof(Particle_Style, time_alpha)},
	Meta_Member{0, Meta_Type_real, 0, "ground_restitution", offsetof(Particle_Style, ground_restitution)},
	Meta_Member{0, Meta_Type_real, 0, "skid_on_bounce_min", offsetof(Particle_Style, skid_on_bounce_min)},
	Meta_Member{0, Meta_Type_real, 0, "skid_on_bounce_max", offsetof(Particle_Style, skid_on_bounce_max)},
	Meta_Member{0, Meta_Type_real, 0, "jitter_on_bounce_mag", offsetof(Particle_Style, jitter_on_bounce_mag)}
};

const Meta_Member Meta_Type_Emitter_Members[] = {
	Meta_Member{1, Meta_Type_Particle, 1, "particles", offsetof(Emitter, particles)},
	Meta_Member{0, Meta_Type_isize, 0, "particles_count", offsetof(Emitter, particles_count)},
	Meta_Member{0, Meta_Type_isize, 0, "particles_capacity", offsetof(Emitter, particles_capacity)},
	Meta_Member{1, Meta_Type_Particle_Style, 1, "styles", offsetof(Emitter, styles)},
	Meta_Member{0, Meta_Type_isize, 0, "styles_count", offsetof(Emitter, styles_count)},
	Meta_Member{0, Meta_Type_isize, 0, "styles_capacity", offsetof(Emitter, styles_capacity)}
};

const Meta_Member Meta_Type_Hitbox_Contact_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "a_id", offsetof(Hitbox_Contact, a_id)},
	Meta_Member{0, Meta_Type_isize, 0, "b_id", offsetof(Hitbox_Contact, b_id)},
	Meta_Member{1, Meta_Type_Entity, 1, "a", offsetof(Hitbox_Contact, a)},
	Meta_Member{1, Meta_Type_Entity, 1, "b", offsetof(Hitbox_Contact, b)}
};

const Meta_Member Meta_Type_Hitbox_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "id", offsetof(Hitbox, id)},
	Meta_Member{1, Meta_Type_Entity, 1, "ref", offsetof(Hitbox, ref)},
	Meta_Member{0, Meta_Type_uint64, 0, "mask", offsetof(Hitbox, mask)},
	Meta_Member{0, Meta_Type_uint64, 0, "group", offsetof(Hitbox, group)},
	Meta_Member{0, Meta_Type_AABB, 0, "box", offsetof(Hitbox, box)}
};

const Meta_Member Meta_Type_Entity_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "id", offsetof(Entity, id)},
	Meta_Member{0, Meta_Type_isize, 0, "body_id", offsetof(Entity, body_id)},
	Meta_Member{1, Meta_Type_Sim_Body, 1, "body", offsetof(Entity, body)},
	Meta_Member{0, Meta_Type_Vec2, 0, "walk_impulse", offsetof(Entity, walk_impulse)},
	Meta_Member{0, Meta_Type_Hitbox, 0, "hitbox", offsetof(Entity, hitbox)},
	Meta_Member{0, Meta_Type_int32, 0, "health", offsetof(Entity, health)},
	Meta_Member{0, Meta_Type_int32, 0, "attack", offsetof(Entity, attack)},
	Meta_Member{0, Meta_Type_real, 0, "attack_interval", offsetof(Entity, attack_interval)},
	Meta_Member{0, Meta_Type_real, 0, "attack_timer", offsetof(Entity, attack_timer)},
	Meta_Member{0, Meta_Type_real, 0, "knockback", offsetof(Entity, knockback)},
	Meta_Member{1, Meta_Type_Animated_Sprite, 1, "anim", offsetof(Entity, anim)},
	Meta_Member{0, Meta_Type_Sprite, 0, "sprite", offsetof(Entity, sprite)},
	Meta_Member{0, Meta_Type_real, 0, "z", offsetof(Entity, z)},
	Meta_Member{0, Meta_Type_real, 0, "shadow_scale", offsetof(Entity, shadow_scale)},
	Meta_Member{0, Meta_Type_isize, 0, "kind", offsetof(Entity, kind)},
	Meta_Member{0, Meta_Type_int32, 0, "counter", offsetof(Entity, counter)},
	Meta_Member{0, Meta_Type_int32, 0, "facing", offsetof(Entity, facing)},
	Meta_Member{0, Meta_Type_Direction, 0, "direction", offsetof(Entity, direction)},
	Meta_Member{0, Meta_Type_uint64, 0, "events", offsetof(Entity, events)},
	Meta_Member{0, Meta_Type_uint64, 0, "flags", offsetof(Entity, flags)},
	Meta_Member{0, Meta_Type_Rituals_Entity_Userdata, 0, "userdata", offsetof(Entity, userdata)}
};

const Meta_Member Meta_Type_Area_Link_Members[] = {
	Meta_Member{0, Meta_Type_Vec2i, 0, "position", offsetof(Area_Link, position)},
	Meta_Member{1, Meta_Type_World_Area_Stub, 1, "link", offsetof(Area_Link, link)}
};

const Meta_Member Meta_Type_World_Area_Stub_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "id", offsetof(World_Area_Stub, id)},
	Meta_Member{0, Meta_Type_usize, 0, "seed", offsetof(World_Area_Stub, seed)},
	Meta_Member{0, Meta_Type_Area_Link, 0, "north", offsetof(World_Area_Stub, north)},
	Meta_Member{0, Meta_Type_Area_Link, 0, "south", offsetof(World_Area_Stub, south)},
	Meta_Member{0, Meta_Type_Area_Link, 0, "west", offsetof(World_Area_Stub, west)},
	Meta_Member{0, Meta_Type_Area_Link, 0, "east", offsetof(World_Area_Stub, east)},
	Meta_Member{0, Meta_Type_World_Area_Biome, 0, "biome", offsetof(World_Area_Stub, biome)}
};

const Meta_Member Meta_Type_World_Area_Members[] = {
	Meta_Member{0, Meta_Type_isize, 0, "id", offsetof(World_Area, id)},
	Meta_Member{1, Meta_Type_World_Area_Stub, 1, "stub", offsetof(World_Area, stub)},
	Meta_Member{1, Meta_Type_World, 1, "world", offsetof(World_Area, world)},
	Meta_Member{0, Meta_Type_Simulator, 0, "sim", offsetof(World_Area, sim)},
	Meta_Member{0, Meta_Type_Tilemap, 0, "map", offsetof(World_Area, map)},
	Meta_Member{0, Meta_Type_Vec2, 0, "offset", offsetof(World_Area, offset)},
	Meta_Member{0, Meta_Type_Vec2, 0, "target", offsetof(World_Area, target)},
	Meta_Member{1, Meta_Type_Entity, 1, "entities", offsetof(World_Area, entities)},
	Meta_Member{0, Meta_Type_bool, 0, "entities_dirty", offsetof(World_Area, entities_dirty)},
	Meta_Member{0, Meta_Type_isize, 0, "entities_count", offsetof(World_Area, entities_count)},
	Meta_Member{0, Meta_Type_isize, 0, "entities_capacity", offsetof(World_Area, entities_capacity)},
	Meta_Member{0, Meta_Type_isize, 0, "next_entity_id", offsetof(World_Area, next_entity_id)},
	Meta_Member{1, Meta_Type_isize, 1, "removed_entities", offsetof(World_Area, removed_entities)},
	Meta_Member{0, Meta_Type_isize, 0, "removed_entities_count", offsetof(World_Area, removed_entities_count)},
	Meta_Member{0, Meta_Type_isize, 0, "removed_entities_capacity", offsetof(World_Area, removed_entities_capacity)},
	Meta_Member{1, Meta_Type_Hitbox, 1, "hitboxes", offsetof(World_Area, hitboxes)},
	Meta_Member{0, Meta_Type_isize, 0, "hitboxes_count", offsetof(World_Area, hitboxes_count)},
	Meta_Member{0, Meta_Type_isize, 0, "hitboxes_capacity", offsetof(World_Area, hitboxes_capacity)},
	Meta_Member{0, Meta_Type_int32, 0, "hitbox_sort_axis", offsetof(World_Area, hitbox_sort_axis)},
	Meta_Member{1, Meta_Type_Hitbox_Contact, 1, "hitbox_contacts", offsetof(World_Area, hitbox_contacts)},
	Meta_Member{0, Meta_Type_isize, 0, "hitbox_contacts_count", offsetof(World_Area, hitbox_contacts_count)},
	Meta_Member{0, Meta_Type_isize, 0, "hitbox_contacts_capacity", offsetof(World_Area, hitbox_contacts_capacity)},
	Meta_Member{1, Meta_Type_Entity, 1, "player", offsetof(World_Area, player)}
};

const Meta_Member Meta_Type_World_Members[] = {
	Meta_Member{1, Meta_Type_char, 1, "name", offsetof(World, name)},
	Meta_Member{1, Meta_Type_World_Area_Stub, 1, "area_stubs", offsetof(World, area_stubs)},
	Meta_Member{0, Meta_Type_isize, 0, "next_area_id", offsetof(World, next_area_id)},
	Meta_Member{0, Meta_Type_isize, 0, "areas_count", offsetof(World, areas_count)},
	Meta_Member{0, Meta_Type_isize, 0, "areas_capacity", offsetof(World, areas_capacity)},
	Meta_Member{0, Meta_Type_isize, 0, "areas_width", offsetof(World, areas_width)},
	Meta_Member{0, Meta_Type_isize, 0, "areas_height", offsetof(World, areas_height)},
	Meta_Member{1, Meta_Type_World_Area, 1, "current_area", offsetof(World, current_area)},
	Meta_Member{0, Meta_Type_usize, 0, "seed", offsetof(World, seed)},
	Meta_Member{0, Meta_Type_Entity, 0, "global_player_entity", offsetof(World, global_player_entity)},
	Meta_Member{0, Meta_Type_Sim_Body, 0, "global_player_body", offsetof(World, global_player_body)},
	Meta_Member{0, Meta_Type_usize, 0, "slowtick_timer", offsetof(World, slowtick_timer)},
	Meta_Member{0, Meta_Type_usize, 0, "slowtick_timer_interval", offsetof(World, slowtick_timer_interval)},
	Meta_Member{0, Meta_Type_Particle_Style, 0, "base_style", offsetof(World, base_style)},
	Meta_Member{0, Meta_Type_Emitter, 0, "emitter", offsetof(World, emitter)}
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
		"Vec3i", 
		"Meta_Type_Vec3i", 
		Meta_Type_Vec3i, 
		Meta_Type_Vec3i_Members, 
		2},
	Meta_Struct_Info {
		"", 
		"Meta_Type_Vec3i_struct0", 
		Meta_Type_Vec3i_struct0, 
		Meta_Type_Vec3i_struct0_Members, 
		3},
	Meta_Struct_Info {
		"Vec4i", 
		"Meta_Type_Vec4i", 
		Meta_Type_Vec4i, 
		Meta_Type_Vec4i_Members, 
		2},
	Meta_Struct_Info {
		"", 
		"Meta_Type_Vec4i_struct0", 
		Meta_Type_Vec4i_struct0, 
		Meta_Type_Vec4i_struct0_Members, 
		4},
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
		25},
	Meta_Struct_Info {
		"Platform", 
		"Meta_Type_Platform", 
		Meta_Type_Platform, 
		Meta_Type_Platform_Members, 
		8},
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
		"None_State", 
		"Meta_Type_None_State", 
		Meta_Type_None_State, 
		Meta_Type_None_State_Members, 
		1},
	Meta_Struct_Info {
		"Menu_State", 
		"Meta_Type_Menu_State", 
		Meta_Type_Menu_State, 
		Meta_Type_Menu_State_Members, 
		7},
	Meta_Struct_Info {
		"Play_State", 
		"Meta_Type_Play_State", 
		Meta_Type_Play_State, 
		Meta_Type_Play_State_Members, 
		10},
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
		"Rituals_Bat_Userdata", 
		"Meta_Type_Rituals_Bat_Userdata", 
		Meta_Type_Rituals_Bat_Userdata, 
		Meta_Type_Rituals_Bat_Userdata_Members, 
		1},
	Meta_Struct_Info {
		"Rituals_Snake_Userdata", 
		"Meta_Type_Rituals_Snake_Userdata", 
		Meta_Type_Rituals_Snake_Userdata, 
		Meta_Type_Rituals_Snake_Userdata_Members, 
		1},
	Meta_Struct_Info {
		"Rituals_Goblin_Knight_Userdata", 
		"Meta_Type_Rituals_Goblin_Knight_Userdata", 
		Meta_Type_Rituals_Goblin_Knight_Userdata, 
		Meta_Type_Rituals_Goblin_Knight_Userdata_Members, 
		2},
	Meta_Struct_Info {
		"Rituals_Enemy_Userdata", 
		"Meta_Type_Rituals_Enemy_Userdata", 
		Meta_Type_Rituals_Enemy_Userdata, 
		Meta_Type_Rituals_Enemy_Userdata_Members, 
		6},
	Meta_Struct_Info {
		"", 
		"Meta_Type_Rituals_Enemy_Userdata_union0", 
		Meta_Type_Rituals_Enemy_Userdata_union0, 
		Meta_Type_Rituals_Enemy_Userdata_union0_Members, 
		3},
	Meta_Struct_Info {
		"Rituals_Player_Userdata", 
		"Meta_Type_Rituals_Player_Userdata", 
		Meta_Type_Rituals_Player_Userdata, 
		Meta_Type_Rituals_Player_Userdata_Members, 
		5},
	Meta_Struct_Info {
		"Rituals_Prop_Userdata", 
		"Meta_Type_Rituals_Prop_Userdata", 
		Meta_Type_Rituals_Prop_Userdata, 
		Meta_Type_Rituals_Prop_Userdata_Members, 
		4},
	Meta_Struct_Info {
		"Rituals_Item_Pickup_Userdata", 
		"Meta_Type_Rituals_Item_Pickup_Userdata", 
		Meta_Type_Rituals_Item_Pickup_Userdata, 
		Meta_Type_Rituals_Item_Pickup_Userdata_Members, 
		2},
	Meta_Struct_Info {
		"Rituals_Health_Pickup_Userdata", 
		"Meta_Type_Rituals_Health_Pickup_Userdata", 
		Meta_Type_Rituals_Health_Pickup_Userdata, 
		Meta_Type_Rituals_Health_Pickup_Userdata_Members, 
		1},
	Meta_Struct_Info {
		"Rituals_Pickup_Userdata", 
		"Meta_Type_Rituals_Pickup_Userdata", 
		Meta_Type_Rituals_Pickup_Userdata, 
		Meta_Type_Rituals_Pickup_Userdata_Members, 
		3},
	Meta_Struct_Info {
		"Unknown_Userdata", 
		"Meta_Type_Unknown_Userdata", 
		Meta_Type_Unknown_Userdata, 
		Meta_Type_Unknown_Userdata_Members, 
		2},
	Meta_Struct_Info {
		"Rituals_Entity_Userdata", 
		"Meta_Type_Rituals_Entity_Userdata", 
		Meta_Type_Rituals_Entity_Userdata, 
		Meta_Type_Rituals_Entity_Userdata_Members, 
		5},
	Meta_Struct_Info {
		"Game_Registry", 
		"Meta_Type_Game_Registry", 
		Meta_Type_Game_Registry, 
		Meta_Type_Game_Registry_Members, 
		6},
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
