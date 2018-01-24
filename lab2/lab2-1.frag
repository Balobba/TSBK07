#version 150

//in vec3 color_to_fragment;
in vec2 tex_to_fragment;
out vec4 out_Color;

void main(void)
{
	//out_Color = vec4(1.0f);
	//out_Color = vec4(color_to_fragment,1.0f);

	//	out_Color = vec4(color_to_fragment,1.0f) +vec4(tex_to_fragment, 1.0f, 1.0f);
	//out_Color = vec4(tex_to_fragment, 1.0f, 1.0f);
	//from the book
	//float a = tan(tex_to_fragment.s*30)/2+0.5;
	//float b = tan(tex_to_fragment.t*30)/2+0.5;
	//cool evolution
	float a = mod(tex_to_fragment.s,tex_to_fragment.t);
	float b = mod(tex_to_fragment.t,tex_to_fragment.s);
	//cool evolution with some tan
	//float a = mod(tex_to_fragment.s,tan(tex_to_fragment.s*30)/2+0.5);
	//float b = mod(tex_to_fragment.t,tex_to_fragment.s);

	out_Color = vec4(a, b, 1.0, 1.0);

}
