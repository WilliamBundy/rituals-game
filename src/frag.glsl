R"shader(
#version 330 core
in vec2 f_texcoords;

out vec4 color;

uniform vec4 local_color;
uniform sampler2D local_texture;


void main()
{
	//color = texture(local_texture, f_texcoords) * local_color;
	color = vec4(1.0, 1.0, 1.0, 1.0);
}

)shader"

