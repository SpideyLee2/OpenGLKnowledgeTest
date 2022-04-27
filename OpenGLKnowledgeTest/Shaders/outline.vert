#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	vec3 FragPos = vec3(view * model * vec4(aPos, 1.0));
	vec3 Normal = normalize(mat3(transpose(inverse(view * model))) * aNormal); // inverses are inefficient on GPU
	gl_Position = projection * view * model * vec4(aPos + aNormal * 0.05, 1.0);
}