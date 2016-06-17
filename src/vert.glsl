R"shader(
#version 330 core
//Offset from origin in pixels (world space)
layout (location = 0) in vec2 v_translate;

//Center of sprite for rotation (model space)
layout (location = 1) in vec2 v_center;

//Angle to render at
layout (location = 2) in float v_angle;

//Amount to scale sprite by
layout (location = 3) in vec2 v_scale;

//x, y, w, h of texture rectangle in 0->1 form
layout (location = 4) in vec4 v_texcoords;

//rgba color, sent to frag shader
layout (location = 5) in vec4 v_color;

out vec2 f_texcoords;
out vec4 f_color;

uniform vec4 screen;
uniform vec2 texture_size;

void main()
{
	float[4] coords_arr = {
		-0.5, -0.5,
		0.5, 0.5
	};
	vec2 coords = vec2(
		coords_arr[gl_VertexID & 2],
		coords_arr[((gl_VertexID & 1) << 1) ^ 3]
	);

	float[4] texcoords_arr = {
		v_texcoords.x, v_texcoords.y, v_texcoords.z, v_texcoords.w
	};

	f_texcoords = vec2(
		texcoords_arr[gl_VertexID & 2],
		texcoords_arr[((gl_VertexID & 1) << 1) ^ 3]
	);

	//coords.x *= v_scale.x * v_texcoords.z;
	//coords.y *= v_scale.y * v_texcoords.w;
	coords -= v_center;
	coords.x *= v_scale.x;
	coords.y *= v_scale.y;
	vec2 rot = vec2(cos(v_angle), sin(v_angle));
	mat2 rotmat = mat2 (
		rot.x, rot.y,
		-rot.y, rot.x
	);
	coords *= rotmat;
	coords += v_translate + v_center;
	mat4 ortho = mat4(
		2 / (screen.z - screen.x), 0, 0, (screen.x + screen.z) / (screen.z - screen.x),
		0, 2 / (screen.y - screen.w), 0, (screen.y + screen.w) / (screen.y - screen.w),
		0, 0, -2 / (-1000 - 1),          (-1000 + 1) / (-1000 - 1),
		0, 0, 0, 1
	);
	gl_Position = ortho * vec4(coords, 0, 1); 
	f_color = v_color;

}
)shader"

