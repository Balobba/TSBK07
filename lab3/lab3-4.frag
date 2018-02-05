#version 150

//in vec3 color_to_fragment;
in vec2 tex_to_fragment;
in vec3 normal_to_fragment;
in vec3 pos_to_fragment;
out vec4 out_Color;

uniform sampler2D texUnit;

//Specular
uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];

uniform mat4 mdlMatrix;

uniform mat4 lookieMat;

void main(void)
{
	mat3 cameraMatrix = mat3(lookieMat);
	vec3 posToFrag = normalize(cameraMatrix*pos_to_fragment);
	//diffuse stuff
	//const vec3 light = vec3(0.58, 0.58, 0.58);
	vec3 light = normalize(lightSourcesDirPosArr[0]);
	float shade = dot(normalize(normal_to_fragment),light);
	//shade = clamp(shade,0,1);
	vec3 shading = vec3(shade);
//	out_Color = vec4(shading,1.0f);

	//specular stuff
	vec3 r,v;
	r = reflect(normalize(light-posToFrag), normalize(normal_to_fragment));
	v = -posToFrag;

	float specularStrength = 0.0;
	if(dot(light, normalize(normal_to_fragment))>0.0){
		specularStrength = max(dot(r,v),0.01);
		specularStrength = pow(specularStrength,specularExponent[0] );
	}
	out_Color = vec4(vec3(0.5*shade+0.5*specularStrength),1.0f);

}
