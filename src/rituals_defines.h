/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float real32;
typedef double real64;

typedef float f32;
typedef double f64;

typedef __m128 vf128;

union vf32x4
{
	vf128 v;
	f32 f[4];
};

#define ShuffleToByte(x, y, z, w) (((x)<<6) | ((y)<<4) | ((z)<<2) | w)
#define vfShuffle(a, x, y, z, w) _mm_shuffle_ps(a, a, ShuffleToByte(x, y, z, w))

typedef ptrdiff_t isize;
typedef size_t usize;


#define Math_Sqrt2 1.414213f
#define Math_InvSqrt2 0.7071067f
#define Math_Pi 3.141592f
#define Math_Tau 6.283185f
#define Math_RadToDeg 57.29577f
#define Math_DegToRad 0.01745329f

#define Allocate(type, count) ((type*)calloc(sizeof(type), count))
#define StackAlloc(type, count) ((type*)_alloca(sizeof(type) * count))

#define isizeof(t) ((isize)sizeof(t))
#define isz(t) (isizeof(t))

#define MixerNumberOfChannels 64

#define FilePathMaxLength 4096

#define PathSeparator ("\\")
#define PathSeparatorChar ('\\')
#define Backspace ('\b')
#define BackspaceStr ("\b")

#define Min(x,y) ((x<y)?x:y)
#define Max(x,y) ((x>y)?x:y)

#define Flag(x) (1 << x)
#define Has_Flag(x, y) (x & y)
#define Enable_Flag(x, y) (x |= y)
#define Disable_Flag(x, y) (x &= ~y)

#define Kilobytes(b) (b * UINT64_C(1024))
#define Megabytes(b) (Kilobytes(b) * UINT64_C(1024))
#define Gigabytes(b) (Megabytes(b) * UINT64_C(1024))

#define _check(s1, s2, state) ((Input->scancodes[SDL_SCANCODE_##s1] == state) || (Input->scancodes[SDL_SCANCODE_##s2] == state))
#define _scancode(s1) ((Input->scancodes[SDL_SCANCODE_##s1]))

