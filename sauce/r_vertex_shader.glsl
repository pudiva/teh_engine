attribute vec3 vertex1;
attribute vec3 vertex2;
attribute vec2 tex_coord;

varying vec2 f_tex_coord;

uniform float vertex_w;

void main()
{
	mat4 model = mat4(
			1,	0,	0,	0,
			0,	0,	-1,	0,
			0,	1,	0,	0,
			0,	0,	-5,	1
		       );

	mat4 projection = mat4(
			1,	0,	0,	0,
			0,	1,	0,	0,
			0,	0,	.1,	-1,
			0,	0,	0,	1
			);

	//vec4 vertex = vec4(vertex1 * (1.0-vertex_w) + vertex2 * vertex_w, 1);
	vec4 vertex = vec4(vertex1, 1);

	gl_Position = projection * model * vertex;
	f_tex_coord = tex_coord;
}
