#line 1 "src\\rituals_main.cpp"












 









#line 24 "src\\rituals_main.cpp"

 





































typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float real32;
typedef double real64;




typedef real32 real;




#line 85 "src\\rituals_main.cpp"

typedef ptrdiff_t isize;
typedef size_t usize;








































#line 1 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_math.cpp"













 

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
	real e[4];
};







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
#line 129 "src\\rituals_main.cpp"
#line 1 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_game.cpp"



























$(exclude)
#line 1 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_sort_macros.cpp"




































































































 





void real_sort(real* array, isize count) { if(count > 1) { real tmp = array[0]; array[0] = array[count / 2]; array[count / 2] = tmp; isize pivot = 0; for(isize i = 1; i < count; ++i) { if((array[i]) < (array[0])) { tmp = array[++pivot]; array[pivot] = array[i]; array[i] = tmp; } } tmp = array[0]; array[0] = array[pivot]; array[pivot] = tmp; real_sort(array, pivot); real_sort(array + pivot + 1, count - (pivot + 1)); } } isize real_search(real key, real* array, isize count) { if(count == 0) return -1; isize min = 0, max = count - 1, mid = 0; real localkey; while(min <= max) { mid = (min + max) / 2; localkey = (array[mid]); if(localkey == key) { return mid; } else if(localkey < key) { min = mid + 1; } else { max = mid - 1; } } return -1; };
void real32_sort(real32* array, isize count) { if(count > 1) { real32 tmp = array[0]; array[0] = array[count / 2]; array[count / 2] = tmp; isize pivot = 0; for(isize i = 1; i < count; ++i) { if((array[i]) < (array[0])) { tmp = array[++pivot]; array[pivot] = array[i]; array[i] = tmp; } } tmp = array[0]; array[0] = array[pivot]; array[pivot] = tmp; real32_sort(array, pivot); real32_sort(array + pivot + 1, count - (pivot + 1)); } } isize real32_search(real32 key, real32* array, isize count) { if(count == 0) return -1; isize min = 0, max = count - 1, mid = 0; real32 localkey; while(min <= max) { mid = (min + max) / 2; localkey = (array[mid]); if(localkey == key) { return mid; } else if(localkey < key) { min = mid + 1; } else { max = mid - 1; } } return -1; };
void real64_sort(real64* array, isize count) { if(count > 1) { real64 tmp = array[0]; array[0] = array[count / 2]; array[count / 2] = tmp; isize pivot = 0; for(isize i = 1; i < count; ++i) { if((array[i]) < (array[0])) { tmp = array[++pivot]; array[pivot] = array[i]; array[i] = tmp; } } tmp = array[0]; array[0] = array[pivot]; array[pivot] = tmp; real64_sort(array, pivot); real64_sort(array + pivot + 1, count - (pivot + 1)); } } isize real64_search(real64 key, real64* array, isize count) { if(count == 0) return -1; isize min = 0, max = count - 1, mid = 0; real64 localkey; while(min <= max) { mid = (min + max) / 2; localkey = (array[mid]); if(localkey == key) { return mid; } else if(localkey < key) { min = mid + 1; } else { max = mid - 1; } } return -1; };
void uint8_sort(uint8* array, isize count) { if(count > 1) { uint8 tmp = array[0]; array[0] = array[count / 2]; array[count / 2] = tmp; isize pivot = 0; for(isize i = 1; i < count; ++i) { if((array[i]) < (array[0])) { tmp = array[++pivot]; array[pivot] = array[i]; array[i] = tmp; } } tmp = array[0]; array[0] = array[pivot]; array[pivot] = tmp; uint8_sort(array, pivot); uint8_sort(array + pivot + 1, count - (pivot + 1)); } } isize uint8_search(uint8 key, uint8* array, isize count) { if(count == 0) return -1; isize min = 0, max = count - 1, mid = 0; uint8 localkey; while(min <= max) { mid = (min + max) / 2; localkey = (array[mid]); if(localkey == key) { return mid; } else if(localkey < key) { min = mid + 1; } else { max = mid - 1; } } return -1; };
void uint16_sort(uint16* array, isize count) { if(count > 1) { uint16 tmp = array[0]; array[0] = array[count / 2]; array[count / 2] = tmp; isize pivot = 0; for(isize i = 1; i < count; ++i) { if((array[i]) < (array[0])) { tmp = array[++pivot]; array[pivot] = array[i]; array[i] = tmp; } } tmp = array[0]; array[0] = array[pivot]; array[pivot] = tmp; uint16_sort(array, pivot); uint16_sort(array + pivot + 1, count - (pivot + 1)); } } isize uint16_search(uint16 key, uint16* array, isize count) { if(count == 0) return -1; isize min = 0, max = count - 1, mid = 0; uint16 localkey; while(min <= max) { mid = (min + max) / 2; localkey = (array[mid]); if(localkey == key) { return mid; } else if(localkey < key) { min = mid + 1; } else { max = mid - 1; } } return -1; };
void uint32_sort(uint32* array, isize count) { if(count > 1) { uint32 tmp = array[0]; array[0] = array[count / 2]; array[count / 2] = tmp; isize pivot = 0; for(isize i = 1; i < count; ++i) { if((array[i]) < (array[0])) { tmp = array[++pivot]; array[pivot] = array[i]; array[i] = tmp; } } tmp = array[0]; array[0] = array[pivot]; array[pivot] = tmp; uint32_sort(array, pivot); uint32_sort(array + pivot + 1, count - (pivot + 1)); } } isize uint32_search(uint32 key, uint32* array, isize count) { if(count == 0) return -1; isize min = 0, max = count - 1, mid = 0; uint32 localkey; while(min <= max) { mid = (min + max) / 2; localkey = (array[mid]); if(localkey == key) { return mid; } else if(localkey < key) { min = mid + 1; } else { max = mid - 1; } } return -1; };
void uint64_sort(uint64* array, isize count) { if(count > 1) { uint64 tmp = array[0]; array[0] = array[count / 2]; array[count / 2] = tmp; isize pivot = 0; for(isize i = 1; i < count; ++i) { if((array[i]) < (array[0])) { tmp = array[++pivot]; array[pivot] = array[i]; array[i] = tmp; } } tmp = array[0]; array[0] = array[pivot]; array[pivot] = tmp; uint64_sort(array, pivot); uint64_sort(array + pivot + 1, count - (pivot + 1)); } } isize uint64_search(uint64 key, uint64* array, isize count) { if(count == 0) return -1; isize min = 0, max = count - 1, mid = 0; uint64 localkey; while(min <= max) { mid = (min + max) / 2; localkey = (array[mid]); if(localkey == key) { return mid; } else if(localkey < key) { min = mid + 1; } else { max = mid - 1; } } return -1; };
void int8_sort(int8* array, isize count) { if(count > 1) { int8 tmp = array[0]; array[0] = array[count / 2]; array[count / 2] = tmp; isize pivot = 0; for(isize i = 1; i < count; ++i) { if((array[i]) < (array[0])) { tmp = array[++pivot]; array[pivot] = array[i]; array[i] = tmp; } } tmp = array[0]; array[0] = array[pivot]; array[pivot] = tmp; int8_sort(array, pivot); int8_sort(array + pivot + 1, count - (pivot + 1)); } } isize int8_search(int8 key, int8* array, isize count) { if(count == 0) return -1; isize min = 0, max = count - 1, mid = 0; int8 localkey; while(min <= max) { mid = (min + max) / 2; localkey = (array[mid]); if(localkey == key) { return mid; } else if(localkey < key) { min = mid + 1; } else { max = mid - 1; } } return -1; };
void int16_sort(int16* array, isize count) { if(count > 1) { int16 tmp = array[0]; array[0] = array[count / 2]; array[count / 2] = tmp; isize pivot = 0; for(isize i = 1; i < count; ++i) { if((array[i]) < (array[0])) { tmp = array[++pivot]; array[pivot] = array[i]; array[i] = tmp; } } tmp = array[0]; array[0] = array[pivot]; array[pivot] = tmp; int16_sort(array, pivot); int16_sort(array + pivot + 1, count - (pivot + 1)); } } isize int16_search(int16 key, int16* array, isize count) { if(count == 0) return -1; isize min = 0, max = count - 1, mid = 0; int16 localkey; while(min <= max) { mid = (min + max) / 2; localkey = (array[mid]); if(localkey == key) { return mid; } else if(localkey < key) { min = mid + 1; } else { max = mid - 1; } } return -1; };
void int32_sort(int32* array, isize count) { if(count > 1) { int32 tmp = array[0]; array[0] = array[count / 2]; array[count / 2] = tmp; isize pivot = 0; for(isize i = 1; i < count; ++i) { if((array[i]) < (array[0])) { tmp = array[++pivot]; array[pivot] = array[i]; array[i] = tmp; } } tmp = array[0]; array[0] = array[pivot]; array[pivot] = tmp; int32_sort(array, pivot); int32_sort(array + pivot + 1, count - (pivot + 1)); } } isize int32_search(int32 key, int32* array, isize count) { if(count == 0) return -1; isize min = 0, max = count - 1, mid = 0; int32 localkey; while(min <= max) { mid = (min + max) / 2; localkey = (array[mid]); if(localkey == key) { return mid; } else if(localkey < key) { min = mid + 1; } else { max = mid - 1; } } return -1; };
void int64_sort(int64* array, isize count) { if(count > 1) { int64 tmp = array[0]; array[0] = array[count / 2]; array[count / 2] = tmp; isize pivot = 0; for(isize i = 1; i < count; ++i) { if((array[i]) < (array[0])) { tmp = array[++pivot]; array[pivot] = array[i]; array[i] = tmp; } } tmp = array[0]; array[0] = array[pivot]; array[pivot] = tmp; int64_sort(array, pivot); int64_sort(array + pivot + 1, count - (pivot + 1)); } } isize int64_search(int64 key, int64* array, isize count) { if(count == 0) return -1; isize min = 0, max = count - 1, mid = 0; int64 localkey; while(min <= max) { mid = (min + max) / 2; localkey = (array[mid]); if(localkey == key) { return mid; } else if(localkey < key) { min = mid + 1; } else { max = mid - 1; } } return -1; };
void usize_sort(usize* array, isize count) { if(count > 1) { usize tmp = array[0]; array[0] = array[count / 2]; array[count / 2] = tmp; isize pivot = 0; for(isize i = 1; i < count; ++i) { if((array[i]) < (array[0])) { tmp = array[++pivot]; array[pivot] = array[i]; array[i] = tmp; } } tmp = array[0]; array[0] = array[pivot]; array[pivot] = tmp; usize_sort(array, pivot); usize_sort(array + pivot + 1, count - (pivot + 1)); } } isize usize_search(usize key, usize* array, isize count) { if(count == 0) return -1; isize min = 0, max = count - 1, mid = 0; usize localkey; while(min <= max) { mid = (min + max) / 2; localkey = (array[mid]); if(localkey == key) { return mid; } else if(localkey < key) { min = mid + 1; } else { max = mid - 1; } } return -1; };
void isize_sort(isize* array, isize count) { if(count > 1) { isize tmp = array[0]; array[0] = array[count / 2]; array[count / 2] = tmp; isize pivot = 0; for(isize i = 1; i < count; ++i) { if((array[i]) < (array[0])) { tmp = array[++pivot]; array[pivot] = array[i]; array[i] = tmp; } } tmp = array[0]; array[0] = array[pivot]; array[pivot] = tmp; isize_sort(array, pivot); isize_sort(array + pivot + 1, count - (pivot + 1)); } } isize isize_search(isize key, isize* array, isize count) { if(count == 0) return -1; isize min = 0, max = count - 1, mid = 0; isize localkey; while(min <= max) { mid = (min + max) / 2; localkey = (array[mid]); if(localkey == key) { return mid; } else if(localkey < key) { min = mid + 1; } else { max = mid - 1; } } return -1; };


#line 30 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_game.cpp"
$(end)
#line 1 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_memory.cpp"
















struct Memory_Arena
{
	uint8* data;
	isize capacity, head;
	isize temp_head;
	Memory_Arena* next;
};

static inline isize mem_align_4(isize p)
{
	usize mod = p & 3;
	return (mod ? p + 4 - mod : p);
}

void init_memory_arena(Memory_Arena* arena, usize size)
{


#line 36 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_memory.cpp"

#line 38 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_memory.cpp"
	arena->capacity = (isize)size;
	arena->head = (isize)arena->data;
	arena->temp_head = -1;
	arena->next = NULL;
}

uint8* arena_push(Memory_Arena* arena, isize size)
{
	isize old_head = arena->head;
	isize new_head = mem_align_4(size + arena->head);
	if((new_head - (isize)arena->data) > arena->capacity) {
		printf("There was an error: %s \n", "An arena was filled");;
		return NULL;
	}
	arena->head = new_head;
	return (uint8*) old_head;

}

void start_temp_arena(Memory_Arena* arena)
{
	arena->temp_head = arena->head;
}

void end_temp_arena(Memory_Arena* arena)
{




#line 69 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_memory.cpp"

#line 71 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_memory.cpp"
	arena->head = arena->temp_head;
	arena->temp_head = -1;
}

void clear_arena(Memory_Arena* arena)
{
	



#line 82 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_memory.cpp"


#line 85 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_memory.cpp"

	if(arena->data == NULL) {
		printf("There was an error: %s \n", "There was an error recommitting memory");;
	}
	
	arena->head = (isize)arena->data;
	arena->temp_head = -1;
}

Memory_Arena* new_memory_arena(usize size, Memory_Arena* src)
{
	Memory_Arena* arena = (Memory_Arena*)arena_push(src, sizeof(Memory_Arena));
	init_memory_arena(arena, size);
	return arena;
}

#line 32 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_game.cpp"
#line 1 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_random.cpp"












static inline uint64 _splitmix64(uint64* x)
{
	*x += UINT64_C(0x9E3779B97F4A7C15);
	uint64 z = *x;
	z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
	z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
	return z ^ (z >> 31);	
}

static inline uint64 _rotate_left(const uint64 t, int64 k)
{
	return (t << k) | (t >> (64 - k));
}

struct Random
{
	uint64 x, y;
};

uint64 next_random_uint64(Random* r)
{
	uint64 a = r->x;
	uint64 b = r->y;
	uint64 result = a + b;

	b ^= a;
	r->x = _rotate_left(a, 55) ^ b ^ (b << 14);
	r->y = _rotate_left(b, 36);
	return result;
}

void init_random(Random* r, uint64 seed) 
{
	_splitmix64(&seed);
	r->x = _splitmix64(&seed);
	r->y = _splitmix64(&seed);
	next_random_uint64(r);
}

real64 next_random_double(Random* r)
{
	uint64 x = next_random_uint64(r);
	return (real64)x / UINT64_MAX;
}

real32 next_random_float(Random* r)
{
	return (real32)(next_random_double(r));
}

real next_random(Random* r) 
{
	return (real)(next_random_double(r));
}

real rand_range(Random* r, real min, real max)
{
	return next_random_double(r) * (max - min) + min;
}

int32 rand_range_int(Random* r, real min, real max)
{
	return (int32)(next_random_double(r) * (max - min) + min);
}

#line 33 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_game.cpp"

enum Button_State
{
	State_Just_Released = -1,
	State_Released = 0,
	State_Pressed = 1,
	State_Just_Pressed = 2,

	Button_State_Count
};


struct Game_Input
{
	isize num_keys_down;
	isize num_mouse_down;
	int8* scancodes;
	int8* keycodes;
	int8* mouse;
	int32 mouse_x;
	int32 mouse_y;
	Vec2i screen_mouse_pos;
	Vec2 mouse_pos;
	bool capture_tabs;
	bool capture_newlines;
	char* text;
	isize text_count;
};

enum Game_State
{
	Game_State_None,
	Game_State_Menu,
	Game_State_Play
};





typedef struct Spritefont Spritefont;
typedef struct OpenGL_Renderer OpenGL_Renderer;
typedef struct Render_Group Renderer_Group;
typedef struct Game_Registry Game_Registry;

struct Game_Main
{
	SDL_Window* window;
	OpenGL_Renderer* renderer;
	Game_State state;
	real scale;
	Vec2 size;
	Vec2i window_size;

	Memory_Arena* meta_arena;
	Memory_Arena* asset_arena;
	Memory_Arena* game_arena;
	Memory_Arena* temp_arena;
	Memory_Arena* renderer_arena;

	
	
	Memory_Arena* registry_arena;
	Memory_Arena* world_arena;
	Memory_Arena* play_arena;

	const char* base_path;
	isize base_path_length;

	Spritefont* body_font;
	Spritefont* title_font;

	Random r;

	Game_Registry* registry;

	
	Game_Input* input;

	uint64 last_frame_time;
};

Game_Main* Game;
OpenGL_Renderer* Renderer;
Renderer_Group* CurrentGroup;
Game_Input* Input;
Spritefont* Body_Font;
Spritefont* Title_Font;

real64 next_random_double()
{
	uint64 x = next_random_uint64(&Game->r);
	return (real64)x / UINT64_MAX;
}

real32 next_random_float()
{
	return (real32)(next_random_double(&Game->r));
}

real next_random() 
{
	return (real)(next_random_double(&Game->r));
}

real rand_range(real min, real max)
{
	return next_random_double(&Game->r) * (max - min) + min;
}

int32 rand_range_int(real min, real max)
{
	return (int32)(next_random_double(&Game->r) * (max - min + 1) + min);
}

void game_set_scale(real scale)
{
	Game->scale = scale;
	Game->size = v2(Game->window_size) * (1.0f / Game->scale);
}

void game_calc_mouse_pos(Vec2 offset)
{
	Input->mouse_pos = v2(Input->mouse_x / Game->scale, Input->mouse_y / Game->scale) + offset;
}




void input_text_append_char(char c)
{
	if((Input->text_count + 1) < (1024)) {
		Input->text[Input->text_count++] = c;
	}
}


bool is_valid_filename_char(char c)
{
	return !((c == '?') || 
			(c == '/') || 
			(c == '\\') ||
			(c == '<') ||
			(c == '>') ||
			(c == '|') || 
			(c == '*') ||
			(c == ':') || 
			(c == '"') ||
			(c <= 31));
}

char replace_filename_char(char c, char replace)
{
	if(!is_valid_filename_char(c)) return replace;
	return c;
}

isize replace_invalid_filename_chars(char* str, isize len, char replace)
{
	for(isize i = 0; i < len; ++i) {
		if(!is_valid_filename_char(str[i])) {
			str[i] = replace;
		}
	}
	while(isspace(str[len]) || (str[len] == '.')) len--;
	return len;
}

isize append_input_text(char* str, isize str_cap, isize str_len, isize insert_from_end=0)
{
	if(Input->text_count <= 0) return str_len;
	isize index = str_len;
	start_temp_arena(Game->temp_arena);
	char* extra_chars = NULL;
	if((insert_from_end <= str_len) && (insert_from_end != 0)) {
		index -= insert_from_end;
		if(index >= 0) {
			extra_chars = ((char*)arena_push(Game->temp_arena, sizeof(char) * insert_from_end + 32));
			memset(extra_chars, 0, insert_from_end + 32);
			memcpy(extra_chars, str + index, insert_from_end);
			extra_chars[insert_from_end] = '\0';
		} else {
			index = 0;
		}
	}

	if((str_len + Input->text_count) > str_cap) {
		if(Input->text[0] != '\b')
			Input->text_count = str_cap - str_len;
	}
	isize bufsize = Input->text_count + str_len;
	char* buffer = ((char*)arena_push(Game->temp_arena, sizeof(char) * bufsize + 32));
	memset(buffer, 0, bufsize);
	memcpy(buffer, str, index);
	memcpy(buffer + index, Input->text, Input->text_count);
	if(extra_chars != NULL) {
		memcpy(buffer + index + Input->text_count, extra_chars, insert_from_end);
	}

	isize idx = 0;
	bool contains_bs = false;
	char* buffer2 = NULL; 
	for(isize i = 0; i < Input->text_count; ++i) {
		if(Input->text[i] == ('\b')) {
			contains_bs = true;
			break;
		}
	}
	contains_bs = true;
	if(contains_bs) {
		buffer2 = ((char*)arena_push(Game->temp_arena, sizeof(char) * bufsize + 32));
		memset(buffer2, 0, bufsize);
		for(isize i = 0; i < bufsize; ++i) {
			if(buffer[i] != ('\b')) {
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
				if(buffer[i] == ('\b') && buffer[i+1] == '\7') {
					i++;
				}
			}
		}
	} else {
		buffer2 = buffer;
		idx = str_len + Input->text_count;
	}
	memcpy(str, buffer2, idx);
	
	Input->text_count = 0;
	if(idx > str_cap) {
		idx = str_cap;
	}
	end_temp_arena(Game->temp_arena);
	return idx;
}

enum Direction 
{
	Direction_North,
	Direction_South,
	Direction_East,
	Direction_West
};

#line 130 "src\\rituals_main.cpp"
void switch_state(Game_State newstate);

#line 1 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_renderer.cpp"


















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

uint32 Sprite_Anchor_Mask = 0xF;

enum Sprite_Flags
{
	SpriteFlag_FlipHoriz = (1 << 4),
	SpriteFlag_FlipVert = (1 << 5)
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
	uint32 flags;
	real sort_offset;
};

struct Sprite4
{
	Sprite e[4];
};

void sort_sprites_on_y_base(Sprite* array, isize count) { if(count > 1) if(count > 12) { Sprite tmp = array[0]; array[0] = array[count / 2]; array[count / 2] = tmp; isize pivot = 0; for(isize i = 1; i < count; ++i) { if((array[i].position.y - array[i].center.y + array[i].sort_offset) < (array[0].position.y - array[0].center.y + array[0].sort_offset)) { tmp = array[++pivot]; array[pivot] = array[i]; array[i] = tmp; } } tmp = array[0]; array[0] = array[pivot]; array[pivot] = tmp; sort_sprites_on_y_base(array, pivot); sort_sprites_on_y_base(array + pivot + 1, count - (pivot + 1)); } else for(isize i = 1; i < count; ++i) { Sprite x = array[i]; isize j = i - 1; while((j >= 0) && ((array[j].position.y - array[j].center.y + array[j].sort_offset) > (x.position.y - x.center.y + x.sort_offset))) { array[j + 1] = array[j]; j--; } array[j+1] = x; } }

void init_sprite(Sprite* s)
{
	s->position = v2(0, 0);
	s->center = v2(0, 0);
	s->angle = 0;
	s->size = v2(32, 32);
	s->texture = rect2(0, 0, 1, 1);
	s->color = v4(1, 1, 1, 1);
	s->flags = Anchor_Center;
	s->sort_offset = 0;
}

struct Render_Group
{
	GLuint texture;
	Vec2i texture_size;
	Vec2 offset;
	Rect2 clip;
	real32 ortho[16];

	real night_amount;
	real night_cutoff;

	Sprite* sprites;
	isize sprites_count, sprites_capacity;
};

struct OpenGL_Renderer
{
	GLuint shader_program, vbo, vao;
	isize u_texturesize, u_orthomat, u_night_amount, u_night_cutoff;
	
	Render_Group* groups;
	isize groups_count;
};

void init_group(Render_Group* group, isize sprites_capacity, Memory_Arena* arena)
{
	group->offset = v2(0, 0);
	group->clip = rect2(0, 0, 0, 0);
	group->night_amount = 0;
	group->night_cutoff = 0;

	group->sprites_capacity = sprites_capacity;
	group->sprites_count = 0;
	group->sprites = ((Sprite*)arena_push(arena, sizeof(Sprite) * sprites_capacity));
}



void init_renderer(OpenGL_Renderer* r, isize group_count, isize group_size, char* vertex_source, char* frag_source, Memory_Arena* arena)
{
	r->groups = ((Render_Group*)arena_push(arena, sizeof(Render_Group) * group_count));
	r->groups_count = group_count;
	for(isize i = 0; i < group_count; ++i) {
		init_group(r->groups + i, group_size / sizeof(Sprite), arena);
	}

	glGenVertexArrays(1, &r->vao);
	glBindVertexArray(r->vao);
	glGenBuffers(1, &r->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, r->vbo);

	usize stride = sizeof(Sprite);
	usize vertex_count = 1;

	

	isize gl_type = GL_FLOAT;


#line 155 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_renderer.cpp"


	
	glVertexAttribPointer(0, 2, gl_type, GL_FALSE, stride, ((GLvoid*)(((void*)&(((Sprite*)(NULL))->position)))));
	glEnableVertexAttribArray(0);  
	glVertexAttribDivisor(0, vertex_count);

	
	glVertexAttribPointer(1, 2, gl_type, GL_FALSE, stride, ((GLvoid*)(((void*)&(((Sprite*)(NULL))->center)))));
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, vertex_count);

	
	glVertexAttribPointer(2, 1, gl_type, GL_FALSE, stride, ((GLvoid*)(((void*)&(((Sprite*)(NULL))->angle)))));
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, vertex_count);

	
	glVertexAttribPointer(3, 2, gl_type, GL_FALSE, stride, ((GLvoid*)(((void*)&(((Sprite*)(NULL))->size)))));
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, vertex_count);

	
	glVertexAttribPointer(4, 4, gl_type, GL_FALSE, stride, ((GLvoid*)(((void*)&(((Sprite*)(NULL))->texture)))));
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, vertex_count);

	
	glVertexAttribPointer(5, 4, gl_type, GL_FALSE, stride, ((GLvoid*)(((void*)&(((Sprite*)(NULL))->color)))));
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(5, vertex_count);

	
	glVertexAttribIPointer(6, 1, GL_UNSIGNED_INT, stride, ((GLvoid*)(((void*)&(((Sprite*)(NULL))->flags)))));
	glEnableVertexAttribArray(6);
	glVertexAttribDivisor(6, vertex_count);
	
	glBindVertexArray(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	


	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_source, NULL);
	glCompileShader(vertex_shader);
	{
		GLint success;
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
		GLsizei log_size;
		char shader_log[4096];
		glGetShaderInfoLog(vertex_shader, 4096, &log_size, shader_log); 
		if(!success) {
			printf("Error compiling vertex shader \n%s \n", shader_log);
		} else {
			printf("Vertex shader compiled successfully\n");
		}
	}	

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &frag_source, NULL);
	glCompileShader(fragment_shader);
	{
		GLint success;
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
		GLsizei log_size;
		char shader_log[4096];
		glGetShaderInfoLog(fragment_shader, 4096, &log_size, shader_log); 
		if(!success) {
			printf("Error compiling frag shader \n%s \n", shader_log);
		} else {
			printf("Frag shader compiled successfully\n");
		}
	}	
	r->shader_program = glCreateProgram();
	glAttachShader(r->shader_program, vertex_shader);
	glAttachShader(r->shader_program, fragment_shader);
	glLinkProgram(r->shader_program);
	glUseProgram(r->shader_program);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	r->u_texturesize = glGetUniformLocation(r->shader_program, "u_texturesize");
	r->u_orthomat = glGetUniformLocation(r->shader_program, "u_orthomat");
	r->u_night_amount = glGetUniformLocation(r->shader_program, "u_night_amount");
	r->u_night_cutoff = glGetUniformLocation(r->shader_program, "u_night_cutoff");
}

