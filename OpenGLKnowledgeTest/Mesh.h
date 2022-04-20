#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>
#include <vector>

#include "Shader.h"

enum class TextureType {
	DIFFUSE,
	SPECULAR
};

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	TextureType type;
	std::string path;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds, std::vector<Texture> texs);

	void draw(Shader& shader);

private:
	unsigned int vao, vbo, ebo;

	void setupMesh();
};

