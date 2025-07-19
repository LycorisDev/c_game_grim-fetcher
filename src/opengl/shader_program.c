#include "grim_fetcher.h"

static GLuint compile_shader(GLenum type, const char *text);
static void   free_shader(GLuint *id);

const char	*g_vertex_shader =
"#version 400 core\n"
"layout(location = 0) in vec2 in_pos;\n"
"layout(location = 1) in vec2 in_tex_coord;\n"
"out vec2 frag_tex_coord;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(in_pos, 0.0, 1.0);\n"
"	frag_tex_coord = in_tex_coord;\n"
"}\n";

const char	*g_fragment_shader =
"#version 400 core\n"
"in vec2 frag_tex_coord;\n"
"uniform sampler2D frame_texture;\n"
"out vec4 frag_color;\n"
"void main()\n"
"{\n"
"	vec3 colors = texture(frame_texture, frag_tex_coord).rgb;\n"
"	frag_color = vec4(colors, 1.0);\n"
"}\n";

int create_shader_program(void)
{
	GLuint vs;
	GLuint fs;
	int    shader_program;

	vs = compile_shader(GL_VERTEX_SHADER, g_vertex_shader);
	fs = compile_shader(GL_FRAGMENT_SHADER, g_fragment_shader);
	if (!vs || !fs)
	{
		free_shader(&vs);
		free_shader(&fs);
		return 0;
	}
	shader_program = glCreateProgram();
	if (!shader_program)
	{
		fprintf(stderr, "Error: Couldn't compile shader program\n");
		free_shader(&vs);
		free_shader(&fs);
		return 0;
	}
	glAttachShader(shader_program, vs);
	glAttachShader(shader_program, fs);
	glLinkProgram(shader_program);
	glUseProgram(shader_program);
	free_shader(&vs);
	free_shader(&fs);
	return shader_program;
}

void free_shader_program(void)
{
	if (man.shader_program)
	{
		glUseProgram(0);
		glDeleteProgram(man.shader_program);
	}
	man.shader_program = 0;
	return;
}

static GLuint compile_shader(GLenum type, const char *text)
{
	GLuint id_shader;

	if ((type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER) || !text)
		return 0;
	id_shader = glCreateShader(type);
	glShaderSource(id_shader, 1, (const GLchar **)&text, 0);
	glCompileShader(id_shader);
	return id_shader;
}

static void free_shader(GLuint *id)
{
	glDeleteShader(*id);
	*id = 0;
	return;
}
