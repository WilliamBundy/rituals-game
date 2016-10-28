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



#ifndef REFLECTED
union Vec2 
{
	struct {
		real x, y;
	};
	real e[2];
};

union Vec3
{
	struct {
		real x, y, z;
	};
	real e[3];
};

union Vec4
{
	struct {
		real x, y, z, w;
	};
	real e[4];
	
};

union Vec2i
{
	struct {
		integer x, y;
	};
	integer e[2];
};

union Vec3i
{
	struct {
		integer x, y, z;
	};
	integer e[3];
};

union Vec4i
{
	struct {
		integer x, y, z, w;
	};
	integer e[4];
	
};

#endif

/* v* functions */

static inline Vec2 v2(real x, real y)
{
	return Vec2{x, y};
}

static inline Vec3 v3(real x, real y, real z)
{
	return Vec3{
		x, y, z
	};
}

static inline Vec4 v4(real x, real y, real z, real w)
{
	return Vec4{
		x, y, z, w
	};
}

static inline Vec2i v2i(integer x, integer y)
{
	return Vec2i {
		x, y
	};
}

static inline Vec3i v3i(integer x, integer y, integer z)
{
	return Vec3i{
		x, y, z
	};
}

static inline Vec4i v4i(integer x, integer y, integer z, integer w)
{
	return Vec4i{
		x, y, z, w
	};
}

#ifdef __cplusplus

static inline Vec2 v2(Vec2i vi)
{
	return v2(vi.x, vi.y);
}

static inline Vec2 v2(Vec3 vi)
{
	return v2(vi.x, vi.y);
}

static inline Vec3 v3(Vec3i vi)
{
    return v3(vi.x, vi.y, vi.z);
}

static inline Vec3 v3(Vec2 vi)
{
    return v3(vi.x, vi.y, 0);
}

static inline Vec3 v3(Vec2 vi, real z)
{
    return v3(vi.x, vi.y, z);
}

static inline Vec4 v4(Vec4i vi)
{
    return v4(vi.x, vi.y, vi.z, vi.w);
}

static inline Vec4 v4(Vec2 vi)
{
    return v4(vi.x, vi.y, 0, 0);
}

static inline Vec4 v4(Vec2 vi, real z, real w)
{
    return v4(vi.x, vi.y, z, w);
}

static inline Vec4 v4(Vec3 vi)
{
    return v4(vi.x, vi.y, vi.z, 0);
}

static inline Vec4 v4(Vec3 vi, real w)
{
    return v4(vi.x, vi.y, vi.z, w);
}

static inline Vec2i v2i(Vec2 vi)
{
    return v2i((integer)vi.x, (integer)vi.y);
}

static inline Vec3i v3i(Vec3 vi)
{
    return v3i((integer)vi.x, (integer)vi.y, (integer)vi.z);
}

static inline Vec4i v4i(Vec4 vi)
{
    return v4i((integer)vi.x, (integer)vi.y, (integer)vi.z, (integer)vi.w);
}
#endif


/* Vector operators */


// Defined operators:
// Negation (-)
// Addition, subtraction (+, -)
// Scalar multiplication (*)
// Hadamard Product / Piecewise multiplication (*)
// Scalar division (*)

// Overloaded: Assignment versions (+=, -=, *=, /=, *=(v))

/* Vec2 */

static inline Vec2 v2_negate(Vec2 a)
{
	return Vec2{-a.x, -a.y};
}

static inline Vec2 v2_sub(Vec2 a, Vec2 b)
{
	return Vec2{a.x - b.x, a.y - b.y};
}

static inline Vec2 v2_add(Vec2 a, Vec2 b)
{
	return Vec2{a.x + b.x, a.y + b.y};
}

static inline Vec2 v2_add_scaled(Vec2 a, Vec2 b, real scale)
{
	return Vec2{a.x + b.x * scale, a.y + b.y * scale};
}


static inline Vec2 v2_mul(Vec2 a, real s)
{
	return Vec2{a.x * s, a.y * s};
}

