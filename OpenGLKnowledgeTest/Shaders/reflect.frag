#version 330 core

out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;

uniform samplerCube skybox;
uniform vec3 cameraPos;

void main() {
	vec3 viewDir = normalize(fragPos - cameraPos);
	vec3 texCoords = reflect(viewDir, normal);
	FragColor = texture(skybox, texCoords);
}