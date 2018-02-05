#version 150

//in vec3 color_to_fragment;
//in vec2 tex_to_fragment;
in vec3 normal_to_fragment;
out vec4 out_Color;

//uniform sampler2D texUnit;
uniform mat4 lookieMat;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade = dot(normalize(normal_to_fragment),light);
	shade = clamp(shade,0,1);
	vec3 shading = vec3(shade);
	out_Color = vec4(shading,1.0f);
}
