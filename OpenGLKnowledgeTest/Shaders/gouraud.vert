#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main() {
	vec4 viewVertPos = view * model * vec4(aPos, 1.0);
	gl_Position = proj * viewVertPos;

	vec3 Normal = normalize(mat3(transpose(inverse(view * model))) * aNormal);
	
	// Ambient lighting
	float ambientStrength = 0.3;
	vec3 ambient = ambientStrength * lightColor;

	// Diffuse lighting
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - vec3(viewVertPos));
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// Specular lighting
	float specularStrength = 0.8;
	vec3 viewDir = normalize(-vec3(viewVertPos));
	float spec = pow(max(dot(reflect(-lightDir, norm), viewDir), 0.0), 64);
	vec3 specular = specularStrength * spec * lightColor;

	color = vec4((specular + diffuse + ambient) * objColor, 1.0);
}