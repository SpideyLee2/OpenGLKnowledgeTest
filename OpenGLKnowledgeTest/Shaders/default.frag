#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light {
	float innerCutoff;
	float outerCutoff;

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
	vec3 spotDir = vec3(0.0, 0.0, -1.0);
	vec3 lightDir = normalize(-FragPos);
	float theta = dot(lightDir, -spotDir);

	float intensity = (theta - light.outerCutoff) / (light.innerCutoff - light.outerCutoff);
	intensity = clamp(intensity, 0.0, 1.0);
	
	// Ambient lighting
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	// Diffuse lighting
	float diff = max(dot(Normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb * intensity;

	// Specular lighting
	vec3 viewDir = normalize(-FragPos);
	float spec = pow(max(dot(reflect(-lightDir, Normal), viewDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb * intensity;

	//vec3 emission = texture(material.emission, TexCoords).rgb;
	FragColor = vec4(specular + diffuse + ambient, 1.0);	
}