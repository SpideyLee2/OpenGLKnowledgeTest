#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main() {
	// Ambient lighting
	float ambientStrength = 0.3;
	vec3 ambient = ambientStrength * lightColor;

	// Diffuse lighting
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(Normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// Specular lighting
	float specularStrength = 0.5;
	vec3 viewDir = normalize(-FragPos);
	float spec = pow(max(dot(reflect(-lightDir, Normal), viewDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	FragColor = vec4((specular + diffuse + ambient) * objColor, 1.0);
}