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

struct Sprite_s
{
	real x, y, cx, cy, tx, ty, tw, th, a, sx, sy, c;
	uint32 color, hash, texture, userdata;
};



struct Renderer
{
	GLuint shader_program, vbo, vao, texture ;
	isize translation_loc, rotation_loc, color_loc, screen_loc, scale_loc, center_loc;
	Vec4 ortho;

	real* sprite_data;

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


void renderer_init(Renderer* renderer, Game* game)
{
	renderer->last_sprite_id = 0;
	glGenVertexArrays(1, &renderer->vao);
	glBindVertexArray(renderer->vao);

	glGenBuffers(1, &renderer->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(real), vertices, GL_STATIC_DRAW);

	/* /Vertex position
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(real) * 4, (GLvoid*)0);
	glEnableVertexAttribArray(0);  

	//Texture coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	*/

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

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	renderer->color_loc = glGetUniformLocation(renderer->shader_program, "local_color");
	renderer->translation_loc = glGetUniformLocation(
			renderer->shader_program, "translation");
	renderer->rotation_loc = glGetUniformLocation(
			renderer->shader_program, "angle");
	renderer->screen_loc = glGetUniformLocation(renderer->shader_program, "screen");
	renderer->scale_loc = glGetUniformLocation(renderer->shader_program, "scale");
	renderer->center_loc = glGetUniformLocation(renderer->shader_program, "center");
	printf("Adding stuff: %d \n", glGetError());
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
	printf("Adding texture: %d \n", glGetError());
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
	if(texture == NULL) {
		printf("There was an error loading %s \n", filename);
	}
	STBI_FREE(data);
	return texture;
}

bool b = 0;
#define _glerror if(!b) {printf("%d: %d \n", t++, glGetError());}
void renderer_start(Renderer* renderer, Game* game)
{
	int32 t = 0;
	
	glUseProgram(renderer->shader_program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderer->texture);
	glBindVertexArray(renderer->vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);
	//glUniform4f(renderer->screen_loc, renderer->ortho.x,
	//		renderer->ortho.y, renderer->ortho.z, renderer->ortho.w);
	glUniform4f(renderer->screen_loc, 0, 0, game->width, game->height);
	glVertexAttribDivisor(
}

void renderer_push_sprite(Renderer* renderer, Sprite* s)
{

	//set all the uniforms
	int32 t = 0;
	
	glUniform2f(renderer->translation_loc, s->tr.position.x, s->tr.position.y);
	glUniform2f(renderer->center_loc, s->tr.center.x, s->tr.center.y);
	glUniform2f(renderer->scale_loc, s->tr.scale_x, s->tr.scale_y);
	glUniform4f(renderer->color_loc, 1.0f,1.0f,1.0f,1.0f);
	glUniform1f(renderer->rotation_loc, s->tr.angle);

	//glDrawArrays(GL_TRIANGLES, 0, 3) ;
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);
	
}

void renderer_draw(Renderer* renderer)
{
}

