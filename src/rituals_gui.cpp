/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 * rituals_gui.cpp
 *
 */
#define AsciiPrintableStart (32)
#define AsciiPrintableEnd   (128)
#define AsciiPrintableCount (128-32)

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
	font->line_padding = 0;
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
	Rect2* glyphs = arena_push_array(arena, Rect2, AsciiPrintableCount);
	isize file_len = strlen(glyphs_file);
	isize start = 0, len = 0;
	for(isize i = 0; i < file_len; ++i) {
		char c = glyphs_file[i++];
		Rect2* r;
		if(c > AsciiPrintableStart && c <= AsciiPrintableEnd) {
			r = &glyphs[c - AsciiPrintableStart];
		} else {
			while(glyphs_file[i] != '\n') i++;
			continue;
		}
		i++;

		start = i;
		while(_isnt_spritefont_separator(glyphs_file[i])) i++;
		len = i - start;
		r->x = dec_str_to_int(glyphs_file + start, len) + offsetx;
		r->x /= Renderer->texture_width;
		i++;

		start = i;
		while(_isnt_spritefont_separator(glyphs_file[i])) i++;
		len = i - start;
		r->y = dec_str_to_int(glyphs_file + start, len) + offsety - 1;
		r->y /= Renderer->texture_height;
		i++;

		start = i;
		while(_isnt_spritefont_separator(glyphs_file[i])) i++;
		len = i - start;
		r->w = dec_str_to_int(glyphs_file + start, len);
		*w = r->w;
		r->w /= Renderer->texture_width;
		i++;
		start = i;

		while(_isnt_spritefont_separator(glyphs_file[i])) i++;
		len = i - start;
		r->h = dec_str_to_int(glyphs_file + start, len) + 2;
		*h = r->h;
		r->h /= Renderer->texture_height;
		i++;
	}
	return glyphs;
}

void load_spritefont(Spritefont* font, char* file_path_in, Vec2i offset)
{
	init_spritefont(font);

	char file_path[FilePathMaxLength];
	isize len = snprintf(file_path, FilePathMaxLength, "%s%s", Game->base_path, file_path_in);
	
	char* str = NULL;
	FILE* fp = fopen(file_path, "r");
	if(fp != NULL) {
		start_temp_arena(Game->temp_arena);
		fseek(fp, 0L, SEEK_END);
		isize size = ftell(fp);
		rewind(fp);
		str = arena_push_array(Game->temp_arena, char, size + 1);
		fread(str, sizeof(char), size, fp);
		str[size] = '\0';
		fclose(fp);
		font->glyphs = parse_spritefont_rectangles(str, Game->asset_arena, 
			offset.x, offset.y, &font->glyph_width, &font->glyph_height);
		end_temp_arena(Game->temp_arena);
	} else {
		Log_Error("Could not find font glyphs file");
		Log_Error(file_path);
	}
}

Spritefont* load_spritefont(char* filepath, Vec2i offset)
{
	Spritefont* font = arena_push_struct(Game->asset_arena, Spritefont);
	load_spritefont(font, filepath, offset);
	return font;
}


Vec2 spritefont_size_text(Spritefont* font, char* text, isize len)
{
	Vec2 position = v2(0, font->glyph_height);
	Vec2 size = v2(font->glyph_width, font->glyph_height);
	int32 wrapped = 0;
	for(isize i = 0; i < len; ++i) {
		char c = text[i];
		
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
		if(c < AsciiPrintableStart || c > AsciiPrintableEnd) continue;
		position.x += size.x + font->character_padding;
	}

	return position;
}

Vec2 spritefont_size_text(Spritefont* font, char* text) 
{
	return spritefont_size_text(font, text, strlen(text));
}

void spritefont_render_text(Spritefont* font,
		char* text, isize len, 
		Vec2 position, 
		int32 max_width = -1, 
		Sprite_Anchor anchor = Anchor_Top_Left,
		real scale = 1.0f,
		Vec2* region = NULL)
{
	Vec2 initial_pos = position;
	position = v2(0, 0);
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
		if(c < AsciiPrintableStart || c > AsciiPrintableEnd) continue;

		s.position = initial_pos + position * scale;
		s.texture = font->glyphs[c - AsciiPrintableStart];
		s.size = size * scale;
		s.color = font->color;
		s.angle = 0;
		s.anchor = anchor;
		
		if((max_width > 0) && (s.position.x + s.size.x > (max_width + initial_pos.x))) {
			position.y += font->glyph_height + font->line_padding;
			position.x = 0;
			wrapped ++;
		}
			
		renderer_push_sprite(&s);
		position.x += size.x + font->character_padding;
	}
	if(region != NULL) {
		*region = position;
		if(wrapped > 0) {
			region->x = max_width;
		}
	}
}

