#version 150

in  vec3 in_Position;
//in 	vec3 vertex_color;
//in 	vec3 in_Normal;
in vec2 inTexCoord;

//out vec3 color_to_fragment;
out vec2 tex_to_fragment;

uniform mat4 myMatrix;
uniform mat4 projectionMatrix;
uniform mat4 rotationMatrix;
uniform mat4 rotationMatrix2;
uniform mat4 mdlMatrix;
uniform mat4 lookieMat;

void main(void)
{

	//color_to_fragment = vertex_color;

	tex_to_fragment = inTexCoord;

	//gl_Position = rotationMatrix*rotationMatrix2*myMatrix*vec4(in_Position, 1.0);
	//gl_Position = vec4(in_Position,1.0);
	//color_to_fragment = vec3(1.0f,1.0f,1.0f) - in_Normal;

	gl_Position = projectionMatrix *lookieMat*mdlMatrix* vec4(in_Position,1.0);
}