Render_Group* render_group(OpenGL_Renderer* r, isize index)
{
	return r->groups + index;
}



void render_start(Render_Group* group)
{
	group->sprites_count = 0;
	group->clip = {0, 0, 0, 0};
}

bool render_has_clip_rect(Render_Group* group)
{
	return 0 != (group->clip.w * group->clip.h);
}

void render_set_clip_rect(Render_Group* group, real x, real y, real w, real h)
{
	group->clip = Rect2 {
		x, y, w, h
	};
}

void render_sort(Render_Group* group, isize offset)
{
	sort_sprites_on_y_base(group->sprites + offset, group->sprites_count - offset);
}

void render_add(Render_Group* group, Sprite* sprite)
{
	Sprite sp = *sprite;

	if(render_has_clip_rect(group)) {
		Rect2 c = group->clip;
		Rect2 r;
		r.position = sp.position;
		r.size = sp.size;
		r.x -= r.w * (0.5f + SpriteAnchorX[sp.flags & Sprite_Anchor_Mask]);
		r.y -= r.h * (0.5f + SpriteAnchorY[sp.flags & Sprite_Anchor_Mask]);

		if(r.x > (c.x + c.w)) return;
		if((r.x + r.w) < c.x) return;
		if(r.y > (c.y + c.h)) return;
		if((r.y + r.h) < c.y) return;

		Rect2_Clip_Info clip = rect2_clip(r, c);

		Vec2 tex_scale = v2(sp.texture.w / sp.size.x, sp.texture.h / sp.size.y);
		Vec2 tdp1 = clip.diff1 * tex_scale;
		Vec2 tdp2 = clip.diff2 * tex_scale;
		Vec2 tp1 = v2(sp.texture.x, sp.texture.y);
		Vec2 tp2 = tp1 + v2(sp.texture.w, sp.texture.h);
		tp1 += tdp1;
		tp2 -= tdp2;
		sp.texture.position = tp1;
		sp.texture.size = tp2 - tp1;
		sp.position = clip.rp1;
		sp.size = clip.rp2 - clip.rp1;
		sp.flags = (~Sprite_Anchor_Mask & sp.flags) | Anchor_Top_Left;
		sp.angle = 0;
	}
	
	sp.texture.x /= group->texture_size.x;
	sp.texture.w /= group->texture_size.x;
	sp.texture.y /= group->texture_size.y;
	sp.texture.h /= group->texture_size.y;

	group->sprites[group->sprites_count++] = sp;

}

void render_add_unclipped(Render_Group* group, Sprite* sprite)
{
	Sprite sp = *sprite;

	sp.texture.x /= group->texture_size.x;
	sp.texture.w /= group->texture_size.x;
	sp.texture.y /= group->texture_size.y;
	sp.texture.h /= group->texture_size.y;

	group->sprites[group->sprites_count++] = sp;
}

void render_add(Render_Group* group, Sprite4* s4)
{
	render_add(group, s4->e + 0);
	render_add(group, s4->e + 1);
	render_add(group, s4->e + 2);
	render_add(group, s4->e + 3);
}

void render_calculate_ortho_matrix(real32* ortho, Vec4 screen, real nearplane, real farplane)
{
	ortho[0] = 2.0f / (screen.z - screen.x);
	ortho[1] = 0;
	ortho[2] = 0;
	ortho[3] = -1.0f * (screen.x + screen.z) / (screen.z - screen.x);

	ortho[4] = 0;
	ortho[5] = 2.0f / (screen.y - screen.w);
	ortho[6] = 0;
	ortho[7] = -1 * (screen.y + screen.w) / (screen.y - screen.w);

	ortho[8] = 0;
	ortho[9] = 0;
	ortho[10] = (-2.0f / (farplane - nearplane));
	ortho[11] = (-1.0f * (farplane + nearplane) / (farplane - nearplane));

	ortho[12] = 0;
	ortho[13] = 0;
	ortho[14] = 0;
	ortho[15] = 1.0f;
}

void render_draw(OpenGL_Renderer* r, Render_Group* group, Vec2 size, real scale)
{
	glUseProgram(r->shader_program);
	group->offset.x = roundf(group->offset.x);
	group->offset.y = roundf(group->offset.y);

	glUniform2f(r->u_texturesize,
		group->texture_size.x,
		group->texture_size.y);

	glUniform1f(r->u_night_amount, group->night_amount);
	glUniform1f(r->u_night_cutoff, group->night_cutoff);

	Vec4 screen = v4(
		group->offset.x, group->offset.y, 
		size.x + group->offset.x,
		size.y + group->offset.y);
	render_calculate_ortho_matrix(group->ortho, screen, 1, -1);
	glUniformMatrix4fv(r->u_orthomat, 
		1, 
		GL_FALSE,
		group->ortho);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, group->texture);
	glBindVertexArray(r->vao);
	glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
	glBufferData(GL_ARRAY_BUFFER, 
			group->sprites_count * sizeof(Sprite), 
			group->sprites, 
			GL_STREAM_DRAW);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, group->sprites_count);

	glBindVertexArray(0);
}



void render_set_current_group(isize index)
{
	CurrentGroup = Renderer->groups + index;
}

void render_start()
{
	render_start(CurrentGroup);
}

bool render_has_clip_rect()
{
	return render_has_clip_rect(CurrentGroup);
}

void render_set_clip_rect(real x, real y, real w, real h)
{
	render_set_clip_rect(CurrentGroup, x, y, w, h);
}

void render_sort(isize offset)
{
	render_sort(CurrentGroup, offset);
}

void render_add(Sprite* sprite)
{
	render_add(CurrentGroup, sprite);
}

void render_add_unclipped(Sprite* sprite)
{
	render_add_unclipped(CurrentGroup, sprite);
}

void render_add(Sprite4* s4)
{
	render_add(CurrentGroup, s4);
}

void render_draw(Vec2 size, real scale)
{
	render_draw(Renderer, CurrentGroup, size, scale);
}




GLuint ogl_add_texture(uint8* data, isize w, isize h) 
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	uint32 error = glGetError();
	if(error != 0) {
		printf("There was an error adding a texture: %d \n", error);
	}
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

GLuint ogl_load_texture(char* filename, isize* w_o, isize* h_o)
{
	int w, h, n;
	char file[4096];
	const char* base_path = SDL_GetBasePath();
	isize len = snprintf(file, 4096, "%s%s", base_path, filename);
	uint8* data = (uint8*)stbi_load(file, &w, &h, &n, STBI_rgb_alpha);
	
	GLuint texture = ogl_add_texture(data, w, h);
	if(texture == NULL) {
		printf("There was an error loading %s \n", filename);
	}
	if(w_o != NULL) *w_o = w;
	if(h_o != NULL) *h_o = h;

	STBI_FREE(data);
	return texture;
}
	


Sprite create_box_primitive(Vec2 pos, Vec2 size, Vec4 color)
{
	Sprite s;
	init_sprite(&s);
	s.position = pos;
	s.texture = rect2(64, 0, 32, 32);
	s.size = size;
	s.color = color;
	return s;
}

void render_box_primitive(Render_Group* group, Vec2 pos, Vec2 size, Vec4 color)
{
	Sprite s = create_box_primitive(pos, size, color);
	render_add(group, &s);
}

Sprite create_line_primitive(Vec2 start, Vec2 end, Vec4 color, int32 thickness)
{
	Vec2 dline = end - start;
	Sprite s;
	if(dline.y == 0) {
		if(dline.x < 0) {
			dline.x *= -1;
			Vec2 temp = end;
			end = start;
			start = temp;
		}
		s = create_box_primitive(start + v2(dline.x / 2, 0), v2(dline.x, thickness), color);
	} else if(dline.x == 0) {
		if(dline.y < 0) {
			dline.y *= -1;
			Vec2 temp = end;
			end = start;
			start = temp;
		}
		s = create_box_primitive(start + v2(0, dline.y / 2), v2(thickness, dline.y), color);
	} else {
		s = create_box_primitive(start + dline/2, v2(sqrtf(v2_dot(dline, dline)), thickness), color);
		real angle = atan2f(dline.y, dline.x);
		s.angle = -angle;
	}
	return s;
}
void render_line_primitive(Render_Group* group, Vec2 start, Vec2 end, Vec4 color, int32 thickness)
{
	Sprite s = create_line_primitive(start, end, color, thickness);
	render_add(group, &s);
}

Sprite4 create_box_outline_primitive(Vec2 center, Vec2 size, Vec4 color, int32 thickness)
{
	size *= 0.5f;	
	Vec2 tl = center - size;
	Vec2 br = center + size;
	Sprite4 s;
	s.e[0] = create_line_primitive(tl, v2(br.x, tl.y), color, thickness);
	s.e[1] = create_line_primitive(v2(br.x, tl.y), br, color, thickness);
	s.e[2] = create_line_primitive(br, v2(tl.x, br.y), color, thickness);
	s.e[3] = create_line_primitive(v2(tl.x, br.y), tl, color, thickness);
	return s;
}

void render_box_outline_primitive(Render_Group* group, Vec2 center, Vec2 size, Vec4 color, int32 thickness)
{
	Sprite4 s = create_box_outline_primitive(center, size, color, thickness);
	render_add(group, &s);
}

Sprite4 create_box_outline_primitive(Vec2 center, Vec2 size, Vec4 color[4], int32 thickness)
{
	size *= 0.5f;	
	Vec2 tl = center - size;
	Vec2 br = center + size;
	Sprite4 s;
	s.e[0] = create_line_primitive(tl, v2(br.x, tl.y), color[0], thickness);
	s.e[1] = create_line_primitive(v2(br.x, tl.y), br, color[1], thickness);
	s.e[2] = create_line_primitive(br, v2(tl.x, br.y), color[2], thickness);
	s.e[3] = create_line_primitive(v2(tl.x, br.y), tl, color[3], thickness);
	return s;
}

void render_box_outline_primitive(Render_Group* group, Vec2 center, Vec2 size, Vec4 color[4], int32 thickness)
{
	Sprite4 s = create_box_outline_primitive(center, size, color, thickness);
	render_add(group, &s);
}

void render_box_primitive(Vec2 pos, Vec2 size, Vec4 color)
{
	render_box_primitive(CurrentGroup, pos, size, color);
}

void render_line_primitive(Vec2 start, Vec2 end, Vec4 color, int32 thickness)
{
	render_line_primitive(CurrentGroup, start, end, color, thickness);
}

void render_box_outline_primitive(Vec2 center, Vec2 size, Vec4 color, int32 thickness)
{
	render_box_outline_primitive(CurrentGroup, center, size, color, thickness);
}

void render_box_outline_primitive(Vec2 center, Vec2 size, Vec4 color[4], int32 thickness)
{
	render_box_outline_primitive(CurrentGroup, center, size, color, thickness);
}
#line 133 "src\\rituals_main.cpp"
#line 1 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_animations.cpp"















struct Animation_Frame
{
	Vec3 position; 
	real angle;
	Vec4 color;
	Vec2 size;
	Rect2 texture;
	real sort_offset;
};

void init_animation_frame(Animation_Frame* fr)
{
	fr->position = v3(0, 0, 0);
	fr->angle = 0;
	fr->color = v4(1, 1, 1, 1);
	fr->size = v2(0, 0);
	fr->texture = rect2(0, 0, 0, 0);
	fr->sort_offset = 0;
}

struct Animation
{
	isize id;
	Animation_Frame* frames;
	isize frames_count, frames_capacity;
	real fps;
	real inv_fps;
	bool looping;
};

void init_animation(Animation* a, real fps, isize capacity, Memory_Arena* arena)
{
	a->id = -1;
	a->frames = ((Animation_Frame*)arena_push(arena, sizeof(Animation_Frame) * capacity));
	a->frames_count = 0;
	a->frames_capacity = 0;
	a->fps = fps;
	a->inv_fps = 1.0f / fps;
	a->looping = true;
}

Animation* make_animaiton_from_strip(Memory_Arena* arena, real fps, Rect2 frame, isize count) 
{
	Animation* anim = ((Animation*)arena_push(arena, sizeof(Animation)));
	init_animation(anim, fps, count, arena);
	anim->frames_count = count;

	for(isize i = 0; i < count; ++i) {
		Animation_Frame* fr = anim->frames + i;
		init_animation_frame(fr);
		fr->texture = frame;
		fr->size = frame.size;
		frame.x += frame.w;
	}

	return anim;
}

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
	s->animations = ((Animation**)arena_push(arena, sizeof(Animation*) * capacity));
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

#line 134 "src\\rituals_main.cpp"
#line 1 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_gui.cpp"





















