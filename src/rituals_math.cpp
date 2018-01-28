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


inline f32 clamp(f32 x, f32 min, f32 max)
{
	if(x < min) x = min;
	else if(x > max) x = max;
	return x;
}

f32 clamp_01(f32 x)
{
	return clamp(x, 0.0f, 1.0f);	
}

f32 lerp(f32 a, f32 b, f32 t)
{
	return (1.0f - t) * a + t * b;
}



#ifndef REFLECTED
union Vec2 
{
	struct {
		f32 x, y;
	};
	f32 e[2];
};

union Vec3
{
	struct {
		f32 x, y, z;
	};
	f32 e[3];
};

union Vec4
{
	struct {
		f32 x, y, z, w;
	};
	f32 e[4];
	
};

union Vec2i
{
	struct {
		i32 x, y;
	};
	i32 e[2];
};

union Vec3i
{
	struct {
		i32 x, y, z;
	};
	i32 e[3];
};

union Vec4i
{
	struct {
		i32 x, y, z, w;
	};
	i32 e[4];
	
};

#endif

/* v* functions */

static inline Vec2 v2(f32 x, f32 y)
{
	return Vec2{x, y};
}

static inline Vec3 v3(f32 x, f32 y, f32 z)
{
	return Vec3{
		x, y, z
	};
}

static inline Vec4 v4(f32 x, f32 y, f32 z, f32 w)
{
	return Vec4{
		x, y, z, w
	};
}

static inline Vec2i v2i(i32 x, i32 y)
{
	return Vec2i {
		x, y
	};
}

static inline Vec3i v3i(i32 x, i32 y, i32 z)
{
	return Vec3i{
		x, y, z
	};
}

static inline Vec4i v4i(i32 x, i32 y, i32 z, i32 w)
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

static inline Vec3 v3(Vec2 vi, f32 z)
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

static inline Vec4 v4(Vec2 vi, f32 z, f32 w)
{
    return v4(vi.x, vi.y, z, w);
}

static inline Vec4 v4(Vec3 vi)
{
    return v4(vi.x, vi.y, vi.z, 0);
}

static inline Vec4 v4(Vec3 vi, f32 w)
{
    return v4(vi.x, vi.y, vi.z, w);
}

static inline Vec2i v2i(Vec2 vi)
{
    return v2i((i32)vi.x, (i32)vi.y);
}

static inline Vec3i v3i(Vec3 vi)
{
    return v3i((i32)vi.x, (i32)vi.y, (i32)vi.z);
}

