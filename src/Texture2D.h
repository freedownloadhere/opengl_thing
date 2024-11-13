#pragma once

#include "glad/glad.h"
#include "stb/stb_image.h"

#include <iostream>

class Texture2D {
private:
	GLuint id;

public:
	Texture2D(const char* imgSource, GLint imgType) {
		stbi_set_flip_vertically_on_load(true);
		int imgWidth{}, imgHeight{}, imgNrCh{};
		auto data = stbi_load(imgSource, &imgWidth, &imgHeight, &imgNrCh, 0);
		if (data == nullptr)
			std::cout << "Failed to load texture " << imgSource << '\n';
		glGenTextures(1, &id);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, imgType, imgWidth, imgHeight, 0, imgType, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}

	void bind(GLuint texSlot) const {
		glActiveTexture(GL_TEXTURE0 + texSlot);
		glBindTexture(GL_TEXTURE_2D, id);
	}
};