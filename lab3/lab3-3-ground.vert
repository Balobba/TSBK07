#version 150

in  vec3 in_Position;

in 	vec3 in_Normal;
in vec2 inTexCoord;

//out vec3 color_to_fragment;
out vec2 tex_to_fragment;
out vec3 normal_to_fragment;

uniform mat4 projectionMatrix;

uniform mat4 mdlMatrix;
uniform mat4 lookieMat;

void main(void)
{
	gl_Position = projectionMatrix *lookieMat*mdlMatrix* vec4(in_Position,1.0);
	mat3 normalMatrix1 = mat3(mdlMatrix);
	vec3 transformedNormal = normalMatrix1*in_Normal;
	normal_to_fragment = transformedNormal;
	//const vec3 light = vec3(-0.58, -0.58, -0.58);
	//color_to_fragment = vec3(dot(transformedNormal,light));
	tex_to_fragment = inTexCoord;
}
