R"shader(
#version 330 core
in vec2 f_texcoords;
in vec4 f_color;


uniform sampler2D local_texture;


void main()
{
	gl_FragColor = texture(local_texture, f_texcoords) * f_color * f_color.aaaa;

}

)shader"

