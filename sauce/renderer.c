#include <assert.h>
#include <stdio.h>
#include "renderer.h"

GLuint r_vertex_shader_id, r_fragment_shader_id, r_program_id;
GLint r_pos_loc, r_color_loc, r_vertex_w_loc, r_texture_sampler_loc;

static GLuint r_shader_init(GLenum type, const GLchar* sauce, GLint len)
{
	GLuint shader_id;
	GLint compiled;

	shader_id = glCreateShader(type);

	if (shader_id == 0)
		return 0;

	glShaderSource(shader_id, 1, (const GLchar**) &sauce, (GLint*) &len); 
	glCompileShader(shader_id);
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		GLint len = 0;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 1)
		{
			char log[len];
			glGetShaderInfoLog(shader_id, len, NULL, log);
			fprintf(stderr, "%s:%d: r_shader_init(): \n%s\n",
					__FILE__, __LINE__, log);
		}

		glDeleteShader(shader_id);
		shader_id = 0;
	}

	return shader_id;
}

static GLuint r_program_init(unsigned c, GLuint* v)
{
	unsigned i;
	GLuint prg_id;
	GLint linked;

	prg_id = glCreateProgram();

	if (prg_id == 0)
		return 0;

	for (i = 0; i < c; ++i)
		glAttachShader(prg_id, v[i]);

	glBindAttribLocation(prg_id, 0, "vertex1");
	glBindAttribLocation(prg_id, 1, "vertex2");
	glBindAttribLocation(prg_id, 2, "tex_coord");

	glLinkProgram(prg_id);
	glGetProgramiv(prg_id, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		GLint len = 0;
		glGetProgramiv(prg_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 1)
		{
			char log[len];
			glGetProgramInfoLog(prg_id, len, NULL, log);
			fprintf(stderr, "%s:%d: r_program_init(): \n%s\n",
					__FILE__, __LINE__, log);
		}

		glDeleteProgram(prg_id);
		prg_id = 0;
	}

	return prg_id;
}

static void r_vertex_shader_init()
{
	r_vertex_shader_id = r_shader_init(
			GL_VERTEX_SHADER,
			(const GLchar*) r_vertex_shader_glsl,
			r_vertex_shader_glsl_len
			);
}

static void r_fragment_shader_init()
{
	r_fragment_shader_id = r_shader_init(
			GL_FRAGMENT_SHADER,
			(const GLchar*) r_fragment_shader_glsl,
			r_fragment_shader_glsl_len
			);
}

void renderer_init()
{
	GLuint v[2];

	r_vertex_shader_init();
	r_fragment_shader_init();

	v[0] = r_vertex_shader_id;
	v[1] = r_fragment_shader_id;

	r_program_id = r_program_init(2, v);

	glUseProgram(r_program_id);
	r_texture_sampler_loc = glGetUniformLocation(r_program_id, "texture_sampler");
	r_vertex_w_loc = glGetUniformLocation(r_program_id, "vertex_w");
	r_pos_loc = glGetUniformLocation(r_program_id, "pos");
	r_color_loc = glGetUniformLocation(r_program_id, "color");

	glEnable(GL_TEXTURE_2D);
}

void renderer_fini()
{
	glDeleteProgram(r_program_id);
	glDeleteShader(r_fragment_shader_id);
	glDeleteShader(r_vertex_shader_id);
}
