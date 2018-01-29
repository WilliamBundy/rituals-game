#ifndef WirmphtEnabled
struct Sprite
{
	i32 flags;
	u32 color;

	union { 
		Vec2 pos;
		struct {f32 x, y;};
	};
	union { Vec2 size; struct{f32 w, h;};};
	union { Vec2 center; struct{f32 cx, cy;};};
	union {
		struct {i16 tx, ty, tw, th;};
		Rect2i texture;
	};
	f32 angle;
	f32 sdf;
	f32 sort_offset;
};

struct wVertex
{
	f32 kind;
	f32 x, y;
	f32 u, v;
	f32 sx, sy;
	u32 color;
};

struct wShader
{
	u32 vert, frag, program;

	i32 uTint;
	i32 uInvTextureSize;
	i32 uPxRange;
};

struct wTexture
{
	i32 w, h;
	u32* pixels;
	u32 glIndex;
};

struct SpriteList
{
	i32 start, count;
	f32 l, b, r, t;
};

struct wRenderGroup
{
	wTexture* texture;
	wShader* shader;
	u32 vao, vbo;

	i32 blank;
	i32 clearOnDraw;

	f32 dpi;
	f32 scale;
	f32 offsetX, offsetY;
	u32 tint;

	float sdfPxRange;

	Sprite* sprites;
	wVertex* verts;
	u16* indices;
	i32 count, capacity;
};
#endif 
typedef struct Sprite Sprite;
enum SpriteFlags
{
	Anchor_Center = 0,
	Anchor_TopLeft = 1,
	Anchor_TopCenter = 2,
	Anchor_TopRight = 3,
	Anchor_RightCenter = 4,
	Anchor_BottomRight = 5,
	Anchor_BottomCenter = 6,
	Anchor_BottomLeft = 7,
	Anchor_LeftCenter = 8,
	Sprite_Hidden = 1<<4,
	Sprite_NoTexture = 1<<5,
	Sprite_RotateCW = 1<<6,
	Sprite_RotateCCW = 1<<7,
	Sprite_FlipHoriz = 1<<8,
	Sprite_FlipVert = 1<<9,
	Sprite_NoAA = 1<<10,
	Sprite_MSDF = 1<<11,
};

wTexture* wLoadTexture(const char* basePath, const char* filename, MemoryArena* arena)
{
	int t = 0;
	wTexture texture = {0};
	char buf[2048];
	snprintf(buf, 2048, "%s%s", basePath, filename);
	i32 w, h, bpp;
	u8* data = stbi_load(buf, &w, &h, &bpp, STBI_rgb_alpha);
	if(w == 0 || h == 0) {
		fprintf(stderr, "Error: Unable to load image %s\n", buf);
		return NULL;
	}

	texture.w = w;
	texture.h = h;
	texture.pixels = (u32*)data;

	wTexture* ptex = (wTexture*)arenaPush(arena, sizeof(wTexture));
	*ptex = texture;
	return ptex;
}

void wUploadTexture(wTexture* texture)
{
	glGenTextures(1, &texture->glIndex);
	glBindTexture(GL_TEXTURE_2D, texture->glIndex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
			texture->w, texture->h, 0, 
			GL_RGBA, GL_UNSIGNED_BYTE, 
			texture->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

static
void wInitDefaultShader(const char* vertShader, const char* fragShader, wShader* shader)
{
	u32 vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vertShader, NULL);
	glCompileShader(vert);
	u32 frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragShader, NULL);
	glCompileShader(frag);
	shader->vert = vert;
	shader->frag = frag;
	shader->program = glCreateProgram();

	glAttachShader(shader->program, vert);
	glBindAttribLocation(shader->program, 0, "vKind");
	glBindAttribLocation(shader->program, 1, "vPos");
	glBindAttribLocation(shader->program, 2, "vUV");
	glBindAttribLocation(shader->program, 3, "vScale");
	glBindAttribLocation(shader->program, 4, "vColor");
	glAttachShader(shader->program, frag);
	glLinkProgram(shader->program);

	glUseProgram(shader->program);
	shader->uInvTextureSize = glGetUniformLocation(
			shader->program, "uInvTextureSize");
	shader->uTint = glGetUniformLocation(shader->program, "uTint");
	shader->uPxRange = glGetUniformLocation(shader->program, "uPxRange");
}

void wInitSprite(Sprite* s)
{
	s->flags = 0;
	s->color = 0xFFFFFFFF;
	s->pos = v2(0, 0);
	s->size = v2(0, 0);
	s->center = v2(0, 0);
	s->tx = 0;
	s->ty = 0;
	s->tw = 0;
	s->th = 0;
	s->sdf = 1.0f;
}

Sprite* wGroupAddRaw(
		wRenderGroup* group,
		i32 flags,
		u32 color,
		f32 x, f32 y,
		f32 w, f32 h,
		i16 tx, i16 ty, i16 tw, i16 th)
{
	Sprite s;
	wInitSprite(&s);
	s.flags = flags;
	s.color = color;
	s.pos = v2(x, y);
	s.size = v2(w, h);
	s.center = v2(0, 0);
	s.tx = tx;
	s.ty = ty;
	s.tw = tw;
	s.th = th;
	group->sprites[group->count++] = s;
	return group->sprites + group->count - 1;
}

