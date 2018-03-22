#version 150

out vec4 outColor;
in vec2 texCoord;
uniform sampler2D tex;
uniform sampler2D textureConcrecte;
uniform sampler2D textureSand;
in vec3 normal_to_frag;
in vec3 pos_to_frag;

uniform mat4 camMatrix;

void main(void)
{
	vec3 light = mat3(camMatrix)*normalize(vec3(0,10,10));
	float shade = dot(normal_to_frag, light);
	shade  = clamp(shade, 0,1);
	vec3 shading =  vec3(shade);
	if(pos_to_frag.y < 10){
		if(pos_to_frag.y <= 3){
			outColor = texture(textureSand, texCoord);//*vec4(0.0, 0.0, 1.0, 1.0f);
		}else{
			outColor = vec4(shading,1.0f) * texture(tex, texCoord);
		}
	}else{
		outColor = vec4(shading,1.0f) * texture(textureConcrecte, texCoord);
	}
}
