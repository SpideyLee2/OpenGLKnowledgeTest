#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform Material material;
uniform Light light;

void main() {
	// Ambient lighting
	vec3 ambient = light.ambient * material.ambient;

	// Diffuse lighting
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(Normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// Specular lighting
	vec3 viewDir = normalize(-FragPos);
	float spec = pow(max(dot(reflect(-lightDir, Normal), viewDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	FragColor = vec4(specular + diffuse + ambient, 1.0);
}