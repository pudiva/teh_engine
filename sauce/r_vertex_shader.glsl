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

	float r, t, l, b, n, f;

	r =  1.0;
	l = -1.0;
	t =  1.0;
	b = -1.0;
	n = 1.0;
	f = 10.0;

	mat4 projection = mat4(
			(2.0*n)/(r-l),	0,		0,			0,
			0,		(2.0*n)/(t-b),	0,			0,
			(r+l)/(r-l),	(t+b)/(t-b),	-(f+n)/(f-n),		-1.0,
			0,		0,		-(2.0*f*n)/(f-n),	0
			);

	//vec4 vertex = vec4(vertex1 * (1.0-vertex_w) + vertex2 * vertex_w, 1);
	vec4 vertex = vec4(vertex1, 1);

	f_tex_coord = tex_coord;
	gl_Position = projection * model * vertex;
}
