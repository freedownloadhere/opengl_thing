#pragma once

#include "glad/glad.h"

class Vbo {
private:
	GLuint id;

public:
	Vbo() {
		glGenBuffers(1, &id);
	}

	void bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}

	void data(const void* data, GLuint size) const {
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
};
