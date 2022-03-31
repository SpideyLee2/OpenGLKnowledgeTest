#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
	gl_Position = proj * view * model * vec4(aPos, 1.0);
	Normal = normalize(mat3(transpose(inverse(view * model))) * aNormal); // inverses are inefficient on GPU
	FragPos = vec3(view * model * vec4(aPos, 1.0));
}