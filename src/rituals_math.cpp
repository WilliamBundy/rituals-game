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

struct Vector2 
{
	real x, y;
};

struct Vector3
{
	real x, y, z;
};

struct Vector4
{
	real x, y, z, w;
};

Vector4 color_to_v4(Color* c)
{
	Vector4 v;
	v.x = clamp_01(c->r / 255.0f);
	v.y = clamp_01(c->g / 255.0f);
	v.z = clamp_01(c->b / 255.0f);
	v.w = clamp_01(c->a / 255.0f);
	return v;
}

Color v4_to_color(Vector4* v)
{
	Color c = {0};
	c.r = (uint8)(clamp_01(v->x) * 255);	
	c.g = (uint8)(clamp_01(v->y) * 255);	
	c.b = (uint8)(clamp_01(v->z) * 255);	
	c.a = (uint8)(clamp_01(v->w) * 255);	
	return c;
}

static inline Vector2 v2(real x, real y)
{
	return Vector2{x, y};
}
static inline Vector2 operator-(Vector2 a)
{
	return Vector2{-a.x, -a.y};
}

static inline Vector2 operator-(Vector2 a, Vector2 b)
{
	return Vector2{a.x - b.x, a.y - b.y};
}

static inline Vector2 operator+(Vector2 a, Vector2 b)
{
	return Vector2{a.x + b.x, a.y + b.y};
}

static inline Vector2 operator*(Vector2 a, real s)
{
	return Vector2{a.x * s, a.y * s};
}

static inline Vector2 operator*(real s, Vector2 a)
{
	return Vector2{a.x * s, a.y * s};
}

static inline Vector2& operator+=(Vector2& a, Vector2 b)
{
	a = a + b;
	return a;
}

static inline Vector2& operator-=(Vector2& a, Vector2 b)
{
	a = a - b;
	return a;
}

static inline Vector2& operator*=(Vector2& a, real b)
{
	a = a * b;
	return a;
}

inline Vector2 v2_perpendicular(Vector2 v)
{
	return Vector2 {
		-v.y, v.x
	};
}

inline real v2_dot(Vector2 a, Vector2 b)
{
	return a.x * b.x + a.y * b.y;
}

inline real v2_cross(Vector2 a, Vector2 b)
{
	return a.x * b.y - a.y * b.x;
}

inline Vector2 v2_from_angle(real normal)
{
	return Vector2 {
		cosf(normal), sinf(normal)
	};
}

struct AABB
{
	Vector2 center;
	real hw, hh;
};

struct Rect2
{
	real x, y, w, h;
};

struct Rect2i
{
	int32 x, y, w, h;
};

union Rectangle
{
	AABB aabb;
	Rect2 rect;
	Rect2i intrect;
};

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

