R"shader(
#version 330 core
in vec2 f_texcoords;
in vec4 f_color;

out vec4 color;

uniform sampler2D local_texture;


void main()
{
	color = texture(local_texture, f_texcoords) * f_color;

}

)shader"

