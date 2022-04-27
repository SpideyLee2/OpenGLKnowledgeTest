#include "Texture2D.h"

Texture2D::Texture2D(std::string dir, int unit, GLenum sWrapType, GLenum tWrapType) : texUnit(unit) {
	std::string imageType = dir.substr(dir.size() - 4);

	glGenTextures(1, &id);
	bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrapType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrapType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, numChannels;
	unsigned char* texData = stbi_load(dir.c_str(), &width, &height, &numChannels, 0);

	if (texData) {
		switch (numChannels) {
			case 1:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, texData);
				break;
			case 2:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, texData);
				break;
			case 3:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
				break;
			case 4:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
				break;
			default:
				std::cout << "Please add support to the Texture2D class for loading images of type '" << imageType
					<< "'. Deleting this texture object and exiting...\n";
				glDeleteTextures(1, &id);
				return;
		}

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load 2D texture data at: " << dir << std::endl;
	}

	unbind();
	stbi_image_free(texData);
}

void Texture2D::bind() {
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture2D::unbind() {
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture2D::getTexUnit() {
	return texUnit;
}