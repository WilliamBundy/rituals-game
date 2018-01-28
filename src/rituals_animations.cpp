struct Animation_Frame
{
	Vec3 position; 
	f32 angle;
	Vec4 color;
	Vec2 size;
	Rect2i texture;
	f32 sort_offset;
};

void init_animation_frame(Animation_Frame* fr)
{
	fr->position = v3(0, 0, 0);
	fr->angle = 0;
	fr->color = v4(1, 1, 1, 1);
	fr->size = v2(0, 0);
	fr->texture = rect2i(0, 0, 0, 0);
	fr->sort_offset = 0;
}

struct Animation
{
	isize id;
	Animation_Frame* frames;
	isize frames_count, frames_capacity;
	f32 fps;
	f32 inv_fps;
	bool looping;
};

void init_animation(Animation* a, f32 fps, isize capacity, MemoryArena* arena)
{
	a->id = -1;
	a->frames = (Animation_Frame*)arenaPush(arena, sizeof(Animation_Frame)* capacity);
	a->frames_count = 0;
	a->frames_capacity = 0;
	a->fps = fps;
	a->inv_fps = 1.0f / fps;
	a->looping = true;
}

Animation* make_animaiton_from_strip(MemoryArena* arena, f32 fps, Rect2i frame, isize count) 
{
	Animation* anim = (Animation*)arenaPush(arena, sizeof(Animation));
	init_animation(anim, fps, count, arena);
	anim->frames_count = count;

	for(isize i = 0; i < count; ++i) {
		Animation_Frame* fr = anim->frames + i;
		init_animation_frame(fr);
		fr->texture = frame;
		fr->size = v2(frame.w, frame.h);
		frame.x += frame.w;
	}

	return anim;
}

#ifndef REFLECTED
struct Animated_Sprite
{
	Animation** animations;
	isize animations_count, animations_capacity;
	isize current_animation, current_frame;
	bool running;
	f32 timer;
};	
#endif

void init_animated_sprite(Animated_Sprite* s, isize capacity, MemoryArena* arena)
{
	s->animations = (Animation**)arenaPush(arena, sizeof(Animation*) * capacity);
	s->animations_count = 0;
	s->animations_capacity = capacity;
	s->running = true;
	s->timer = 0.0f;
	s->current_frame = -1;
	s->current_animation = -1;
}

void animated_sprite_update(Animated_Sprite* s, f32 timestep)
{
	s->timer += timestep;
	Animation* anim = s->animations[s->current_animation];

	if(s->timer > anim->inv_fps) {
		s->current_frame++;
		s->timer = 0;
		if(s->current_frame >= anim->frames_count) {
			if(anim->looping) {
				s->current_frame = 0;
			} else {
				s->current_frame = anim->frames_count - 1;
			}
		}
	}
}


isize add_animation(Animated_Sprite* s, Animation* a) 
{
	if(s->animations_count >= s->animations_capacity) {
		return -1;
	}
	s->current_animation = s->animations_count;
	s->current_frame = 0;
	a->id = s->animations_count;
	s->animations[s->animations_count++] = a;

	return s->animations_count - 1;
}

isize play_animation(Animated_Sprite* s, isize anim_index)
{
	if(anim_index < 0 || anim_index >= s->animations_count) {
		return 0;
	}
	if(s->current_animation == anim_index) {
		Animation* anim = s->animations[s->current_animation];
		if(!anim->looping) {
			s->current_frame = 0;
		}
		return 0;
	} 
	s->current_frame = 0;
	s->current_animation = anim_index;
	s->timer = 0;
	return 1;
}

