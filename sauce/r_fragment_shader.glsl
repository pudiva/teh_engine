uniform sampler2D texture_sampler;
varying vec2 f_tex_coord;

void main()
{
	mat2 asd = mat2(1, 0, 0, -1);

	//gl_FragColor = vec4(f_tex_coord, 0, 1);
	gl_FragColor = texture2D(texture_sampler, f_tex_coord);
	//gl_FragColor = vec4(1, 0, 0, 1);
}
