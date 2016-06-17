R"shader(
#version 330 core
layout (location = 0) in vec2 v_position;
layout (location = 1) in vec2 v_texcoords;

out vec2 f_texcoords;

uniform vec2 center;
uniform vec2 scale;
uniform vec2 translation;
uniform float angle;
uniform vec4 screen;
//uniform mat3 local_cameramatrix;

void main()
{
	//1: apply scale to position
	// p' = p + (p - center) * scale
	vec2 p = vec2(
		v_position.x + (v_position.x - center.x) * scale.x,
		v_position.y + (v_position.y - center.y) * scale.y);
	mat2 tr = mat2(
		 cos(angle), sin(angle), 
		-sin(angle), cos(angle)
	);
	//screen.x = l
	//screen.y = t
	//screen.z = r
	//screen.w = b
	//far = -1000
	//near = 1
	mat4 ortho = mat4(
		2 / (screen.z - screen.x), 0, 0, -1 * (screen.x + screen.z) / (screen.z - screen.x),
		0, 2 / (screen.y - screen.w), 0, -1 * (screen.y + screen.w) / (screen.y - screen.w),
		0, 0, -2 / (-1000 - 1) 	   , -1 * (-1000 + 1) / (-1000 - 1),
		0, 0, 0, 1
	);
	/*mat4 ortho = mat4(
		screen.x, 0, 0, screen.y,
		0, screen.z, 0, screen.w,
		0, 0, 0.001998001998001998, -0.998001998001998,
		0, 0, 0, 1
	);*/


	vec2 p2 = tr * p;
	p2 += translation;
	gl_Position = vec4(p2, 0, 1) * ortho;
	f_texcoords = v_texcoords;
}

)shader"

