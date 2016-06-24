/*
 * rituals_gui.cpp
 *
 */

#define Ascii_Printable_Start (32)
#define Ascii_Printable_End   (128)
#define Ascii_Printable_Count (128-32)

struct Spritefont
{
	int32 line_padding;
	int32 character_padding;
	int32 tab_size;

	int32 glyph_width, glyph_height;
	Rect2* glyphs;
	Vec4 color;
};

void init_spritefont(Spritefont* font)
{
	font->line_padding = 8;
	font->character_padding = 0;
	font->tab_size = 4; // * space width?
	font->glyphs = NULL;
	font->color = v4(1, 1, 1, 1);
}

static inline int32 dec_str_to_int(char* str, isize len)
{
	int32 result = 0;
	for(isize i = 0; i < len; ++i) {
		result = result * 10 + str[i] - '0';
	}
	return result;
}

static inline bool _isnt_spritefont_separator(char c)
{
	return (c != ' ') && (c != '\n') && (c != '\0');
}

Rect2* parse_spritefont_rectangles(char* glyphs_file, Memory_Arena* arena, int32 offsetx, int32 offsety, int32* w, int32* h)
{
	Rect2* glyphs = Arena_Push_Array(arena, Rect2, Ascii_Printable_Count);
	isize file_len = strlen(glyphs_file);
	isize start = 0, len = 0;
	for(isize i = 0; i < file_len; ++i) {
		char c = glyphs_file[i++];
		Rect2* r;
		if(c > Ascii_Printable_Start && c <= Ascii_Printable_End) {
			r = &glyphs[c - Ascii_Printable_Start];
		} else {
			while(glyphs_file[i] != '\n') i++;
			continue;
		}
		i++;

		start = i;
		while(_isnt_spritefont_separator(glyphs_file[i])) i++;
		len = i - start;
		r->x = dec_str_to_int(glyphs_file + start, len) + offsetx;
		r->x /= renderer->texture_width;
		i++;

		start = i;
		while(_isnt_spritefont_separator(glyphs_file[i])) i++;
		len = i - start;
		r->y = dec_str_to_int(glyphs_file + start, len) + offsety;
		r->y /= renderer->texture_height;
		i++;

		start = i;
		while(_isnt_spritefont_separator(glyphs_file[i])) i++;
		len = i - start;
		r->w = dec_str_to_int(glyphs_file + start, len);
		*w = r->w;
		r->w /= renderer->texture_width;
		i++;
		start = i;

		while(_isnt_spritefont_separator(glyphs_file[i])) i++;
		len = i - start;
		r->h = dec_str_to_int(glyphs_file + start, len);
		*h = r->h;
		r->h /= renderer->texture_height;
		i++;
	}
	return glyphs;
}

void load_spritefont(Spritefont* font, char* file_path_in, Vec2i offset)
{
	init_spritefont(font);

	char file_path[File_Path_Max_Length];
	isize len = snprintf(file_path, File_Path_Max_Length, "%s%s", game->base_path, file_path_in);
	
	char* str = NULL;
	FILE* fp = fopen(file_path, "r");
	if(fp != NULL) {
		start_temp_arena(game->temp_arena);
		fseek(fp, 0L, SEEK_END);
		isize size = ftell(fp);
		rewind(fp);
		str = Arena_Push_Array(game->temp_arena, char, size + 1);
		fread(str, sizeof(char), size, fp);
		str[size] = '\0';
		fclose(fp);
		font->glyphs = parse_spritefont_rectangles(str, game->asset_arena, 
			offset.x, offset.y, &font->glyph_width, &font->glyph_height);
		end_temp_arena(game->temp_arena);
	} else {
		Log_Error("Could not find font glyphs file");
		Log_Error(file_path);
	}
}

Spritefont* load_spritefont(char* filepath, Vec2i offset)
{
	Spritefont* font = Arena_Push_Struct(game->asset_arena, Spritefont);
	load_spritefont(font, filepath, offset);
	return font;
}

void spritefont_render_text(Spritefont* font,
		char* text, isize len, 
		Vec2 position, 
		int32 max_width, 
		Vec2* region)
{
	Vec2 initial_pos = position;
	Vec2 size = v2(font->glyph_width, font->glyph_height);
	Sprite s;
	int32 wrapped = 0;
	for(isize i = 0; i < len; ++i) {
		char c = text[i];
		
		init_sprite(&s);
		switch(c) {
			case '\n':
				position.y += font->glyph_height + font->line_padding;
				position.x = 0;
				wrapped ++;
				break;
			case '\t':
				position.x += font->glyph_width * font->tab_size;
				break;
			case '\r':
				continue;
			default: 
				break;
		}
		if(c < Ascii_Printable_Start || c > Ascii_Printable_End) continue;

		s.position = position;
		s.texture = font->glyphs[c - Ascii_Printable_Start];
		s.size = size;
		s.color = font->color;
		
		if((max_width > 0) && (s.position.x + s.size.x > (max_width + initial_pos.x))) {
			position.y += font->glyph_height + font->line_padding;
			position.x = 0;
			wrapped ++;
		}
			
		renderer_push_sprite(&s);
		position.x += size.x + font->character_padding;
	}
	if(region != NULL) {
		*region = position - initial_pos;
		if(wrapped > 0) {
			region->x = max_width;
		}
	}
}

void spritefont_render_text(Spritefont* font, char* text, Vec2 position)
{
	spritefont_render_text(font,
		text, strlen(text), 
		position, 
		-1, 
		NULL);
}

void render_body_text(char* text, Vec2 position)
{
	spritefont_render_text(body_font, text, position);
}

void render_title_text(char* text, Vec2 position)
{
	spritefont_render_text(title_font, text, position);
}

bool gui_add_button(Vec2 position, char* text)
{
	Vec2 dmouse = v2(
			input->mouse_x / game->scale, 
			input->mouse_y / game->scale) + renderer->offset;
	int32 state = 0;
	if(aabb_intersect(aabb(position, 5 * 16, 24), aabb(dmouse, 0, 0))) {
		state = 1;
		if(input->mouse[SDL_BUTTON_LEFT] >= State_Pressed) {
			state = 2;
		}
	}

	Sprite s;
	init_sprite(&s);
	s.position = position;
	s.size = v2(5 * 16, 24);
	s.texture = Get_Texture_Coordinates(renderer->texture_width - 5 * 16, 6 * 16 + s.size.y * state, 5 * 16, 24);
	renderer_push_sprite(&s);
	body_font->color = v4(0, 0, 0, 1);
	render_body_text(text, v2(
				position.x - s.size.x/2 + 8 + body_font->glyph_width / 2,
				position.y));

	return state > 0 && input->mouse[SDL_BUTTON_LEFT] == State_Just_Released;
}
