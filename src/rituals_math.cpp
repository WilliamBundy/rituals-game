/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 * rituals_math.cpp
 * where all the math and geometry 
 */ 

inline real clamp(real x, real min, real max)
{
	if(x < min) x = min;
	else if(x > max) x = max;
	return x;
}

real clamp_01(real x)
{
	return clamp(x, 0.0f, 1.0f);	
}

real lerp(real a, real b, real t)
{
	return (1.0f - t) * a + t * b;
}

union Vec2 
{
	struct {
		real x, y;
	};
	real e[2];
};


static inline Vec2 operator-(Vec2 a)
{
	return Vec2{-a.x, -a.y};
}

static inline Vec2 operator-(Vec2 a, Vec2 b)
{
	return Vec2{a.x - b.x, a.y - b.y};
}

static inline Vec2 operator+(Vec2 a, Vec2 b)
{
	return Vec2{a.x + b.x, a.y + b.y};
}

static inline Vec2 operator*(Vec2 a, real s)
{
	return Vec2{a.x * s, a.y * s};
}

static inline Vec2 operator/(real s, Vec2 a)
{
	return a * (1.0f / s);
}
static inline Vec2 operator/(Vec2 a, real s)
{
	return a * (1.0f / s);
}

static inline Vec2 operator*(real s, Vec2 a)
{
	return Vec2{a.x * s, a.y * s};
}

static inline Vec2 operator*(Vec2 a, Vec2 b)
{
	return Vec2{a.x * b.x, a.y * b.y};
}

static inline Vec2& operator+=(Vec2& a, Vec2 b)
{
	a = a + b;
	return a;
}

static inline Vec2& operator-=(Vec2& a, Vec2 b)
{
	a = a - b;
	return a;
}

static inline Vec2& operator*=(Vec2& a, real b)
{
	a = a * b;
	return a;
}

union Vec2i
{
	struct {
		int32 x, y;
	};
	int32 e[2];
};


union Vec3
{
	struct {
		real x, y, z;
	};
	real e[3];
};

static inline Vec3 v3(real x, real y, real z)
{
	return Vec3{
		x, y, z
	};
}

static inline Vec3 v3(Vec2 a, real z)
{
	return v3(a.x, a.y, z);
}

static inline Vec3 v3(Vec2 a)
{
	return v3(a.x, a.y, 0);
}

static inline Vec3 operator-(Vec3 a)
{
	return Vec3{-a.x, -a.y, -a.z};
}

