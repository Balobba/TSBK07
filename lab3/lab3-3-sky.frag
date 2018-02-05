#version 150

in vec2 tex_to_fragment;

out vec4 out_Color;

uniform sampler2D texUnit;

void main(void)
{
	vec4 tex = texture(texUnit, tex_to_fragment);

	out_Color = tex;
}
