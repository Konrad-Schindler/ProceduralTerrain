#version 460 core

out vec4 FragColor;

uniform vec3 lightDirection;
uniform vec3 cameraPos;
uniform bool stylized;
uniform float near;
uniform float far;

in vec3 position;
in vec3 normal;
in vec4 fragPosLightSpace;

uniform sampler2D shadowMap;

bool inShadow(vec4 fragPosLightSpace)
{
	// move coordinate from [-w, w] -> [-1, 1] -> [0, 1]
	vec4 fragPos = (fragPosLightSpace / fragPosLightSpace.w + 1) * 0.5;
	float depthInShadowMap = texture(shadowMap, fragPos.xy).r;
	return depthInShadowMap < fragPos.z;
}

void main()
{
	float angle = dot(normalize(-lightDirection), normal);

	FragColor = position.y > 40 ? vec4(0.6, 0.6, 0.6, 1) : vec4(0.5, 0.25, 0.1, 1);

	if (stylized) {
		FragColor *= (angle > 0.2 ? angle > 0.6 ? angle > 0.8 ? 1 : 0.66 : 0.33 : 0.0);
	} else {
		FragColor *= angle;
	}
	FragColor = inShadow(fragPosLightSpace) ? vec4(1, 0, 0, 0) : FragColor;
}