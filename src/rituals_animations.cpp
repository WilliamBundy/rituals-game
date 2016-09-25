

/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 * rituals_animations.cpp
 */


struct Animation_Frame
{
	Vec3 position; 
	real angle;
	Vec4 color;
	Vec2 size;
	Rect2 texture;
	real sort_offset;
};

#define AnimationFramesCapacity (256)
struct Animation
{
	isize id;
	Animation_Frame* frames;
	isize frames_count;
	real fps;
	real inv_fps;
	bool looping;
};

struct Animated_Sprite
{
	Animation** animations;
	isize animations_count, animations_capacity;
	isize current_animation, current_frame;
	bool running;
	real timer;
};	

void init_animated_sprite(Animated_Sprite* s, isize capacity, Memory_Arena* arena)
{
	s->animations = arena_push_array(arena, Animation*, capacity);
	s->animations_count = 0;
	s->animations_capacity = capacity;
	s->running = true;
	s->timer = 0.0f;
	s->current_frame = -1;
	s->current_animation = -1;
}

void animated_sprite_update(Animated_Sprite* s, real timestep)
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

