/*
 *
 *
 */


struct Sprite
{
	usize id;

	isize texture_id;
	Rect2 texture_region;
	Transform tr;
};


struct Renderer
{
	GLuint shader_program, vbo, vao, texture, ebo;
	isize translation_loc, rotation_loc, color_loc;

	real vertices[16];
	GLuint indices[6];

	isize last_sprite_id; 
};


void sprite_init(Sprite* s, Renderer* renderer)
{
	s->id = renderer->last_sprite_id++;
	s->texture_id = renderer->texture;
	s->texture_region = Rect2{
		0, 0, 1.0f, 1.0f
	};
	s->tr.position = v2(0, 0);
	s->tr.angle = 0;
	s->tr.center = v2(0, 0);
	s->tr.scale_x = 1.0f;
	s->tr.scale_y = 1.0f;
}

void renderer_init(Renderer* renderer)
{
	renderer->last_sprite_id = 0;
	real vertices[] = {
		 0.5f,  0.5f,   1.0f, 1.0f, // _tcx(32, 512),  _tcy(0, 512),  // Top Right
		 0.5f, -0.5f,   1.0f, 0.0f, // _tcx(32, 512),  _tcy(32, 512),  // Bottom Right
		-0.5f, -0.5f,   0.0f, 0.0f, // _tcx(0, 512),  _tcy(32, 512),  // Bottom Left
		-0.5f,  0.5f,   0.0f, 1.0f, // _tcx(0 , 512),  _tcy(0, 512)   // Top Left 
	};  
	real indices[] = {
		0, 1, 3, 
		1, 2, 3
	};

	glGenVertexArrays(1, &renderer->vao);
	glBindVertexArray(renderer->vao);

	glGenBuffers(1, &renderer->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
	glBufferData(GL_ARRAY_BUFFER, isz(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &renderer->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(renderer->indices), renderer->indices, GL_STATIC_DRAW);

	//Texture coords
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);  

	//Texture coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertex_shader_src = 
#include "vert.glsl"
		;
	glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
	glCompileShader(vertex_shader);

	{
		GLint success;
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
		GLsizei log_size;
		char shader_log[4096];
		glGetShaderInfoLog(vertex_shader, 4096, &log_size, shader_log); 

		if(!success) {
			printf("Error compiling vertex shader \n%s \n", shader_log);
		} else {
			printf("Vertex shader compiled successfully\n");
		}
	}	
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* fragment_shader_src = 
#include "frag.glsl"
		;
	glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
	glCompileShader(fragment_shader);
	{
		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
		if(!success) {
			printf("Error compiling frag shader %.*s \n", 512, infoLog);
		} else {
			printf("Frag shader compiled successfully\n");
		}
	}	

	renderer->shader_program = glCreateProgram();
	glAttachShader(renderer->shader_program, vertex_shader);
	glAttachShader(renderer->shader_program, fragment_shader);
	glLinkProgram(renderer->shader_program);

	glUseProgram(renderer->shader_program);

	renderer->color_loc = glGetUniformLocation(renderer->shader_program, "local_color");
	renderer->translation_loc = glGetUniformLocation(
			renderer->shader_program, "translation");
	renderer->rotation_loc = glGetUniformLocation(
			renderer->shader_program, "rotation");

}

GLuint ogl_add_texture(uint8* data, isize w, isize h) 
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//TODO(will) do error checking?
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}
	

GLuint ogl_load_texture(char* filename)
{
	int w, h, n;
	char file[File_Path_Max_Length];
	const char* base_path = SDL_GetBasePath();
	isize len = snprintf(file, File_Path_Max_Length, "%s%s", base_path, filename);
	uint8* data = (uint8*)stbi_load(file, &w, &h, &n, STBI_rgb_alpha);
	//TODO(will) do error checking
	GLuint texture = ogl_add_texture(data, w, h);


	STBI_FREE(data);
	return texture;
}

bool b = 0;
#define _glerror if(!b) {printf("%d: %d \n", t++, glGetError());}
void renderer_setup(Renderer* renderer)
{
	int32 t = 0;
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderer->texture);
	glBindVertexArray(renderer->vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);
}

void renderer_draw_sprite(Renderer* renderer, Sprite* s)
{

	//set all the uniforms
	int32 t = 0;
	glUniform2f(renderer->translation_loc, s->tr.position.x, s->tr.position.y);
	glUniform4f(renderer->color_loc, 1.0f,1.0f,1.0f,1.0f);
	glUniform1f(renderer->rotation_loc, s->tr.angle);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
}
