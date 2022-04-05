#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
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
in vec2 TexCoords;

out vec4 FragColor;

uniform Material material;
uniform Light light;

void main() {
	// Ambient lighting
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	// Diffuse lighting
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(Normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

	// Specular lighting
	vec3 viewDir = normalize(-FragPos);
	float spec = pow(max(dot(reflect(-lightDir, Normal), viewDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

	vec3 emission = texture(material.emission, TexCoords).rgb;

	FragColor = vec4(specular + diffuse + ambient + emission, 1.0);
}