Sprite spriteMake(
		i32 flags,
		u32 color,
		Vec2 pos, 
		f32 w, f32 h,
		i16 tx, i16 ty, i16 tw, i16 th)
{
	Sprite s;
	wInitSprite(&s);
	s.flags = flags;
	s.color = color;
	s.pos = pos;
	s.size = v2(w, h);
	s.center = v2(0, 0);
	s.tx = tx;
	s.ty = ty;
	s.tw = tw;
	s.th = th;
	return s;
}

Sprite* wGetSprite(wRenderGroup* group)
{
	Sprite* s = group->sprites + group->count++;
	wInitSprite(s);
	return s;
}

void wGroupInit(wRenderGroup* group, i32 cap, wShader* shader, wTexture* texture)
{
	group->dpi = 72.0f;
	group->scale = 1;
	group->clearOnDraw = 1;
	group->offsetX = 0;
	group->offsetY = 0;
	group->tint = 0xFFFFFFFF;

	group->sdfPxRange = 8.0f;
	group->texture = texture;
	group->shader = shader;

	if(!texture->glIndex) {
		wUploadTexture(texture);
	}

	group->capacity = cap;
	group->sprites = (Sprite*)malloc(sizeof(Sprite) * group->capacity);
	group->verts = (wVertex*)malloc(sizeof(wVertex) * 4 * group->capacity);
	group->indices = (u16*)malloc(sizeof(u16) * 6 * group->capacity);
	group->count = 0;

	glGenBuffers(1, &group->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, group->vbo);

	i32 i = 0;
	i32 stride = sizeof(wVertex);
	#define primMember(name) (void*)offsetof(wVertex, name)

	glVertexAttribPointer(i, 1, GL_FLOAT, 0, stride, primMember(kind));
	glEnableVertexAttribArray(i);
	i++;

	glVertexAttribPointer(i, 2, GL_FLOAT, 0, stride, primMember(x));
	glEnableVertexAttribArray(i);
	i++;

	glVertexAttribPointer(i, 2, GL_FLOAT, 0, stride, primMember(u));
	glEnableVertexAttribArray(i);
	i++;

	glVertexAttribPointer(i, 2, GL_FLOAT, 0, stride, primMember(sx));
	glEnableVertexAttribArray(i);
	i++;

	glVertexAttribPointer(i, 4, GL_UNSIGNED_BYTE, 1, stride, primMember(color));
	glEnableVertexAttribArray(i);
	i++;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


#define GetSpriteSortIndex(s) (s.pos.y + s.sort_offset)
void wGroupSort(wRenderGroup* group, isize start, isize end)
{
	Sprite* array = group->sprites + start;
	isize count = end - start;
	for(isize i = 1; i < count; ++i) {
		isize j = i - 1;
		f32 tempY = GetSpriteSortIndex(array[i]);
		if(GetSpriteSortIndex(array[j]) > tempY) {
			Sprite temp = array[i];
			while((j >= 0) && (GetSpriteSortIndex(array[j]) > tempY)) {
				array[j + 1] = array[j];
				j--;
			}
			array[j + 1] = temp;
		}
	}
}

float SoffsetX[] = {0.0, 0.5, 0.0, -0.5, -0.5, -0.5,  0.0,  0.5, 0.5};
float SoffsetY[] = {0.0, 0.5, 0.5,  0.5,  0.0, -0.5, -0.5, -0.5, 0.0};

static
void groupProcessSprites(f32 width, f32 height, wRenderGroup* group)
{
	vf128 groupScale = _mm_set_ps1(group->scale);
	vf128 offsetXs = _mm_set_ps1(group->offsetX);
	vf128 offsetYs = _mm_set_ps1(group->offsetY);
	vf128 viewportXs = _mm_set_ps1(1.0f / width);
	vf128 viewportYs = _mm_set_ps1(1.0f / height);
	for(isize i = 0; i < group->count; ++i) {
		Sprite* s = group->sprites + i;
		if(s->flags & Sprite_Hidden) continue;
		isize i4 = i * 4;
		wVertex* p = group->verts + i4;
		u16* indexes = group->indices + (i*6);
		indexes[0] = i4 + 0;
		indexes[1] = i4 + 1;
		indexes[2] = i4 + 2;
		indexes[3] = i4 + 1;
		indexes[4] = i4 + 2;
		indexes[5] = i4 + 3;

		f32 uvrect[4];
		uvrect[0] = (f32)s->tx;
		uvrect[1] = (f32)s->ty;
		uvrect[2] = (f32)(s->tx + s->tw); 
		uvrect[3] = (f32)(s->ty + s->th);

		int f = s->flags & 0xf;
		vf128 xs = _mm_add_ps(_mm_set_ps(-0.5, -0.5, 0.5, 0.5),
				_mm_set1_ps(SoffsetX[f]));
		vf128 ys = _mm_add_ps(_mm_set_ps(0.5, -0.5, 0.5, -0.5),
				_mm_set1_ps(SoffsetY[f]));
		vf128 uvxs = _mm_set_ps(uvrect[0], uvrect[0], uvrect[2], uvrect[2]);
		vf128 uvys = _mm_set_ps(uvrect[3], uvrect[1], uvrect[3], uvrect[1]);

		if(s->flags & Sprite_FlipHoriz) {
			uvxs = vfShuffle(uvxs, 2, 3, 0, 1);
		}

		if(s->flags & Sprite_FlipVert) {
			uvys = vfShuffle(uvys, 2, 3, 0, 1);
		}

		f32 scaleX = s->size.x;
		f32 scaleY = s->size.y;
		if(s->flags & Sprite_RotateCW) {
			uvxs = vfShuffle(uvxs, 3, 1, 2, 0);
			uvys = vfShuffle(uvys, 3, 1, 2, 0);
			scaleX = s->size.y;
			scaleY = s->size.x;
		}

		if(s->flags & Sprite_RotateCCW) {
			uvxs = vfShuffle(uvxs, 2, 0, 3, 1);
			uvys = vfShuffle(uvys, 2, 0, 3, 1);
			scaleX = s->size.y;
			scaleY = s->size.x;
		}

		{
			vf128 scaleXs = _mm_set_ps1(scaleX * group->scale);
			vf128 scaleYs = _mm_set_ps1(scaleY * group->scale);
			xs = _mm_mul_ps(xs, scaleXs);
			ys = _mm_mul_ps(ys, scaleYs);
		}

		{
			vf128 pxs = _mm_set_ps1(s->pos.x);
			pxs = _mm_mul_ps(pxs, groupScale);
			xs = _mm_add_ps(xs, pxs);
			xs = _mm_sub_ps(xs, offsetXs);

			vf128 pys = _mm_set_ps1(s->pos.y);
			pys = _mm_mul_ps(pys, groupScale);
			ys = _mm_add_ps(ys, pys);
			ys = _mm_sub_ps(ys, offsetYs);
		}

		/* Normalize the position to -1, 1 based on the window size
		 * Essentially the orthographic matrix transform, flattened
		 * pos * vec2(2, -2) / viewportWH - vec2(1, -1)
		 */
		{
			vf128 number = _mm_set_ps1(2);
			xs = _mm_mul_ps(xs, number);
			xs = _mm_mul_ps(xs, viewportXs);
			number = _mm_set_ps1(1);
			xs = _mm_sub_ps(xs, number);

			number = _mm_set_ps1(-2);
			ys = _mm_mul_ps(ys, number);
			ys = _mm_mul_ps(ys, viewportYs);
			number = _mm_set_ps1(-1);
			ys = _mm_sub_ps(ys, number);
		}

		vf32x4 x = {xs}, y = {ys}, uvx = {uvxs}, uvy = {uvys};
		f32 ssx = x.f[2] - x.f[0];
		ssx /= s->tw;
		f32 ssy = y.f[1] - y.f[0];
		ssy /= s->th;
		for(isize j = 0; j < 4; ++j) {
			p[j].x = x.f[j];
			p[j].y = y.f[j];
			p[j].u = uvx.f[j];
			p[j].v = uvy.f[j];
			p[j].color = s->color; 
			p[j].sx = ssx;
			p[j].sy = ssy;
			if(s->flags & Sprite_MSDF) {
				p[j].kind = s->sdf;
			} else if(s->flags & Sprite_NoTexture) {
				p[j].kind = 40.0f;
			} else if(s->flags & Sprite_NoAA) {
				p[j].kind = 11.0f;
			} else {
				p[j].kind = 16.0f;
			}
		}
	}
}

void wGroupDraw(i32 width, i32 height, wRenderGroup* group)
{
	if(group->count == 0) return;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	wShader* shader = group->shader;
	glUseProgram(shader->program);

	glUniform2f(shader->uInvTextureSize,
			1.0f / (f32)group->texture->w, 
			1.0f / (f32)group->texture->h);
	glUniform4f(shader->uTint, 
			(f32)(group->tint & 0xFF) / 255.0f, 
			(f32)((group->tint >> 8) & 0xFF) / 255.0f,
			(f32)((group->tint >> 16) & 0xFF) / 255.0f,
			(f32)((group->tint >> 24) & 0xFF) / 255.0f);
	glUniform1f(shader->uPxRange, group->sdfPxRange);

	glBindTexture(GL_TEXTURE_2D, group->texture->glIndex);

	groupProcessSprites(width, height, group);
	glBindBuffer(GL_ARRAY_BUFFER, group->vbo);
	glBufferData(GL_ARRAY_BUFFER,
			sizeof(wVertex) * 4 * group->count,
			group->verts,
			GL_STREAM_DRAW);

	glDrawElements(GL_TRIANGLES,
			group->count * 6,
			GL_UNSIGNED_SHORT,
			group->indices);
		
	if(group->clearOnDraw) {
		group->count = 0;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