//TODO(will) fix--implement and fix wordwrap
void spritefont_render_text_ex(Spritefont* font,
		char* text, isize len, 
		Vec2 position, 
		int32 max_width = -1, 
		Sprite_Anchor anchor = Anchor_Top_Left,
		real scale = 1.0f,
		Vec2* region = NULL)
{
	Vec2 initial_pos = position;
	position = v2(0, 0);
	Vec2 size = v2(font->glyph_width, font->glyph_height);
	Sprite s;
	int32 wrapped = 0;
	isize wordstart = -1;
	for(isize i = 0; i < len; ++i) {
		char c = text[i];
		init_sprite(&s);
		if(wordstart == -1) {
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
				case ' ':
					//position.x += font->glyph_width;
					continue;
				default: 
					if(c >= AsciiPrintableStart && c <= AsciiPrintableEnd) {
						wordstart = i;
					}

					break;
			}
		} else {
			if(isspace(c) || (i==len-1)) {
				isize chars = i - wordstart;
				real word_width = (chars * font->glyph_width) * scale;
				if((max_width > 0) && ((position.x + word_width) > (max_width))) {
					position.y += font->glyph_height + font->line_padding;
					position.x = 0;
					wrapped++;
				}
				for(isize j = wordstart; j <= i; ++j) {
					c = text[j];
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
						case ' ':
							position.x += font->glyph_width;
							continue;
						default:
							break;
					}

					if(c < AsciiPrintableStart || c > AsciiPrintableEnd) {
						continue;
					}
					s.position = initial_pos + position * scale;
					s.texture = font->glyphs[c - AsciiPrintableStart];
					s.size = size * scale;
					s.color = font->color;
					s.angle = 0;
					s.anchor = anchor;
					renderer_push_sprite(&s);
					position.x += size.x + font->character_padding;
				}
				wordstart = -1;
				i--;
			}
		}
	}
	if(region != NULL) {
		*region = position + v2(0, font->glyph_height);
		if(wrapped > 0) {
			region->x = max_width;
		}
	}
}

void spritefont_render_text(Spritefont* font, char* text, Vec2 position, Sprite_Anchor anchor = Anchor_Top_Left, real scale = 1.0f)
{
	spritefont_render_text(font,
		text, strlen(text), 
		position,-1,  anchor, scale);
}

void spritefont_render_text_background(Spritefont* font, char* text, Vec2 position, Vec4 background) 
{
	Vec2 text_size = spritefont_size_text(font, text);
	Sprite s; 
	init_sprite(&s);
	s.size = v2(text_size.x + 16,  Body_Font->glyph_height + 8);
	s.position = position - v2(8, 4);
	s.color = background;
	s.texture = Get_Texture_Coordinates(64, 0, 32, 32);
	s.anchor = Anchor_Top_Left;
	renderer_push_sprite(&s);

}

void render_body_text(char* text, Vec2 position, bool background=false, real scale = 1.0f)
{
	if(background) {
		spritefont_render_text_background(Body_Font, text, position, v4(0, 0, 0, 0.8f));
	}
	spritefont_render_text(Body_Font, text, position, Anchor_Top_Left, scale);
}

void render_title_text(char* text, Vec2 position)
{
	spritefont_render_text(Title_Font, text, position);
}

#define _color(x, y, z, w) Vec4{ (real)(x), (real)(y), (real)(z), (real)(w) }

bool gui_query_mouse(Rect2 region, Vec2 parent)
{
	region.position -= parent;
	Rect2 r = region;
	Vec2 p = Input->mouse_pos;
	Vec2 rp1, rp2;
	if(renderer_has_clip_rect()) {
		Rect2 c = Renderer->clip; 
		if(r.x > (c.x + c.w)) return false;
		if((r.x + r.w) < c.x) return false;
		if(r.y > (c.y + c.h)) return false;
		if((r.y + r.h) < c.y) return false;
		Rect2_Clip_Info clip = rect2_clip(r, c);
		rp1 = clip.rp1;
		rp2 = clip.rp2;
	} else {
		rp1 = r.position;
		rp2 = r.position + r.size;
	}
	return (p.x >= rp1.x) && (p.x <= rp2.x) && 
		   (p.y >= rp1.y) && (p.y <= rp2.y);
}



