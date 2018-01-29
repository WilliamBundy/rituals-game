// Random number generators
static inline u64 _splitmix64(u64* x)
{
	*x += UINT64_C(0x9E3779B97F4A7C15);
	u64 z = *x;
	z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
	z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
	return z ^ (z >> 31);	
}

static inline u64 _rotate_left(const u64 t, i64 k)
{
	return (t << k) | (t >> (64 - k));
}

#ifndef WirmphtEnabled
struct Random
{
	u64 x, y;
};
#endif

u64 next_random_u64(Random* r)
{
	u64 a = r->x;
	u64 b = r->y;
	u64 result = a + b;

	b ^= a;
	r->x = _rotate_left(a, 55) ^ b ^ (b << 14);
	r->y = _rotate_left(b, 36);
	return result;
}

void init_random(Random* r, u64 seed) 
{
	_splitmix64(&seed);
	r->x = _splitmix64(&seed);
	r->y = _splitmix64(&seed);
	next_random_u64(r);
}

f64 next_random_double(Random* r)
{
	u64 x = next_random_u64(r);
	return (f64)x / UINT64_MAX;
}

f32 next_random_float(Random* r)
{
	return (f32)(next_random_double(r));
}

f32 next_random(Random* r) 
{
	return (f32)(next_random_double(r));
}

f32 rand_range(Random* r, f32 min, f32 max)
{
	return next_random_double(r) * (max - min) + min;
}

i32 rand_range_int(Random* r, f32 min, f32 max)
{
	return (i32)(next_random_double(r) * (max - min) + min);
}