static inline Vec2 v2_div(Vec2 a, real s)
{
	return v2_mul(a, 1.0f / s);
}

static inline Vec2 v2_component_mul(Vec2 a, Vec2 b)
{
	return Vec2{a.x * b.x, a.y * b.y};
}

/* Vec3 */

static inline Vec3 v3_negate(Vec3 a)
{
	return Vec3{-a.x, -a.y, -a.z};
}

static inline Vec3 v3_sub(Vec3 a, Vec3 b)
{
	return Vec3{a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline Vec3 v3_add(Vec3 a, Vec3 b)
{
	return Vec3{a.x + b.x, a.y + b.y, a.z + b.z};
}

static inline Vec3 v3_add_scaled(Vec3 a, Vec3 b, real scale)
{
	return Vec3{a.x + b.x * scale, a.y + b.y * scale, a.z + b.z * scale};
}

static inline Vec3 v3_mul(Vec3 a, real s)
{
	return Vec3{a.x * s, a.y * s, a.z * s};
}

static inline Vec3 v3_div(Vec3 a, real s)
{
	return v3_mul(a, 1.0f/s);
}

static inline Vec3 v3_component_mul(Vec3 a, Vec3 b)
{
	return Vec3{a.x * b.x, a.y * b.y, a.z * b.z};
}

/* Vec4 */

static inline Vec4 v4_negate(Vec4 a)
{
    return Vec4{-a.x, -a.y, -a.z, -a.w};
}

static inline Vec4 v4_add(Vec4 a, Vec4 b)
{
	return Vec4{a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

static inline Vec4 v4_add_scaled(Vec4 a, Vec4 b, real scale)
{
	return Vec4{a.x + b.x * scale, a.y + b.y * scale, a.z + b.z * scale, a.w + b.w * scale};
}

static inline Vec4 v4_sub(Vec4 a, Vec4 b)
{
	return Vec4{a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

static inline Vec4 v4_compenent_mul(Vec4 a, Vec4 b)
{
	return Vec4{a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}

static inline Vec4 v4_mul(Vec4 a, real b)
{
	return Vec4{a.x * b, a.y * b, a.z * b, a.w * b};
}

static inline Vec4 v4_div(Vec4 a, real b)
{
	return  v4_mul(a, 1.0f/b);
}

/* Vec2i */

static inline Vec2i v2i_negate(Vec2i a)
{
	return Vec2i{-a.x, -a.y};
}

static inline Vec2i v2i_sub(Vec2i a, Vec2i b)
{
	return Vec2i{a.x - b.x, a.y - b.y};
}

static inline Vec2i v2i_add(Vec2i a, Vec2i b)
{
	return Vec2i{a.x + b.x, a.y + b.y};
}

static inline Vec2i v2i_add_scaled(Vec2i a, Vec2i b, real scale)
{
	return Vec2i{a.x + (integer)(b.x * scale), a.y + (integer)(b.y * scale)};
}
static inline Vec2i v2i_addi_scaled(Vec2i a, Vec2i b, integer scale)
{
	return Vec2i{a.x + (integer)(b.x * scale), a.y + (integer)(b.y * scale)};
}

static inline Vec2i v2i_mul(Vec2i a, real s)
{
	return Vec2i{(integer)(a.x * s), (integer)(a.y * s)};
}

static inline Vec2i v2i_muli(Vec2i a, integer s)
{
	return Vec2i{(integer)(a.x * s), (integer)(a.y * s)};
}

static inline Vec2i v2i_div(Vec2i a, real s)
{
	return v2i_mul(a, 1.0f / s);
}

static inline Vec2i v2i_divi(Vec2i a, integer s)
{
	return Vec2i{a.x / s, a.y / s};
}


static inline Vec2i v2i_component_mul(Vec2i a, Vec2i b)
{
	return Vec2i{a.x * b.x, a.y * b.y};
}

/* Vec3i */

static inline Vec3i v3i_negate(Vec3i a)
{
	return Vec3i{-a.x, -a.y, -a.z};
}

static inline Vec3i v3i_sub(Vec3i a, Vec3i b)
{
	return Vec3i{a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline Vec3i v3i_add(Vec3i a, Vec3i b)
{
	return Vec3i{a.x + b.x, a.y + b.y, a.z + b.z};
}

static inline Vec3i v3i_add_scaled(Vec3i a, Vec3i b, real scale)
{
	return Vec3i{a.x + (integer)(b.x * scale), a.y + (integer)(b.y * scale), a.z + (integer)(b.z * scale)};
}

static inline Vec3i v3i_addi_scaled(Vec3i a, Vec3i b, integer scale)
{
	return Vec3i{a.x + (integer)(b.x * scale), a.y + (integer)(b.y * scale), a.z + (integer)(b.z * scale)};
}

static inline Vec3i v3i_mul(Vec3i a, real s)
{
	return Vec3i{(integer)(a.x * s), (integer)(a.y * s), (integer)(a.z * s)};
}

static inline Vec3i v3i_muli(Vec3i a, integer s)
{
	return Vec3i{a.x * s, a.y * s, a.z * s};
}

static inline Vec3i v3i_div(Vec3i a, real s)
{
	return v3i_mul(a, 1.0f/s);
}

static inline Vec3i v3i_divi(Vec3i a, integer s)
{
	return Vec3i{a.x / s, a.y / s, a.z / s};
}


static inline Vec3i v3i_component_mul(Vec3i a, Vec3i b)
{
	return Vec3i{a.x * b.x, a.y * b.y, a.z * b.z};
}

/* Vec4i */

static inline Vec4i v4i_negate(Vec4i a)
{
    return Vec4i{-a.x, -a.y, -a.z, -a.w};
}

static inline Vec4i v4i_add(Vec4i a, Vec4i b)
{
	return Vec4i{a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

static inline Vec4i v4i_add_scaled(Vec4i a, Vec4i b, real scale)
{
	return Vec4i{a.x + (integer)(b.x * scale), a.y + (integer)(b.y * scale), a.z + (integer)(b.z * scale), a.w + (integer)(b.w * scale)};
}

static inline Vec4i v4i_addi_scaled(Vec4i a, Vec4i b, integer scale)
{
	return Vec4i{a.x + (integer)(b.x * scale), a.y + (integer)(b.y * scale), a.z + (integer)(b.z * scale), a.w + (integer)(b.w * scale)};
}

static inline Vec4i v4i_sub(Vec4i a, Vec4i b)
{
	return Vec4i{a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

static inline Vec4i v4i_compenent_mul(Vec4i a, Vec4i b)
{
	return Vec4i{a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}

static inline Vec4i v4i_mul(Vec4i a, real b)
{
	return Vec4i{(integer)(a.x * b), (integer)(a.y * b), (integer)(a.z * b), (integer)(a.w * b)};
}
static inline Vec4i v4i_muli(Vec4i a, integer b)
{
	return Vec4i{(integer)(a.x * b), (integer)(a.y * b), (integer)(a.z * b), (integer)(a.w * b)};
}

static inline Vec4i v4i_divi(Vec4i a, real b)
{
	return Vec4i{(integer)(a.x / b), (integer)(a.y / b), (integer)(a.z / b), (integer)(a.w / b)};
}

static inline Vec4i v4i_div(Vec4i a, real b)
{
	return  v4i_mul(a, 1.0f/b);
}

#ifdef __cplusplus
/* Operator Overloads */

/* Vec2 */

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

static inline Vec2 operator/(Vec2 a, real s)
{
	return a * (1.0f / s);
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

static inline Vec2& operator*=(Vec2& a, Vec2 b)
{
	a = a * b;
	return a;
}

static inline Vec2& operator/=(Vec2& a, real b)
{
	a = a / b;
	return a;
}

/* Vec3 */

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

static inline Vec3 operator*(real s, Vec3 a)
{
	return Vec3{a.x * s, a.y * s, a.z * s};
}

static inline Vec3 operator/(Vec3 a, real s)
{
	return a * (1.0f / s);
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

static inline Vec3& operator*=(Vec3& a, Vec3 b)
{
	a = a * b;
	return a;
}

static inline Vec3& operator/=(Vec3& a, real b)
{
	a = a / b;
	return a;
}


/* Vec4 */

static inline Vec4 operator-(Vec4 a)
{
    return Vec4{-a.x, -a.y, -a.z, -a.w};
}

static inline Vec4 operator+(Vec4 a, Vec4 b)
{
	return Vec4{a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

static inline Vec4 operator-(Vec4 a, Vec4 b)
{
	return Vec4{a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

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

static inline Vec4 operator+=(Vec4& a, Vec4 b)
{
    a = a + b;
    return a;
}

static inline Vec4 operator-=(Vec4& a, Vec4 b)
{
    a = a - b;
    return a;
}

static inline Vec4 operator*=(Vec4& a, real b)
{
	a = a * b;
	return a;
}

static inline Vec4 operator/=(Vec4& a, real b)
{
	a = a / b;
	return a;
}


static inline Vec4 operator*=(Vec4& a, Vec4 b)
{
    a = a * b;
    return a;
}

/* Vec2i */

static inline Vec2i operator-(Vec2i a)
{
	return Vec2i{-a.x, -a.y};
}

static inline Vec2i operator-(Vec2i a, Vec2i b)
{
	return Vec2i{a.x - b.x, a.y - b.y};
}

static inline Vec2i operator+(Vec2i a, Vec2i b)
{
	return Vec2i{a.x + b.x, a.y + b.y};
}

static inline Vec2i operator*(Vec2i a, integer s)
{
	return Vec2i{a.x * s, a.y * s};
}

static inline Vec2i operator*(integer s, Vec2i a)
{
	return Vec2i{a.x * s, a.y * s};
}

static inline Vec2i operator/(Vec2i a, integer s)
{
	return a * (1.0f / s);
}

static inline Vec2i operator*(Vec2i a, Vec2i b)
{
	return Vec2i{a.x * b.x, a.y * b.y};
}

static inline Vec2i& operator+=(Vec2i& a, Vec2i b)
{
	a = a + b;
	return a;
}

static inline Vec2i& operator-=(Vec2i& a, Vec2i b)
{
	a = a - b;
	return a;
}

static inline Vec2i& operator*=(Vec2i& a, integer b)
{
	a = a * b;
	return a;
}

static inline Vec2i& operator*=(Vec2i& a, Vec2i b)
{
	a = a * b;
	return a;
}

static inline Vec2i& operator/=(Vec2i& a, integer b)
{
	a = a / b;
	return a;
}

/* Vec3i */

static inline Vec3i operator-(Vec3i a)
{
	return Vec3i{-a.x, -a.y, -a.z};
}

static inline Vec3i operator-(Vec3i a, Vec3i b)
{
	return Vec3i{a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline Vec3i operator+(Vec3i a, Vec3i b)
{
	return Vec3i{a.x + b.x, a.y + b.y, a.z + b.z};
}

static inline Vec3i operator*(Vec3i a, integer s)
{
	return Vec3i{a.x * s, a.y * s, a.z * s};
}

static inline Vec3i operator*(integer s, Vec3i a)
{
	return Vec3i{a.x * s, a.y * s, a.z * s};
}

static inline Vec3i operator/(Vec3i a, integer s)
{
	return a * (1.0f / s);
}

static inline Vec3i operator*(Vec3i a, Vec3i b)
{
	return Vec3i{a.x * b.x, a.y * b.y, a.z * b.z};
}

static inline Vec3i& operator+=(Vec3i& a, Vec3i b)
{
	a = a + b;
	return a;
}

static inline Vec3i& operator-=(Vec3i& a, Vec3i b)
{
	a = a - b;
	return a;
}

static inline Vec3i& operator*=(Vec3i& a, integer b)
{
	a = a * b;
	return a;
}

static inline Vec3i& operator*=(Vec3i& a, Vec3i b)
{
	a = a * b;
	return a;
}

static inline Vec3i& operator/=(Vec3i& a, integer b)
{
	a = a / b;
	return a;
}

/* Vec4i */

static inline Vec4i operator-(Vec4i a)
{
    return Vec4i{-a.x, -a.y, -a.z, -a.w};
}

static inline Vec4i operator+(Vec4i a, Vec4i b)
{
	return Vec4i{a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

static inline Vec4i operator-(Vec4i a, Vec4i b)
{
	return Vec4i{a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

static inline Vec4i operator*(Vec4i a, Vec4i b)
{
	return Vec4i{a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}

static inline Vec4i operator*(Vec4i a, integer b)
{
	return Vec4i{a.x * b, a.y * b, a.z * b, a.w * b};
}

static inline Vec4i operator*(integer b, Vec4i a) 
{
	return a * b;
}

static inline Vec4i operator/(Vec4i a, integer b)
{
	return a * (1.0f/b);
}

static inline Vec4i operator+=(Vec4i& a, Vec4i b)
{
    a = a + b;
    return a;
}

static inline Vec4i operator-=(Vec4i& a, Vec4i b)
{
    a = a - b;
    return a;
}

static inline Vec4i operator*=(Vec4i& a, integer b)
{
	a = a * b;
	return a;
}

static inline Vec4i operator/=(Vec4i& a, integer b)
{
	a = a / b;
	return a;
}

static inline Vec4i operator*=(Vec4i& a, Vec4i b)
{
    a = a * b;
    return a;
}

#endif



static inline Vec2 v2_perpendicular(Vec2 v)
{
	return Vec2 {
		-v.y, v.x
	};
}

static inline real v2_dot(Vec2 a, Vec2 b)
{
	return a.x * b.x + a.y * b.y;
}

static inline real v2_len_sq(Vec2 a)
{
	return a.x * a.x + a.y * a.y;
}

static inline real v2_len(Vec2 a)
{
    return sqrtf(v2_len_sq(a));
}

static inline real v2_cross(Vec2 a, Vec2 b)
{
	return a.x * b.y - a.y * b.x;
}

static inline Vec2 v2_from_angle(real normal)
{
	return Vec2 {
		cosf(normal), sinf(normal)
	};
}

static inline real v2_to_angle(Vec2 v)
{
	return atan2f(v.y, v.x);
}

static inline Vec2 v2_normalize(Vec2 v)
{
	real mag = sqrt(v.x*v.x + v.y * v.y);
	return v2(v.x / mag, v.y / mag);
}


static inline real v3_dot(Vec3 a, Vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline real v3_len_sq(Vec3 a)
{
	return a.x * a.x + a.y * a.y + a.z * a.z;
}

static inline real v3_len(Vec3 a)
{
	return sqrtf(v3_len_sq(a));
}


static inline Vec3 v3_cross(Vec3 a, Vec3 b)
{
    Vec3 v;
    v.x = (a.y * b.z - b.y * a.z);
    v.y = (a.x * b.z - b.x * a.z);
	v.z = (a.x * b.y - a.y * b.x);

    return v;
}

static inline Vec3 v3_normalize(Vec3 v)
{
	real mag = v3_len(v);
	return v3(v.x / mag, v.y / mag, v.z / mag);
}

static inline real v4_dot(Vec4 a, Vec4 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

static inline real v4_len_sq(Vec4 a)
{
	return a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w;
}

static inline real v4_len(Vec4 a)
{
	return sqrtf(v4_len_sq(a));
}

static inline Vec4 v4_normalize(Vec4 v)
{
	real mag = v4_len(v);
	return v4(v.x / mag, v.y / mag, v.z / mag, v.w / mag);
}

static inline Vec4 hex_to_v4(uint32 rgba)
{
#define _c(n) (uint8)((rgba >> (n)) & 0xFF)
	uint8 r = _c(24);
	uint8 g = _c(16);
	uint8 b = _c(8);
	uint8 a = _c(0);
#undef _c
	Vec4 color;
#define _f(n) ((real)(n)/255.0f)
	color.x = _f(r);
	color.y = _f(g);
	color.z = _f(b);
	color.w = _f(a);
#undef _f
	return color;
}




#ifndef REFLECTED
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

struct Rect2_Clip_Info
{
	Vec2 rp1;
	Vec2 rp2;
	Vec2 diff1;
	Vec2 diff2;
};

struct Rect2i
{
	integer x, y, w, h;
};

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
#endif

#define AABB_x1(b) (b.center.x - b.hw)
#define AABB_x2(b) (b.center.x + b.hw)
#define AABB_y1(b) (b.center.y - b.hh)
#define AABB_y2(b) (b.center.y + b.hh)


static inline bool operator==(Rect2 a, Rect2 b)
{
	return (a.x == b.x) && (a.y == b.y) && (a.w == b.w) && (a.h == b.h);
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

static inline Rect2 rect2(Vec2 topleft, Vec2 size)
{
	return Rect2{
		topleft.x, topleft.y, size.x, size.y
	};
}

static inline Rect2i rect2i(integer x, integer y, integer w, integer h)
{
	return Rect2i {
		x, y, w, h
	};
}

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

static inline bool rect_contains_point(Rect2 r, Vec2 p)
{
	return (p.x > r.x) && (p.y > r.y) &&
		   (p.x < (r.x+r.w)) && (p.y < (r.y + r.h));
}

static inline bool aabb_intersect_f(real dx, real w, real dy, real h)
{
	if(fabsf(dx) > (w)) return false;
	if(fabsf(dy) > (h)) return false;
	return true;
}

static inline bool aabb_intersect(AABB a, AABB b)
{
	if(fabsf(b.center.x - a.center.x) > (b.hw + a.hw)) return false;
	if(fabsf(b.center.y - a.center.y) > (b.hh + a.hh)) return false;
	return true;
}

static inline Vec2 aabb_overlap_v2(AABB a, AABB b)
{
	real sx = (a.hw + b.hw) - fabsf(b.center.x - a.center.x);
	real sy = (a.hh + b.hh) - fabsf(b.center.y - a.center.y);
	if(sx > sy) {
		sx = 0;
		if(a.center.y > b.center.y) {
			sy *= -1;
		}
	} else {
		sy = 0;
		if(a.center.x > b.center.x) {
			sx *= -1;
		}
	}
    return v2(sx, sy);
}

static inline void aabb_overlap(AABB a, AABB b, Vec2* s)
{
	real sx = (a.hw + b.hw) - fabsf(b.center.x - a.center.x);
	real sy = (a.hh + b.hh) - fabsf(b.center.y - a.center.y);
	if(sx > sy) {
		sx = 0;
		if(a.center.y > b.center.y) {
			sy *= -1;
		}
	} else {
		sy = 0;
		if(a.center.x > b.center.x) {
			sx *= -1;
		}
	}
    s->x = sx;
    s->y = sy;
}

static inline AABB rect_to_aabb(Rect2 r)
{
	AABB b = {0};
	b.hw = r.w / 2;
	b.hh = r.h / 2;
	b.center = v2(r.x + b.hw, r.y + b.hh);
	return b;
}

static inline Rect2 aabb_to_rect(AABB b)
{
	Rect2 r = {0};
	r.x = b.center.x - b.hw;
	r.y = b.center.y - b.hh;
	r.w = b.hw * 2;
	r.h = b.hh * 2;
	return r;
}

static inline isize modulus(isize a, isize b)
{
	if(b == 0) return -1;
	if(a == -b) return 0;
	else if(a < 0) a = b + (a % b);
	return a % b;
}

static inline AABB extents_to_aabb(Vec2 tl, Vec2 br)
{
	AABB a;
	a.hext = (br - tl) * 0.5f;
	a.center = tl + a.hext;
	return a;
}
static inline AABB extents_to_aabb(real left, real top, real right, real bottom)
{
	return extents_to_aabb(v2(left, top), v2(right, bottom));	
}