Vec4 Gui_ButtonTint = _color(0.88, 0.89, 1, 1);
Vec4 Gui_ButtonRestColor = _color(.8, .8, .8, 1) * Gui_ButtonTint;
Vec4 Gui_ButtonActiveColor = _color(.9, .9, .9, 1) * Gui_ButtonTint;
Vec4 Gui_ButtonDownColor = _color(.5, .5, .5, 1) * Gui_ButtonTint;
Vec4 Gui_ButtonTextColor = _color(0, 0, 0, 1) ;
Vec4 Gui_ButtonTextDownColor = Gui_ButtonTextColor;

Vec4 Gui_ButtonOutlineRestColor = _color(.9, .9, .9, 1) * Gui_ButtonTint;
Vec4 Gui_ButtonOutlineActiveColor = _color(1, 1, 1, 1) * Gui_ButtonTint;
Vec4 Gui_ButtonOutlineDownColor = _color(.4, .4, .4, 1) * Gui_ButtonTint;
Vec4 Gui_ButtonOutlineRestColor2 = _color(0.5, 0.5, 0.5, 1) * Gui_ButtonTint;
Vec4 Gui_ButtonOutlineActiveColor2 = _color(0.5, 0.5, 0.5, 1) * Gui_ButtonTint;
Vec4 Gui_ButtonOutlineDownColor2 = _color(.7, .7, .7, 1) * Gui_ButtonTint;

bool gui_add_button(Vec2 position, char* text, Vec2 minimum_size)
{
	int32 state = 0;

	Vec2 txs = spritefont_size_text(Body_Font, text, strlen(text));
	if(txs.x < minimum_size.x) txs.x = minimum_size.x;
	if(txs.y < minimum_size.y) txs.y = minimum_size.y;
	
	Rect2 region;
	region.position = position;
	region.size = txs + v2(16, 8);
	if(gui_query_mouse(region, v2(0,0))) {
		state = 1;
		if(Input->mouse[SDL_BUTTON_LEFT] >= State_Pressed) {
			state = 2;
		}
	}
	Sprite s = get_box_sprite(position, txs + v2(16, 8), Gui_ButtonRestColor);
	s.anchor = Anchor_Top_Left;
	Vec4 color1 = Gui_ButtonOutlineRestColor;
	Vec4 color2 = Gui_ButtonOutlineRestColor;
	switch(state) {
		case 0:
			color1 = Gui_ButtonOutlineRestColor;
			color2 = Gui_ButtonOutlineRestColor2;
			Body_Font->color = Gui_ButtonTextColor;
			s.color = Gui_ButtonRestColor;
			break;
		case 1:
			Body_Font->color = Gui_ButtonTextColor;
			color1 = Gui_ButtonOutlineActiveColor;
			color2 = Gui_ButtonOutlineActiveColor2;
			s.color = Gui_ButtonActiveColor;
			break;
		case 2:
			Body_Font->color = Gui_ButtonTextDownColor;
			color1 = Gui_ButtonOutlineDownColor;
			color2 = Gui_ButtonOutlineDownColor2;
			s.color = Gui_ButtonDownColor;
			break;
		default:
			break;
	}
	//s.position -= v2(8, 4);
	renderer_push_sprite(&s);
	render_body_text(text, v2(8, 4) + v2(
				position.x,
				position.y + (s.size.y - txs.y) / 2 - 4));
	Vec4 colors[4] = {
		color1, color1,
		color2, color2
	};
	draw_box_outline(s.position + s.size/2, s.size, colors, 2);

	return (state > 0) && (Input->mouse[SDL_BUTTON_LEFT] == State_Just_Released);
}

bool gui_add_button(Vec2 position, char* text)
{
	return gui_add_button(position, text, v2(0, 0));
}

bool gui_add_checkbox(Vec2 position, char* text, bool* value)
{
	return *value;
}

