#pragma once

#include "glad/glad.h"

class Ebo {
private:
	GLuint id;

public:
	Ebo() {
		glGenBuffers(1, &id);
	}

	void bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}

	void data(const int indicesData[]) const {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesData), indicesData, GL_STATIC_DRAW);
	}
};
