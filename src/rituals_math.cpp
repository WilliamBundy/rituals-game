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

union Color
{
	struct {
		uint8 r, g, b, a;
	};
	uint32 rgba;
};

union Vec2 
{
	struct {
		real x, y;
	};
	real e[2];
};

union Vec2i
{
	struct {
		int32 x, y;
	};
	int32 e[2];
};


struct Vec3
{
	real x, y, z;
};

struct Vec4
{
	real x, y, z, w;
};

Vec4 v4(real x, real y, real z, real w)
{
	return Vec4{
		x, y, z, w
	};
}


Vec4 color_to_v4(Color* c)
{
	Vec4 v;
	v.x = clamp_01(c->r / 255.0f);
	v.y = clamp_01(c->g / 255.0f);
	v.z = clamp_01(c->b / 255.0f);
	v.w = clamp_01(c->a / 255.0f);
	return v;
}

Color v4_to_color(Vec4* v)
{
	Color c = {0};
	c.r = (uint8)(clamp_01(v->x) * 255);	
	c.g = (uint8)(clamp_01(v->y) * 255);	
	c.b = (uint8)(clamp_01(v->z) * 255);	
	c.a = (uint8)(clamp_01(v->w) * 255);	
	return c;
}

static inline Vec2 v2(real x, real y)
{
	return Vec2{x, y};
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

static inline Vec2i v2i(Vec2 v)
{
	return v2i((int32)v.x, (int32)v.y);
}

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

static inline Vec2 operator*(real s, Vec2 a)
{
	return Vec2{a.x * s, a.y * s};
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

struct AABB
{
	
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

struct Rect2
{
	real x, y, w, h;
};

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

union Rectangle
{
	AABB aabb;
	Rect2 rect;
	Rect2i intrect;
};

static inline bool aabb_intersect(AABB* a, AABB* b)
{
	if(fabsf(b->center.x - a->center.x) > (b->hw + a->hw)) return false;
	if(fabsf(b->center.y - a->center.y) > (b->hh + a->hh)) return false;
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

struct Transform
{
	Vec2 position;
	Vec2 center;
	real angle;
	real scale_x;
	real scale_y;
};

union Mat2
{
	struct {
		real a1, b1, a2, b2;
	};
	real e[4];
};

union Mat3
{
	struct {
		real a1, b1, c1, a2, b2, c2, a3, b3, c3;
	};
	real e[9];
};

Vec2 mul_mat3_vec2_affline(Mat3* m, Vec2* v)
{
	real rx = m->a1 * v->x + m->a2 * v->y + m->a3;
	real ry = m->b1 * v->x + m->b2 * v->y + m->b3;
	return v2(rx, ry);
}

//TODO(will) apply scale from transform
Vec2 apply_transform_vec2(Transform* tr, Vec2* v)
{
	real x = v->x - tr->center.x;
	real y = v->y - tr->center.y;
	real lcos = cosf(tr->angle);
	real lsin = sinf(tr->angle);
	real rx =  lcos * x + lsin * y + tr->position.x;
	real ry = -lsin * x + lcos * y + tr->position.y;
	rx += tr->center.x;
	ry += tr->center.y;
	return v2(rx, ry);
}

Vec2 undo_transform_vec2(Transform* tr, Vec2* v)
{
	real x = v->x - tr->center.x;
	real y = v->y - tr->center.y;
	real lcos = cosf(-tr->angle);
	real lsin = sinf(-tr->angle);
	real rx =  lcos * x + lsin * y - tr->position.x;
	real ry = -lsin * x + lcos * y - tr->position.y;
	rx += tr->center.x;
	ry += tr->center.y;
	return v2(rx, ry);
}
	

Mat3 transform_to_mat3(Transform* tr)
{
	Mat3 m = {0};
	m.a1 = cosf(tr->angle);
	m.a2 = sinf(tr->angle);
	m.a3 = tr->position.x;
	m.b1 = -m.a2;
	m.b2 = m.a1;
	m.b3 = tr->position.y;
	return m;
}