void gui_add_slider(Vec2 position, Vec2 size, char* label, real min, real max, int precision, real* in_value, bool* active)
{
	Sprite bg = get_box_sprite(position, size, Gui_ButtonDownColor);
	bg.anchor = Anchor_Top_Left;
	renderer_push_sprite(&bg);

	char min_str[16];
	char max_str[16];
	isize min_str_len = snprintf(min_str, 16, "%.*f", precision, min);
	isize max_str_len = snprintf(max_str, 16, "%.*f", precision, max);
	Vec2 min_str_reg;
	Body_Font->color = v4(1, 1, 1, 1);
    spritefont_render_text(Body_Font,
		min_str, min_str_len,
		position, 
		-1, 
		Anchor_Top_Left,
		1.0f,
		&min_str_reg);
	Vec2 max_str_reg;
	Vec2 begin = position + v2(min_str_reg.x, 0);
	Vec2 end = position + v2(size.x - Body_Font->glyph_width * (strlen(label) + max_str_len + 1), 0);
    spritefont_render_text(Body_Font,
		max_str, max_str_len,
		end, 
		-1, 
		Anchor_Top_Left,
		1.0f,
		&max_str_reg); 
	render_body_text(label, position + v2(size.x - Body_Font->glyph_width * strlen(label), 0));

	real value = 0;
	real perc = .5; 
	if(in_value != NULL) {
		value = *in_value;
		perc = (value - min) / (max - min);
	}
	begin.x += 8;
	end.x -= 8;
	Vec2 line_offset = v2(0, 8);
	draw_line(begin + line_offset, end + line_offset, v4(1, 1, 1, 0.9f), 2);
	Sprite handle = get_box_sprite(begin + line_offset, v2(8, 12), v4(1, 1, 1, 1));
	handle.position.x += perc * (end.x - begin.x);
	
	Rect2 r;
	r.position = handle.position - handle.size / 2;
	r.size = handle.size;

	if(gui_query_mouse(r, v2(0, 0))) {
		if(active != NULL) {
			if(Input->mouse[SDL_BUTTON_LEFT] == State_Just_Pressed) {
				*active = true; 
			}
		}
	} 
	if(Input->mouse[SDL_BUTTON_LEFT] <= State_Released) {
		if(active != NULL) {
			*active = false;
		}
	}
	if(active != NULL && *active) {
		Vec2 m = Input->mouse_pos;
		m.y = 0;
		if(m.x < begin.x) m.x = begin.x;
		else if (m.x > end.x) m.x = end.x;
		m.x -= begin.x;
		m.x /= (end.x - begin.x);
		perc = m.x;
		handle.position.x = begin.x + line_offset.x + perc * (end.x - begin.x);
		if(in_value != NULL) {
			*in_value = perc * (max - min);
		}
		char number[32];
		isize len = snprintf(number, 32, "%.*f", precision + 1, *in_value);
		render_body_text(number, handle.position - v2(len * Body_Font->glyph_width / 2, Body_Font->glyph_height + 4 + handle.size.y / 2), true);
	}

	renderer_push_sprite(&handle);
	







	return; 
}


struct Gui_Window_Handle
{
	Vec2 position;
	char* title;
	int32 z;
};

struct Gui_Text_Input_Handle
{
	char* buffer;
	isize buffer_length, buffer_capacity;
	isize max_chars_by_width;
	isize cursor, selection_start;
	bool active;
	bool accept_newlines;
	bool accept_tabs;
};

void init_text_input_handle(Gui_Text_Input_Handle* handle, char* buf, isize capacity, isize length)
{
	handle->buffer_length = 0;
	handle->cursor = 0;
	handle->selection_start = -1;
	handle->active = false;
	handle->accept_newlines = false;
	handle->accept_tabs = false;
	handle->max_chars_by_width = capacity;
	handle->buffer = buf;
	handle->buffer_capacity = capacity;
	handle->buffer_length = length;
}

void init_text_input_handle(Gui_Text_Input_Handle* handle, real width, Memory_Arena* arena)
{
	isize max_chars_by_width = (isize)(width / Body_Font->glyph_width);
	isize buffer_capacity = max_chars_by_width * 2;
	char* buffer = arena_push_array(arena, char, buffer_capacity);
	init_text_input_handle(handle, buffer, buffer_capacity, 0);
	handle->max_chars_by_width = max_chars_by_width;
}

