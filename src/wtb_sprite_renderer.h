/* 
 * wtb_sprite_renderer.h
 * In the public domain, otherwise
 * Copyright 2016, William Bundy
 * Under the MIT and WTFPL licences.
 */ 


enum Sprite_Anchor
{
	Anchor_Center = 0,
	Anchor_Top_Left = 1,
	Anchor_Top = 2,
	Anchor_Top_Right = 3,
	Anchor_Right = 4,
	Anchor_Bottom_Right = 5,
	Anchor_Bottom = 6 ,
	Anchor_Bottom_Left = 7,
	Anchor_Left = 8
};

real SpriteAnchorX[] = {
	0.0f,
	-0.5f,
	0.0f,
	0.5f, 
	0.5f,
	0.5f, 
	0.0f, 
	-0.5f,
	-0.5f
};

real SpriteAnchorY[] = {
	0.0f,
	-0.5f,
	-0.5f,
	-0.5f,
	0.0f,
	0.5f,
	0.5f,
	0.5f,
	0.0f
};


struct Sprite
{
	Vec2 position;
	Vec2 center;
	real angle;
	Vec2 size;
	Rect2 texture;
	Vec4 color;
	uint32 anchor;
	Vec2 sort_point_offset;
};

struct Draw_List
{
	Vec2 origin;

	Sprite* sprites;
	isize sprites_count;
};

struct Renderer
{
	GLuint shader_program, vbo, vao, texture;
	isize u_screen, u_texturesize, u_window, u_orthomat;
	
	Draw_List* draw_lists;
	isize draw_lists_count;
};

void render_start(Renderer* r, isize list_index)
{

}

void render_add(Renderer* r, Sprite* sprite, isize list_index)
{
	Draw_List* list = r->draw_lists + list_index;
	list->sprites[list->sprites_count++] = *sprite;
}

void render_draw(Renderer* r, isize list_index)
{
	Draw_List* list = r->draw_lists + list_index;
	glBindVertexArray(r->vao);
	glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
	glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STREAM_DRAW);
	glBufferData(GL_ARRAY_BUFFER, list->sprites_count * sizeof(Sprite), list->sprites, GL_STREAM_DRAW);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, list->sprites_count);
	glBindVertexArray(0);
}



