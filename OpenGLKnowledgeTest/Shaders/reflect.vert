#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 normal;
out vec3 fragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	fragPos = vec3(model * vec4(aPos, 1.0));
	normal = normalize(mat3(transpose(inverse(model))) * aNormal);
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}