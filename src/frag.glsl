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
	if(avg < u_night_cutoff) {
		color.rgb *= u_night_amount;
	}

	final_color = color;

}

)shader"

