enum Button_State
{
	Button_JustUp = -1,
	Button_Up = 0,
	Button_Down = 1,
	Button_JustDown = 2,

	Button_State_Count
};

#define InputTextCapacity (1024)
struct Game_Input
{
	isize num_keys_down;
	isize num_mouse_down;
	i8* scancodes;
	i8* keycodes;
	i8* mouse;
	i32 mouse_x;
	i32 mouse_y;
	Vec2i screen_mouse_pos;
	Vec2 mouse_pos;
	bool capture_tabs;
	bool capture_newlines;
	char* text;
	isize text_count;
};

void input_init(Game_Input* input, MemoryArena* arena)
{
	input->scancodes = arenaPush<i8, SDL_NUM_SCANCODES>(arena);
	input->keycodes =  arenaPush<i8, SDL_NUM_SCANCODES>(arena);
	input->mouse =  arenaPush<i8, 16>(arena);
	input->mouse_pos = v2(0, 0);
	input->text = arenaPush<char, InputTextCapacity>(arena);
	input->text_count = 0;
	input->capture_newlines = false;
	input->capture_tabs = false;
}

void input_text_append_char(Game_Input* input, char c)
{
	if((input->text_count + 1) < InputTextCapacity) {
		input->text[input->text_count++] = c;
	}
}

isize append_input_text(Game_Input* input, char* str, isize str_cap, isize str_len, isize insert_from_end, MemoryArena* tempArena)
{
	if(input->text_count <= 0) return str_len;
	isize index = str_len;

	MemoryArena* arena = tempArena;
	arenaStartTemp(arena);
	char* extra_chars = NULL;
	if((insert_from_end <= str_len) && (insert_from_end != 0)) {
		index -= insert_from_end;
		if(index >= 0) {
			extra_chars = (char*)arenaPush(arena, insert_from_end + 32);
			memset(extra_chars, 0, insert_from_end + 32);
			memcpy(extra_chars, str + index, insert_from_end);
			extra_chars[insert_from_end] = '\0';
		} else {
			index = 0;
		}
	}

	if((str_len + input->text_count) > str_cap) {
		if(input->text[0] != '\b')
			input->text_count = str_cap - str_len;
	}
	isize bufsize = input->text_count + str_len;
	char* buffer = (char*)arenaPush(arena, bufsize+32);
	memset(buffer, 0, bufsize);
	memcpy(buffer, str, index);
	memcpy(buffer + index, input->text, input->text_count);
	if(extra_chars != NULL) {
		memcpy(buffer + index + input->text_count, extra_chars, insert_from_end);
	}

	isize idx = 0;
	bool contains_bs = false;
	char* buffer2 = NULL; 
	for(isize i = 0; i < input->text_count; ++i) {
		if(input->text[i] == Backspace) {
			contains_bs = true;
			break;
		}
	}
	contains_bs = true;
	if(contains_bs) {
		buffer2 = (char*)arenaPush(arena, bufsize + 32);
		memset(buffer2, 0, bufsize);
		for(isize i = 0; i < bufsize; ++i) {
			if(buffer[i] != Backspace) {
				buffer2[idx] = buffer[i];
				idx++;
			} else if(idx != 0) {
				if(buffer[i+1] == '\7') {
					while(idx >= 0 && isspace(buffer[idx])) idx--;
					while(idx >= 0 && !isspace(buffer[idx])) idx--;
					while(idx >= 0 && isspace(buffer[idx])) idx--;
					idx++;
					i++;
				} else {
					idx--;
				}
			} else {
				if(buffer[i] == Backspace && buffer[i+1] == '\7') {
					i++;
				}
			}
		}
	} else {
		buffer2 = buffer;
		idx = str_len + input->text_count;
	}
	memcpy(str, buffer2, idx);
	
	input->text_count = 0;
	if(idx > str_cap) {
		idx = str_cap;
	}
	arenaEndTemp(arena);
	return idx;
}