static inline Vec4i v4i(Vec4 vi)
{
    return v4i((i32)vi.x, (i32)vi.y, (i32)vi.z, (i32)vi.w);
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

static inline Vec2 v2_add_scaled(Vec2 a, Vec2 b, f32 scale)
{
	return Vec2{a.x + b.x * scale, a.y + b.y * scale};
}


static inline Vec2 v2_mul(Vec2 a, f32 s)
{
	return Vec2{a.x * s, a.y * s};
}

static inline Vec2 v2_div(Vec2 a, f32 s)
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

static inline Vec3 v3_add_scaled(Vec3 a, Vec3 b, f32 scale)
{
	return Vec3{a.x + b.x * scale, a.y + b.y * scale, a.z + b.z * scale};
}

static inline Vec3 v3_mul(Vec3 a, f32 s)
{
	return Vec3{a.x * s, a.y * s, a.z * s};
}

static inline Vec3 v3_div(Vec3 a, f32 s)
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

static inline Vec4 v4_add_scaled(Vec4 a, Vec4 b, f32 scale)
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

static inline Vec4 v4_mul(Vec4 a, f32 b)
{
	return Vec4{a.x * b, a.y * b, a.z * b, a.w * b};
}

static inline Vec4 v4_div(Vec4 a, f32 b)
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

static inline Vec2i v2i_add_scaled(Vec2i a, Vec2i b, f32 scale)
{
	return Vec2i{a.x + (i32)(b.x * scale), a.y + (i32)(b.y * scale)};
}
static inline Vec2i v2i_addi_scaled(Vec2i a, Vec2i b, i32 scale)
{
	return Vec2i{a.x + (i32)(b.x * scale), a.y + (i32)(b.y * scale)};
}

static inline Vec2i v2i_mul(Vec2i a, f32 s)
{
	return Vec2i{(i32)(a.x * s), (i32)(a.y * s)};
}

static inline Vec2i v2i_muli(Vec2i a, i32 s)
{
	return Vec2i{(i32)(a.x * s), (i32)(a.y * s)};
}

static inline Vec2i v2i_div(Vec2i a, f32 s)
{
	return v2i_mul(a, 1.0f / s);
}

static inline Vec2i v2i_divi(Vec2i a, i32 s)
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

static inline Vec3i v3i_add_scaled(Vec3i a, Vec3i b, f32 scale)
{
	return Vec3i{a.x + (i32)(b.x * scale), a.y + (i32)(b.y * scale), a.z + (i32)(b.z * scale)};
}

static inline Vec3i v3i_addi_scaled(Vec3i a, Vec3i b, i32 scale)
{
	return Vec3i{a.x + (i32)(b.x * scale), a.y + (i32)(b.y * scale), a.z + (i32)(b.z * scale)};
}

static inline Vec3i v3i_mul(Vec3i a, f32 s)
{
	return Vec3i{(i32)(a.x * s), (i32)(a.y * s), (i32)(a.z * s)};
}

static inline Vec3i v3i_muli(Vec3i a, i32 s)
{
	return Vec3i{a.x * s, a.y * s, a.z * s};
}

static inline Vec3i v3i_div(Vec3i a, f32 s)
{
	return v3i_mul(a, 1.0f/s);
}

static inline Vec3i v3i_divi(Vec3i a, i32 s)
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

static inline Vec4i v4i_add_scaled(Vec4i a, Vec4i b, f32 scale)
{
	return Vec4i{a.x + (i32)(b.x * scale), a.y + (i32)(b.y * scale), a.z + (i32)(b.z * scale), a.w + (i32)(b.w * scale)};
}

static inline Vec4i v4i_addi_scaled(Vec4i a, Vec4i b, i32 scale)
{
	return Vec4i{a.x + (i32)(b.x * scale), a.y + (i32)(b.y * scale), a.z + (i32)(b.z * scale), a.w + (i32)(b.w * scale)};
}

static inline Vec4i v4i_sub(Vec4i a, Vec4i b)
{
	return Vec4i{a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

static inline Vec4i v4i_compenent_mul(Vec4i a, Vec4i b)
{
	return Vec4i{a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}

static inline Vec4i v4i_mul(Vec4i a, f32 b)
{
	return Vec4i{(i32)(a.x * b), (i32)(a.y * b), (i32)(a.z * b), (i32)(a.w * b)};
}
static inline Vec4i v4i_muli(Vec4i a, i32 b)
{
	return Vec4i{(i32)(a.x * b), (i32)(a.y * b), (i32)(a.z * b), (i32)(a.w * b)};
}

static inline Vec4i v4i_divi(Vec4i a, f32 b)
{
	return Vec4i{(i32)(a.x / b), (i32)(a.y / b), (i32)(a.z / b), (i32)(a.w / b)};
}

static inline Vec4i v4i_div(Vec4i a, f32 b)
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

static inline Vec2 operator*(Vec2 a, f32 s)
{
	return Vec2{a.x * s, a.y * s};
}

static inline Vec2 operator*(f32 s, Vec2 a)
{
	return Vec2{a.x * s, a.y * s};
}

static inline Vec2 operator/(Vec2 a, f32 s)
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

static inline Vec2& operator*=(Vec2& a, f32 b)
{
	a = a * b;
	return a;
}

static inline Vec2& operator*=(Vec2& a, Vec2 b)
{
	a = a * b;
	return a;
}

static inline Vec2& operator/=(Vec2& a, f32 b)
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

static inline Vec3 operator*(Vec3 a, f32 s)
{
	return Vec3{a.x * s, a.y * s, a.z * s};
}

static inline Vec3 operator*(f32 s, Vec3 a)
{
	return Vec3{a.x * s, a.y * s, a.z * s};
}

static inline Vec3 operator/(Vec3 a, f32 s)
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

static inline Vec3& operator*=(Vec3& a, f32 b)
{
	a = a * b;
	return a;
}

static inline Vec3& operator*=(Vec3& a, Vec3 b)
{
	a = a * b;
	return a;
}

static inline Vec3& operator/=(Vec3& a, f32 b)
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

static inline Vec4 operator*(Vec4 a, f32 b)
{
	return Vec4{a.x * b, a.y * b, a.z * b, a.w * b};
}
static inline Vec4 operator*(f32 b, Vec4 a) 
{
	return a * b;
}
static inline Vec4 operator/(Vec4 a, f32 b)
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

static inline Vec4 operator*=(Vec4& a, f32 b)
{
	a = a * b;
	return a;
}

static inline Vec4 operator/=(Vec4& a, f32 b)
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

static inline Vec2i operator*(Vec2i a, i32 s)
{
	return Vec2i{a.x * s, a.y * s};
}

static inline Vec2i operator*(i32 s, Vec2i a)
{
	return Vec2i{a.x * s, a.y * s};
}

static inline Vec2i operator/(Vec2i a, i32 s)
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

static inline Vec2i& operator*=(Vec2i& a, i32 b)
{
	a = a * b;
	return a;
}

static inline Vec2i& operator*=(Vec2i& a, Vec2i b)
{
	a = a * b;
	return a;
}

static inline Vec2i& operator/=(Vec2i& a, i32 b)
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

static inline Vec3i operator*(Vec3i a, i32 s)
{
	return Vec3i{a.x * s, a.y * s, a.z * s};
}

static inline Vec3i operator*(i32 s, Vec3i a)
{
	return Vec3i{a.x * s, a.y * s, a.z * s};
}

static inline Vec3i operator/(Vec3i a, i32 s)
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

static inline Vec3i& operator*=(Vec3i& a, i32 b)
{
	a = a * b;
	return a;
}

static inline Vec3i& operator*=(Vec3i& a, Vec3i b)
{
	a = a * b;
	return a;
}

static inline Vec3i& operator/=(Vec3i& a, i32 b)
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

static inline Vec4i operator*(Vec4i a, i32 b)
{
	return Vec4i{a.x * b, a.y * b, a.z * b, a.w * b};
}

static inline Vec4i operator*(i32 b, Vec4i a) 
{
	return a * b;
}

static inline Vec4i operator/(Vec4i a, i32 b)
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

static inline Vec4i operator*=(Vec4i& a, i32 b)
{
	a = a * b;
	return a;
}

static inline Vec4i operator/=(Vec4i& a, i32 b)
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

static inline f32 v2_dot(Vec2 a, Vec2 b)
{
	return a.x * b.x + a.y * b.y;
}

static inline f32 v2_len_sq(Vec2 a)
{
	return a.x * a.x + a.y * a.y;
}

static inline f32 v2_len(Vec2 a)
{
    return sqrtf(v2_len_sq(a));
}

static inline f32 v2_cross(Vec2 a, Vec2 b)
{
	return a.x * b.y - a.y * b.x;
}

static inline Vec2 v2_from_angle(f32 normal)
{
	return Vec2 {
		cosf(normal), sinf(normal)
	};
}

static inline f32 v2_to_angle(Vec2 v)
{
	return atan2f(v.y, v.x);
}

static inline Vec2 v2_normalize(Vec2 v)
{
	f32 mag = sqrt(v.x*v.x + v.y * v.y);
	return v2(v.x / mag, v.y / mag);
}


static inline f32 v3_dot(Vec3 a, Vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline f32 v3_len_sq(Vec3 a)
{
	return a.x * a.x + a.y * a.y + a.z * a.z;
}

static inline f32 v3_len(Vec3 a)
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
	f32 mag = v3_len(v);
	return v3(v.x / mag, v.y / mag, v.z / mag);
}

static inline f32 v4_dot(Vec4 a, Vec4 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

static inline f32 v4_len_sq(Vec4 a)
{
	return a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w;
}

static inline f32 v4_len(Vec4 a)
{
	return sqrtf(v4_len_sq(a));
}

static inline Vec4 v4_normalize(Vec4 v)
{
	f32 mag = v4_len(v);
	return v4(v.x / mag, v.y / mag, v.z / mag, v.w / mag);
}

static inline Vec4 hex_to_v4(u32 rgba)
{
#define _c(n) (u8)((rgba >> (n)) & 0xFF)
	u8 r = _c(24);
	u8 g = _c(16);
	u8 b = _c(8);
	u8 a = _c(0);
#undef _c
	Vec4 color;
#define _f(n) ((f32)(n)/255.0f)
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
		f32 x, y, w, h;
	};
	struct {
		Vec2 position, size;
	};
	f32 e[4];
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
	i16 x, y, w, h;
};

union AABB
{
	struct {
		Vec2 center;
		union {
			struct {
				f32 hw, hh;
			};
			struct {
				Vec2 hext;
			};
		};
	};
	f32 e[4];
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

static inline AABB aabb(Vec2 c, f32 hw, f32 hh)
{
	return AABB{
		c, hw, hh
	};
}

static inline AABB aabb(f32 x, f32 y, f32 hw, f32 hh)
{
	return AABB{
		v2(x, y), hw, hh
	};
}

static inline Rect2 rect2(f32 x, f32 y, f32 w, f32 h)
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

static inline Rect2i rect2i(i16 x, i16 y, i16 w, i16 h)
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

static inline bool aabb_intersect_f(f32 dx, f32 w, f32 dy, f32 h)
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
	f32 sx = (a.hw + b.hw) - fabsf(b.center.x - a.center.x);
	f32 sy = (a.hh + b.hh) - fabsf(b.center.y - a.center.y);
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
	f32 sx = (a.hw + b.hw) - fabsf(b.center.x - a.center.x);
	f32 sy = (a.hh + b.hh) - fabsf(b.center.y - a.center.y);
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
static inline AABB extents_to_aabb(f32 left, f32 top, f32 right, f32 bottom)
{
	return extents_to_aabb(v2(left, top), v2(right, bottom));	
}


#define ColorWhite (0xFFFFFFFF)
#define ColorMediumGrey (0xAAAAAAFF)
#define ColorBlack (0x000000FF)

