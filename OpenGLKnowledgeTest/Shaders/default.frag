#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	//sampler2D emission;
	float shininess;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
#define NUM_POINT_LIGHTS 4

struct SpotLight {
	float innerCutoff;
	float outerCutoff;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform SpotLight spotLight;

vec3 CalcDirLight();
vec3 CalcPointLight(PointLight pointLight);
vec3 CalcSpotLight();

void main() {
	vec3 color = vec3(0.0, 0.0, 0.0);
	color += CalcDirLight();
	for(int i = 0; i < NUM_POINT_LIGHTS; ++i) {
		color += CalcPointLight(pointLights[i]);
	}
	color += CalcSpotLight();

	FragColor = vec4(color, 1.0);
}

vec3 CalcDirLight() {
	vec3 lightDir = normalize(-dirLight.direction);

	// Ambient lighting
	vec3 ambient = dirLight.ambient * texture(material.diffuse, TexCoords).rgb;

	// Diffuse lighting
	float diff = max(dot(Normal, lightDir), 0.0);
	vec3 diffuse = dirLight.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

	// Specular lighting
	vec3 viewDir = normalize(-FragPos);
	float spec = pow(max(dot(reflect(-lightDir, Normal), viewDir), 0.0), material.shininess);
	vec3 specular = dirLight.specular * spec * texture(material.specular, TexCoords).rgb;

	// Emission lighting
	//vec3 emission = texture(material.emission, TexCoords).rgb;

	return (specular + diffuse + ambient);
}

vec3 CalcPointLight(PointLight pointLight) {
	vec3 lightDir = normalize(pointLight.position - FragPos);

	float d = distance(FragPos, pointLight.position);
	float attenuation = 1.0 / (pointLight.constant + pointLight.linear * d + pointLight.quadratic * (d * d));
	
	// Ambient lighting
	vec3 ambient = pointLight.ambient * texture(material.diffuse, TexCoords).rgb;

	// Diffuse lighting
	float diff = max(dot(Normal, lightDir), 0.0);
	vec3 diffuse = pointLight.diffuse * diff * texture(material.diffuse, TexCoords).rgb * attenuation;

	// Specular lighting
	vec3 viewDir = normalize(-FragPos);
	float spec = pow(max(dot(reflect(-lightDir, Normal), viewDir), 0.0), material.shininess);
	vec3 specular = pointLight.specular * spec * texture(material.specular, TexCoords).rgb * attenuation;

	// Emission lighting
	//vec3 emission = texture(material.emission, TexCoords).rgb;
	
	return (specular + diffuse + ambient);	
}

vec3 CalcSpotLight() {
	float d = distance(FragPos, vec3(0, 0, 0));
	float attenuation = 1.0 / (spotLight.constant + spotLight.linear * d + spotLight.quadratic * (d * d));

	vec3 spotDir = vec3(0.0, 0.0, -1.0);
	vec3 lightDir = normalize(-FragPos);
	float theta = dot(lightDir, -spotDir);

	float intensity = (theta - spotLight.outerCutoff) / (spotLight.innerCutoff - spotLight.outerCutoff);
	intensity = clamp(intensity, 0.0, 1.0);
	
	// Ambient lighting
	vec3 ambient = spotLight.ambient * texture(material.diffuse, TexCoords).rgb;

	// Diffuse lighting
	float diff = max(dot(Normal, lightDir), 0.0);
	vec3 diffuse = spotLight.diffuse * diff * texture(material.diffuse, TexCoords).rgb * attenuation * intensity;

	// Specular lighting
	vec3 viewDir = normalize(-FragPos);
	float spec = pow(max(dot(reflect(-lightDir, Normal), viewDir), 0.0), material.shininess);
	vec3 specular = spotLight.specular * spec * texture(material.specular, TexCoords).rgb * attenuation * intensity;

	// Emission lighting
	//vec3 emission = texture(material.emission, TexCoords).rgb;

	return (specular + diffuse + ambient);
}