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

//flags first 4 bits: anchor, 1<<4+ flags
layout (location = 6) in uint v_flags;

out vec2 f_pixel;
out vec2 f_texcoords;
out vec4 f_color;

uniform mat4 u_orthomat;

void main()
{
	float[36] coords_arr = float[](
		-0.5, -0.5,
		0.5, 0.5,
		0.0, 0.0,
		1.0, 1.0,
		-0.5, 0.0, 
		0.5, 1.0,
		-1.0, 0.0,
		0.0, 1.0,
		-1.0, -0.5,
		0.0, 0.5,
		-1.0, -1.0,
		0.0, 0.0,
		-0.5, -1.0,
		0.5, 0.0,
		0.0, -1.0,
		1.0, 0.0,
		0.0, -0.5,
		1.0, 0.5
	);

	uint vertex_x = uint(gl_VertexID & 2);
	uint vertex_y = uint(((gl_VertexID & 1) << 1) ^ 3);
	uint v_anchor = v_flags & 0xF;
	vertex_x += uint(4) * v_anchor;
	vertex_y += uint(4) * v_anchor;

	uint v_fliphoriz = v_flags & (1<<4);
	uint v_flipvert = v_flags & (1<<5);


	vec2 coords = vec2(
		coords_arr[vertex_x],
		coords_arr[vertex_y]
	);

	vec4 tex_rect = vec4(
		v_texcoords.x, v_texcoords.y,
		v_texcoords.x + v_texcoords.z, 
		v_texcoords.y + v_texcoords.w
	);

	if(v_fliphoriz == uint(1)) {
		tex_rect = tex_rect.zyxw;
	}
	if(v_flipvert == uint(1)) {
		tex_rect = tex_rect.xwzy;
	}


	tex_rect.x += v_fliphoriz;
	tex_rect.y += v_flipvert;

	float[4] texcoords_arr = float[](
			tex_rect.x, tex_rect.y,
			tex_rect.z, tex_rect.w
	);

	f_texcoords = vec2(
		texcoords_arr[gl_VertexID & 2],
		texcoords_arr[((gl_VertexID & 1) << 1) ^ 3]
	);
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
	gl_Position = vec4(coords, 0, 1) * u_orthomat;
	f_pixel = gl_Position.xy;
	f_color = v_color;

}
)shader"

