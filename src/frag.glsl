/* 
Copyright (c) 2016 William Bundy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

R"shader(
#version 330 core
in vec2 f_pixel;
in vec2 f_texcoords;
in vec4 f_color;

uniform vec2 texture_size;
// window size and scale, x=width, y=height, z=scale
uniform vec3 window;

uniform vec4 screen;
uniform sampler2D local_texture;

void main()
{
	//vec2 uv = floor(f_texcoords) + 0.5;

	vec2 pixel = f_pixel;
	vec2 uv = f_texcoords;
	#if 0
	uv.x *= texture_size.x;
	uv.y *= texture_size.y;
	//uv = floor(uv) + 0.5;
	uv += 1.0 - clamp((1.0 - fract(pixel)) * window.z, 0.0, 1.0);
	uv.x /= texture_size.x;
	uv.y /= texture_size.y;
	#endif
	gl_FragColor = texture(local_texture, uv) * vec4(f_color.rgb, 1) * f_color.aaaa;
}

)shader"

