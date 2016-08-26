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

//anchor (0 - 8)
layout (location = 6) in uint v_anchor;

out vec2 f_pixel;
out vec2 f_texcoords;
out vec4 f_color;

uniform vec4 screen;
uniform mat4 ortho;

void main()
{
	float[36] coords_arr = float[](
//	float[4] coords_center = float[](
		-0.5, -0.5,
		0.5, 0.5,
//	float[4] coords_top_left = float[](
		0.0, 0.0,
		1.0, 1.0,
//	float[4] coords_top = float[](
		-0.5, 0.0, 
		0.5, 1.0,
//	float[4] coords_top_right = float[](
		-1.0, 0.0,
		0.0, 1.0,
//	float[4] coords_right = float[](
		-1.0, -0.5,
		0.0, 0.5,
//	float[4] coords_bottom_right = float[](
		-1.0, -1.0,
		0.0, 0.0,
//	float[4] coords_bottom = float[](
		-0.5, -1.0,
		0.5, 0.0,
//	float[4] coords_bottom_left = float[](
		0.0, -1.0,
		1.0, 0.0,
//	float[4] coords_left = float[](
		0.0, -0.5,
		1.0, 0.5
	);

	uint vertex_x = uint(gl_VertexID & 2);
	uint vertex_y = uint(((gl_VertexID & 1) << 1) ^ 3);
	vertex_x += uint(4) * v_anchor;
	vertex_y += uint(4) * v_anchor;


	vec2 coords = vec2(
		coords_arr[vertex_x],
		coords_arr[vertex_y]
	);

	float[4] texcoords_arr = float[](
		v_texcoords.x, v_texcoords.y,
		v_texcoords.x + v_texcoords.z, 
		v_texcoords.y + v_texcoords.w
	);

	f_texcoords = vec2(
		texcoords_arr[gl_VertexID & 2],
		texcoords_arr[((gl_VertexID & 1) << 1) ^ 3]
	);

	//coords.x *= v_size.x * v_texcoords.z;
	//coords.y *= v_size.y * v_texcoords.w;
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
	/*
 	mat4 ortho = mat4(
		2 / (screen.z - screen.x), 0, 0, -1 * (screen.x + screen.z) / (screen.z - screen.x),
		0, 2 / (screen.y - screen.w), 0, -1 * (screen.y + screen.w) / (screen.y - screen.w),
		0, 0,          -2 / (-1 - 1), -1 * (-1 + 1) / (-1 - 1),
		0, 0, 0, 1
	);
	*/
	gl_Position = vec4(coords, 0, 1) * ortho; 
	f_pixel = gl_Position.xy;
	f_color = v_color;

}
)shader"

