/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


// Random number generators
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

#ifndef REFLECTED
struct Random
{
	uint64 x, y;
};
#endif

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

