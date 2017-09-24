uniform sampler2D texture_sampler;
varying vec2 f_tex_coord;

void main()
{
	mat2 T = mat2(
			1,	0,
			0,	-1
		     );

	gl_FragColor = texture2D(texture_sampler, T*f_tex_coord);
}
