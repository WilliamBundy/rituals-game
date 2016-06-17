R"shader(
#version 330 core
layout (location = 0) in vec2 v_translate;
layout (location = 1) in vec2 v_center;
layout (location = 2) in float v_angle;
layout (location = 3) in vec2 v_scale;
layout (location = 4) in vec4 v_texcoords;
layout (location = 5) in vec4 v_color;

out vec2 f_texcoords;

uniform vec4 screen;

void main()
{
	float[4] coords_arr = {
		-0.5, -10.5,
		0.5, 0.5
	};
	vec2 coords = vec2(
		coords_arr[gl_VertexID & 2],
		coords_arr[((gl_VertexID & 1) << 1) ^ 3]
	);


	/*	
	//1: apply scale to position
	// p' = p + (p - center) * scale
	//vec2 p = vec2(
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
	);


	vec2 p2 = tr * p;
	p2 += translation;
	gl_Position = vec4(p2, 0, 1) * ortho;
	f_texcoords = v_texcoords;
*/
	f_texcoords = v_texcoords.xy;
}

)shader"