static inline Vec3 operator-(Vec3 a, Vec3 b)
{
	return Vec3{a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline Vec3 operator+(Vec3 a, Vec3 b)
{
	return Vec3{a.x + b.x, a.y + b.y, a.z + b.z};
}

static inline Vec3 operator*(Vec3 a, real s)
{
	return Vec3{a.x * s, a.y * s, a.z * s};
}

static inline Vec3 operator/(real s, Vec3 a)
{
	return a * (1.0f / s);
}
static inline Vec3 operator/(Vec3 a, real s)
{
	return a * (1.0f / s);
}

static inline Vec3 operator*(real s, Vec3 a)
{
	return Vec3{a.x * s, a.y * s, a.z * s};
}

static inline Vec3 operator*(Vec3 a, Vec3 b)
{
	return Vec3{a.x * b.x, a.y * b.y, a.z * b.z};
}

static inline Vec3& operator+=(Vec3& a, Vec3 b)
{
	a = a + b;
	return a;
}

static inline Vec3& operator-=(Vec3& a, Vec3 b)
{
	a = a - b;
	return a;
}

static inline Vec3& operator*=(Vec3& a, real b)
{
	a = a * b;
	return a;
}


union Vec4
{
	struct {
		real x, y, z, w;
	};
	real e[4];
	
};


static inline Vec4 operator*(Vec4 a, Vec4 b)
{
	return Vec4{a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}

static inline Vec4 operator*(Vec4 a, real b)
{
	return Vec4{a.x * b, a.y * b, a.z * b, a.w * b};
}
static inline Vec4 operator*(real b, Vec4 a) 
{
	return a * b;
}

static inline Vec4 operator/(Vec4 a, real b)
{
	return a * (1.0f/b);
}
static inline Vec4 operator*=(Vec4& a, real b)
{
	a = a * b;
	return a;
}

static inline Vec4 operator*=(Vec4& a, Vec4 b)
{
	a = a * b;
	return a;
}


static inline Vec4 v4(real x, real y, real z, real w)
{
	return Vec4{
		x, y, z, w
	};
}

Vec4 hex_to_v4(uint32 hex) 
{
	Vec4 v;
	v.x = (hex >> 24 & 0xFF) / 255.0f;
	v.y = (hex >> 16 & 0xFF) / 255.0f;
	v.z = (hex >> 8 & 0xFF) / 255.0f;
	v.w = (hex & 0xFF) / 255.0f;
	return v;
}


static inline Vec2 v2(real x, real y)
{
	return Vec2{x, y};
}
static inline Vec2 v2(Vec3 v)
{
	return v2(v.x, v.y);
}

static inline Vec2 v2(Vec2i vi)
{
	return v2(vi.x, vi.y);
}

static inline Vec2i v2i(int32 x, int32 y)
{
	return Vec2i {
		x, y
	};
}



inline Vec2 v2_perpendicular(Vec2 v)
{
	return Vec2 {
		-v.y, v.x
	};
}

inline real v2_dot(Vec2 a, Vec2 b)
{
	return a.x * b.x + a.y * b.y;
}

inline real v2_cross(Vec2 a, Vec2 b)
{
	return a.x * b.y - a.y * b.x;
}

inline Vec2 v2_from_angle(real normal)
{
	return Vec2 {
		cosf(normal), sinf(normal)
	};
}

inline real v2_to_angle(Vec2 v)
{
	return atan2f(v.y, v.x);
}

inline Vec2 v2_normalize(Vec2 v)
{
	real mag = sqrt(v.x*v.x + v.y * v.y);
	return v2(v.x / mag, v.y / mag);
}

union AABB
{
	real e[4];
	struct {
		Vec2 center;
		union {
			struct {
				real hw, hh;
			};
			struct {
				Vec2 hext;
			};
		};
	};
};

#define AABB_x1(b) (b.center.x - b.hw)
#define AABB_x2(b) (b.center.x + b.hw)
#define AABB_y1(b) (b.center.y - b.hh)
#define AABB_y2(b) (b.center.y + b.hh)


union Rect2
{
	struct {
		real x, y, w, h;
	};
	struct {
		Vec2 position, size;
	};
	real e[4];
};


static inline bool operator==(Rect2 a, Rect2 b)
{
	return (a.x == b.x) && (a.y == b.y) && (a.w == b.w) && (a.h == b.h);
}



struct Rect2_Clip_Info
{
	Vec2 rp1;
	Vec2 rp2;
	Vec2 diff1;
	Vec2 diff2;
};



static inline Rect2_Clip_Info rect2_clip(Rect2 r, Rect2 clip)
{
	Vec2 rp1 = r.position;
	Vec2 rp2 = rp1 + r.size;
	Vec2 cp1 = clip.position;
	Vec2 cp2 = cp1 + clip.size;
	Vec2 diff1 = v2(0, 0);
	Vec2 diff2 = v2(0, 0);

	if(rp1.x < cp1.x) {
		diff1.x = cp1.x - rp1.x;
		rp1.x = cp1.x;
	}

	if(rp2.x > cp2.x) {
		diff2.x = rp2.x - cp2.x;
		rp2.x = cp2.x;
	}
	if(rp1.y < cp1.y) {
		diff1.y = cp1.y - rp1.y;
		rp1.y = cp1.y;
	}

	if(rp2.y > cp2.y) {
		diff2.y = rp2.y - cp2.y;
		rp2.y = cp2.y;
	}

	return Rect2_Clip_Info{
		rp1, rp2, diff1, diff2
	};
}

struct Rect2i
{
	int32 x, y, w, h;
};

static inline Rect2i rect2i(int32 x, int32 y, int32 w, int32 h)
{
	return Rect2i {
		x, y, w, h
	};
}

static inline bool aabb_intersect(AABB* a, AABB* b)
{
	if(fabsf(b->center.x - a->center.x) > (b->hw + a->hw)) return false;
	if(fabsf(b->center.y - a->center.y) > (b->hh + a->hh)) return false;
	return true;
}

static inline bool aabb_intersect(AABB a, AABB b)
{
	if(fabsf(b.center.x - a.center.x) > (b.hw + a.hw)) return false;
	if(fabsf(b.center.y - a.center.y) > (b.hh + a.hh)) return false;
	return true;
}

static inline void aabb_overlap(AABB* a, AABB* b, Vec2* s)
{
	s->x = (a->hw + b->hw) - fabsf(b->center.x - a->center.x);
	s->y = (a->hh + b->hh) - fabsf(b->center.y - a->center.y);
	if(s->x > s->y) {
		s->x = 0;
		if(a->center.y > b->center.y) {
			s->y *= -1;
		}
	} else {
		s->y = 0;
		if(a->center.x > b->center.x) {
			s->x *= -1;
		}
	}
}


static inline AABB aabb(Vec2 c, real hw, real hh)
{
	return AABB{
		c, hw, hh
	};
}

static inline AABB aabb(real x, real y, real hw, real hh)
{
	return AABB{
		v2(x, y), hw, hh
	};
}

static inline Rect2 rect2(real x, real y, real w, real h)
{
	return Rect2{
		x, y, w, h
	};
}

static inline Rect2 intrect_to_rect(Rect2i* ir)
{
	Rect2 rr = {0};
	rr.x = (real)ir->w;
	rr.h = (real)ir->h;
	rr.x = (real)ir->x;
	rr.y = (real)ir->y;
	return rr;
}

static inline Rect2i rect_to_intrect(Rect2* ir)
{
	Rect2i rr = {0};
	rr.x = (int32)ir->w;
	rr.h = (int32)ir->h;
	rr.x = (int32)ir->x;
	rr.y = (int32)ir->y;
	return rr;
}

static inline AABB rect_to_aabb(Rect2* r)
{
	AABB b = {0};
	b.hw = r->w / 2;
	b.hh = r->h / 2;
	b.center = v2(r->x + b.hw, r->y + b.hh);
	return b;
}

static inline AABB intrect_to_aabb(Rect2i* r)
{
	Rect2 rr = intrect_to_rect(r);
	return rect_to_aabb(&rr);
} 

static inline Rect2 aabb_to_rect(AABB* b)
{
	Rect2 r = {0};
	r.x = b->center.x - b->hw;
	r.y = b->center.y - b->hh;
	r.w = b->hw * 2;
	r.h = b->hh * 2;
	return r;
}

static inline Rect2i aabb_to_intrect(AABB* b)
{
	Rect2 r = aabb_to_rect(b);
	return rect_to_intrect(&r);
}
static inline isize modulus(isize a, isize b)
{
	if(b == 0) return -1;
	if(a == -b) return 0;
	else if(a < 0) a = b + (a % b);
	return a % b;
}
