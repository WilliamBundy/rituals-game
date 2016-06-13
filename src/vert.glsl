R"shader(
#version 330 core
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_color;
layout (location = 2) in vec2 v_texcoords;

out vec3 f_color;
out vec2 f_texcoords;

mat3 a;
float w;

void main()
{
	w = 0.5 * gl_InstanceID;
	a = mat3(
		cos(w) * 0.5, sin(w), w / 8,
		-sin(w), cos(w), w / 8,
		0, 0, 1
	);
	gl_Position = vec4(v_position * a, 1.0);
	f_color = v_color;
	f_texcoords = v_texcoords;
}

)shader"

