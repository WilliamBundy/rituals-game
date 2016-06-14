R"shader(
#version 330 core
layout (location = 0) in vec2 v_position;
layout (location = 1) in vec2 v_texcoords;

out vec2 f_texcoords;

//uniform mat3 local_modelmatrix;
//uniform mat3 local_cameramatrix;

void main()
{
	gl_Position = vec4(v_position, -1.0, 1.0);
	f_texcoords = v_texcoords;
}

)shader"

