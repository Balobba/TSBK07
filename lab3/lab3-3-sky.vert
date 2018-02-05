#version 150

in  vec3 in_Position;
in vec2 inTexCoord;

out vec2 tex_to_fragment;

uniform mat4 projectionMatrix;

uniform mat4 mdlMatrix;
uniform mat4 lookieMat;

void main(void)
{
	gl_Position = projectionMatrix *mat4(mat3(lookieMat))* vec4(in_Position,1.0);
	tex_to_fragment = inTexCoord;
}
