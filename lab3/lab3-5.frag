#version 150

//in vec3 color_to_fragment;
in vec2 tex_to_fragment;
in vec3 normal_to_fragment;
in vec3 pos_to_fragment;
out vec4 out_Color;

uniform sampler2D texMask;
uniform sampler2D texRut;

//Specular
uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];

uniform mat4 mdlMatrix;

uniform mat4 lookieMat;

uniform bool hasTex;

void main(void)
{
	mat3 cameraMatrix = mat3(lookieMat);
	//diffuse stuff
	//const vec3 light = vec3(0.58, 0.58, 0.58);

	vec3 tmpOutColor = vec3(0,0,0);
	for(int i = 0; i<4; i++){
		//transform light to view coord
		vec3 light = (cameraMatrix * normalize(lightSourcesDirPosArr[i]));
		float shade = dot(normalize(normal_to_fragment),light);
		shade = clamp(shade,0,1);

		//specular stuff
		vec3 r,v;

		if(isDirectional[i]){
			r = reflect(normalize(-light), normalize(normal_to_fragment));
		}else{
			r = reflect(normalize(pos_to_fragment-light), normalize(normal_to_fragment));
		}

		v = normalize(-pos_to_fragment);

		float specularStrength = dot(r,v);
		if(specularStrength>0.0){
			specularStrength = pow(specularStrength,150.0);
		}
			specularStrength = max(specularStrength,0.0);

		tmpOutColor += vec3(0.5*shade+0.7*specularStrength)*lightSourcesColorArr[i];
	}
	//out_Color = vec4(vec3(0.5*shade+0.7*specularStrength)*lightSourcesColorArr[0],1.0f);

		if(hasTex){
			vec4 tex,tex2;
			if(tex_to_fragment.s > tex_to_fragment.t){
				tex = texture(texRut, tex_to_fragment);
			}else{
				tex = texture(texMask, tex_to_fragment);
			}
			out_Color = 2*tex*vec4(tmpOutColor,1.0f);
		}else{
				out_Color = vec4(tmpOutColor,1.0f);
		}
}
