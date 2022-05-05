#version 330 core

out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;

uniform samplerCube skybox;
uniform vec3 cameraPos;
uniform float initMat;
uniform float refractMat;

void main() {
	float ratio = initMat / refractMat;
	vec3 viewDir = normalize(fragPos - cameraPos);
	vec3 texCoords = refract(viewDir, normal, ratio);
	FragColor = texture(skybox, texCoords);
}