$(exclude)
const Rect2 Gohufont_Glyphs[] = {
#line 1 "c:\\users\\william\\stuff\\development\\rituals\\src\\gohufont.glyphs"
	Rect2{0 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{12 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{24 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{36 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{48 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{60 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{72 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{84 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{96 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{108 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{120 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{132 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{144 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{156 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{168 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{180 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{192 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{204 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{216 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{228 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{240 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{252 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{264 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{276 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{288 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{300 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{312 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{324 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{336 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{348 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{360 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{372 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{384 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{396 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{408 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{420 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{432 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{444 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{456 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{468 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{480 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{492 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{504 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{516 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{528 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{540 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{552 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{564 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{576 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{588 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{600 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{612 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{624 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{636 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{648 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{660 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{672 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{684 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{696 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{708 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{720 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{732 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{744 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{756 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{768 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{780 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{792 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{804 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{816 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{828 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{840 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{852 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{864 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{876 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{888 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{900 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{912 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{924 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{936 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{948 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{960 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{972 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{984 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{996 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{1008 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{1020 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{1032 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{1044 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{1056 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{1068 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{1080 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{1092 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{1104 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{1116 + (2048-1142), 0 + 0, 8, 14}, 
	Rect2{1128 + (2048-1142), 0 + 0, 8, 14} 
#line 25 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_gui.cpp"
};
$(end)

struct Spritefont
{
	int32 line_padding;
	int32 character_padding;
	int32 tab_size;

	int32 glyph_width, glyph_height;
	const Rect2* glyphs;
	Vec4 color;
};

void init_spritefont(Spritefont* font, const Rect2* glyphs)
{
	font->line_padding = 0;
	font->character_padding = 0;
	font->tab_size = 4; 
	font->glyphs = glyphs;
    font->glyph_width = glyphs[1].w;
    font->glyph_height = glyphs[1].h;
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
		if(c < (32) || c > (128)) continue;
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
		if(c < (32) || c > (128)) continue;

		s.position = initial_pos + position * scale;
		s.texture = font->glyphs[c - (32)];
		s.size = size * scale;
		s.color = font->color;
		s.angle = 0;
		s.flags = anchor;
		
		if((max_width > 0) && (s.position.x + s.size.x > (max_width + initial_pos.x))) {
			position.y += font->glyph_height + font->line_padding;
			position.x = 0;
			wrapped ++;
		}
			
		render_add(&s);
		position.x += size.x + font->character_padding;
	}
	if(region != NULL) {
		*region = position;
		if(wrapped > 0) {
			region->x = max_width;
		}
	}
}


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
					
					continue;
				default: 
					if(c >= (32) && c <= (128)) {
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

					if(c < (32) || c > (128)) {
						continue;
					}
					s.position = initial_pos + position * scale;
					s.texture = font->glyphs[c - (32)];
					s.size = size * scale;
					s.color = font->color;
					s.angle = 0;
					s.flags = anchor;
					render_add(&s);
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

void spritefont_render_text_background(Spritefont* font, char* text, Vec2 position, real scale, Vec4 background) 
{
	Vec2 text_size = spritefont_size_text(font, text) * scale;
	Sprite s; 
	init_sprite(&s);
	s.size = v2(text_size.x + 8,  text_size.y + 4);
	s.position = position - v2(4 - scale, 2);
	s.color = background;
	s.texture = rect2(64, 0, 32, 32);
	s.flags = Anchor_Top_Left;
	font->color = v4(1, 1, 1, 1);
	render_add(&s);

}
Vec4 Gui_TextBackgroundColor = Vec4{0, 0, 0, 0.8f};
void render_body_text(char* text, Vec2 position, bool background=false, real scale = 1.0f)
{
	if(background) {
		spritefont_render_text_background(Body_Font, text, position, scale, Gui_TextBackgroundColor);
	}
	spritefont_render_text(Body_Font, text, position, Anchor_Top_Left, scale);
}

void render_title_text(char* text, Vec2 position)
{
	spritefont_render_text(Title_Font, text, position);
}



bool gui_query_mouse(Rect2 region, Vec2 parent)
{
	region.position -= parent;
	Rect2 r = region;
	game_calc_mouse_pos(Renderer->groups[0].offset);
	Vec2 p = Input->mouse_pos;
	Vec2 rp1, rp2;
	if(render_has_clip_rect(CurrentGroup)) {
		Rect2 c = Renderer->groups[0].clip; 
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


Vec4 Color_White = Vec4{ (real)(1), (real)(1), (real)(1), (real)(1) };
Vec4 Color_Black = Vec4{ (real)(0), (real)(0), (real)(0), (real)(1) };
Vec4 Gui_Tint = Vec4{ (real)(0.88), (real)(0.89), (real)(1), (real)(1) };
Vec4 Gui_ButtonRestColor = Vec4{ (real)(.8), (real)(.8), (real)(.8), (real)(1) } * Gui_Tint;
Vec4 Gui_ButtonActiveColor = Vec4{ (real)(.9), (real)(.9), (real)(.9), (real)(1) } * Gui_Tint;
Vec4 Gui_ButtonDownColor = Vec4{ (real)(.5), (real)(.5), (real)(.5), (real)(1) } * Gui_Tint;
Vec4 Gui_ButtonTextColor = Vec4{ (real)(0), (real)(0), (real)(0), (real)(1) };
Vec4 Gui_ButtonTextDownColor = Gui_ButtonTextColor;

Vec4 Gui_ButtonOutlineRestColor = Vec4{ (real)(.9), (real)(.9), (real)(.9), (real)(1) } * Gui_Tint;
Vec4 Gui_ButtonOutlineActiveColor = Vec4{ (real)(1), (real)(1), (real)(1), (real)(1) } * Gui_Tint;
Vec4 Gui_ButtonOutlineDownColor = Vec4{ (real)(.4), (real)(.4), (real)(.4), (real)(1) } * Gui_Tint;
Vec4 Gui_ButtonOutlineRestColor2 = Vec4{ (real)(0.5), (real)(0.5), (real)(0.5), (real)(1) } * Gui_Tint;
Vec4 Gui_ButtonOutlineActiveColor2 = Vec4{ (real)(0.5), (real)(0.5), (real)(0.5), (real)(1) } * Gui_Tint;
Vec4 Gui_ButtonOutlineDownColor2 = Vec4{ (real)(.7), (real)(.7), (real)(.7), (real)(1) } * Gui_Tint;

Vec4 Gui_TextInputRestColor = Vec4{ (real)(0.3), (real)(0.3), (real)(0.3), (real)(1) } * Gui_Tint;
Vec4 Gui_TextInputHotColor = Vec4{ (real)(0.6), (real)(0.6), (real)(0.6), (real)(1) } * Gui_Tint;
Vec4 Gui_TextInputActiveColor = Vec4{ (real)(0.5), (real)(0.5), (real)(0.5), (real)(1) } * Gui_Tint;

Vec4 Gui_TextInputOutlineRestColor = Vec4{ (real)(0.2), (real)(0.2), (real)(0.2), (real)(1) } * Gui_Tint;
Vec4 Gui_TextInputOutlineHotColor = Vec4{ (real)(0.5), (real)(0.5), (real)(0.5), (real)(1) } * Gui_Tint;
Vec4 Gui_TextInputOutlineActiveColor = Vec4{ (real)(0.4), (real)(0.4), (real)(0.4), (real)(1) } * Gui_Tint;
Vec4 Gui_TextInputOutlineRestColor2 = Vec4{ (real)(0.4), (real)(0.4), (real)(0.4), (real)(1) } * Gui_Tint;
Vec4 Gui_TextInputOutlineHotColor2 = Vec4{ (real)(0.7), (real)(0.7), (real)(0.7), (real)(1) } * Gui_Tint;
Vec4 Gui_TextInputOutlineActiveColor2 = Vec4{ (real)(0.7), (real)(0.7), (real)(0.7), (real)(1) } * Gui_Tint;



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
	Sprite s = create_box_primitive(position, txs + v2(16, 8), Gui_ButtonRestColor);
	s.flags = Anchor_Top_Left;
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
	
	render_add(&s);
	render_body_text(text, v2(8, 4) + v2(
				position.x,
				position.y + (s.size.y - txs.y) / 2 - 4));
	Vec4 colors[4] = {
		color1, color1,
		color2, color2
	};
	render_box_outline_primitive(s.position + s.size/2, s.size, colors, 2);

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

void gui_add_slider(Vec2 position, Vec2 size, char* label, real min, real max, int precision, real* in_value, bool* active, bool show_bounds = false)
{
	Sprite bg = create_box_primitive(position, size, Gui_ButtonDownColor);
	bg.flags = Anchor_Top_Left;
	render_add(&bg);
	Vec4 color1 = Gui_ButtonOutlineDownColor;
	Vec4 color2 = Gui_ButtonOutlineDownColor2;
	Vec4 colors[4] = {
		color2, color2,
		color1, color1
	};
	render_box_outline_primitive(position + size/2, size, colors, 2);

	position += v2(4, size.y / 2 - Body_Font->glyph_height / 2);
	size -= v2(8, 2);

	Body_Font->color = v4(1, 1, 1, 1);
	Vec2 begin, end;
	end = position + v2(
			size.x - Body_Font->glyph_width * (strlen(label)),
			0);
	render_body_text(label, end);
	if(show_bounds) {
		char min_str[16];
		char max_str[16];
		isize min_str_len = snprintf(min_str, 16, "%.*f", precision, min);
		isize max_str_len = snprintf(max_str, 16, "%.*f", precision, max);

		Vec2 min_str_reg;
		spritefont_render_text(Body_Font,
				min_str, min_str_len,
				position, -1, Anchor_Top_Left, 1.0f, &min_str_reg);
		Vec2 max_str_reg;
		begin = position + v2(min_str_reg.x, 0);
		end.x -= (max_str_len+1) * Body_Font->glyph_width;
		spritefont_render_text(Body_Font,
				max_str, max_str_len,
				end, -1, Anchor_Top_Left, 1.0f, &max_str_reg); 
	} else {
		char max_str[16];
		isize max_str_len = snprintf(max_str, 16, "%.*f", precision, max);
		Vec2 val_region;
		char val_str[16];
		isize val_str_len = snprintf(val_str, 16, "%.*f", precision, *in_value);
		spritefont_render_text(Body_Font,
				val_str, val_str_len,
				position, -1, Anchor_Top_Left, 1.0f, &val_region);
		begin = v2(Body_Font->glyph_width * max_str_len + position.x, position.y);


	}

	real value = 0;
	real perc = .5; 
	if(in_value != NULL) {
		value = *in_value;
		perc = (value - min) / (max - min);
	}
	begin.x += 8;
	end.x -= 8;
	Vec2 line_offset = v2(0, 8);
	render_line_primitive(begin + line_offset, end + line_offset, v4(1, 1, 1, 0.9f), 2);
	Sprite handle = create_box_primitive(begin + line_offset, v2(8, 12), Gui_Tint);
	handle.position.x += perc * (end.x - begin.x);
	
	Rect2 r;
	r.position = handle.position - handle.size / 2;
	r.size = handle.size;

	if(gui_query_mouse(r, v2(0, 0))) {
		handle.color = v4(1, 1, 1, 1);
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
		handle.color = v4(1, 1, 1, 1);
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
		if(show_bounds) {
			char number[32];
			isize len = snprintf(number, 32, "%.*f", precision + 1, *in_value);
			render_body_text(number, handle.position - v2(
						len * Body_Font->glyph_width / 2, 
						Body_Font->glyph_height + 4 + handle.size.y / 2), true);
		}
	}

	render_add(&handle);
	
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
	char* buffer = ((char*)arena_push(arena, sizeof(char) * buffer_capacity));
	init_text_input_handle(handle, buffer, buffer_capacity, 0);
	handle->max_chars_by_width = max_chars_by_width;
}

void gui_add_text_input(Gui_Text_Input_Handle* handle, Vec2 position, Vec2 size, char* default_text=NULL)
{
	Vec2 dmouse = v2(
			Input->mouse_x / Game->scale, 
			Input->mouse_y / Game->scale) + Renderer->groups[0].offset;
	Vec2 text_offset = v2(4, (size.y-Body_Font->glyph_height)/2);
	size.x += 8;
	Sprite s = create_box_primitive(position + size / 2, size, Gui_TextInputRestColor);
	bool mouse_over = aabb_intersect(aabb(position + size / 2, size.x / 2, size.y / 2), aabb(dmouse, 0, 0));
	
	if(handle->active) {
		s.color = Gui_TextInputActiveColor;
		if(Input->mouse[SDL_BUTTON_LEFT] == State_Just_Pressed) {
			if(!mouse_over) {
				handle->active = false;
			} else {
				Vec2 localmouse = dmouse - position;
				localmouse -= text_offset;
				handle->cursor = roundf(localmouse.x / Body_Font->glyph_width);
			}
		} 
	
	
	
	} else {
		if(mouse_over) s.color = Gui_TextInputHotColor;
		if(Input->mouse[SDL_BUTTON_LEFT] == State_Just_Pressed) {
			if(mouse_over) {
				handle->active = true;
			}
		}
	}

	render_add(&s);

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

		
		render_line_primitive(text_offset + position + v2(handle->cursor * Body_Font->glyph_width + 1, 0),
				text_offset + position + v2(handle->cursor * Body_Font->glyph_width + 1, Body_Font->glyph_height), v4(1, 1, 1, 1), 1);

	}
	
	if(mouse_over && !handle->active) {
		Body_Font->color = Color_Black;
	} else {
		Body_Font->color = Color_White;
	}
	
	if(!handle->active && default_text != NULL && handle->buffer_length == 0) {
		Body_Font->color *= 0.8;
		Body_Font->color.w = 1.0f;
		spritefont_render_text(Body_Font,
				default_text, strlen(default_text),
				position + text_offset); 
	} else {
		spritefont_render_text(Body_Font,
				handle->buffer, handle->buffer_length,
				position + text_offset); 
	}

	Vec4 color1, color2;
	if(!mouse_over && !handle->active) {
		color1 = Gui_TextInputOutlineRestColor;
		color2 = Gui_TextInputOutlineRestColor2;
	} else if(mouse_over && !handle->active) {
		color1 = Gui_TextInputOutlineHotColor;
		color2 = Gui_TextInputOutlineHotColor2;
	} else if(handle->active) {
		color1 = Gui_TextInputOutlineActiveColor;
		color2 = Gui_TextInputOutlineActiveColor2;
	}
	Vec4 colors[4] = {
		color1, color1,
		color2, color2
	};
	render_box_outline_primitive(s.position, s.size, colors, 2);
}

struct Gui_Context
{
	Vec2 last_pos;
	Vec2 next_pos;
	isize axis;
};

#line 135 "src\\rituals_main.cpp"

typedef struct World World;
struct Play_State
{
	usize current_time = 0, prev_time = 0;
	real accumulator = 0;
	World* world;

	bool running;

	Vec2i world_xy;
	bool delete_world_on_stop;
	bool save_world_on_stop;
};
Play_State* play_state;


struct Menu_State
{
	char save_dir[4096];
	isize save_dir_len;
	tinydir_dir saves;
	bool saves_dirty;
	Gui_Text_Input_Handle handle;
	isize delete_index;
};
Menu_State* menu_state;

#line 1 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_game_info.cpp"
















 

typedef struct Entity Entity;
typedef struct World_Area World_Area;
typedef struct World World;

typedef void (*Entity_On_Activate)(Entity* entity, World_Area* area);

typedef int32 Tile;

struct Tile_Info
{
	isize id;
	usize hash;

	Rect2 texture;
	
	Rect2 bottom_texture;
	Rect2 top_texture;

	real movement_modifier;
	real friction;
	
	const char* name;

	int32 max_damage;
	bool immune_to_damage;
	char* break_to_name;
	isize break_to_id;

	
	bool solid;
	uint64 body_mask;
	uint64 body_group;

	bool has_top_texture;
	bool has_bottom_texture;
};

struct Tile_State
{
	isize id;
	int32 damage;
};

void init_tile_state(Tile_State* state, isize id)
{
	state->id = id;
	state->damage = 0;
}


enum Item_Info_Types
{
	Item_None,

	Item_Hooknife,
	Item_Rope,
	Item_Book,
	Item_Rock,

	Item_Info_Types_Count
};

struct Item_Info
{
	isize id;
	usize hash;
	const char* name;
	int32 max_stack;

	Rect2 texture;
};

enum Entity_Event_Type
{
	EntityEvent_None = (1 << 0),
	EntityEvent_Walk = (1 << 1),
	EntityEvent_Interact = (1 << 2),
	EntityEvent_Animate = (1 << 3),
	EntityEvent_FrameTick = (1 << 4),
	EntityEvent_SlowTick = (1 << 5),
	EntityEvent_Destroy = (1 << 6), 
	EntityEvent_Activate = (1 << 7), 
	
	EntityEvent_Hit = (1 << 8),
	EntityEvent_Contact = (1 << 9),
};	

enum Rituals_Entity_Kinds
{
	EntityKind_Prop,
	EntityKind_Static,
	EntityKind_Player,
	EntityKind_Enemy,
	EntityKind_Bullet,
	EntityKind_Pickup,
};


enum Rituals_Enemy_Kind
{
	EnemyKind_Slime,
	EnemyKind_Bat,
	EnemyKind_Snake,
	EnemyKind_Goblin_Knight,
};

enum Rituals_Pickup_Kind
{
	PickupKind_Item,
	PickupKind_Health
};

union Rituals_Entity_Userdata
{
	struct {
		Rituals_Entity_Kinds contains;
		isize subtype;
		isize amount;
		isize quality;
	} prop;
	struct {
		isize held_entity_id;
		real heal_cooldown;
		real heal_timer;
		int32 heal_rate;
		int32 heal_to_interval;
		
	} player;
	struct {
		isize kind;
		isize mode;
		real speed;
		real alert_dist;
		real follow_dist;
		
		union {
			struct {
				Vec2 perch;
			} bat;
			struct {
				real chase_speed_modifier;
			} snake;
			struct {
				Vec2 patrol_start;
				Vec2 patrol_end;
			} goblin_knight;
		};
	} enemy;
	struct {
		isize kind;
		union {
			struct {
				isize id, count;
			} item;
			struct {
				isize amount;
			} health;
		};
	} pickup;
	struct {
		void* ptr;
		isize size;
	} unknown;
};


union MyUnion
{
	struct {
		real x, y, z, w;
	};
	struct {
		uint32 aa, bb, cc, dd;
	};
	struct {
		char* p, *q, *r, *s;
	};
};
#line 164 "src\\rituals_main.cpp"
#line 1 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_game_registry.cpp"













 

usize hash_str(const char* str)
{
	isize len = strlen(str);
	usize hash = 1;
	for(isize i = 0; i < len; ++i) {
		hash = hash * 65599 + str[i];	
	}
	return hash;
}

struct Game_Registry
{
	Tile_Info* tiles;
	usize* tiles_hash;
	isize tiles_count;

	Item_Info* items;
	usize* items_hash;
	isize items_count;
	
	
};

Game_Registry* Registry;


void init_game_registry(Game_Registry* registry, Memory_Arena* arena)
{
	$(exclude)





$(end)

	do { registry->tiles = ((Tile_Info*)arena_push(arena, sizeof(Tile_Info) * (256))); registry->tiles_hash = ((usize*)arena_push(arena, sizeof(usize) * (256))); registry->tiles_count = 0; } while(0);
	do { registry->items = ((Item_Info*)arena_push(arena, sizeof(Item_Info) * (256))); registry->items_hash = ((usize*)arena_push(arena, sizeof(usize) * (256))); registry->items_count = 0; } while(0);
}



$(exclude)






























$(end)

Tile_Info* lookup_tile(const char* name) { usize hash = hash_str(name); isize index = usize_search(hash, Registry->tiles_hash, Registry->tiles_count); return Registry->tiles + index; } struct _game_registry__Tile_Info_pair { Tile_Info v; usize k; }; void _game_registry__Tile_Info_pair_sort(_game_registry__Tile_Info_pair* array, isize count) { if(count > 1) if(count > 12) { _game_registry__Tile_Info_pair tmp = array[0]; array[0] = array[count / 2]; array[count / 2] = tmp; isize pivot = 0; for(isize i = 1; i < count; ++i) { if((array[i].k) < (array[0].k)) { tmp = array[++pivot]; array[pivot] = array[i]; array[i] = tmp; } } tmp = array[0]; array[0] = array[pivot]; array[pivot] = tmp; _game_registry__Tile_Info_pair_sort(array, pivot); _game_registry__Tile_Info_pair_sort(array + pivot + 1, count - (pivot + 1)); } else for(isize i = 1; i < count; ++i) { _game_registry__Tile_Info_pair x = array[i]; isize j = i - 1; while((j >= 0) && ((array[j].k) > (x.k))) { array[j + 1] = array[j]; j--; } array[j+1] = x; } } void sort_registered_tiles() { start_temp_arena(Game->temp_arena); _game_registry__Tile_Info_pair* pairs = ((_game_registry__Tile_Info_pair*)arena_push(Game->temp_arena, sizeof(_game_registry__Tile_Info_pair) * Registry->tiles_count)); for(isize i = 0; i < Registry->tiles_count; ++i) { auto pair = pairs + i; pair->v = Registry->tiles[i]; pair->k = Registry->tiles_hash[i]; } _game_registry__Tile_Info_pair_sort(pairs, Registry->tiles_count); for(isize i = 0; i < Registry->tiles_count; ++i) { auto pair = pairs + i; Registry->tiles[i] = pair->v; Registry->tiles_hash[i] = pair->k; Registry->tiles[i].id = i; } } 
Item_Info* lookup_item(const char* name) { usize hash = hash_str(name); isize index = usize_search(hash, Registry->items_hash, Registry->items_count); return Registry->items + index; } struct _game_registry__Item_Info_pair { Item_Info v; usize k; }; void _game_registry__Item_Info_pair_sort(_game_registry__Item_Info_pair* array, isize count) { if(count > 1) if(count > 12) { _game_registry__Item_Info_pair tmp = array[0]; array[0] = array[count / 2]; array[count / 2] = tmp; isize pivot = 0; for(isize i = 1; i < count; ++i) { if((array[i].k) < (array[0].k)) { tmp = array[++pivot]; array[pivot] = array[i]; array[i] = tmp; } } tmp = array[0]; array[0] = array[pivot]; array[pivot] = tmp; _game_registry__Item_Info_pair_sort(array, pivot); _game_registry__Item_Info_pair_sort(array + pivot + 1, count - (pivot + 1)); } else for(isize i = 1; i < count; ++i) { _game_registry__Item_Info_pair x = array[i]; isize j = i - 1; while((j >= 0) && ((array[j].k) > (x.k))) { array[j + 1] = array[j]; j--; } array[j+1] = x; } } void sort_registered_items() { start_temp_arena(Game->temp_arena); _game_registry__Item_Info_pair* pairs = ((_game_registry__Item_Info_pair*)arena_push(Game->temp_arena, sizeof(_game_registry__Item_Info_pair) * Registry->items_count)); for(isize i = 0; i < Registry->items_count; ++i) { auto pair = pairs + i; pair->v = Registry->items[i]; pair->k = Registry->items_hash[i]; } _game_registry__Item_Info_pair_sort(pairs, Registry->items_count); for(isize i = 0; i < Registry->items_count; ++i) { auto pair = pairs + i; Registry->items[i] = pair->v; Registry->items_hash[i] = pair->k; Registry->items[i].id = i; } }

isize Tile_Void = 0;
isize Tile_Sand = 0;
isize Tile_Grass = 0;
isize Tile_Dense_Grass = 0;
isize Tile_Dug_Earth = 0;
isize Tile_Tree_Wall = 0;
isize Tile_Shallow_Water = 0;
isize Tile_Water = 0;
isize Tile_Stone_Road = 0;
isize Tile_Cracked_Road = 0;
isize Tile_Pristine_Road = 0;
isize Tile_Slab_Wall = 0;
void finalize_game_registry()
{
	sort_registered_tiles();
	sort_registered_items();

	for(isize i = 0; i < Registry->tiles_count; ++i) {
		Tile_Info* t = Registry->tiles + i;
		t->break_to_id = lookup_tile(t->break_to_name)->id;
	}

	
	Tile_Void = lookup_tile("void")->id;
	Tile_Sand = lookup_tile("sand")->id;
	Tile_Grass = lookup_tile("grass")->id;
	Tile_Dense_Grass = lookup_tile("dense_grass")->id;
	Tile_Dug_Earth = lookup_tile("dug_earth")->id;
	Tile_Tree_Wall = lookup_tile("tree_wall")->id;
	Tile_Shallow_Water = lookup_tile("shallow_water")->id;
	Tile_Water = lookup_tile("water")->id;
	Tile_Stone_Road = lookup_tile("stone_road")->id;
	Tile_Cracked_Road = lookup_tile("cracked_road")->id;
	Tile_Pristine_Road = lookup_tile("pristine_road")->id;
	Tile_Slab_Wall = lookup_tile("slab_wall")->id;
}








Tile_Info* add_tile_info(const char* name, real movement_modifier, real friction, Rect2 texture, bool solid, char* break_to_name)
{
	Tile_Info* t = Registry->tiles + Registry->tiles_count++;
	t->id = Registry->tiles_count - 1;
	t->hash = hash_str(name);
	t->texture = texture;
	t->name = name;
	t->solid = solid;
	t->movement_modifier = movement_modifier;
	t->friction = friction;
	t->max_damage = 5;
	t->immune_to_damage = false;
	t->break_to_id = 0;
	t->break_to_name = break_to_name;

	Registry->tiles_hash[Registry->tiles_count - 1] = t->hash;

	return t;
}

void register_all_rituals_tile_info()
{
	Tile_Info* tile_void = add_tile_info("void", (real)(1.0), (real)0.5, rect2((32) * (0), (32) * (0), (32), (32)), true, "dug_earth");
	tile_void->texture = rect2(0, 0, 0, 0);

	Tile_Info* tile_sand = add_tile_info("sand", (real)(0.9), (real)0.8, rect2((32) * (0), (32) * (1), (32), (32)), false, "dug_earth");
	Tile_Info* tile_grass = add_tile_info("grass", (real)(1), (real)0.5, rect2((32) * (1), (32) * (1), (32), (32)), false, "dug_earth");
	Tile_Info* tile_dense_grass = add_tile_info("dense_grass", (real)(1), (real)0.5, rect2((32) * (2), (32) * (1), (32), (32)), false, "dug_earth");
	Tile_Info* tile_dug_earth = add_tile_info("dug_earth", (real)(0.9), (real)0.5, rect2((32) * (3), (32) * (1), (32), (32)), false, "dug_earth");
	tile_dug_earth->immune_to_damage = true;

	Tile_Info* tile_tree_wall = add_tile_info("tree_wall", (real)(1), (real)1, rect2((32) * (1), (32) * (4), (32), (32)), true, "dug_earth");
	tile_tree_wall->has_top_texture = true;
	tile_tree_wall->top_texture = rect2((32) * (1), (32) * (3), (32), (32));
	tile_tree_wall->has_bottom_texture = true;
	tile_tree_wall->bottom_texture = rect2((32) * (2), (32) * (4), (32), (32));


	Tile_Info* tile_water = add_tile_info("water", (real)(1), (real)1, rect2((32) * (1), (32) * (2), (32), (32)), true, "dug_earth");
	tile_water->body_group = 1;
	Tile_Info* tile_shallow_water = add_tile_info("shallow_water", (real)(0.7), (real)0.99, rect2((32) * (0), (32) * (2), (32), (32)), false, "dug_earth");
	Tile_Info* tile_stone_road = add_tile_info("stone_road", (real)(1.2), (real)0.99, rect2((32) * (2), (32) * (2), (32), (32)), false, "dug_earth");
	Tile_Info* tile_cracked_road = add_tile_info("cracked_road", (real)(1.2), (real)0.99, rect2((32) * (3), (32) * (2), (32), (32)), false, "dug_earth");
	Tile_Info* tile_pristine_road = add_tile_info("pristine_road", (real)(1.2), (real)0.99, rect2((32) * (4), (32) * (2), (32), (32)), false, "dug_earth");
	tile_stone_road->max_damage = 20;
	Tile_Info* tile_slab_wall = add_tile_info("slab_wall", (real)(1), (real)1, rect2((32) * (0), (32) * (3), (32), (32)), true, "dug_earth");
	
	
	tile_slab_wall->has_bottom_texture = true;
	tile_slab_wall->bottom_texture = rect2((32) * (0), (32) * (4), (32), (32));
}



Item_Info* add_item_type(const char* name, int32 max_stack, Rect2 texture)
{
	Item_Info* item = Registry->items + Registry->items_count++; 
	item->id = Registry->items_count - 1;
	item->name = name;
	item->max_stack = max_stack;
	item->texture = texture;
	item->hash = hash_str(name);
	Registry->items_hash[Registry->items_count - 1] = item->hash;
	return item;
}

void register_all_rituals_item_info()
{
	Item_Info* item_none = add_item_type("none", (0), rect2((32) * (0), (32) * (0), (32), (32)));
	Item_Info* item_hooknife = add_item_type("hooknife", (1), rect2((32) * (0), (32) * (5), (32), (32)));
	Item_Info* item_rope = add_item_type("rope", (8), rect2((32) * (1), (32) * (5), (32), (32)));
	Item_Info* item_book = add_item_type("book", (64), rect2((32) * (2), (32) * (5), (32), (32)));
	Item_Info* item_rock = add_item_type("rock", (64), rect2((32) * (3), (32) * (0), (32), (32)));
}

void register_everything_in_rituals()
{
	register_all_rituals_item_info();
	register_all_rituals_tile_info();
}
#line 165 "src\\rituals_main.cpp"

#line 1 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_inventory.cpp"
















struct Item_Stack
{
	isize id;
	Item_Info* info;

	int32 number;
};

void init_item_stack(Item_Stack* stack, Item_Info* info)
{
	stack->id = 0;
	stack->info = info;
	stack->number = 1;
}




Item_Stack* new_item_stack(Item_Info* info, Memory_Arena* arena)
{
	Item_Stack* stack = ((Item_Stack*)arena_push(arena, sizeof(Item_Stack)));
	init_item_stack(stack, info);
	return stack;
}

struct Inventory
{
	Item_Stack** contents;
	isize w, h, size;
};

void init_inventory(Inventory* inventory, isize w, isize h, Memory_Arena* arena)
{
	isize size = w * h;
	inventory->w = w;
	inventory->h = h;
	inventory->size = size;
	inventory->contents = ((Item_Stack**)arena_push(arena, sizeof(Item_Stack*) * size));
}

bool inventory_add_item(Inventory* inventory, Item_Stack** origin_slot)
{
	for(isize i = 0; i < inventory->h; ++i) {
		for(isize j = 0; j < inventory->w; ++j) {
			Item_Stack** slot = inventory->contents + (i * inventory->w + j);
			
			if (*slot == NULL || 
				(*slot)->info == NULL || 
				(*slot)->info->id == Item_None) {

				*slot = *origin_slot;
				*origin_slot = NULL;
				return true;
			}
		}
	}
	return false;
}



void render_inventory(Inventory* inventory, Vec2 position)
{
	Sprite s;
	for(isize i = 0; i < inventory->h; ++i) {
		for(isize j = 0; j < inventory->w; ++j) {
			init_sprite(&s);
			
			s.position = position + v2((32) * j, (32) * i);
			s.flags = Anchor_Top_Left;
			s.size = v2(32, 32);

					
			render_add(CurrentGroup, &s);

			Item_Stack* at = inventory->contents[i * inventory->w + j];
			if(at == NULL || at->info == NULL || at->info->id == Item_None) {
				continue;
			}

			s.texture = at->info->texture;
			render_add(CurrentGroup, &s);
		}
	}
}

#line 167 "src\\rituals_main.cpp"

#line 1 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_tilemap.cpp"















struct Tilemap
{
	isize w, h;
	Tile* tiles;
	Tile_State* states;
};




void init_tilemap(Tilemap* tilemap, isize w, isize h, Memory_Arena* arena)
{
	tilemap->w = w;
	tilemap->h = h;
	tilemap->tiles = ((Tile*)arena_push(arena, sizeof(Tile) * w * h));
	tilemap->states = ((Tile_State*)arena_push(arena, sizeof(Tile_State) * w * h));
}

void generate_tilemap(Tilemap* tilemap, uint64 seed)
{
	start_temp_arena(Game->temp_arena);
	isize minified_w = tilemap->w / 8;
	isize minified_h = tilemap->h / 8;
	real* initial = ((real*)arena_push(Game->temp_arena, sizeof(real) * minified_w * minified_h));

	Random rs;
	Random* r = &rs;
	init_random(r, seed);

	for(isize i = 0; i < minified_h; ++i) {
		for(isize j = 0; j < minified_w; ++j) {
			real rr = rand_range(r, 0, 2);
			initial[i * minified_w + j] = (rr * rr) / 4.0f;
		}
	}

	minified_w *= 2;
	minified_h *= 2;
	real* second = ((real*)arena_push(Game->temp_arena, sizeof(real) * minified_w * minified_h));
		
	for(isize i = 0; i < minified_h; ++i) {
		for(isize j = 0; j < minified_w; ++j) {
			isize sample_x = j / 2;
			isize sample_y = i / 2;
			real val = rand_range(r, 0, 2.0f);
			real prev = initial[sample_y * minified_w / 2 + sample_x]; 
			second[i * minified_w + j] = prev + (val * val) / 4.0f;
		}
	}


	minified_w *= 2;
	minified_h *= 2;
	real* third = ((real*)arena_push(Game->temp_arena, sizeof(real) * minified_w * minified_h));
		
	for(isize i = 0; i < minified_h; ++i) {
		for(isize j = 0; j < minified_w; ++j) {
			isize sample_x = j / 2;
			isize sample_y = i / 2;
			third[i * minified_w + j] = second[sample_y * minified_w / 2 + sample_x] + rand_range(r, 0, 1.0f);
			real val = rand_range(r, 0, 2.0f);
			real prev = second[sample_y * minified_w / 2 + sample_x]; 
			third[i * minified_w + j] = prev + (val * val) / 4.0f;
		}
	}

	minified_w *= 2;
	minified_h *= 2;
	real* fourth = ((real*)arena_push(Game->temp_arena, sizeof(real) * minified_w * minified_h));
		
	for(isize i = 0; i < minified_h; ++i) {
		for(isize j = 0; j < minified_w; ++j) {
			isize sample_x = j / 2;
			isize sample_y = i / 2;
			real val = rand_range(r, 0, 2.0f);
			real prev = third[sample_y * minified_w / 2 + sample_x]; 
			fourth[i * minified_w + j] = prev + (val * val) / 4.0f;
			fourth[i * minified_w + j] /= 4.0f;
		}
	}

	bool flag = false;
	for(isize i = 0; i < tilemap->h; ++i) {
		for(isize j = 0; j < tilemap->w; ++j) {
			isize index = i * tilemap->w + j;
			if( i == 0 || 
				i == tilemap->h - 1 || 
				j == 0 ||
				j == tilemap->w - 1) {
				tilemap->tiles[index] = Tile_Tree_Wall;
				flag = true;
			}

			if((i >= (tilemap->h/2 - 1) &&
				i <= (tilemap->h/2 + 1)) ||
			   (j >= (tilemap->w/2 - 1) && 
				j <= (tilemap->w/2 + 1))) {
				tilemap->tiles[index] = Tile_Stone_Road;
				if(rand_range(r, 0, 1) < 0.25f) {
					tilemap->tiles[index] = Tile_Cracked_Road;
				
				
				
				}
				flag = true;
			}

			if(flag) {
				flag = false;
				continue;
			}


			real height = fourth[index];
			if(height < 0.10f) {
				tilemap->tiles[index] = Tile_Water;
			}
			else if(height < 0.22f) {
				
				tilemap->tiles[index] = Tile_Shallow_Water;
			} else if(height < 0.25f) {
				
				tilemap->tiles[index] = Tile_Sand;
			} else if(height < 0.4f) {
				
				tilemap->tiles[index] = Tile_Grass;
			} else if(height < 0.5f) {
				
				tilemap->tiles[index] = Tile_Dense_Grass;
			} else if(height < 0.7f) {
				
				tilemap->tiles[index] = Tile_Tree_Wall;
			} else  {
				tilemap->tiles[index] = Tile_Tree_Wall;
			}
		}
	}
	for(isize i = 0; i < tilemap->h; ++i) {
		for(isize j = 0; j < tilemap->w; ++j) {
			isize index = i * tilemap->w + j;
			init_tile_state(tilemap->states + index, tilemap->tiles[index]);
		}
	}

	end_temp_arena(Game->temp_arena);

}

Tile tilemap_get_at(Tilemap* tilemap, isize x, isize y)
{
	isize i = y * tilemap->w + x;
	if(i < 0 || i >= (tilemap->w * tilemap->h)) return 0;
	return tilemap->tiles[i];
}

Tile tilemap_get_at(Tilemap* tilemap, Vec2 pos)
{
	isize x = (isize)(pos.x / (32));
	isize y = (isize)(pos.y / (32));
	return tilemap_get_at(tilemap, x, y);
}

Tile_State* tilemap_get_state_at(Tilemap* tilemap, isize x, isize y)
{
	isize i = y * tilemap->w + x;
	if(i < 0 || i >= (tilemap->w * tilemap->h)) return NULL;
	return tilemap->states + i;
}

Tile_State* tilemap_get_state_at(Tilemap* tilemap, Vec2 pos)
{
	isize x = (isize)(pos.x / (32));
	isize y = (isize)(pos.y / (32));
	return tilemap_get_state_at(tilemap, x, y);
}

bool tilemap_set_at(Tilemap* tilemap, isize x, isize y, Tile value)
{
	isize i = y * tilemap->w + x;
	if(i < 0 || i >= (tilemap->w * tilemap->h)) return true;
	tilemap->tiles[i] = value;
	return false;
}

bool tilemap_set_at(Tilemap* tilemap, Vec2 pos, Tile value)
{
	isize x = (isize)(pos.x / (32));
	isize y = (isize)(pos.y / (32));
	return tilemap_set_at(tilemap, x, y, value);
}

void update_tile_state_at(Tilemap* map, isize x, isize y)
{
	Tile_State* state = tilemap_get_state_at(map, x, y);
	if(state != NULL) {
		Tile_Info* info = Registry->tiles + tilemap_get_at(map, x, y);
		if(info->immune_to_damage) {
			state->damage = 0;
		} else {
			if(state->damage >= info->max_damage) {
				init_tile_state(state, info->break_to_id);
				tilemap_set_at(map, x, y, info->break_to_id);
			}
		}

	}
}

void update_tile_state_at(Tilemap* map, Vec2 pos)
{
	isize x = (isize)(pos.x / (32));
	isize y = (isize)(pos.y / (32));
	return update_tile_state_at(map, x, y);
}



void render_tilemap(Tilemap* tilemap, Vec2 pos)
{
	Sprite s;
	for(isize i = 0; i < tilemap->h; ++i) {
		for(isize j = 0; j < tilemap->w; ++j) {
			Tile_Info* t = Registry->tiles + tilemap->tiles[i * tilemap->w + j];
			init_sprite(&s);
			s.position = v2(j*32, i*32) + pos;
			s.size = v2(32, 32);
			s.texture = t->texture;
			render_add(&s);
		}
	}
}

isize render_tilemap(Tilemap* tilemap, Vec2 pos, Rect2 area)
{
	isize startx = area.x / (32) - 1;
	isize starty = area.y / (32) - 1;
	isize endx = (area.x + area.w) / (32) + 1;
	isize endy = (area.y + area.h) / (32) + 1;
	if(startx < 0) startx = 0;
	if(starty < 0) starty = 0;
	if(endx > tilemap->w) endx = tilemap->w;
	if(endy > tilemap->h) endy = tilemap->h;
	Sprite s;
	for(isize i = starty; i < endy; ++i) {
		for(isize j = startx; j < endx; ++j) {
			Tile tile = tilemap->tiles[i * tilemap->w + j];
			Tile_Info* t = Registry->tiles + tile;
			init_sprite(&s);
			s.position = v2(j*(32) + (16), i*(32) + (16)) + pos;
			s.size = v2((32), (32));

			if(t->has_bottom_texture && tilemap_get_at(tilemap, j, i + 1) != tile) {
				s.texture = t->bottom_texture;
			} else {
				s.texture = t->texture;
				render_add(&s);
			}
			render_add(&s);

			Tile_State* state = tilemap->states + (i * tilemap->w + j);
			if(state->damage > 0) {
				real dmgp = (real)(state->damage) / (real)(t->max_damage);
				dmgp *= 3;
				int32 frame = (int32)dmgp;
				s.texture = rect2((32) * ((5 + frame)), (32) * (0), (32), (32));
				render_add(&s);
			}
		}
	}
	isize unsorted_index = Renderer->groups[0].sprites_count;
	for(isize i = starty; i < endy; ++i) {
		for(isize j = startx; j < endx; ++j) {
				Tile tile = tilemap->tiles[i * tilemap->w + j];
				Tile bottom_tile = tilemap_get_at(tilemap, j, i + 1);
				if(tile != bottom_tile) {
					Tile_Info* bottom = Registry->tiles + tilemap_get_at(tilemap, j, i + 1);
					if(bottom->has_top_texture) {
						init_sprite(&s);
						s.position = v2(j*(32) + (16), (i+1)*(32)) + pos;
						s.size = v2((32), (32));
						s.texture = bottom->top_texture;
						s.flags = Anchor_Bottom;
						s.sort_offset = 4;
						render_add(&s);
					}
				}

		}
	}
	return unsorted_index;
}

#line 169 "src\\rituals_main.cpp"

#line 1 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_simulation.cpp"














enum Sim_Body_Flags
{
	Body_Flag_None,
	Body_Flag_Static = (1 << 1),
	Body_Flag_No_Friction = (1 << 2),
	Body_Flag_Sensor = (1 << 3),
	Body_Flag_Always_Contact = (1 << 4),
};


typedef struct Entity Entity;
struct Sim_Body
{
	isize id;
	AABB shape;
	Vec2 velocity, force, collision_vel;
	real inv_mass, restitution, damping;
	uint64 flags;
	uint64 group;
	uint64 mask;
	isize entity_id;
	Entity* entity;
};

struct Sim_Contact
{
	isize a_id;
	isize b_id;
	Sim_Body* static_b;
	Vec2 overlap;
	real mag;
	Vec2 normal;
};



void body_sort_on_x(Sim_Body* array, isize count) { if(count > 1) if(count > 12) { Sim_Body tmp = array[0]; array[0] = array[count / 2]; array[count / 2] = tmp; isize pivot = 0; for(isize i = 1; i < count; ++i) { if((array[i].shape.center.x - array[i].shape.hw) < (array[0].shape.center.x - array[0].shape.hw)) { tmp = array[++pivot]; array[pivot] = array[i]; array[i] = tmp; } } tmp = array[0]; array[0] = array[pivot]; array[pivot] = tmp; body_sort_on_x(array, pivot); body_sort_on_x(array + pivot + 1, count - (pivot + 1)); } else for(isize i = 1; i < count; ++i) { Sim_Body x = array[i]; isize j = i - 1; while((j >= 0) && ((array[j].shape.center.x - array[j].shape.hw) > (x.shape.center.x - x.shape.hw))) { array[j + 1] = array[j]; j--; } array[j+1] = x; } }
void body_sort_on_y(Sim_Body* array, isize count) { if(count > 1) if(count > 12) { Sim_Body tmp = array[0]; array[0] = array[count / 2]; array[count / 2] = tmp; isize pivot = 0; for(isize i = 1; i < count; ++i) { if((array[i].shape.center.y - array[i].shape.hh) < (array[0].shape.center.y - array[0].shape.hh)) { tmp = array[++pivot]; array[pivot] = array[i]; array[i] = tmp; } } tmp = array[0]; array[0] = array[pivot]; array[pivot] = tmp; body_sort_on_y(array, pivot); body_sort_on_y(array + pivot + 1, count - (pivot + 1)); } else for(isize i = 1; i < count; ++i) { Sim_Body x = array[i]; isize j = i - 1; while((j >= 0) && ((array[j].shape.center.y - array[j].shape.hh) > (x.shape.center.y - x.shape.hh))) { array[j + 1] = array[j]; j--; } array[j+1] = x; } }


void body_sort_static_first(Sim_Body* array, isize count) { if(count > 1) if(count > 12) { Sim_Body tmp = array[0]; array[0] = array[count / 2]; array[count / 2] = tmp; isize pivot = 0; for(isize i = 1; i < count; ++i) { if((!(array[i].flags & Body_Flag_Static)) < (!(array[0].flags & Body_Flag_Static))) { tmp = array[++pivot]; array[pivot] = array[i]; array[i] = tmp; } } tmp = array[0]; array[0] = array[pivot]; array[pivot] = tmp; body_sort_static_first(array, pivot); body_sort_static_first(array + pivot + 1, count - (pivot + 1)); } else for(isize i = 1; i < count; ++i) { Sim_Body x = array[i]; isize j = i - 1; while((j >= 0) && ((!(array[j].flags & Body_Flag_Static)) > (!(x.flags & Body_Flag_Static)))) { array[j + 1] = array[j]; j--; } array[j+1] = x; } }


void body_sort_on_id(Sim_Body* array, isize count) { if(count > 1) if(count > 12) { Sim_Body tmp = array[0]; array[0] = array[count / 2]; array[count / 2] = tmp; isize pivot = 0; for(isize i = 1; i < count; ++i) { if((array[i].id) < (array[0].id)) { tmp = array[++pivot]; array[pivot] = array[i]; array[i] = tmp; } } tmp = array[0]; array[0] = array[pivot]; array[pivot] = tmp; body_sort_on_id(array, pivot); body_sort_on_id(array + pivot + 1, count - (pivot + 1)); } else for(isize i = 1; i < count; ++i) { Sim_Body x = array[i]; isize j = i - 1; while((j >= 0) && ((array[j].id) > (x.id))) { array[j + 1] = array[j]; j--; } array[j+1] = x; } }
isize body_search_for_id(isize key, Sim_Body* array, isize count) { if(count == 0) return -1; isize min = 0, max = count - 1, mid = 0; isize localkey; while(min <= max) { mid = (min + max) / 2; localkey = (array[mid].id); if(localkey == key) { return mid; } else if(localkey < key) { min = mid + 1; } else { max = mid - 1; } } return -1; }

void init_body(Sim_Body* b)
{
	b->shape = aabb(v2(0, 0), 0, 0);
	b->inv_mass = 1.0f;
	b->restitution = 0.3f;
	b->velocity = v2(0,0);
	b->damping = 0.5f;
	b->force = v2(0, 0);
	b->flags = Body_Flag_None;
	b->mask = 0;
	b->group = 0;
}


struct Sim_Grid_Cell
{
	Sim_Body* body;
	Sim_Grid_Cell* next;
};

struct Sim_Static_Grid
{
	Sim_Grid_Cell* cell_storage;
	isize cell_storage_count, cell_storage_capacity;

	Sim_Grid_Cell** cells;
	isize cells_length;
	Vec2i size;
};

void init_static_grid(Sim_Static_Grid* grid, Vec2i size, isize capacity, Memory_Arena* arena)
{
	grid->cell_storage = ((Sim_Grid_Cell*)arena_push(arena, sizeof(Sim_Grid_Cell) * capacity));
	grid->cell_storage_capacity = capacity;
	grid->cell_storage_count = 0;

	size.x += 1;
	size.y += 1;

	grid->cells = ((Sim_Grid_Cell**)arena_push(arena, sizeof(Sim_Grid_Cell*) * size.x * size.y));
	grid->size = size;
	grid->cells_length = size.x * size.y;
}

Sim_Grid_Cell* get_next_grid_cell(Sim_Static_Grid* grid)
{
	if(grid->cell_storage_count >= grid->cell_storage_capacity) {
		
		printf("There was an error: %s \n", "Ran out of grid space");;
		grid->cell_storage_count = 0;
	}
	Sim_Grid_Cell* cell = grid->cell_storage + grid->cell_storage_count++;
	cell->body = NULL;
	cell->next = NULL;
	return cell;
}


void build_static_grid(Sim_Static_Grid* grid, Sim_Body* bodies, isize count)
{
	isize width = grid->size.x;
	for(isize i = 0; i < count; ++i) {
		Sim_Body* b = bodies + i;
		AABB s = b->shape;
		isize min_x, max_x, min_y, max_y;
		min_x = (s.center.x - s.hw) / ((32) * 4.0f);
		min_y = (s.center.y - s.hh) / ((32) * 4.0f);
		max_x = (s.center.x + s.hw) / ((32) * 4.0f);
		max_y = (s.center.y + s.hh) / ((32) * 4.0f);

		if(min_x < 0) min_x = 0;
		if(max_x >= grid->size.x) min_x = grid->size.x - 1;

		if(min_y < 0) min_y = 0;
		if(max_y >= grid->size.y) min_y = grid->size.y - 1;

		for(isize y = min_y; y <= max_y; ++y) {
			for(isize x = min_x; x <= max_x; ++x) {
				isize index = x + y * width;
				if(grid->cells[index] == NULL) {
					grid->cells[index] = get_next_grid_cell(grid);			
				}
				Sim_Grid_Cell* c = grid->cells[index];
				if(c->body != NULL) {
					grid->cells[index] = get_next_grid_cell(grid);
					grid->cells[index]->next = c;
					c = grid->cells[index];
				}
				c->body = b;
			}
		}
	}
}

struct Simulator
{
	Sim_Body* static_bodies;
	isize static_bodies_count, static_bodies_capacity;

	Sim_Static_Grid* grid;

	Sim_Body* bodies;
	isize bodies_count, bodies_capacity, next_body_id;
	Sim_Contact* contacts;
	isize contacts_count, contacts_capacity;

	isize sort_axis;
};

Sim_Body* sim_get_next_static_body(Simulator* sim)
{
	if(sim->bodies_count + 1 > sim->bodies_capacity) {
		printf("There was an error: %s \n", "Ran out of bodies");;
		return NULL;
	}

	Sim_Body* e = sim->static_bodies + sim->static_bodies_count++;
	init_body(e);
	e->id = -1;

	return e;
}

Sim_Body* sim_get_next_body(Simulator* sim)
{
	if(sim->bodies_count + 1 > sim->bodies_capacity) {
		printf("There was an error: %s \n", "Ran out of bodies");;
		return NULL;
	}

	Sim_Body* e = sim->bodies + sim->bodies_count++;
	init_body(e);

	e->id = sim->next_body_id++;

	return e;
}

void init_simulator(Simulator* sim, isize cap, Memory_Arena* arena)
{
	sim->bodies_count = 0;
	sim->bodies_capacity = cap;
	sim->static_bodies_capacity = cap;
	sim->static_bodies_count = 0;
	sim->sort_axis = 0;
	sim->next_body_id = 0;
	sim->grid = ((Sim_Static_Grid*)arena_push(arena, sizeof(Sim_Static_Grid)));
	init_static_grid(sim->grid, v2i((256)/4, (256)/4), cap ,arena);
	sim->bodies = ((Sim_Body*)arena_push(arena, sizeof(Sim_Body) * cap));
	sim->static_bodies = ((Sim_Body*)arena_push(arena, sizeof(Sim_Body) * cap));
	sim->contacts_capacity = ((512>cap / 16)?512:cap / 16);
	sim->contacts = ((Sim_Contact*)arena_push(arena, sizeof(Sim_Contact) * sim->contacts_capacity));
	
}

Sim_Body* sim_find_body(Simulator* sim, isize id)
{
	isize index = body_search_for_id(id, sim->bodies, sim->bodies_count);
	return index == -1? NULL: sim->bodies + index;
}

void sim_sort_bodies_on_id(Simulator* sim)
{
	body_sort_on_id(sim->bodies, sim->bodies_count);
}

void sim_remove_body(Simulator* sim, isize body)
{
	isize index = body_search_for_id(body, sim->bodies, sim->bodies_count);
	sim->bodies[index] = sim->bodies[--sim->bodies_count];
	sim_sort_bodies_on_id(sim);
}

void sim_remove_body(Simulator* sim, Sim_Body* body)
{
	sim_remove_body(sim, body->id);
}

Sim_Body* sim_query_aabb(Simulator* sim, AABB query)
{
	for(isize i = 0; i < sim->bodies_count; ++i) {
		Sim_Body* a = sim->bodies + i;
		if(aabb_intersect(&a->shape, &query)) {
			return a;
		}
	}
	return NULL;
}






int32 _do_collide_bodies(Sim_Body* a, Sim_Body* b, Simulator* sim, bool do_sweep)
{
	if(do_sweep)
	if(sim->sort_axis == 0) {
		if((b->shape.center.x - b->shape.hw) > (a->shape.center.x + a->shape.hw)) {
			return -1;
		}
	} else if(sim->sort_axis == 1) {
		if((b->shape.center.y - b->shape.hh) > (a->shape.center.y + a->shape.hh)) {
			return -1;
		}
	}

	if((a->flags & Body_Flag_Static)) {
		if((b->flags & Body_Flag_Static)) {
			return 0;
		}
	}
	uint64 ma = a->mask & b->group;
	if(ma != 0) {
		return 0;
	}

	uint64 mb = a->group & b->mask;
	if(mb != 0) {
		return 0;		
	}

	return aabb_intersect(&a->shape, &b->shape);
}


void _separate_bodies(Sim_Body* a, Sim_Body* b, bool capture_contacts, int32 times, Simulator* sim)
{
	uint32 a_is_static = (a->flags & Body_Flag_Static);
	uint32 b_is_static = (b->flags & Body_Flag_Static);

	Vec2 overlap;
	aabb_overlap(&a->shape, &b->shape, &overlap);
	real ovl_mag = sqrtf(v2_dot(overlap, overlap));
	if (ovl_mag < 0.0001f) return;
	Vec2 normal = overlap * (1.0f / ovl_mag);

	if(capture_contacts && 
			((times == 1) || 
			 (a->flags & Body_Flag_Always_Contact) || 
			 (b->flags & Body_Flag_Always_Contact))) {
		Sim_Contact c;
		c.a_id = a->id;
		c.b_id = b->id;
		if(b_is_static) {
			c.static_b = b;
			c.b_id = -1;
		} else {
			c.static_b = NULL;
		}
		c.overlap = overlap;
		c.normal = normal;
		c.mag = ovl_mag;
		if(sim->contacts_count < sim->contacts_capacity) {
			sim->contacts[sim->contacts_count++] = c;
		}
	}


	if((a->flags & Body_Flag_Sensor) ||
			(b->flags & Body_Flag_Sensor)) {
		return;
	}
	if(a_is_static && !b_is_static) {
		b->shape.center += overlap;
		Vec2 relative_velocity = b->velocity;
		real velocity_on_normal = v2_dot(relative_velocity, normal);
		if(velocity_on_normal > 0) return;

		real e = ((a->restitution<b->restitution)?a->restitution:b->restitution);
		real mag = -1.0f * (1.0f + e) * velocity_on_normal;
		mag /= b->inv_mass;
		Vec2 impulse = mag * normal;
		b->collision_vel += b->inv_mass * impulse;
	} else if(!a_is_static && b_is_static) {
		a->shape.center -= overlap;

		Vec2 relative_velocity = -a->velocity;
		real velocity_on_normal = v2_dot(relative_velocity, normal);
		if(velocity_on_normal > 0) return;

		real e = ((a->restitution<b->restitution)?a->restitution:b->restitution);
		real mag = -1.0f * (1.0f + e) * velocity_on_normal;
		mag /= a->inv_mass + 0;
		Vec2 impulse = mag * normal;
		a->collision_vel -= a->inv_mass * impulse;
	} else {
		Vec2 separation = ((ovl_mag - (0.8f)>0)?ovl_mag - (0.8f):0) 
			* (1.0f / (a->inv_mass + b->inv_mass)) * 0.5f * normal;
		a->shape.center -= a->inv_mass * separation;
		b->shape.center += b->inv_mass * separation;

		Vec2 relative_velocity = b->velocity - a->velocity;
		real velocity_on_normal = v2_dot(relative_velocity, normal);
		if(velocity_on_normal > 0) return;

		real e = ((a->restitution<b->restitution)?a->restitution:b->restitution);
		real mag = -1.0f * (1.0f + e) * velocity_on_normal;
		mag /= a->inv_mass + b->inv_mass;
		Vec2 impulse = mag * normal;
		a->collision_vel -= a->inv_mass * impulse;
		b->collision_vel += b->inv_mass * impulse;
	}
}

void sim_update(Simulator* sim, Tilemap* map, real dt, bool capture_contacts = true)
{
	if(capture_contacts)
		sim->contacts_count = 0;
	Sim_Body *a, *b;
	for(isize times = 0; times < (8); ++times) {
		if(sim->sort_axis == 0) {
			body_sort_on_x(sim->bodies, sim->bodies_count);
		} else if(sim->sort_axis == 1) {
			body_sort_on_y(sim->bodies, sim->bodies_count);
		}
		Vec2 center_sum1 = v2(0, 0);
		Vec2 center_sum2 = v2(0, 0);
		Vec2 variance = v2(0, 0);
		for(isize i = 0; i < sim->bodies_count; ++i) {
			a = sim->bodies + i;

			
			center_sum1 += a->shape.center;
			for(isize q = 0; q < 2; ++q) {
				center_sum2.e[q] += a->shape.center.e[q] * a->shape.center.e[q];
			}

			AABB s = a->shape;
			isize min_x, max_x, min_y, max_y;
			min_x = (s.center.x - s.hw) / ((32) * 4.0f);
			min_y = (s.center.y - s.hh) / ((32) * 4.0f);
			max_x = (s.center.x + s.hw) / ((32) * 4.0f);
			max_y = (s.center.y + s.hh) / ((32) * 4.0f);

			if(min_x < 0) min_x = 0;
			if(max_x >= sim->grid->size.x) max_x = sim->grid->size.x - 1;

			if(min_y < 0) min_y = 0;
			if(max_y >= sim->grid->size.y) max_y = sim->grid->size.y - 1;

			for(isize y = min_y; y <= max_y; ++y) {
				for(isize x = min_x; x <= max_x; ++x) {
					isize index = x + y * sim->grid->size.x;
					Sim_Grid_Cell* c = sim->grid->cells[index];
					if(c == NULL) continue;
					
					do {
						b = c->body;
						if(_do_collide_bodies(a, b, sim, false)) 
							_separate_bodies(a, b, capture_contacts, times, sim);
					} while(c = c->next);
				}
			}

			for(isize j = i + 1; j < sim->bodies_count; ++j) {
				b = sim->bodies + j;


				int32 out = _do_collide_bodies(a, b, sim, true);
				if(out == -1) break;
				else if(out == 1) {
					_separate_bodies(a, b, capture_contacts, times, sim);
				}
			}
		}

		for(isize i = 0; i < 2; ++i) {
			variance.e[i] = center_sum2.e[i] - center_sum1.e[i] * center_sum1.e[i] / 
				sim->bodies_count;
		}
		
		if(variance.x > variance.y) {
			sim->sort_axis = 0;
		} else {
			sim->sort_axis = 1;
		}

		for(isize i = 0; i < sim->bodies_count; ++i) {
			a = sim->bodies + i;
			if((a->flags & Body_Flag_Static)) continue;
			Vec2 iter_acl = (a->force * a->inv_mass) / ((real)(8));
			Vec2 new_vel = a->velocity + (dt * iter_acl);
			Vec2 dpos = (a->velocity + new_vel) * 0.5f;
			dpos *= 1.0f / ((real)(8));
			a->shape.center += dpos * dt;
			a->shape.center += a->collision_vel / ((real)(8)) * dt;
			a->velocity = new_vel;
			Tile_Info* tile = Registry->tiles + tilemap_get_at(map, a->shape.center);
			real damping = 1.0f;
			if((a->flags & Body_Flag_No_Friction)) {
				damping = a->damping;
			} else {
				damping = sqrtf(a->damping * a->damping + 
					tile->friction * tile->friction) * 0.7071067f;
			}
			for(isize i = 0; i < ((real)(8)); ++i) {
				damping *= damping;
			}
			a->velocity *= damping;
			a->velocity += a->collision_vel;
			a->collision_vel = v2(0, 0);
		}

	}
	body_sort_on_id(sim->bodies, sim->bodies_count);
}

isize _tw, _th;
Tile* _tiles;
Tile_Info* _get_at(isize x, isize y)
{
	if((x < 0) || (x > _tw) || (y < 0) || (y > _th)) return Registry->tiles;
	isize index = y * _tw + x;
	if((index < 0) || (index >= _tw * _th)) return Registry->tiles;
	return Registry->tiles + _tiles[index];

}

void generate_statics_for_tilemap(Simulator* sim, Tilemap* tilemap)
{
	start_temp_arena(Game->temp_arena);
	_tw = tilemap->w;
	_th = tilemap->h;
	isize map_size = tilemap->w * tilemap->h;
	Tile* tiles = ((Tile*)arena_push(Game->temp_arena, sizeof(Tile) * map_size + 1));
	memcpy(tiles, tilemap->tiles, sizeof(Tile) * map_size);
	_tiles = tiles;
	isize work = 0;

	Rect2i* rects = ((Rect2i*)arena_push(Game->temp_arena, sizeof(Rect2i) * map_size / 2));
	isize rects_count = 0;
	isize last_rects = 0;
	do {
		last_rects = rects_count;
		for(isize y = 0; y < tilemap->h; ++y) {
			for(isize x = 0; x < tilemap->w; ++x) {
				if(_get_at(x, y)->solid) {
					if(!_get_at(x, y - 1)->solid) {
						Rect2i* r = rects + rects_count++;
						r->x = x;
						r->y = y;
						r->w = 1;
						r->h = 1;
						Tile_Info *first, *here, *up;
						first = _get_at(x, y);
						do {
							x++;
							here = _get_at(x, y);
							up = _get_at(x, y-1);
						}
						while(  here->solid && !up->solid &&
								(here->body_mask == first->body_mask) &&
								(here->body_group == first->body_group) &&
								(x < tilemap->w));


						if(x != r->x) {
							r->w = x - r->x;
						}
					}	
				}
			}
		}

		for(isize i = last_rects; i < rects_count; ++i) {
			Rect2i* r = rects + i;
			bool solid = true;
			isize y = r->y;
			Tile_Info *first, *here;
			first = _get_at(r->x, r->y);
			while(solid && (y < tilemap->h)) {
				for(isize local_x = 0; local_x < r->w; ++local_x) {
					here = _get_at(r->x + local_x, y + 1);
					solid = solid && here->solid &&
						(here->body_mask == first->body_mask) &&
						(here->body_group == first->body_group);
					if(!solid) break;
				}
				if(solid) {
					y++;
					r->h++;
				}
			}
		}

		for(isize i = 0; i < rects_count; ++i) {
			Rect2i* r = rects + i;
			for(isize local_y = 0; local_y < r->h; ++local_y) {
				for(isize local_x = 0; local_x < r->w; ++local_x) {
					isize index = (local_y + r->y) * tilemap->w + (local_x + r->x);
					
					tiles[index] = 0;
				}
			}
		}
		work = 0;
		for(isize i = 0; i < map_size; ++i) {
			work += (int32)(Registry->tiles[tiles[i]].solid);
		}
	} while(work);
	
	_tiles = tilemap->tiles;
	for(isize i = 0; i < rects_count; ++i) {
		Rect2i* r = rects + i;
		Tile_Info* first = _get_at(r->x, r->y);
		Sim_Body* e = sim_get_next_static_body(sim);
		e->shape.center.x = (r->x + r->w / 2.0f) * (32);
		e->shape.center.y = (r->y + r->h / 2.0f) * (32);
		e->shape.hw = r->w * (16);
		e->shape.hh = r->h * (16);
		e->restitution = 0.3f;
		e->inv_mass = 0.0f;
		e->flags = Body_Flag_Static;
		e->group = first->body_group;
		e->mask = first->body_mask;
	}

	build_static_grid(sim->grid, sim->static_bodies, sim->static_bodies_count);

	end_temp_arena(Game->temp_arena);
}

#line 171 "src\\rituals_main.cpp"

#line 1 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_particles.cpp"













struct Particle
{
	Vec3 position;
	Vec3 velocity;
	real scale;
	real angle;
	real angular_vel;
	int32 frame;
	int32 time;
	int32 total_time;
	int32 style_index;
};

struct Particle_Style
{
	int32 id;
	Rect2 texture;
	Vec2 size;
	Vec4 color;
	Vec4 shadow_color;

	Vec3 acceleration;

	real impulse_min;
	real impulse_max;
	
	real angle_min;
	real angle_max;

	real angle_vel_min;
	real angle_vel_max;

	real scale_min;
	real scale_max;

	int32 frame_min;
	int32 frame_max;
	int32 max_frames;
	int32 framerate;

	int32 time_min;
	int32 time_max;

	bool time_scaling;
	bool time_alpha;
	real ground_restitution;
	real skid_on_bounce_min;
	real skid_on_bounce_max;
	real jitter_on_bounce_mag;
};

Particle_Style copy_particle_style(Particle_Style s,
		Vec2 impulse_range, Vec2i time_range)
		 
{
	s.impulse_min = impulse_range.x;
	s.impulse_max = impulse_range.y;
	s.time_min = time_range.x;
	s.time_max = time_range.y;
	return s;
}

Particle_Style make_particle_style(
		Rect2 texture, 
		Vec2 size, 
		Vec4 color, 
		Vec4 shadow_color,
		Vec3 acl, 
		Vec2 impulse_range, 
		Vec2 angle_range, 
		Vec2 angle_vel_range, 
		Vec2 scale_range, 
		Vec2i frame_range, 
		int32 max_frames, 
		int32 framerate, 
		Vec2i time_range, 
		bool time_scaling,
		real ground_restitution,
		Vec2 skid_on_bounce_range,
		real jitter_on_bounce_mag)
{
	Particle_Style s;
	s.texture = texture;
	s.size = size;
	s.color = color;
	s.shadow_color = shadow_color;
	s.acceleration = acl;
	s.impulse_min = impulse_range.x;
	s.impulse_max = impulse_range.y;
	s.angle_min = angle_range.x;
	s.angle_max = angle_range.y;
	s.angle_vel_min = angle_vel_range.x;
	s.angle_vel_max = angle_vel_range.y;
	s.scale_min = scale_range.x;
	s.scale_max = scale_range.y;
	s.frame_min = frame_range.x;
	s.frame_max = frame_range.y;
	s.max_frames = max_frames;
	s.framerate = framerate;
	s.time_min = time_range.x;
	s.time_max = time_range.y;
	s.time_scaling = time_scaling;
	s.ground_restitution = ground_restitution;
	s.skid_on_bounce_min = skid_on_bounce_range.x;
	s.skid_on_bounce_max = skid_on_bounce_range.y;
	s.jitter_on_bounce_mag = jitter_on_bounce_mag;
	return s;
}

void init_particle(Particle* p, Vec3 pos, Vec3 vel, real scale, real angle, real anglev, int32 frame, int32 time, int32 style_index)
{
	p->position = pos;
	p->velocity = vel;
	p->scale = scale;
	p->angle = angle;
	p->angular_vel = anglev;
	p->frame = frame;
	p->time = time;
	p->total_time = time;
	p->style_index = style_index;
}


struct Emitter
{
	Particle* particles;
	isize particles_count, particles_capacity;

	Particle_Style* styles;
	isize styles_count, styles_capacity;
};

void init_emitter(Emitter* e, isize max_particles, Memory_Arena* arena)
{
	e->particles_count = 0; 
	e->styles_capacity = (256);
	e->styles = ((Particle_Style*)arena_push(arena, sizeof(Particle_Style) * (256)));
	e->particles_capacity = max_particles;
	e->particles = ((Particle*)arena_push(arena, sizeof(Particle) * max_particles));
}

void emitter_spawn(Emitter* e, Vec3 pos, Vec2 angle_range, isize count, Particle_Style style) 
{
	
	if(e == NULL || e->particles == NULL) return;

	isize next_style = e->styles_count++;
	next_style %= e->styles_capacity;
	style.id = next_style;
	e->styles[next_style] = style;

	for(isize i = 0; i < count; ++i) {
		isize next_particle = e->particles_count++;
		next_particle %= e->particles_capacity;
		Particle* p = e->particles + next_particle;
		real mag = rand_range(style.impulse_min, style.impulse_max);
		Vec2 impulse = v2_from_angle(rand_range(angle_range.x, angle_range.y)) * mag;
		real quarter_h = pos.z / 4;
		init_particle(p, pos,
				v3(impulse, rand_range(-quarter_h, quarter_h)), 
				rand_range(style.scale_min, style.scale_max), 
				rand_range(style.angle_min, style.angle_max),
				rand_range(style.angle_vel_min, style.angle_vel_max),
				rand_range_int(style.frame_min, style.frame_max), 
				rand_range_int(style.time_min, style.time_max),
				next_style);
	}
}

void emitter_render(Emitter* e, Simulator* sim, real dt)
{
	if(e == NULL || e->particles == NULL) return;

	isize count = e->particles_count;
	if(count > e->particles_capacity) {
		count = e->particles_capacity;
	}

	Particle_Style style = {0};
	style.id = -1;
	Sim_Static_Grid* grid = sim->grid;
	Vec2i gridsize = grid->size;
	for(isize i = 0; i < count; ++i) {
		Particle* p = e->particles + i;
		if(p->time <= 0) continue;
		if(style.id != p->style_index) {
			style = e->styles[p->style_index];
		}
		real tscale = (real)p->time / (real)p->total_time;
		tscale /= 2;
		tscale += 0.5f;
		p->time--;

		Sprite s;
		init_sprite(&s);
		Vec3 prev_vel = p->velocity;
		p->velocity += style.acceleration * dt;
		p->position += (p->velocity + prev_vel) * 0.5f * dt;
		p->velocity *= 0.99f;
		
		if(p->position.z < 0) {
			p->position.z = 0;
			p->velocity.z *= -1 * style.ground_restitution;
			p->velocity += style.jitter_on_bounce_mag * v3(
					rand_range(-1, 1),
					rand_range(-1, 1), 0);
			p->velocity.x *= rand_range(
					style.skid_on_bounce_min,
					style.skid_on_bounce_max);
			p->velocity.y *= rand_range(
					style.skid_on_bounce_min,
					style.skid_on_bounce_max);
		}

		isize gx,gy;
		gx = p->position.x / ((32) * 4.0f);
		gy = p->position.y / ((32) * 4.0f);
		if(gx < 0) gx = 0;
		else if(gx >= gridsize.x) gx = gridsize.x - 1;
		if(gy < 0) gy = 0;
		else if(gy >= gridsize.y) gy = gridsize.y - 1;
		Sim_Grid_Cell* c = grid->cells[gx + gy * gridsize.x];
		if(c != NULL) {
			do {
				Sim_Body* b = c->body;
				if(b->group == 1) continue;
				AABB shape = b->shape;
				AABB point = aabb(v2(p->position), 0, 0);
				if(aabb_intersect(&shape, &point)) {
					Vec2 overlap;
					aabb_overlap(&shape, &point, &overlap);
					Vec2 newpos = v2(p->position);
					newpos += overlap;

					if(overlap.y == 0) {
						p->velocity.x *= -1;
					} else {
						p->velocity.y *= -1;
					}

					p->velocity *= style.ground_restitution;

					p->position.x = newpos.x;
					p->position.y = newpos.y;
					
					
				}
			} while(c = c->next);
		}

		p->angle += p->angular_vel * dt;
		s.position = v2(p->position.x, p->position.y - p->position.z);
		s.sort_offset = p->position.z;
		s.angle = p->angle;
		s.size = style.size * p->scale * tscale;
		s.texture = style.texture;
		s.texture.x += s.texture.w * p->frame;
		s.color = style.color;
		if(style.time_alpha) {
			s.color.w *= tscale;
		}
		render_add(&s);
		s.position = v2(p->position.x, p->position.y);
		s.color = style.shadow_color;
		if(style.time_alpha) {
			s.color.w *= tscale;
		}
		s.sort_offset = -1;
		render_add(&s);
	}
}

#line 173 "src\\rituals_main.cpp"

#line 1 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_world_area.cpp"















typedef struct Entity Entity;
struct Hitbox_Contact
{
	isize a_id;
	isize b_id;

	Entity* a;
	Entity* b;
};

struct Hitbox
{
	isize id;
	Entity* ref;
	uint64 mask;
	uint64 group;
	AABB box;
};

void _hitbox_sort_on_x_axis(Hitbox* array, isize count) { if(count > 1) if(count > 12) { Hitbox tmp = array[0]; array[0] = array[count / 2]; array[count / 2] = tmp; isize pivot = 0; for(isize i = 1; i < count; ++i) { if(((array[i].box.center.x - array[i].box.hw)) < ((array[0].box.center.x - array[0].box.hw))) { tmp = array[++pivot]; array[pivot] = array[i]; array[i] = tmp; } } tmp = array[0]; array[0] = array[pivot]; array[pivot] = tmp; _hitbox_sort_on_x_axis(array, pivot); _hitbox_sort_on_x_axis(array + pivot + 1, count - (pivot + 1)); } else for(isize i = 1; i < count; ++i) { Hitbox x = array[i]; isize j = i - 1; while((j >= 0) && (((array[j].box.center.x - array[j].box.hw)) > ((x.box.center.x - x.box.hw)))) { array[j + 1] = array[j]; j--; } array[j+1] = x; } }

void _hitbox_sort_on_y_axis(Hitbox* array, isize count) { if(count > 1) if(count > 12) { Hitbox tmp = array[0]; array[0] = array[count / 2]; array[count / 2] = tmp; isize pivot = 0; for(isize i = 1; i < count; ++i) { if(((array[i].box.center.y - array[i].box.hh)) < ((array[0].box.center.y - array[0].box.hh))) { tmp = array[++pivot]; array[pivot] = array[i]; array[i] = tmp; } } tmp = array[0]; array[0] = array[pivot]; array[pivot] = tmp; _hitbox_sort_on_y_axis(array, pivot); _hitbox_sort_on_y_axis(array + pivot + 1, count - (pivot + 1)); } else for(isize i = 1; i < count; ++i) { Hitbox x = array[i]; isize j = i - 1; while((j >= 0) && (((array[j].box.center.y - array[j].box.hh)) > ((x.box.center.y - x.box.hh)))) { array[j + 1] = array[j]; j--; } array[j+1] = x; } }

enum Entity_Flags
{
	EntityFlag_None,
	EntityFlag_Tail = (1 << 1),
	EntityFlag_SameShadow = (1 << 2)
};
struct Entity
{
	isize id;
	isize body_id;
	Sim_Body* body;
	Vec2 walk_impulse;

	Hitbox hitbox;
	int32 health;
	int32 attack;
	real attack_interval;
	real attack_timer;
	real knockback;

	Animated_Sprite* anim;
	Sprite sprite;
	real z;
	real shadow_scale;
	isize kind;

	int32 counter;

	int32 facing;
	Direction direction;

	uint64 events;
	uint64 flags;
	
	Rituals_Entity_Userdata userdata;
};


void entity_sort_on_id(Entity* array, isize count) { if(count > 1) if(count > 12) { Entity tmp = array[0]; array[0] = array[count / 2]; array[count / 2] = tmp; isize pivot = 0; for(isize i = 1; i < count; ++i) { if((array[i].id) < (array[0].id)) { tmp = array[++pivot]; array[pivot] = array[i]; array[i] = tmp; } } tmp = array[0]; array[0] = array[pivot]; array[pivot] = tmp; entity_sort_on_id(array, pivot); entity_sort_on_id(array + pivot + 1, count - (pivot + 1)); } else for(isize i = 1; i < count; ++i) { Entity x = array[i]; isize j = i - 1; while((j >= 0) && ((array[j].id) > (x.id))) { array[j + 1] = array[j]; j--; } array[j+1] = x; } }
isize entity_search_for_id(isize key, Entity* array, isize count) { if(count == 0) return -1; isize min = 0, max = count - 1, mid = 0; isize localkey; while(min <= max) { mid = (min + max) / 2; localkey = (array[mid].id); if(localkey == key) { return mid; } else if(localkey < key) { min = mid + 1; } else { max = mid - 1; } } return -1; }

typedef struct World_Area_Stub World_Area_Stub;
struct Area_Link
{
	Vec2i position;
	World_Area_Stub* link;
};

enum World_Area_Biome
{
	AreaBiome_Grassland,
	AreaBiome_Desert
};




struct World_Area_Stub
{
	isize id;
	usize seed;
	Area_Link north;
	Area_Link south;
	Area_Link west;
	Area_Link east;
	World_Area_Biome biome;
};

struct World_Area
{
	isize id;
	World_Area_Stub* stub;
	World* world;
	Simulator sim;
	Tilemap map;
	Vec2 offset;
	Vec2 target;

	Entity* entities;
	bool entities_dirty;
	isize entities_count, entities_capacity, next_entity_id;

	isize* removed_entities;
	isize removed_entities_count, removed_entities_capacity;

	Hitbox* hitboxes;
	isize hitboxes_count, hitboxes_capacity;
	int32 hitbox_sort_axis;
	Hitbox_Contact* hitbox_contacts;
	isize hitbox_contacts_count, hitbox_contacts_capacity;

	Entity* player;
};

void init_world_area(World_Area* area, Memory_Arena* arena)
{
	init_simulator(&area->sim, ((256) * (256)), arena);
	init_tilemap(&area->map, 
			(256),
			(256),
			arena);

	area->id = -1;
	area->entities = ((Entity*)arena_push(arena, sizeof(Entity) * ((256) * (256))));
	area->entities_count = 0;
	area->entities_capacity = ((256) * (256));
	area->removed_entities_capacity = 256;
	area->removed_entities = ((isize*)arena_push(arena, sizeof(isize) * ((256) * (256))));
	area->removed_entities_count = 0;
	area->next_entity_id = 0;
	area->entities_dirty = false;
	area->hitboxes = ((Hitbox*)arena_push(arena, sizeof(Hitbox) * ((256) * (256))));
	area->hitboxes_count = 0;
	area->hitboxes_capacity = ((256) * (256));
	area->hitbox_contacts = ((Hitbox_Contact*)arena_push(arena, sizeof(Hitbox_Contact) * ((256) * (256))));
	area->hitbox_contacts_count = 0;
	area->hitbox_contacts_capacity = ((256) * (256));
}

void init_entity(Entity* entity)
{
	entity->id = 0;
	entity->body_id = 0;
	init_sprite(&entity->sprite);
	entity->counter = 0;
	entity->health = 100;
	entity->attack = 0;
	entity->attack_interval = 0.5f;
	entity->attack_timer = 0;
	entity->z = 0;
	entity->shadow_scale = 1.0f;
	entity->anim = NULL;
}

Entity* world_area_get_next_entity(World_Area* area)
{
	if(area->entities_count + 1 >= area->entities_capacity) {
		printf("There was an error: %s \n", "Ran out of entities");;
		return NULL;
	}

	Entity* e = area->entities + area->entities_count++;
	init_entity(e);
	e->body = sim_get_next_body(&area->sim);
	e->body_id = e->body->id;
	e->id = area->next_entity_id++;
	e->body->entity = e;
	e->body->entity_id = e->id;
	return e;
}

Entity* world_area_find_entity(World_Area* area, isize id)
{
	if(area->entities_dirty) {
		entity_sort_on_id(area->entities, area->entities_count);
	}
	isize index = entity_search_for_id(id, area->entities, area->entities_count);
	return index == -1 ? NULL : area->entities + index;
}

void world_area_sort_entities_on_id(World_Area* area)
{
	entity_sort_on_id(area->entities, area->entities_count);
}

void world_area_synchronize_entities_and_bodies(World_Area* area)
{
	world_area_sort_entities_on_id(area);
	sim_sort_bodies_on_id(&area->sim);
	for(isize i = 0; i < area->entities_count; ++i) {
		Entity* e = area->entities + i;
		if(e->body_id == -1) continue;
		Sim_Body* b = sim_find_body(&area->sim, e->body_id);
		if(b == NULL) continue;
		b->entity = e;
		b->entity_id = e->id;
		e->body = b;
	}
}




bool world_area_remove_entity(World_Area* area, Entity* entity)
{
	if(area->removed_entities_count >= area->removed_entities_capacity) {
		printf("Ran out of room for removing entities\n");
		return true;
	}
	area->removed_entities[area->removed_entities_count++] = entity->id;
	return false;
}

void world_area_on_destroy_entity(Entity* e, World_Area* area, World* world);
void world_area_remove_entity_internal(World_Area* area, isize id)
{
	isize index = entity_search_for_id(id, area->entities, area->entities_count);
	if(index == -1) {
		return;
	}
	Entity* entity = area->entities + index;
	world_area_on_destroy_entity(entity, area, area->world);
	sim_remove_body(&area->sim, entity->body_id);
	area->entities[index] = area->entities[--area->entities_count];
	world_area_synchronize_entities_and_bodies(area);
}

void world_area_process_removed_entities(World_Area* area)
{
	for(isize i = 0; i < area->removed_entities_count; ++i) {
		isize e = area->removed_entities[i];
		world_area_remove_entity_internal(area, e);
	}
	area->removed_entities_count = 0;
}
void world_area_build_hitboxes(World_Area* area)
{
	area->hitboxes_count = 0;
	for(isize i = 0; i < area->entities_count; ++i) {
		Entity* e = area->entities + i;
		if(e->body == NULL) continue;
		Hitbox* h = area->hitboxes + area->hitboxes_count++;
		h->id = e->id;
		h->ref = e;
		AABB box = e->hitbox.box;
		h->box.center = e->sprite.position + box.center;
		
		if(v2_dot(box.hext, box.hext) > 1) {
			h->box.hext = box.hext;
		} else {
			h->box.hext = e->body->shape.hext;
			e->hitbox.box.hext = h->box.hext;
			e->hitbox.box.center.y -= e->body->shape.hh;
		}
	}
}

void world_area_process_hitboxes(World_Area* area)
{
	if(area->hitboxes_count == 0) return;

	area->hitbox_contacts_count = 0;

	if(area->hitbox_sort_axis == 0) {
		_hitbox_sort_on_x_axis(area->hitboxes, area->hitboxes_count);
	} else {
		_hitbox_sort_on_y_axis(area->hitboxes, area->hitboxes_count);
	}
	Vec2 center_sum1 = v2(0, 0);
	Vec2 center_sum2 = v2(0, 0);
	Vec2 variance = v2(0, 0);
	for(isize i = 0; i < area->hitboxes_count; ++i) {
		Hitbox* a = area->hitboxes + i;
		center_sum1 += a->box.center;
		for(isize q = 0; q < 2; ++q) {
			center_sum2.e[q] += a->box.center.e[q] * a->box.center.e[q];
		}

		for(isize j = i + 1; j < area->hitboxes_count; ++j) {
			Hitbox* b = area->hitboxes + j;
			
				
			
			uint64 ma = a->mask & b->group;
			uint64 mb = a->group & b->mask;
			if(ma != 0 || mb != 0) {
				continue;		
			}

			if(area->hitbox_sort_axis == 0) {
				if((b->box.center.x - b->box.hw) > (a->box.center.x + a->box.hw)) {
					break;
				}
			} else if(area->hitbox_sort_axis == 1) {
				if((b->box.center.y - b->box.hh) > (a->box.center.y + a->box.hh)) {
					break;
				}
			}

			if(aabb_intersect(&a->box, &b->box)) {
				Hitbox_Contact* c = area->hitbox_contacts + area->hitbox_contacts_count++;
				c->a = a->ref;
				c->b = b->ref;
				c->a_id = a->id;
				c->b_id = b->id;
			}
		}
	}
	for(isize i = 0; i < 2; ++i) {
		variance.e[i] = center_sum2.e[i] - center_sum1.e[i] * center_sum1.e[i] / 
			area->hitboxes_count;
	}

	if(variance.x > variance.y) {
		area->hitbox_sort_axis = 0;
	} else {
		area->hitbox_sort_axis = 1;
	}
}
#line 175 "src\\rituals_main.cpp"
#line 1 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_world.cpp"















struct World
{
	char* name;

	World_Area_Stub* area_stubs;
	isize next_area_id;
	isize areas_count, areas_capacity;
	isize areas_width, areas_height;

	World_Area* current_area;
	usize seed;

	Entity global_player_entity;
	Sim_Body global_player_body;

	usize slowtick_timer;
	usize slowtick_timer_interval;

	Particle_Style base_style;
	Emitter emitter;
};


isize Anim_Standing;
isize Anim_Walking;


void init_world(World* world, isize width, isize height, usize seed, Memory_Arena* arena)
{
	world->seed = seed;
	world->areas_capacity = width * height * 2;
	world->area_stubs = ((World_Area_Stub*)arena_push(arena, sizeof(World_Area_Stub) * world->areas_capacity));
	world->areas_count = 0;
	world->areas_width = width;
	world->areas_height = height;
	world->next_area_id = 0;
	world->current_area = NULL;
	world->slowtick_timer = 0;
	world->slowtick_timer_interval = 60;

	init_entity(&world->global_player_entity);
	init_body(&world->global_player_body);

	Entity* e = &world->global_player_entity;
	Sim_Body* b = &world->global_player_body;
	e->sprite.texture = rect2(0, 0, 32, 32);
	e->anim = ((Animated_Sprite*)arena_push(arena, sizeof(Animated_Sprite))); 
	init_animated_sprite(e->anim, 64, arena);
	Anim_Standing = add_animation(e->anim, make_animaiton_from_strip(arena, 12, rect2(0, 0, 32, 32), 1));
	Anim_Walking = add_animation(e->anim, make_animaiton_from_strip(arena, 12, rect2(0, 11 * 32, 32, 32), 4));
	
	b->shape.hext = v2(5, 5);
	e->hitbox.box.hext = b->shape.hext + v2(1, 1);
	e->hitbox.box.center.y -= b->shape.hh;
	e->sprite.size = v2(32, 32);
	b->group = 1;
	e->sprite.flags = Anchor_Bottom;
	b->damping = 0.5f;
	b->restitution = 0;
	b->flags = Body_Flag_No_Friction;
	e->kind = EntityKind_Player;
	auto p = &e->userdata.player;
	p->heal_cooldown = 5.0f;
	p->heal_rate = 1;
	p->heal_to_interval = 25;
	p->heal_timer = 0;

	world->base_style = make_particle_style(
			rect2(64, 0, 32, 32),
			v2(4, 4),
			v4(1, 0.9f, 0, 1),
			v4(0, 0, 0, 0.3f),
			v3(0, 0, -600),
			v2(0, 0), 
			v2(-3.141592f, 3.141592f),
			v2(0, 0),
			v2(0.5f, 1),
			v2i(0, 0),
			0, 0,
			v2i(10, 60),
			true,
			0.5f, 
			v2(0.4f, 0.6f),
			50
			);
	init_emitter(&world->emitter, 8192, arena);
}


void world_area_init_player(World_Area* area, Vec2i tile_pos, bool move_player=true)
{
	Entity* player_entity = world_area_find_entity(area, 0);
	Sim_Body* player = sim_find_body(&area->sim, player_entity->body_id);
	*player_entity = area->world->global_player_entity;
	*player = area->world->global_player_body;

	if(move_player) {
		player->shape.center = v2(tile_pos.x * (32), tile_pos.y * (32));
	}

	area->offset = player->shape.center;
	area->target = player->shape.center;
}

void world_area_deinit_player(World_Area* area, bool move_player=true)
{
	Entity* player_entity = world_area_find_entity(area, 0);
	Sim_Body* player = sim_find_body(&area->sim, player_entity->body_id);
	area->world->global_player_entity = *player_entity;
	area->world->global_player_body = *player;
}

void deserialize_area(World_Area* area, FILE* file, Memory_Arena* arena);
FILE* get_area_file(const char* name, isize id, const char* mode);
FILE* get_world_file(const char* name, const char* mode);
int check_path(char* path);
World_Area* world_load_area(World* world, isize id, Memory_Arena* arena)
{
	FILE* fp = get_area_file(world->name, id, "rb");
	World_Area* area = NULL;
	if(fp != NULL) {
		area = ((World_Area*)arena_push(arena, sizeof(World_Area)));
		deserialize_area(area, fp, arena);
		area->world = world;
		area->stub = world->area_stubs + id;
		for(isize i = 0; i < area->entities_count; ++i) {
			Entity* e = area->entities + i;
		}
		world_area_synchronize_entities_and_bodies(area);

		fclose(fp);
	}

	return area;
}


Entity* rituals_spawn_enemy(World_Area* area, isize enemykind, Vec2 position);
void generate_world_area(World* world, World_Area* area, World_Area_Stub* stub)
{
	area->stub = stub;
	area->world = world;
	Random rand;
	Random* r = &rand;
	init_random(r, stub->seed);
	generate_tilemap(&area->map, stub->seed);
	area->id = stub->id;
	
	for(isize i = 0; i < (256) * 4; ++i) {
		Entity* e = world_area_get_next_entity(area);
		Sim_Body* b = sim_find_body(&area->sim, e->body_id);
		e->sprite.texture = rect2(8*32, 16, 32, 48);
		b->shape.hw = 15;
		b->shape.hh = 11;
		b->inv_mass = 1.0f;
		e->sprite.size = v2(32, 48);
		e->sprite.flags = Anchor_Bottom;
		e->shadow_scale = 1/0.75f;  
		e->kind = EntityKind_Prop;
		auto p = &e->userdata.prop;
		p->contains = EntityKind_Pickup;
		p->subtype = PickupKind_Health;
		p->amount = rand_range_int(r, 1, 2);
		p->quality = 25;

		do {
			b->shape.center = v2(
					rand_range(r, 0, area->map.w * 32),
					rand_range(r, 0, area->map.h * 32));
		}
		while (Registry->tiles[tilemap_get_at(&area->map, b->shape.center)].solid);
	}

	
	for(isize i = 0; i < (256); ++i) {
		Entity* e = world_area_get_next_entity(area);
		Sim_Body* b = sim_find_body(&area->sim, e->body_id);
		e->sprite.texture = rect2(0, 5*32, 96, 144);
		b->shape.hw = 16;
		b->shape.hh = 15;
		b->inv_mass = 1.0f;
		b->flags = Body_Flag_Static;
		e->sprite.size = v2(96, 144) * 2;
		e->sprite.flags = Anchor_Bottom;
		e->sprite.sort_offset = -60;
		e->sprite.center = v2(2, -b->shape.hh * 2);
		e->kind = EntityKind_Static;
		
		
		do {
			b->shape.center = v2(
					rand_range(r, 0, area->map.w * 32),
					rand_range(r, 0, area->map.h * 32));
		}
		while (Registry->tiles[tilemap_get_at(&area->map, b->shape.center)].solid);
	}

	for(isize i = 0; i < (256) ; ++i) {
		Vec2 pos;
		do {
			pos = v2(
					rand_range(r, 0, area->map.w * 32),
					rand_range(r, 0, area->map.h * 32));
		}
		while (Registry->tiles[tilemap_get_at(&area->map, pos)].solid);
		rituals_spawn_enemy(area, rand_range_int(r, 0, 4), pos);
	}

	generate_statics_for_tilemap(&area->sim, &area->map);
}

void serialize_world(World* world);
void world_switch_current_area(World* world, Area_Link link, Memory_Arena* arena)
{
	if(link.link == NULL) return;
	world_area_deinit_player(world->current_area);
	
	
	serialize_world(world);
	clear_arena(arena);
	World_Area* new_area = world_load_area(world, link.link->id, arena);
	if(new_area == NULL) {
		new_area = ((World_Area*)arena_push(arena, sizeof(World_Area)));
		init_world_area(new_area, arena);
		generate_world_area(world, new_area, link.link);
	}
	world_area_init_player(new_area, link.position);
	world->current_area = new_area;
}

void world_start_in_area(World* world, World_Area_Stub* area, Memory_Arena* arena)
{
	World_Area* new_area = world_load_area(world, area->id, arena);
	if(new_area == NULL) {
		new_area = ((World_Area*)arena_push(arena, sizeof(World_Area)));
		init_world_area(new_area, arena);
		generate_world_area(world, new_area, area);
		world_area_init_player(new_area, v2i((256) / 2, (256) /2));
	} else {
		world_area_init_player(new_area, v2i(0, 0),  false);
	}
	world_area_synchronize_entities_and_bodies(new_area);
	world->current_area = new_area;
}

int recursively_delete_folder(char* path, bool append_base_path);
void world_delete_self(World* world)
{
	char world_path[4096];
	isize len = snprintf(world_path, 4096, "%s/%s", menu_state->save_dir, world->name);
	recursively_delete_folder(world_path, false);
	menu_state->saves_dirty = true;
}


void generate_world(char* name, World* world)
{
	world->name = name;

	for(isize i = 0; i < world->areas_height; ++i) {
		for(isize j = 0; j < world->areas_width; ++j) {
			isize index = i * world->areas_width + j;
			World_Area_Stub* stub = world->area_stubs + world->areas_count++; 
			stub->id = world->next_area_id++;
			stub->seed = world->seed + stub->id;
			isize north_link = modulus(i - 1, world->areas_height) * world->areas_width + j;
			isize south_link = modulus(i + 1, world->areas_height) * world->areas_width + j;
			isize west_link = i * world->areas_width + modulus(j - 1, world->areas_width);
			isize east_link = i * world->areas_width + modulus(j + 1, world->areas_width);

			stub->north = Area_Link {
				v2i((256) / 2,  (256) - 1), 
					world->area_stubs + north_link
			};
			stub->south = Area_Link {
				v2i((256) / 2, 1),
					world->area_stubs + south_link
			};
			stub->west = Area_Link {
				v2i((256) - 1, (256) / 2),
					world->area_stubs + west_link
			};
			stub->east = Area_Link {
				v2i(1, (256) / 2),
					world->area_stubs + east_link
			};

		}
	}
}



void rituals_walk_entities(Entity* entities, isize count, World_Area* area, World* world);
void rituals_animate_entities(Entity* entities, isize count, World_Area* area, World* world);
void rituals_interact_entities(Entity* entities, isize count, World_Area* area, World* world);

void rituals_hit_entities(Hitbox_Contact* contacts, isize count, World_Area* area, World* world);
void rituals_contact_entities(Sim_Contact* contacts, isize count, World_Area* area, World* world);

bool rituals_frametick_entities(Entity* entities, isize count, World_Area* area, World* world);
void rituals_slowtick_entities(Entity* entities, isize count, World_Area* area, World* world);

void rituals_on_destroy_entity(Entity* entity, World_Area* area, World* world);
void rituals_on_activate_entity(Entity* entity, World_Area* area, World* world);

void world_area_walk_entities(World_Area* area, World* world)
{
	rituals_walk_entities(area->entities, area->entities_count, area, world);
}

void world_area_animate_entities(World_Area* area, World* world)
{
	rituals_animate_entities(area->entities, area->entities_count, area, world);
}

bool world_area_frametick_entities(World_Area* area, World* world)
{
	return rituals_frametick_entities(area->entities, area->entities_count, area, world);
}

void world_area_slowtick_entities(World_Area* area, World* world)
{
	rituals_frametick_entities(area->entities, area->entities_count, area, world);
}

void world_area_on_destroy_entity(Entity* e, World_Area* area, World* world)
{
	rituals_on_destroy_entity(e, area, world);	
}

void world_area_on_activate_entity(Entity* e, World_Area* area, World* world)
{
	rituals_on_activate_entity(e, area, world);
}

void world_area_hit_entities(World_Area* area, World* world)
{
	rituals_hit_entities(area->hitbox_contacts, area->hitbox_contacts_count, area, world);
}

void world_area_contact_entities(World_Area* area, World* world)
{
	rituals_contact_entities(area->sim.contacts, area->sim.contacts_count, area, world);
}

void world_area_interact(World_Area* area, World* world)
{
	rituals_interact_entities(area->entities, area->entities_count, area, world);
}

void world_area_render(World_Area* area, World* world)
{	
	render_set_current_group(0);
	Vec2 target = area->target;
	area->offset += (target - area->offset) * 0.1f;
	area->offset -= Game->size * 0.5f;
	if(area->offset.x < 0) 
		area->offset.x = 0;
	else if((area->offset.x + Game->size.x) > area->map.w * (32))
		area->offset.x = area->map.w * (32) - Game->size.x;

	if(area->offset.y < 0) 
		area->offset.y = 0;
	else if((area->offset.y + Game->size.y) > area->map.h * (32))
		area->offset.y = area->map.h * (32) - Game->size.y;

	CurrentGroup->offset = area->offset;
	area->offset += Game->size * 0.5f;

	render_start();

	Rect2 screen = rect2(
			area->offset.x - Game->size.x / 2,
			area->offset.y - Game->size.y / 2, 
			Game->size.x, Game->size.y);

	isize sprite_count_offset = render_tilemap(&area->map, v2(0,0), screen);

	world_area_animate_entities(area, world);

	emitter_render(&world->emitter, &area->sim,  (1.0f/60.0f));

	render_sort(sprite_count_offset);
	char buf[256];
	Gui_TextBackgroundColor = v4(0, 0, 0, 0.4f);


	for(isize i = 0; i < area->entities_count; ++i) {
		Entity* e = area->entities + i;
		if(e->kind != EntityKind_Enemy && e->kind != EntityKind_Player) continue;
		
		isize len = snprintf(buf, 256, "%d", e->health);
		render_body_text(buf, e->sprite.position - v2(Body_Font->glyph_width * len / 4, e->sprite.size.y + 16), true, 0.5f);
	}
#line 412 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_world.cpp"









#line 422 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_world.cpp"
	render_draw(Game->size, Game->scale);



	render_set_current_group(1);
	render_start();
	snprintf(buf, 256, "Area %d | Entities %d | Frame Time %d", area->id, area->entities_count, Game->last_frame_time);
	render_body_text(buf, v2(16, 16), true);
	render_draw(Game->size, Game->scale);
#line 432 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_world.cpp"

}


void init_play_state();
int recursively_delete_folder(char* path, bool append_base_path);
void world_area_update(World_Area* area, World* world)
{
	game_set_scale(2);
	game_calc_mouse_pos(area->offset - Game->size * 0.5f);
	
	
	world_area_synchronize_entities_and_bodies(area);
	area->player = world_area_find_entity(area, 0);
	if(!play_state->running) {
		world_area_render(area, world);
		return;
	}

	play_state->current_time = SDL_GetTicks();
	real dt = (play_state->current_time - play_state->prev_time) / 1000.0;
	dt = clamp(dt, 0, 1.2f);
	play_state->accumulator += dt;
	play_state->prev_time = play_state->current_time;
	world_area_synchronize_entities_and_bodies(area);
	area->player = world_area_find_entity(area, 0);

	bool exit = world_area_frametick_entities(area, world);
	if(exit) return;
	world_area_slowtick_entities(area, world);

	world_area_walk_entities(area, world);
	
	for(isize i = 0; i < area->entities_count; ++i) {
		Entity* e = area->entities + i;
		e->body->velocity += e->walk_impulse;
		if(e->attack_timer > 0) {
			e->attack_timer -= (1.0f/60.0f);
		}
	}

	sim_update(&area->sim, &area->map, (1.0f/60.0f), true);
	
	Vec2 target = area->player->body->shape.center;
	area->target = target;
	if(target.x < 0) {
		world_switch_current_area(play_state->world, area->stub->west, Game->play_arena);
		play_state->world_xy.x--;
	} else if(target.x > area->map.w * (32)) {
		world_switch_current_area(play_state->world, area->stub->east, Game->play_arena);
		play_state->world_xy.x++;
	} else if(target.y < 0) {
		world_switch_current_area(play_state->world, area->stub->north, Game->play_arena);
		play_state->world_xy.y--;
	} else if(target.y > area->map.h * (32)) {
		world_switch_current_area(play_state->world, area->stub->south, Game->play_arena);
		play_state->world_xy.y++;
	}
	world_area_interact(area, world);
	world_area_render(area, world);

	if(Input->mouse[SDL_BUTTON_LEFT] == State_Just_Pressed) {
		Vec2 pv = area->player->walk_impulse * (1.0f/60.0f);
		real player_mag = v2_dot(pv, pv);
		player_mag = sqrtf(player_mag);
		for(isize i = 0; i < 2; ++i) {
			int32 particle_multiplier = 2;
			Entity* e = world_area_get_next_entity(area);
			e->kind = EntityKind_Bullet;
			e->sprite = create_box_primitive(area->player->sprite.position, v2(2, 2), v4(1, 0.25f, 0, 1));
			e->sprite.position += v2(
					rand_range(&Game->r, -3, 3),
					rand_range(&Game->r, -3, 3));
			e->body->shape.center = e->sprite.position;
			e->sprite.flags = Anchor_Bottom;
			e->body->shape.hext = v2(1, 1);
			e->body->flags = Body_Flag_No_Friction | Body_Flag_Always_Contact;
			e->body->damping = 1.0f;
			e->body->mask = 3;
			e->body->group = 1;
			e->attack = 6;
			e->attack += (int32)roundf(rand_range(&Game->r, -0.15, 0.15) * e->attack);
			e->hitbox.mask = (1 << 2);
			e->hitbox.box.hext = e->body->shape.hext * 1.5f;
			e->hitbox.box.hh = 8;
			e->hitbox.box.center.y = -8;
			e->flags = EntityFlag_Tail;
			e->z = 16;
			e->shadow_scale = 8.0f;

			Vec2 dmouse =  Input->mouse_pos - e->sprite.position; 
			real a = v2_to_angle(dmouse);
			
			Particle_Style style = make_particle_style(
					rect2(64, 0, 32, 32),
					v2(4, 4),
					hex_to_v4(0xFF8822FF),
					v4(0, 0, 0, 0.1f),
					v3(0, 0, 100),
					v2(0, 0),
					v2(-3.141592f, 3.141592f),
					v2(-3.141592f, 3.141592f), 
					v2(.75, 1.25),
					v2i(0, 0),
					0, 0,
					v2i(5, 10),
					true,
					0.5f, 
					v2(0.4f, 0.6f),
					50);


			style.time_scaling = false;
			style.time_alpha = true;
			style.impulse_min = 0 + player_mag;
			style.impulse_max = 200 + player_mag;
			Vec2 angle_range = v2(a - 0.5f, a + 0.5f);
			emitter_spawn(&world->emitter, 
					v3(area->player->sprite.position + pv, 16), 
					angle_range,
					4 * particle_multiplier,
					style);
			style.color = hex_to_v4(0xFF4400FF);
			emitter_spawn(&world->emitter, 
					v3(area->player->sprite.position + pv, 16), 
					angle_range,
					4 * particle_multiplier,
					style);
					
			angle_range = v2(a - 0.2f, a + 0.2f);

			
			style.time_alpha = false;
			style.time_scaling = true;
			style.color = hex_to_v4(0xFFDD33FF);
			style.impulse_min = 25 + player_mag;
			style.impulse_max = 150 + player_mag;
			style.time_min = 15;
			style.time_max = 35;
			style.acceleration.z = -300;
			emitter_spawn(&world->emitter, 
					v3(area->player->sprite.position + pv, 16), 
					angle_range,
					2 * particle_multiplier,
					style);

			style.impulse_min = 0 + player_mag;
			style.impulse_max = 100 + player_mag;
			style.time_min = 15;
			style.time_max = 45;
			style.time_alpha = true;
			style.time_scaling = false;
			style.scale_min = 1.5f;
			style.scale_max = 2.5f; 
			style.color = hex_to_v4(0xEEEEEE88);
			style.acceleration.z = 400;
			angle_range = v2(a - 0.75f, a + 0.75f);
			emitter_spawn(&world->emitter, 
					v3(area->player->sprite.position + pv, 16), 
					angle_range,
					1 * particle_multiplier,
					style);
			style.color = hex_to_v4(0xFFFFFF33);
			emitter_spawn(&world->emitter, 
					v3(area->player->sprite.position + pv, 16), 
					angle_range,
					4 * particle_multiplier,
					style);
			
					
			a += rand_range(&Game->r, -5, 5) * 0.01745329f;

			e->body->velocity = v2_from_angle(a) * (600 - rand_range(&Game->r, 0, 200));
			area->player->body->velocity -= e->body->velocity;
		}
	}

	world_area_build_hitboxes(area);
	world_area_process_hitboxes(area);
	world_area_hit_entities(area, world);
	world_area_contact_entities(area, world);
	world_area_process_removed_entities(area);
	
}




#line 176 "src\\rituals_main.cpp"
#line 1 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_entity_events.cpp"













 

void rituals_prop_drop_on_break(World_Area* area, Entity* a) 
{
	if(a->kind != EntityKind_Prop) {
		printf("Could not drop on break: recieved entity type %d", a->kind);
		return;
	}
	auto p = &a->userdata.prop;
	if(p->amount == 0) return;
	Vec2 pos = a->body->shape.center;
	switch(p->contains) {
		case EntityKind_Prop:
			break;
		case EntityKind_Enemy:
			break;
		case EntityKind_Pickup:
			switch(p->subtype) {
				case PickupKind_Item:
					break;
				case PickupKind_Health:
					for(isize i = 0; i < p->amount; ++i) {
						Entity* e = world_area_get_next_entity(area);
						e->kind = EntityKind_Pickup;
						auto eud = &e->userdata.pickup;
						eud->kind = PickupKind_Health;
						eud->health.amount = p->quality;
						real size_mag = (a->body->shape.hw + a->body->shape.hh);
						e->body->shape.center = pos + v2(
								rand_range(&Game->r, -1, 1) * size_mag,
								rand_range(&Game->r, -1, 1) * size_mag
								);
						e->body->shape.hext = v2(4,4);
						e->sprite.flags = Anchor_Bottom;
						e->sprite.texture = rect2(
								9*32, 0, 16, 16);
						e->sprite.size = v2(8, 8);
						e->body->group = 1;
						e->hitbox.box.hext = e->body->shape.hext * 2.0;
						e->hitbox.box.center.y += e->sprite.size.y / 2;
					}
					break;
			}
			break;
		default: 
			break;
	}
}


Entity* rituals_spawn_enemy(World_Area* area, isize enemykind, Vec2 position) 
{
	Entity* e = world_area_get_next_entity(area);
	e->kind = EntityKind_Enemy;
	e->userdata.enemy.kind = enemykind;
	e->sprite.texture = rect2(
			enemykind * 32, 10*32, 32, 32);
	e->sprite.size = v2(32, 32);
	e->hitbox.box = aabb(v2(0, e->sprite.size.y * -0.5f), 16, 16);
	e->hitbox.mask = (1 << 2);
	e->body->shape.center = position;
	e->sprite.flags = Anchor_Bottom;
	e->body->shape.hext = v2(8, 5);
	e->body->group = 2;
	e->attack = 5;
	e->attack_interval = 0.25f;
	e->knockback = 5000;
	auto enemy = &e->userdata.enemy;
	enemy->mode = 0;
	switch(enemy->kind) {
		case EnemyKind_Slime:
			enemy->speed = 200;
			enemy->alert_dist = 128;
			enemy->follow_dist = 512;
			e->health = 100;
			e->hitbox.box.hext *= 0.85;
			e->hitbox.box.center.y += e->sprite.size.y * 0.85 / 8;
			break;
		case EnemyKind_Bat:
			enemy->speed = 600;
			enemy->alert_dist = 64;
			enemy->follow_dist = 512;
			enemy->bat.perch = e->body->shape.center;
			e->body->flags = Body_Flag_No_Friction;
			e->hitbox.box.hh *= 0.8;
			e->hitbox.box.center.y += e->sprite.size.y / 4 + 4;
			e->sprite.size *= 0.5f;
			e->body->shape.hext *= 0.5f;
			e->hitbox.box.hext *= 0.5f;
			e->health = 50;
			break;
		case EnemyKind_Snake:
			enemy->speed = 200;
			enemy->alert_dist = 128; 
			enemy->follow_dist = 512;
			enemy->snake.chase_speed_modifier = 3.0f;
			e->hitbox.box.hh *= 0.6;
			e->hitbox.box.center.y += e->sprite.size.y / 4 + 4;
			e->sprite.size *= 0.75f;
			e->body->shape.hext *= 0.75f;
			e->hitbox.box.hext *= 0.75f;
			e->health = 50;
			break;
		case EnemyKind_Goblin_Knight:
			enemy->goblin_knight.patrol_start = e->sprite.position;
			enemy->goblin_knight.patrol_end = e->sprite.position + v2(512, 0);
			enemy->alert_dist = 128;
			enemy->follow_dist = 800;
			enemy->speed = 250;
			e->health = 300;
			break;
	}
	return e;
}

void rituals_on_activate_entity(Entity* entity, World_Area* area, World* world)
{

}

void rituals_on_destroy_entity(Entity* e, World_Area* area, World* world)
{
	if(e->kind == EntityKind_Bullet) {
			emitter_spawn(&world->emitter, 
					v3(e->sprite.position, 16), 
					v2(-3.141592f, 3.141592f),
					4,
					copy_particle_style(world->base_style, v2(50, 150), v2i(10, 45)));
	}
}

bool rituals_frametick_entities(Entity* entities, isize count, World_Area* area, World* world)
{
	for(isize i = 0; i < count; ++i) {
		Entity* e = entities + i;
		if(e->kind == EntityKind_Player) {
			auto p = &e->userdata.player;
			if(p->heal_timer > 0) {
				p->heal_timer -= (1.0f/60.0f);
			}

			int32 last_health = e->health;
			int32 ivl = p->heal_to_interval;
			if(last_health % ivl != 0)  {
				if(p->heal_timer <= 0) {
					int32 new_health = e->health + p->heal_rate;
					if(((new_health % ivl) < (last_health % ivl))) {
						while(new_health % ivl != 0) {
							new_health--;
						}
					}
					e->health = new_health;
				}
			}

			if(e->health <= 0) {
				play_state->delete_world_on_stop = true;
				play_state->save_world_on_stop = false;
				switch_state(Game_State_Menu);
				return true;
			}
		}
	}
	return false;
}

void rituals_slowtick_entities(Entity* entities, isize count, World_Area* area, World* world)
{
	for(isize i = 0; i < count; ++i) {
		Entity* e = entities + i;
	}
}



void rituals_walk_entities(Entity* entities, isize count, World_Area* area, World* world)
{
	for(isize i = 0; i < count; ++i) {
		Entity* e = entities + i;
		e->walk_impulse = Vec2{};	
		if(e->kind == EntityKind_Static || e->kind == EntityKind_Prop) {
			continue;
		} else if(e->kind == EntityKind_Player) {
			real movespeed = 800;
			Vec2 move_impulse = v2(0, 0);

			if(((Input->scancodes[SDL_SCANCODE_LEFT] == State_Pressed) || (Input->scancodes[SDL_SCANCODE_A] == State_Pressed))) {
				move_impulse.x -= movespeed;
			}
			if(((Input->scancodes[SDL_SCANCODE_RIGHT] == State_Pressed) || (Input->scancodes[SDL_SCANCODE_D] == State_Pressed))) {
				move_impulse.x += movespeed;
			}
			if(((Input->scancodes[SDL_SCANCODE_UP] == State_Pressed) || (Input->scancodes[SDL_SCANCODE_W] == State_Pressed))) {
				move_impulse.y -= movespeed;
			}
			if(((Input->scancodes[SDL_SCANCODE_DOWN] == State_Pressed) || (Input->scancodes[SDL_SCANCODE_S] == State_Pressed))) {
				move_impulse.y += movespeed;
			}

			if(fabsf(move_impulse.x * move_impulse.y) > 0.01f) {
				move_impulse *= 0.7071067f;
			}

			Tile_Info* player_tile = Registry->tiles + tilemap_get_at(&area->map, e->body->shape.center);

			move_impulse *= player_tile->movement_modifier;
			e->walk_impulse = move_impulse;
			if(v2_dot(move_impulse, move_impulse) > 0.001f) {
				play_animation(e->anim, Anim_Walking);
			} else {
				play_animation(e->anim, Anim_Standing);
			}
		} else if(e->kind == EntityKind_Enemy) {
			auto enemy = &e->userdata.enemy;
			Vec2 walk = Vec2{};
			Vec2 dpos = e->body->shape.center - area->player->body->shape.center;
			real mag = v2_dot(dpos, dpos);

			switch(enemy->kind) {
				case EnemyKind_Slime:
				case EnemyKind_Goblin_Knight:
				case EnemyKind_Snake:
					if(enemy->mode == 0) {
						if(mag < (enemy->alert_dist * enemy->alert_dist)) {
							enemy->mode = 1;
						}
					} else if(enemy->mode == 1) {
						if(mag < (enemy->follow_dist * enemy->follow_dist)) {
							real angle = atan2f(dpos.y, dpos.x);
							walk.x = cosf(angle) * -enemy->speed;
							walk.y = sinf(angle) * -enemy->speed;
						} else {
							enemy->mode = 0;
						}
					}
					break;
					
					
					
				case EnemyKind_Bat:
					if(enemy->mode == 0) {
						dpos = e->body->shape.center - enemy->bat.perch;
						real player_dist = mag;
						mag = v2_dot(dpos, dpos);
						if(mag > 16) {
							real angle = atan2f(dpos.y, dpos.x);
							walk.x = cosf(angle) * -enemy->speed;
							walk.y = sinf(angle) * -enemy->speed;
						} else if(player_dist < (enemy->alert_dist * enemy->alert_dist)) {
							enemy->mode = 1;
						}
					} else if(enemy->mode == 1) {
						Vec2 perch_dpos = e->body->shape.center - enemy->bat.perch;
						real perch_dist2 = v2_dot(perch_dpos, perch_dpos);
						if(perch_dist2 < (enemy->follow_dist * enemy->follow_dist)) {
							real angle = atan2f(dpos.y, dpos.x);
							walk.x = cosf(angle) * -enemy->speed;
							walk.y = sinf(angle) * -enemy->speed;
						} else {
							enemy->mode = 0;
						}
					}
			}

			switch(enemy->kind) {
				case EnemyKind_Slime:
					
				case EnemyKind_Goblin_Knight:
				case EnemyKind_Bat:
					e->walk_impulse = walk;
					break;
				case EnemyKind_Snake:
					e->walk_impulse = enemy->mode == 1 ? 
						enemy->snake.chase_speed_modifier * walk :
						walk;
					break;
			}
		}
		if(e->walk_impulse.x < -1.0f) {
			e->facing = -1;
		} else if (e->walk_impulse.x > 1.0f) {
			e->facing = 1;
		}

	}
}

void rituals_animate_entities(Entity* entities, isize count, World_Area* area, World* world)
{
	Sprite shadow;
	init_sprite(&shadow);
	shadow.flags = Anchor_Center;
	shadow.texture = rect2(96, 16, 32, 16);
	shadow.color.w = 0.3f;
	real render_distance = Game->size.x + 256;
	render_distance *= render_distance;
	for(isize i = 0; i < count; ++i) {
		Entity* e = entities + i;
		
		if(e->facing == -1) {
			(e->sprite.flags |= SpriteFlag_FlipHoriz);
		} else if(e->facing == 1) {
			(e->sprite.flags &= ~SpriteFlag_FlipHoriz);
		}

		Sim_Body* b = e->body;
		if (b != NULL) {
			e->sprite.position = b->shape.center;
			e->sprite.position.y += b->shape.hh;
			if((e->flags & EntityFlag_Tail)) {
				Vec2 v = b->velocity / 30.0f; 
				Sprite s = e->sprite;
				for(isize i = 0; i < 16; ++i) {
					Sprite ss = s;
					ss.position.y -= e->z;
					render_add(&ss);
					s.position -= v / 16;
					s.color = Color_White;
					s.color.w = lerp(1.0f, 0.0f, i/16.0);
					s.color.w *= s.color.w;
					s.sort_offset -= 10;
				}
			}
		}

		Vec2 dv_player = area->player->sprite.position - e->sprite.position;
		real dist_player = v2_dot(dv_player, dv_player);
		if(dist_player > 
			(render_distance + e->sprite.size.x * e->sprite.size.x)) {
			continue;
		}

		shadow.position = e->sprite.position;

		Sprite s = e->sprite;
		s.position.y -= e->z;
		s.sort_offset += e->z;
		if(e->anim != NULL) {
			if(e->anim->current_animation != -1) {
				animated_sprite_update(e->anim, (1.0f/60.0f));
				Animation* anim = e->anim->animations[e->anim->current_animation];
				Animation_Frame* frame = anim->frames + e->anim->current_frame;
				s.position += v2(frame->position);
				s.angle += frame->angle;
				s.color *= frame->color;
				s.size = frame->size;
				s.texture = frame->texture;
				s.sort_offset += frame->sort_offset;
				s.position.y -= frame->position.z;
				s.sort_offset += frame->position.z;
			}
		}

		render_add(&s);
		
		shadow.sort_offset = -4;
		if((e->flags & EntityFlag_SameShadow)) {
			shadow = s;
			shadow.position = e->sprite.position;
			shadow.sort_offset = -1;
			shadow.color = v4(0, 0, 0, 0.3f);
			shadow.flags = e->sprite.flags;
			shadow.size *= e->shadow_scale;
		} else {
			shadow.size = e->sprite.size * 0.75f * e->shadow_scale;
			shadow.size.y = shadow.size.x / 2;
			shadow.flags = Anchor_Center;
			shadow.texture = rect2(96, 16, 32, 16);
			shadow.color.w = 0.3f;
			shadow.sort_offset += e->sprite.sort_offset;
		}
		render_add(&shadow);


#line 389 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_entity_events.cpp"
	}
}
void rituals_interact_entities(Entity* entities, isize count, World_Area* area, World* world)
{
	for(isize i = 0; i < count; ++i) {
		Entity* e = entities + i;
		if(e->kind == EntityKind_Player) {
			Sim_Body* player = e->body;
			auto ud = &e->userdata.player;
			if(Input->scancodes[SDL_SCANCODE_SPACE] >= State_Pressed) {
				Sprite s;
				init_sprite(&s);
				s.position = player->shape.center;
				s.size = v2(16, 16);
				s.texture = rect2(64, 0, 16, 16);
				s.color = v4(1, 1, 1, 1);
				switch(e->direction) {
					case Direction_North:
						s.position.y -= s.size.y + player->shape.hh;
						break;
					case Direction_South:
						s.position.y += s.size.y + player->shape.hh;
						break;
					case Direction_East:
						s.position.x += s.size.x + player->shape.hw;
						break;
					case Direction_West:
						s.position.x -= s.size.x + player->shape.hh;
						break;
				}

				if(Input->scancodes[SDL_SCANCODE_SPACE] == State_Just_Pressed) {
					
					Sim_Body* touching = sim_query_aabb(&area->sim, 
							aabb(s.position, s.size.x / 2, s.size.y / 2));
					if(touching != NULL) {
						if(!(touching->flags & Body_Flag_Static)) 
							ud->held_entity_id = touching->entity_id;
					}
				}
			} else {
				ud->held_entity_id = -1;
			}

			if(ud->held_entity_id > 0) {
				Entity* s = world_area_find_entity(area, ud->held_entity_id);
				if(s != NULL) {
					Sim_Body* b = s->body;
					Vec2 target = player->shape.center; 
					Vec2 diff = b->shape.hext + player->shape.hext + v2(8, 8);
					switch(e->direction) {
						case Direction_North:
							target.y -= diff.y;
							break;
						case Direction_South:
							target.y += diff.y;
							break;
						case Direction_East:
							target.x += diff.x;
							break;
						case Direction_West:
							target.x -= diff.x;
							break;
					}

					Vec2 impulse = (target - b->shape.center);
					if(v2_dot(impulse, impulse) > (4 * (32) * (32))) {
						ud->held_entity_id = -1;
					}
					impulse *= 60;
					if(v2_dot(impulse, impulse) < (1000 * 1000)) 
						b->velocity += impulse;
				}
			}
		}
	}
}

void rituals_hit_entities(Hitbox_Contact* contacts, isize count, World_Area* area, World* world)
{
	for(isize i = 0; i < count; ++i) {
		Hitbox_Contact* c = contacts + i;
		Entity* a = c->a;
		Entity* b = c->b;

		
		
		
		if(a->kind > b->kind) {
			Entity* tmp = b;
			b = a;
			a = tmp;
		}

		switch(a->kind) {
			case EntityKind_Prop:
				if(b->kind == EntityKind_Bullet) {
					a->health -= b->attack;
					if(a->health <= 0) {
						rituals_prop_drop_on_break(area, a);
						world_area_remove_entity(area, a);
					}
					world_area_remove_entity(area, b);
				}
				break;
			case EntityKind_Player:
				if(b->kind == EntityKind_Enemy) {
					if(b->attack_timer <= 0.0f) {
						b->attack_timer = b->attack_interval;
						a->health -= b->attack;
						real angle = v2_to_angle(b->walk_impulse);
						angle += rand_range(&Game->r, -10, 10) * 0.01745329f;
						a->body->velocity += (v2_from_angle(angle) * b->knockback);
						a->userdata.player.heal_timer = a->userdata.player.heal_cooldown;
					}
				} else if(b->kind == EntityKind_Pickup) {
					if(b->userdata.pickup.kind == PickupKind_Health) {
						a->health += b->userdata.pickup.health.amount;
						world_area_remove_entity(area, b);
					}
				}
				break;
			case EntityKind_Enemy:
				if(b->kind == EntityKind_Bullet) {
					a->health -= b->attack;
					a->body->velocity += b->body->velocity * 4;
					a->userdata.enemy.mode = 1;
					if(a->health <= 0) {
						world_area_remove_entity(area, a);
					}
					world_area_remove_entity(area, b);
				}
				break;
		}
		
		
	}
}

void rituals_entity_on_contact_terrain(Entity* e, World_Area* area, World* world)
{
	if(e->kind == EntityKind_Bullet) {
		world_area_remove_entity(area, e);
	}
}

void rituals_contact_entities(Sim_Contact* contacts, isize count, World_Area* area, World* world)
{
	Simulator* sim = &area->sim;
	for(isize i = 0; i < count; ++i) {
		Sim_Contact* c = contacts + i;
		Sim_Body* body_a = sim_find_body(sim, c->a_id);
		if(body_a == NULL) continue;
		Sim_Body* body_b;
		if(c->b_id == -1) {
			body_b = c->static_b;
		} else {
			body_b = sim_find_body(sim, c->b_id);
		}
		if(body_b == NULL) continue;

		Entity* a = body_a->entity;
		Entity* b = body_b->entity;
		if(a == NULL && b != NULL) {
			rituals_entity_on_contact_terrain(b, area, world);
		} else if(a != NULL && b == NULL) {
			rituals_entity_on_contact_terrain(a, area, world);
		} else if(a != NULL && b != NULL) {
			if(a->kind > b->kind) {
				Entity* tmp = b;
				b = a;
				a = tmp;
			}

			if(b->kind == EntityKind_Bullet) {
				if(a->kind != EntityKind_Bullet && a->kind != EntityKind_Player) {
					world_area_remove_entity(area, b);
				}
			}
		}

	}
}
#line 177 "src\\rituals_main.cpp"

#line 1 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_play_state.cpp"














void init_play_state()
{
	clear_arena(Game->play_arena);
	clear_arena(Game->world_arena);
	play_state = ((Play_State*)arena_push(Game->game_arena, sizeof(Play_State)));
	play_state->delete_world_on_stop = false;
	play_state->save_world_on_stop = true;
	play_state->world = ((World*)arena_push(Game->world_arena, sizeof(World)));
	play_state->running = true;
}

void deserialize_world(World* world, FILE* world_file);
void start_play_state(char* world_name_in)
{
	World* world = play_state->world;
	isize wnl = strlen(world_name_in);
	char* world_name = ((char*)arena_push(Game->world_arena, sizeof(char) * wnl + 1));
	memcpy(world_name, world_name_in, wnl+1);
	FILE* fp = get_world_file(world_name, "rb");





#line 40 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_play_state.cpp"
	{
	
		init_world(world, 4, 4, next_random_uint64(&Game->r), Game->world_arena);
		generate_world(world_name, world);
		world_start_in_area(
				world,
				world->area_stubs, 
				Game->play_arena);
	}
}

void play_state_update()
{
	if(Game->state != Game_State_Play) return;
	if(((Input->scancodes[SDL_SCANCODE_ESCAPE])) == State_Just_Pressed) {
		play_state->running = !play_state->running;
	}
	world_area_update(play_state->world->current_area, play_state->world);


	if(!play_state->running) {
		Renderer->groups[0].offset = Vec2{};
		render_start(CurrentGroup);
		
		Sprite s = create_box_primitive(v2(-100, -100), Game->size + v2(100, 100), v4(0, 0, 0, 0.75f));
		s.flags = Anchor_Top_Left;
		render_add(&s);

		Body_Font->color = v4(1, 1, 1, 1);
		render_body_text("Paused", v2(32, 32), false, 4.0f);
		if(gui_add_button(v2(32, Game->size.y / 2), "Exit to main menu", v2(144, 0))) {
			serialize_world(play_state->world);
			init_play_state();
			Game->state = Game_State_Menu;
		}
		render_draw(Game->size, Game->scale);
	}
}

void play_state_stop()
{
	printf("Stopping play state... \n");
	if(play_state->save_world_on_stop) {
		serialize_world(play_state->world);
	}
	if(play_state->delete_world_on_stop) {
		world_delete_self(play_state->world);
	}
	init_play_state();
}

#line 179 "src\\rituals_main.cpp"

#line 1 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_serialization.cpp"














 

int delete_file(char* path, isize path_length, char* file)
{
	char buf[4096];
	snprintf(buf, 4096, "%.*s/%s", path_length, path, file);


#line 24 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_serialization.cpp"

#line 26 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_serialization.cpp"
}

int delete_folder(char* path, isize path_length)
{
	path[path_length] = '\0';


#line 34 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_serialization.cpp"

#line 36 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_serialization.cpp"
}

int _recursive_delete(const char* last_path, isize last_path_len, char* path)
{
	isize buf_size = last_path_len + 1 + strlen(path);
	char* buf = ((char*)arena_push(Game->temp_arena, sizeof(char) * buf_size));
	isize len = snprintf(buf, 4096, "%.*s/%s", last_path_len, last_path, path);
	tinydir_dir dir;
	tinydir_open_sorted(&dir, buf);
	for(usize i = 0; i < dir.n_files; ++i) {
		tinydir_file f;
		tinydir_readfile_n(&dir, &f, i);
		if(f.name[0] != '.') {
			if(f.is_dir) {
				_recursive_delete(buf, len, f.name);
			} else {
				delete_file(buf, len, f.name);
			}
		}
	}
	tinydir_close(&dir);
	delete_folder(buf, len);
		
	return 1;
}


int recursively_delete_folder(char* path, bool append_base_path = false)
{
	start_temp_arena(Game->temp_arena);
	if(append_base_path) {
		_recursive_delete(Game->base_path, Game->base_path_length-1, path); 
	} else {
		_recursive_delete(path, strlen(path), "");
	}
	end_temp_arena(Game->temp_arena);
	return 1;
}

int check_path(char* path)
{


#line 80 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_serialization.cpp"


#line 83 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_serialization.cpp"
}

void check_dir(char* dir)
{




#line 92 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_serialization.cpp"



#line 96 "c:\\users\\william\\stuff\\development\\rituals\\src\\rituals_serialization.cpp"
}

void serialize_tile_state(Tile_State* state, FILE* file)
{
	fwrite(&state->id, sizeof(isize), 1, file);
	fwrite(&state->damage, sizeof(int32), 1, file);
}

void deserialize_tile_state(Tile_State* state, FILE* file)
{
	fread(&state->id, sizeof(isize), 1, file);
	fread(&state->damage, sizeof(int32), 1, file);
}

void deserialize_tilemap(Tilemap* map, FILE* file, Memory_Arena* arena)
{
	fread(&map->w, sizeof(isize), 1, file);
	fread(&map->h, sizeof(isize), 1, file);
	isize size = map->w * map->h;
	map->tiles = ((Tile*)arena_push(arena, sizeof(Tile) * size));
	map->states = ((Tile_State*)arena_push(arena, sizeof(Tile_State) * size));
	fread(map->tiles, sizeof(Tile), size, file);
	for(isize i = 0; i < size; ++i) {
		deserialize_tile_state(map->states + i, file);
	}
}

void serialize_tilemap(Tilemap* map, FILE* file)
{
	fwrite(&map->w, sizeof(isize), 1, file);
	fwrite(&map->h, sizeof(isize), 1, file);
	isize size = map->w * map->h;
	fwrite(map->tiles, sizeof(Tile), size, file);
	for(isize i = 0; i < size; ++i) {
		serialize_tile_state(map->states + i, file);
	}
}
void serialize_sim_body(Sim_Body* body, FILE* file)
{
	fwrite(&body->id, sizeof(isize), 1, file);
	fwrite(&body->shape.e, sizeof(real), 4, file);
	fwrite(&body->velocity.e, sizeof(real), 2, file);
	fwrite(&body->force.e, sizeof(real), 2, file);
	fwrite(&body->collision_vel.e, sizeof(real), 2, file);
	fwrite(&body->inv_mass, sizeof(real), 1, file);
	fwrite(&body->restitution, sizeof(real), 1, file);
	fwrite(&body->damping, sizeof(real), 1, file);
	fwrite(&body->flags, sizeof(uint64), 1, file);
	fwrite(&body->group, sizeof(uint64), 1, file);
	fwrite(&body->mask, sizeof(uint64), 1, file);
	fwrite(&body->entity_id, sizeof(isize), 1, file);
}

void deserialize_sim_body(Sim_Body* body, FILE* file)
{
	fread(&body->id, sizeof(isize), 1, file);
	fread(&body->shape.e, sizeof(real), 4, file);
	fread(&body->velocity.e, sizeof(real), 2, file);
	fread(&body->force.e, sizeof(real), 2, file);
	fread(&body->collision_vel.e, sizeof(real), 2, file);
	fread(&body->inv_mass, sizeof(real), 1, file);
	fread(&body->restitution, sizeof(real), 1, file);
	fread(&body->damping, sizeof(real), 1, file);
	fread(&body->flags, sizeof(uint64), 1, file);
	fread(&body->group, sizeof(uint64), 1, file);
	fread(&body->mask, sizeof(uint64), 1, file);
	fread(&body->entity_id, sizeof(isize), 1, file);
}

void serialize_simulator(Simulator* sim, FILE* file)
{
	fwrite(&sim->bodies_count, sizeof(isize), 1, file);
	fwrite(&sim->bodies_capacity, sizeof(isize), 1, file);
	fwrite(&sim->contacts_capacity, sizeof(isize), 1, file);
	fwrite(&sim->next_body_id, sizeof(isize), 1, file);
	fwrite(&sim->sort_axis, sizeof(isize), 1, file);
	for(isize i = 0; i < sim->bodies_count; ++i) {
		serialize_sim_body(sim->bodies + i, file);
	}
}
void deserialize_simulator(Simulator* sim, FILE* file, Memory_Arena* arena)
{
	fread(&sim->bodies_count, sizeof(isize), 1, file);
	fread(&sim->bodies_capacity, sizeof(isize), 1, file);
	fread(&sim->contacts_capacity, sizeof(isize), 1, file);
	fread(&sim->next_body_id, sizeof(isize), 1, file);
	fread(&sim->sort_axis, sizeof(isize), 1, file);
	sim->bodies = ((Sim_Body*)arena_push(arena, sizeof(Sim_Body) * sim->bodies_capacity));
	sim->contacts = ((Sim_Contact*)arena_push(arena, sizeof(Sim_Contact) * sim->contacts_capacity));
	sim->contacts_count = 0;
	for(isize i = 0; i < sim->bodies_count; ++i) {
		deserialize_sim_body(sim->bodies + i, file);
	}
}

void serialize_sprite(Sprite* s, FILE* file)
{
	fwrite(&s->position.e, sizeof(real), 2, file);
	fwrite(&s->center.e, sizeof(real), 2, file);
	fwrite(&s->angle, sizeof(real), 1, file);
	fwrite(&s->size.e, sizeof(real), 2, file);
	fwrite(&s->texture.e, sizeof(real), 4, file);
	fwrite(&s->color.e, sizeof(real), 4, file);
	fwrite(&s->flags, sizeof(uint32), 1, file);
	fwrite(&s->sort_offset, sizeof(real), 1, file);
}

void deserialize_sprite(Sprite* s, FILE* file)
{
	fread(&s->position.e, sizeof(real), 2, file);
	fread(&s->center.e, sizeof(real), 2, file);
	fread(&s->angle, sizeof(real), 1, file);
	fread(&s->size.e, sizeof(real), 2, file);
	fread(&s->texture.e, sizeof(real), 4, file);
	fread(&s->color.e, sizeof(real), 4, file);
	fread(&s->flags, sizeof(uint32), 1, file);
	fread(&s->sort_offset, sizeof(real), 1, file);
}

void deserialize_rituals_entity_userdata(Entity* e, FILE* file)
{
	switch(e->kind) {
		case EntityKind_Prop: {
			auto prop = &e->userdata.prop;
			fread(&prop->contains, sizeof(Rituals_Entity_Kinds), 1, file);
			fread(&prop->subtype, sizeof(isize), 1, file);
			fread(&prop->amount, sizeof(isize), 1, file);
			fread(&prop->quality, sizeof(isize), 1, file);

		} break;
		case EntityKind_Player: {
			auto plr = &e->userdata.player;
			fread(&plr->held_entity_id, sizeof(isize), 1, file);
			fread(&plr->heal_cooldown, sizeof(real), 1, file);
			fread(&plr->heal_timer, sizeof(real), 1, file);
			fread(&plr->heal_rate, sizeof(isize), 1, file);
			fread(&plr->heal_to_interval, sizeof(isize), 1, file);
			
		} break;
		case EntityKind_Enemy: {
			auto enemy = &e->userdata.enemy;
			fread(&enemy->kind, sizeof(isize), 1, file);
			fread(&enemy->mode, sizeof(isize), 1, file);
			fread(&enemy->speed, sizeof(real), 1, file);
			fread(&enemy->alert_dist, sizeof(real), 1, file);
			fread(&enemy->follow_dist, sizeof(real), 1, file);
			switch(enemy->kind) {
				case EnemyKind_Bat:
					fread(&enemy->bat.perch.e, sizeof(real), 2, file);
					break;
				case EnemyKind_Snake:
					fread(&enemy->snake.chase_speed_modifier, sizeof(real), 1, file);
					break;
				case EnemyKind_Goblin_Knight:
					fread(&enemy->goblin_knight.patrol_start.e, sizeof(real), 2, file);
					fread(&enemy->goblin_knight.patrol_end.e, sizeof(real), 2, file);
					break;
				default:
					break;
			}
		} break;
		case EntityKind_Pickup: {
			auto p = &e->userdata.pickup;
			fread(&p->kind, sizeof(isize), 1, file);
			switch(p->kind) {
				case PickupKind_Item:
					fread(&p->item.id, sizeof(isize), 1, file);
					fread(&p->item.count, sizeof(isize), 1, file);
					break;
				case PickupKind_Health:
					fread(&p->health.amount, sizeof(isize), 1, file);
					break;
			}
		} break;
		default:
			break;
	}
}

void deserialize_hitbox(Hitbox* hb, FILE* file)
{
	fread(&hb->mask, sizeof(uint64), 1, file);
	fread(&hb->box.e, sizeof(real), 4, file);
}
void deserialize_entity(Entity* entity, FILE* file)
{
	fread(&entity->id, sizeof(isize), 1, file);
	fread(&entity->body_id, sizeof(isize), 1, file);
	deserialize_sprite(&entity->sprite, file);

	deserialize_hitbox(&entity->hitbox, file);
	fread(&entity->health, sizeof(int32), 1, file);
	fread(&entity->attack, sizeof(int32), 1, file);
	fread(&entity->attack_interval, sizeof(real), 1, file);
	fread(&entity->attack_timer, sizeof(real), 1, file);
	fread(&entity->knockback, sizeof(real), 1, file);

	fread(&entity->counter, sizeof(int32), 1, file);
	fread(&entity->facing, sizeof(int32), 1, file);
	
	fread(&entity->direction, sizeof(Direction), 1, file);
	fread(&entity->kind, sizeof(isize), 1, file);
	fread(&entity->events, sizeof(uint64), 1, file);
	fread(&entity->flags, sizeof(uint64), 1, file);
	
	deserialize_rituals_entity_userdata(entity, file);
}

void deserialize_area(World_Area* area, FILE* area_file, Memory_Arena* arena)
{
	fread(&area->id, sizeof(isize), 1, area_file);
	printf("loading %d area\n", area->id);
	fread(&area->entities_count, sizeof(isize), 1, area_file);
	fread(&area->entities_capacity, sizeof(isize), 1, area_file);
	fread(&area->next_entity_id, sizeof(isize), 1, area_file);
	fread(area->offset.e, sizeof(real), 2, area_file);
	area->entities = ((Entity*)arena_push(arena, sizeof(Entity) * area->entities_capacity));
	for(isize i = 0; i < area->entities_count; ++i) {
		deserialize_entity(area->entities + i, area_file);
	}

	area->entities_capacity = ((256) * (256));

	area->hitboxes_count = 0;
	area->hitboxes_capacity = ((256) * (256));
	area->hitboxes = ((Hitbox*)arena_push(arena, sizeof(Hitbox) * ((256) * (256))));

	area->hitbox_contacts_count = 0;
	area->hitbox_contacts_capacity = ((256) * (256));
	area->hitbox_contacts = ((Hitbox_Contact*)arena_push(arena, sizeof(Hitbox_Contact) * ((256) * (256))));

	area->removed_entities_count = 0;
	area->removed_entities_capacity = 256;
	area->removed_entities = ((isize*)arena_push(arena, sizeof(isize) * 256));
	deserialize_tilemap(&area->map, area_file, arena);
	deserialize_simulator(&area->sim, area_file, arena);
}

void serialize_rituals_entity_userdata(Entity* e, FILE* file)
{
	switch(e->kind) {
		case EntityKind_Prop: {
			auto prop = &e->userdata.prop;
			fwrite(&prop->contains, sizeof(Rituals_Entity_Kinds), 1, file);
			fwrite(&prop->subtype, sizeof(isize), 1, file);
			fwrite(&prop->amount, sizeof(isize), 1, file);
			fwrite(&prop->quality, sizeof(isize), 1, file);

		} break;
		case EntityKind_Player: {
			auto plr = &e->userdata.player;
			fwrite(&plr->held_entity_id, sizeof(isize), 1, file);
			fwrite(&plr->heal_cooldown, sizeof(real), 1, file);
			fwrite(&plr->heal_timer, sizeof(real), 1, file);
			fwrite(&plr->heal_rate, sizeof(isize), 1, file);
			fwrite(&plr->heal_to_interval, sizeof(isize), 1, file);
			
		} break;
		case EntityKind_Enemy: {
			auto enemy = &e->userdata.enemy;
			fwrite(&enemy->kind, sizeof(isize), 1, file);
			fwrite(&enemy->mode, sizeof(isize), 1, file);
			fwrite(&enemy->speed, sizeof(real), 1, file);
			fwrite(&enemy->alert_dist, sizeof(real), 1, file);
			fwrite(&enemy->follow_dist, sizeof(real), 1, file);
			switch(enemy->kind) {
				case EnemyKind_Bat:
					fwrite(&enemy->bat.perch.e, sizeof(real), 2, file);
					break;
				case EnemyKind_Snake:
					fwrite(&enemy->snake.chase_speed_modifier, sizeof(real), 1, file);
					break;
				case EnemyKind_Goblin_Knight:
					fwrite(&enemy->goblin_knight.patrol_start.e, sizeof(real), 2, file);
					fwrite(&enemy->goblin_knight.patrol_end.e, sizeof(real), 2, file);
					break;
				default:
					break;
			}
		} break;
		case EntityKind_Pickup: {
			auto p = &e->userdata.pickup;
			fwrite(&p->kind, sizeof(isize), 1, file);
			switch(p->kind) {
				case PickupKind_Item:
					fwrite(&p->item.id, sizeof(isize), 1, file);
					fwrite(&p->item.count, sizeof(isize), 1, file);
					break;
				case PickupKind_Health:
					fwrite(&p->health.amount, sizeof(isize), 1, file);
					break;
			}
		} break;
		default:
			break;
	}
}


void serialize_hitbox(Hitbox* hb, FILE* file)
{
	fwrite(&hb->mask, sizeof(uint64), 1, file);
	fwrite(&hb->box.e, sizeof(real), 4, file);
}

void serialize_entity(Entity* entity, FILE* file)
{
	fwrite(&entity->id, sizeof(isize), 1, file);
	fwrite(&entity->body_id, sizeof(isize), 1, file);
	serialize_sprite(&entity->sprite, file);

	serialize_hitbox(&entity->hitbox, file);
	fwrite(&entity->health, sizeof(int32), 1, file);
	fwrite(&entity->attack, sizeof(int32), 1, file);
	fwrite(&entity->attack_interval, sizeof(real), 1, file);
	fwrite(&entity->attack_timer, sizeof(real), 1, file);
	fwrite(&entity->knockback, sizeof(real), 1, file);

	fwrite(&entity->counter, sizeof(int32), 1, file);
	fwrite(&entity->facing, sizeof(int32), 1, file);
	
	fwrite(&entity->direction, sizeof(Direction), 1, file);
	fwrite(&entity->kind, sizeof(isize), 1, file);
	fwrite(&entity->events, sizeof(uint64), 1, file);
	fwrite(&entity->flags, sizeof(uint64), 1, file);
	
	serialize_rituals_entity_userdata(entity, file);
}

void serialize_area(World_Area* area, FILE* area_file)
{
	fwrite(&area->id, sizeof(isize), 1, area_file);
	printf("saving %d area\n", area->id);
	fwrite(&area->entities_count, sizeof(isize), 1, area_file);
	fwrite(&area->entities_capacity, sizeof(isize), 1, area_file);
	fwrite(&area->next_entity_id, sizeof(isize), 1, area_file);
	fwrite(area->offset.e, sizeof(real), 2, area_file);
	for(isize i = 0; i < area->entities_count; ++i) {
		serialize_entity(area->entities + i, area_file);
	}
	serialize_tilemap(&area->map, area_file);
	serialize_simulator(&area->sim, area_file);
	fclose(area_file);
}

void serialize_area_link(Area_Link* link, FILE* fp)
{
	fwrite(&link->position.x, sizeof(int32), 1, fp);
	fwrite(&link->position.y, sizeof(int32), 1, fp);
	fwrite(&link->link->id, sizeof(isize), 1, fp);
}

void deserialize_area_link(Area_Link* link, World* world, FILE* fp)
{
	fread(&link->position.x, sizeof(int32), 1, fp);
	fread(&link->position.y, sizeof(int32), 1, fp);
	isize linkid = 0;
	fread(&linkid, sizeof(isize), 1, fp);
	link->link = world->area_stubs + linkid;
}

void serialize_world_area_stub(World_Area_Stub* stub, FILE* fp)
{
	fwrite(&stub->id, sizeof(isize), 1, fp);
	fwrite(&stub->seed, sizeof(usize), 1, fp);
	serialize_area_link(&stub->north, fp);
	serialize_area_link(&stub->south, fp);
	serialize_area_link(&stub->east, fp);
	serialize_area_link(&stub->west, fp);
	fwrite(&stub->biome, sizeof(World_Area_Biome), 1, fp);
}

void deserialize_world_area_stub(World_Area_Stub* stub, World* world, FILE* fp)
{
	fread(&stub->id, sizeof(isize), 1, fp);
	fread(&stub->seed, sizeof(usize), 1, fp);
	deserialize_area_link(&stub->north, world, fp);
	deserialize_area_link(&stub->south, world, fp);
	deserialize_area_link(&stub->east, world, fp);
	deserialize_area_link(&stub->west, world, fp);
	fread(&stub->biome, sizeof(World_Area_Biome), 1, fp);
}



FILE* get_world_file(const char* name, const char* mode)
{
	char save_dir[4096];
	snprintf(save_dir, 4096, "%ssave/", Game->base_path);
	check_dir(save_dir);
	snprintf(save_dir, 4096, "%ssave/%s", Game->base_path, name);
	check_dir(save_dir);
	
	snprintf(save_dir, 4096, "%ssave/%s/world.dat", 
			Game->base_path,
			name);
	
	FILE* world_file = fopen(save_dir, mode);
	return world_file;
}

FILE* get_area_file(const char* name, isize id, const char* mode)
{
	char save_dir[4096];
	snprintf(save_dir, 4096, "%ssave/", Game->base_path);
	check_dir(save_dir);
	snprintf(save_dir, 4096, "%ssave/%s", Game->base_path, name);
	check_dir(save_dir);
	
	snprintf(save_dir, 4096, "%ssave/%s/areas", Game->base_path, name);
	check_dir(save_dir);
	snprintf(save_dir, 4096, "%ssave/%s/areas/area_%d.dat",
			Game->base_path,
			name,
			id);

	FILE* area_file = fopen(save_dir, mode);
	return area_file;
}

void serialize_world(World* world)
{
	world_area_deinit_player(world->current_area);
	printf("saving %d current_area_id\n", world->current_area->id);
	if(world->name[0] == '\0') {
		printf("Could not save world -- name was null\n");
		return;
	}
	FILE* world_file = get_world_file(world->name, "wb");
	if(world_file != NULL) {
		isize namelen = strlen(world->name);
		fwrite(&namelen, sizeof(isize), 1, world_file);
		fwrite(world->name, sizeof(char), strlen(world->name), world_file);
		fwrite(&world->areas_count, sizeof(isize), 1, world_file);
		fwrite(&world->areas_capacity, sizeof(isize), 1, world_file);
		fwrite(&world->areas_width, sizeof(isize), 1, world_file);
		fwrite(&world->areas_height, sizeof(isize), 1, world_file);

		serialize_entity(&world->global_player_entity, world_file);
		serialize_sim_body(&world->global_player_body, world_file);

		fwrite(&world->next_area_id, sizeof(isize), 1, world_file);
		fwrite(&world->current_area->id, sizeof(isize), 1, world_file);
		for(isize i = 0; i < world->areas_count; ++i) {
			serialize_world_area_stub(world->area_stubs + i, world_file);
		}
		FILE* area_file = get_area_file(world->name, world->current_area->id, "wb");

		if(area_file != NULL) {
			serialize_area(world->current_area, area_file);
		} else {
			printf("Could not open area file\n");
		}
		fclose(world_file);
	} else {
		printf("Could not open world file \n");
	}
}

void deserialize_world(World* world, FILE* world_file)
{
	isize namelen = 0;
	fread(&namelen, sizeof(isize), 1, world_file);
	world->name = ((char*)arena_push(Game->world_arena, sizeof(char) * namelen+1));
	fread((char*)world->name, sizeof(char), namelen, world_file);
	world->name[namelen] = '\0';
	fread(&world->areas_count, sizeof(isize), 1, world_file);
	fread(&world->areas_capacity, sizeof(isize), 1, world_file);
	fread(&world->areas_width, sizeof(isize), 1, world_file);
	fread(&world->areas_height, sizeof(isize), 1, world_file);
	deserialize_entity(&world->global_player_entity, world_file);
	deserialize_sim_body(&world->global_player_body, world_file);
	fread(&world->next_area_id, sizeof(isize), 1, world_file);
	isize current_area_id = 0;
	fread(&current_area_id, sizeof(isize), 1, world_file);
	world->area_stubs = ((World_Area_Stub*)arena_push(Game->world_arena, sizeof(World_Area_Stub) * world->areas_capacity));
	for(isize i = 0; i < world->areas_count; ++i) {
		deserialize_world_area_stub(world->area_stubs + i, world, world_file);
	}
	printf("loading %d current_area_id\n", current_area_id);
	world_start_in_area(world, world->area_stubs + current_area_id, Game->play_arena);
	fclose(world_file);
}



#line 181 "src\\rituals_main.cpp"

void init_menu_state()
{	
	menu_state = ((Menu_State*)arena_push(Game->game_arena, sizeof(Menu_State)));
	init_text_input_handle(&menu_state->handle, 256, Game->game_arena);
	menu_state->delete_index = -1;

	menu_state->save_dir_len = snprintf(menu_state->save_dir, 4096, "%ssave", Game->base_path);
	check_dir(menu_state->save_dir);
	tinydir_open_sorted(&menu_state->saves, menu_state->save_dir);
}

void stop_state()
{
	switch(Game->state) {
		case Game_State_None:
			break;
		case Game_State_Menu:
			break;
		case Game_State_Play:
			
			play_state_stop();
			break;
		default:
			break;
	}
	Game->state = Game_State_None;
}

void start_state()
{

}

void switch_state(Game_State newstate)
{
	stop_state();
	Game->state = newstate;
	start_state();
}


bool b = false;
real r = 0;
void main_menu_update()
{
	game_set_scale(2.0f);
	Renderer->groups[0].offset = v2(0, 0);
	render_start();
	
	real lasty = 32;
	Body_Font->color = v4(1, 1, 1, 1);
	render_body_text("Rituals", v2(32, 32), false, 4.0f);
	lasty += 16;
	lasty += Body_Font->glyph_height * 4;
	gui_add_text_input(&menu_state->handle, 
			v2(32, lasty), 
			v2(256, Body_Font->glyph_height + 8), 
			"Enter new world name");

	if(gui_add_button(v2(256 + 32 + 16 , lasty), "Create", v2(64, 0))) {
		menu_state->saves_dirty = true;
	}
	if(Input->scancodes[SDL_SCANCODE_RETURN] == State_Just_Pressed) {
		menu_state->saves_dirty = true;
	}

	if(menu_state->saves_dirty) {
		tinydir_close(&menu_state->saves);
		char buf[4096];
	 	snprintf(buf, 4096, "%ssave/%.*s",
				Game->base_path, 
				menu_state->handle.buffer_length, 
				menu_state->handle.buffer);
		check_dir(buf);
		
		tinydir_open_sorted(&menu_state->saves, menu_state->save_dir);
		menu_state->handle.buffer_length = 0;
		menu_state->saves_dirty = false;
	}
	lasty += 32 + 16;

	for(usize i = 0; i < menu_state->saves.n_files; ++i) {
		tinydir_file file;
		tinydir_readfile_n(&menu_state->saves, &file, i);
		if(file.is_dir && (file.name[0] != '.')) {
			if(gui_add_button(v2(32, lasty), file.name, v2(144, 0))) {
				recursively_delete_folder(file.path);
				start_play_state(file.name);
				Game->state = Game_State_Play;
			}
			if(menu_state->delete_index != i) {
				if(gui_add_button(v2(32 + 144 + 32, lasty), "Delete", v2(64, 0))) {
					menu_state->delete_index = i;
				}
			} else {

				if(gui_add_button(v2(32 + 144 + 32, lasty), "Cancel", v2(64, 0))) {
					menu_state->delete_index = -1;
				}
				if(gui_add_button(v2(32 + 144 + 32 + 64 + 32, lasty), "Confirm", v2(64, 0))) {
					printf("[%s] was deleted \n", file.path);
					recursively_delete_folder(file.path);
					menu_state->delete_index = -1;
					menu_state->saves_dirty = true;
				}
			}
			lasty += 32;
		}
	}

	if(menu_state->saves_dirty) {
		tinydir_close(&menu_state->saves);
		tinydir_open_sorted(&menu_state->saves, menu_state->save_dir);
	}

	render_draw(Game->size, Game->scale);
}



void test_update()
{
	game_set_scale(2.0);
	render_start();

	render_draw(Game->size, Game->scale);
}

void update()
{
	switch(Game->state) {
		case Game_State_None:
			test_update();
			break;
		case Game_State_Menu:
			SDL_StartTextInput();
			main_menu_update();
			break;
		case Game_State_Play:
			SDL_StopTextInput();
			play_state_update();
			break;
		default:
			break;
	}
}

void load_assets()
{
	isize w, h;
	Renderer->groups[0].texture = ogl_load_texture("data/graphics.png", &w, &h);
	Renderer->groups[0].texture_size.x = w;
	Renderer->groups[0].texture_size.y = h;
	Renderer->groups[1].texture = Renderer->groups[0].texture;
	Renderer->groups[1].texture_size = Renderer->groups[0].texture_size;

	Game->body_font = ((Spritefont*)arena_push(Game->asset_arena, sizeof(Spritefont)));
	init_spritefont(Game->body_font, Gohufont_Glyphs);
	Body_Font = Game->body_font;

	init_game_registry(Registry, Game->registry_arena); 
	register_everything_in_rituals();
	finalize_game_registry();

	Game->state = Game_State_Menu;
	init_play_state();
	init_menu_state();


}


void update_screen()
{
	SDL_GetWindowSize(Game->window, &Game->window_size.x, &Game->window_size.y);
	glViewport(0, 0, Game->window_size.x, Game->window_size.y);
	Game->size = v2(Game->window_size) * Game->scale;
}

float tt = 0;
int main(int argc, char** argv)
{
	
	printf("%d \n", (usize)&(((Sprite*)(NULL))->center));
	
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("There was an error: %s \n", "Could not init SDL");; 
		printf("There was an error: %s \n", SDL_GetError());;
		return 1;
	}

	
	

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	
		
		SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	

	int32 window_display_index = 0;

	window_display_index = 1;
#line 395 "src\\rituals_main.cpp"
	SDL_Window* window = SDL_CreateWindow("Rituals", 
			SDL_WINDOWPOS_CENTERED_DISPLAY(window_display_index), 
			SDL_WINDOWPOS_CENTERED_DISPLAY(window_display_index),
			1280, 720, 
			SDL_WINDOW_OPENGL | 
			SDL_WINDOW_RESIZABLE |
			SDL_WINDOW_MOUSE_FOCUS |
			SDL_WINDOW_INPUT_FOCUS);

	if(window == NULL) {
		printf("There was an error: %s \n", "Could not create window");;
		printf("There was an error: %s \n", SDL_GetError());;
		return 1;
	}

	printf("%s \n", SDL_GetError());
	SDL_GLContext glctx = SDL_GL_CreateContext(window);

	if(ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		printf("There was an error: %s \n", "Could not load OpenGL 3.3 functions...");;
		return 1;
	}















#line 433 "src\\rituals_main.cpp"

	{






			 
		
		bool gl_checks[64];
		char* gl_names[64];
		int gl_vals[64];
		int gl_exp_vals[64];
		isize gl_check_count = 0;

		int _SDL_GL_RED_SIZE_val; int _SDL_GL_RED_SIZE_success = SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &_SDL_GL_RED_SIZE_val); gl_checks[gl_check_count++] = _SDL_GL_RED_SIZE_val == 8; gl_names[gl_check_count - 1] = "SDL_GL_RED_SIZE"; gl_vals[gl_check_count - 1] = _SDL_GL_RED_SIZE_val; gl_exp_vals[gl_check_count - 1] = 8;;
		int _SDL_GL_GREEN_SIZE_val; int _SDL_GL_GREEN_SIZE_success = SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &_SDL_GL_GREEN_SIZE_val); gl_checks[gl_check_count++] = _SDL_GL_GREEN_SIZE_val == 8; gl_names[gl_check_count - 1] = "SDL_GL_GREEN_SIZE"; gl_vals[gl_check_count - 1] = _SDL_GL_GREEN_SIZE_val; gl_exp_vals[gl_check_count - 1] = 8;;
		int _SDL_GL_BLUE_SIZE_val; int _SDL_GL_BLUE_SIZE_success = SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &_SDL_GL_BLUE_SIZE_val); gl_checks[gl_check_count++] = _SDL_GL_BLUE_SIZE_val == 8; gl_names[gl_check_count - 1] = "SDL_GL_BLUE_SIZE"; gl_vals[gl_check_count - 1] = _SDL_GL_BLUE_SIZE_val; gl_exp_vals[gl_check_count - 1] = 8;;
		int _SDL_GL_ALPHA_SIZE_val; int _SDL_GL_ALPHA_SIZE_success = SDL_GL_GetAttribute(SDL_GL_ALPHA_SIZE, &_SDL_GL_ALPHA_SIZE_val); gl_checks[gl_check_count++] = _SDL_GL_ALPHA_SIZE_val == 8; gl_names[gl_check_count - 1] = "SDL_GL_ALPHA_SIZE"; gl_vals[gl_check_count - 1] = _SDL_GL_ALPHA_SIZE_val; gl_exp_vals[gl_check_count - 1] = 8;;
		int _SDL_GL_DOUBLEBUFFER_val; int _SDL_GL_DOUBLEBUFFER_success = SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &_SDL_GL_DOUBLEBUFFER_val); gl_checks[gl_check_count++] = _SDL_GL_DOUBLEBUFFER_val == 1; gl_names[gl_check_count - 1] = "SDL_GL_DOUBLEBUFFER"; gl_vals[gl_check_count - 1] = _SDL_GL_DOUBLEBUFFER_val; gl_exp_vals[gl_check_count - 1] = 1;;
		int _SDL_GL_CONTEXT_MAJOR_VERSION_val; int _SDL_GL_CONTEXT_MAJOR_VERSION_success = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &_SDL_GL_CONTEXT_MAJOR_VERSION_val); gl_checks[gl_check_count++] = _SDL_GL_CONTEXT_MAJOR_VERSION_val == 3; gl_names[gl_check_count - 1] = "SDL_GL_CONTEXT_MAJOR_VERSION"; gl_vals[gl_check_count - 1] = _SDL_GL_CONTEXT_MAJOR_VERSION_val; gl_exp_vals[gl_check_count - 1] = 3;;
		int _SDL_GL_CONTEXT_MINOR_VERSION_val; int _SDL_GL_CONTEXT_MINOR_VERSION_success = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &_SDL_GL_CONTEXT_MINOR_VERSION_val); gl_checks[gl_check_count++] = _SDL_GL_CONTEXT_MINOR_VERSION_val == 3; gl_names[gl_check_count - 1] = "SDL_GL_CONTEXT_MINOR_VERSION"; gl_vals[gl_check_count - 1] = _SDL_GL_CONTEXT_MINOR_VERSION_val; gl_exp_vals[gl_check_count - 1] = 3;;
		int _SDL_GL_FRAMEBUFFER_SRGB_CAPABLE_val; int _SDL_GL_FRAMEBUFFER_SRGB_CAPABLE_success = SDL_GL_GetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, &_SDL_GL_FRAMEBUFFER_SRGB_CAPABLE_val); gl_checks[gl_check_count++] = _SDL_GL_FRAMEBUFFER_SRGB_CAPABLE_val == 1; gl_names[gl_check_count - 1] = "SDL_GL_FRAMEBUFFER_SRGB_CAPABLE"; gl_vals[gl_check_count - 1] = _SDL_GL_FRAMEBUFFER_SRGB_CAPABLE_val; gl_exp_vals[gl_check_count - 1] = 1;;
		int _SDL_GL_ACCELERATED_VISUAL_val; int _SDL_GL_ACCELERATED_VISUAL_success = SDL_GL_GetAttribute(SDL_GL_ACCELERATED_VISUAL, &_SDL_GL_ACCELERATED_VISUAL_val); gl_checks[gl_check_count++] = _SDL_GL_ACCELERATED_VISUAL_val == 1; gl_names[gl_check_count - 1] = "SDL_GL_ACCELERATED_VISUAL"; gl_vals[gl_check_count - 1] = _SDL_GL_ACCELERATED_VISUAL_val; gl_exp_vals[gl_check_count - 1] = 1;;
		int _SDL_GL_CONTEXT_PROFILE_MASK_val; int _SDL_GL_CONTEXT_PROFILE_MASK_success = SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &_SDL_GL_CONTEXT_PROFILE_MASK_val); gl_checks[gl_check_count++] = _SDL_GL_CONTEXT_PROFILE_MASK_val == SDL_GL_CONTEXT_PROFILE_CORE; gl_names[gl_check_count - 1] = "SDL_GL_CONTEXT_PROFILE_MASK"; gl_vals[gl_check_count - 1] = _SDL_GL_CONTEXT_PROFILE_MASK_val; gl_exp_vals[gl_check_count - 1] = SDL_GL_CONTEXT_PROFILE_CORE;;

		for(isize i = 0; i < gl_check_count; ++i) {
			printf("%s %s: wanted %d, got %d \n", 
					gl_names[i], 
					gl_checks[i] ? "succeeeded" : "failed", 
					gl_exp_vals[i], 
					gl_vals[i]);
		}

	}	

	
	Game = ((Game_Main*)calloc(sizeof(Game_Main), 1));
	{
		Game->last_frame_time = 1;
		Game->window = window;
		Game->state = Game_State_None;
		Game->meta_arena = ((Memory_Arena*)calloc(sizeof(Memory_Arena), 1));
		init_memory_arena(Game->meta_arena, (((isize)sizeof(Memory_Arena))) * 20);
		Game->game_arena = new_memory_arena(((64 * UINT64_C(1024)) * UINT64_C(1024)), Game->meta_arena);
		Game->asset_arena = new_memory_arena(((512 * UINT64_C(1024)) * UINT64_C(1024)), Game->meta_arena);
		Game->temp_arena = new_memory_arena((((4 * UINT64_C(1024)) * UINT64_C(1024)) * UINT64_C(1024)), Game->meta_arena);
		Game->play_arena = new_memory_arena((((4 * UINT64_C(1024)) * UINT64_C(1024)) * UINT64_C(1024)), Game->meta_arena);
		Game->renderer_arena = new_memory_arena(((256 * UINT64_C(1024)) * UINT64_C(1024)), Game->meta_arena);
		Game->world_arena = new_memory_arena(((1024 * UINT64_C(1024)) * UINT64_C(1024)), Game->meta_arena);
		Game->registry_arena = new_memory_arena(((2 * UINT64_C(1024)) * UINT64_C(1024)), Game->meta_arena);

		Game->base_path = SDL_GetBasePath();
		Game->base_path_length = strlen(Game->base_path);

		Game->input = ((Game_Input*)arena_push(Game->game_arena, sizeof(Game_Input)));
		Input = Game->input;
		Input->scancodes = ((int8*)arena_push(Game->game_arena, sizeof(int8) * SDL_NUM_SCANCODES));
		Input->keycodes = ((int8*)arena_push(Game->game_arena, sizeof(int8) * SDL_NUM_SCANCODES));
		Input->mouse = ((int8*)arena_push(Game->game_arena, sizeof(int8) * 16));
		Input->mouse_pos = 	v2(0, 0);
		Input->text = ((char*)arena_push(Game->game_arena, sizeof(char) * (1024)));
		Input->text_count = 0;
		Input->capture_newlines = false;
		Input->capture_tabs = false;

		init_random(&Game->r, time(NULL));
		
		Game->window_size = v2i(1280, 720);
		Game->scale = 1.0f;

		Game->renderer = ((OpenGL_Renderer*)arena_push(Game->game_arena, sizeof(OpenGL_Renderer)));

		char* vert_src = 
#line 1 "c:\\users\\william\\stuff\\development\\rituals\\src\\vert.glsl"
R"shader(
#version 330 core
//Offset from origin in pixels (world space)
layout (location = 0) in vec2 v_translate;

//Center of sprite for rotation (model space)
layout (location = 1) in vec2 v_center;

//Angle to render at
layout (location = 2) in float v_angle;

//Amount to scale sprite by
layout (location = 3) in vec2 v_size;

//x, y, w, h of texture rectangle in 0->1 form
layout (location = 4) in vec4 v_texcoords;

//rgba color, sent to frag shader
layout (location = 5) in vec4 v_color;

//flags first 4 bits: anchor, 1<<4+ flags
layout (location = 6) in uint v_flags;

out vec2 f_pixel;
out vec2 f_texcoords;
out vec4 f_color;

uniform mat4 u_orthomat;

void main()
{
	float[36] coords_arr = float[](
		-0.5, -0.5,
		0.5, 0.5,
		0.0, 0.0,
		1.0, 1.0,
		-0.5, 0.0, 
		0.5, 1.0,
		-1.0, 0.0,
		0.0, 1.0,
		-1.0, -0.5,
		0.0, 0.5,
		-1.0, -1.0,
		0.0, 0.0,
		-0.5, -1.0,
		0.5, 0.0,
		0.0, -1.0,
		1.0, 0.0,
		0.0, -0.5,
		1.0, 0.5
	);

	uint vertex_x = uint(gl_VertexID & 2);
	uint vertex_y = uint(((gl_VertexID & 1) << 1) ^ 3);
	uint v_anchor = v_flags & 0xF;
	vertex_x += uint(4) * v_anchor;
	vertex_y += uint(4) * v_anchor;

	uint v_fliphoriz = v_flags & uint(1<<4);
	uint v_flipvert = v_flags & uint(1<<5);


	vec2 coords = vec2(
		coords_arr[vertex_x],
		coords_arr[vertex_y]
	);

	vec4 tex_rect = vec4(
		v_texcoords.x, v_texcoords.y,
		v_texcoords.x + v_texcoords.z, 
		v_texcoords.y + v_texcoords.w
	);

	if(v_fliphoriz >= uint(1)) {
		tex_rect = tex_rect.zyxw;
	}
	if(v_flipvert >= uint(1)) {
		tex_rect = tex_rect.xwzy;
	}

	float[4] texcoords_arr = float[](
			tex_rect.x, tex_rect.y,
			tex_rect.z, tex_rect.w
	);

	f_texcoords = vec2(
		texcoords_arr[gl_VertexID & 2],
		texcoords_arr[((gl_VertexID & 1) << 1) ^ 3]
	);
	coords.x *= v_size.x;
	coords.y *= v_size.y;
	vec2 rot = vec2(cos(v_angle), sin(v_angle));
	mat2 rotmat = mat2 (
		rot.x, rot.y,
		-rot.y, rot.x
	);
	coords *= rotmat;
	coords += v_translate;
	coords -= v_center;
	gl_Position = vec4(coords, 0, 1) * u_orthomat;
	f_pixel = gl_Position.xy;
	f_color = v_color;

}
)shader"

#line 509 "src\\rituals_main.cpp"
			;
		 char* frag_src = 
#line 1 "c:\\users\\william\\stuff\\development\\rituals\\src\\frag.glsl"










R"shader(
#version 330 core
in vec2 f_pixel;
in vec2 f_texcoords;
in vec4 f_color;

uniform vec2 u_texturesize;
uniform float u_night_amount;
uniform float u_night_cutoff;
uniform sampler2D u_texture0;

out vec4 final_color;

void main()
{
	vec4 color = texture(u_texture0, f_texcoords) * f_color;

	// Nighttime effect
	// Darkens everything except very light colors.
	float avg = (color.r + color.g + color.b) / 3;
	//avg *= avg;
	if(avg < u_night_cutoff) {
		color.rgb *= u_night_amount;
	}

	final_color = color;

}

)shader"

#line 512 "src\\rituals_main.cpp"
			;
		init_renderer(Game->renderer, 4, ((32 * UINT64_C(1024)) * UINT64_C(1024)), vert_src, frag_src, Game->renderer_arena);
		
		Game->registry = ((Game_Registry*)arena_push(Game->game_arena, sizeof(Game_Registry)));

		Registry = Game->registry;
		Renderer = Game->renderer;
		CurrentGroup = Renderer->groups;
	}

	load_assets();

	bool running = true;
	SDL_Event event;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	

	play_state->current_time = SDL_GetTicks();
	play_state->prev_time = play_state->current_time;
	isize input_text_length = 0;
	while(running) {
		uint64 start_ticks = SDL_GetTicks();

		for(int64 i = 0; i < SDL_NUM_SCANCODES; ++i) {
			int8* t = Game->input->scancodes + i;
			if(*t == State_Just_Released) {
				*t = State_Released;
			} else if(*t == State_Just_Pressed) {
				*t = State_Pressed;
			}
			t = Game->input->keycodes + i;
			if(*t == State_Just_Released) {
				*t = State_Released;
			} else if(*t == State_Just_Pressed) {
				*t = State_Pressed;
			}
		}
		for(int64 i = 0; i < 16; ++i) {
			int8* t = Game->input->mouse + i;
			if(*t == State_Just_Released) {
				*t = State_Released;
			} else if(*t == State_Just_Pressed) {
				*t = State_Pressed;
			}
		}
		while(SDL_PollEvent(&event)) {
			
			switch(event.type) {
				case SDL_QUIT:
					stop_state();
					running = false;
					return 0;
					break;
				case SDL_WINDOWEVENT:
					update_screen();
					break;
				case SDL_TEXTINPUT:
					input_text_length = strlen(event.text.text);
					if((input_text_length + Input->text_count) < (1024)) {
						memcpy(Input->text + Input->text_count, 
								event.text.text, 
								input_text_length);
						Input->text_count += input_text_length;
						input_text_length = 0;
					}
					break;
				case SDL_KEYDOWN:
					Game->input->num_keys_down++;
					if((event.key.keysym.sym == SDLK_BACKSPACE)) {
						input_text_append_char(('\b'));
						if(event.key.keysym.mod & KMOD_CTRL) {
							input_text_append_char('\7');
						}
					} else if(event.key.keysym.sym == SDLK_RETURN || 
							event.key.keysym.sym == SDLK_RETURN2) {
						if(Input->capture_newlines) {
							input_text_append_char('\n');
						}
					} else if(event.key.keysym.sym == SDLK_TAB) {
						if(Input->capture_tabs) {
							input_text_append_char('\t');
						}
					}

					if(!event.key.repeat) {
						Game->input->scancodes[event.key.keysym.scancode] = State_Just_Pressed;
						if(event.key.keysym.sym < SDL_NUM_SCANCODES) {
							Game->input->keycodes[event.key.keysym.sym] = State_Just_Pressed;
						}
					}
					break;
				case SDL_KEYUP:
					Game->input->num_keys_down--;
					if(!event.key.repeat) {
						Game->input->scancodes[event.key.keysym.scancode] = State_Just_Released;
						if(event.key.keysym.sym < SDL_NUM_SCANCODES) {
							Game->input->keycodes[event.key.keysym.sym] = State_Just_Released;
						}
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					Game->input->num_mouse_down++;
					Game->input->mouse[event.button.button] = State_Just_Pressed;
					break;
				case SDL_MOUSEBUTTONUP:
					Game->input->num_mouse_down--;
					Game->input->mouse[event.button.button] = State_Just_Released;
					break;
			}
		}
	
		int mx, my;
		SDL_GetMouseState(&mx, &my);
		Input->mouse_x = mx;
		Input->mouse_y = my;
		Input->screen_mouse_pos = v2i(mx, my);
		game_calc_mouse_pos(Renderer->groups[0].offset);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		update();

		uint64 frame_ticks = SDL_GetTicks() - start_ticks;

		
		if(frame_ticks < 16) {
			
			SDL_Delay(16 - frame_ticks);
		}
#line 642 "src\\rituals_main.cpp"
		SDL_GL_SwapWindow(window);
		Game->last_frame_time = SDL_GetTicks() - start_ticks;
	}

	SDL_Quit();
	return 0;
}

