#version 150

//in vec3 color_to_fragment;
//in vec2 tex_to_fragment;
in vec3 normal_to_fragment;
out vec4 out_Color;

uniform sampler2D texUnit;

void main(void)
{

	const vec3 light = vec3(0.58, 0.58, 0.58);

	vec3 shading = vec3(dot(normalize(normal_to_fragment),light));
	out_Color = vec4(shading,1.0f);
}