void gui_add_text_input(Gui_Text_Input_Handle* handle, Vec2 position, Vec2 size)
{
	Vec2 dmouse = v2(
			Input->mouse_x / Game->scale, 
			Input->mouse_y / Game->scale) + Renderer->offset;
	Vec2 text_offset = v2(4, (size.y-Body_Font->glyph_height)/2);
	size.x += 8;
	Sprite s = get_box_sprite(position + size / 2, size, Gui_ButtonDownColor);
	renderer_push_sprite(&s);
	
	if(handle->active) {
		if(Input->mouse[SDL_BUTTON_LEFT] == State_Just_Pressed) {
			if(!aabb_intersect(aabb(position + size / 2, size.x / 2, size.y / 2), aabb(dmouse, 0, 0))) {
				handle->active = false;
			} else {
				Vec2 localmouse = dmouse - position;
				localmouse -= text_offset;
				handle->cursor = roundf(localmouse.x / Body_Font->glyph_width);
			}
		}
	} else {
		if(Input->mouse[SDL_BUTTON_LEFT] == State_Just_Pressed) {
			if(aabb_intersect(aabb(position + size / 2, size.x / 2, size.y / 2), aabb(dmouse, 0, 0))) {
				handle->active = true;
			}
		}
	}

	if(handle->active) {
		Input->capture_newlines = handle->accept_newlines;
		Input->capture_tabs = handle->accept_tabs;
		isize last_len = handle->buffer_length;
		handle->buffer_length = append_input_text(handle->buffer,
				handle->max_chars_by_width,
				handle->buffer_length,
				handle->buffer_length - handle->cursor);
		if(Input->keycodes[SDLK_v] == State_Just_Pressed) {
			if(Input->scancodes[SDL_SCANCODE_LCTRL] == State_Pressed) {
				if(SDL_HasClipboardText()) {
					char* clipboard_text = SDL_GetClipboardText();
					isize text_len = strlen(clipboard_text);
					char* input_text = Input->text;
					isize input_len = Input->text_count;
					Input->text = clipboard_text;
					Input->text_count = text_len;
					handle->buffer_length = append_input_text(handle->buffer,
							handle->buffer_length,
							handle->buffer_length - handle->cursor);
					Input->text = input_text;
					Input->text_count = input_len;
				}
			}
		}
		handle->cursor += handle->buffer_length - last_len;
		//TODO(will) handle repeat with cooldown system
		if(Input->scancodes[SDL_SCANCODE_LEFT] == State_Just_Pressed) {
			if(Input->scancodes[SDL_SCANCODE_LCTRL]) {
				while(handle->cursor >= 0 && isspace(handle->buffer[handle->cursor])) handle->cursor--;
				while(handle->cursor >= 0 && !isspace(handle->buffer[handle->cursor])) handle->cursor--;
			} else {
				handle->cursor--;
			}
		} 
		if(Input->scancodes[SDL_SCANCODE_RIGHT] == State_Just_Pressed) {
			if(Input->scancodes[SDL_SCANCODE_LCTRL]) {
				while(handle->cursor >= 0 && isspace(handle->buffer[handle->cursor])) handle->cursor++;
				while(handle->cursor >= 0 && !isspace(handle->buffer[handle->cursor])) handle->cursor++;
			} else {
				handle->cursor++;
			}
		}

		if(handle->cursor < 0) handle->cursor = 0;
		else if(handle->cursor >= handle->buffer_length) 
			handle->cursor = handle->buffer_length;

		//TODO(will) cursor blinking
		draw_line(text_offset + position + v2(handle->cursor * Body_Font->glyph_width + 1, 0),
				text_offset + position + v2(handle->cursor * Body_Font->glyph_width + 1, Body_Font->glyph_height), v4(1, 1, 1, 1), 1);

	}


	Body_Font->color = v4(1, 1, 1, 1);
	spritefont_render_text(Body_Font,
		handle->buffer, handle->buffer_length,
		position + text_offset); 
	Vec4 color1 = Gui_ButtonOutlineDownColor;
	Vec4 color2 = Gui_ButtonOutlineDownColor2;
	Vec4 colors[4] = {
		color1, color1,
		color2, color2
	};
	draw_box_outline(s.position, s.size, colors, 2);
}

struct Gui_Context
{
	Vec2 last_pos;
	Vec2 next_pos;
	isize axis;
};

