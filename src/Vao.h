#pragma once

#include "glad/glad.h"

class Vao {
private:
	GLuint id;
	GLuint stride;

	GLuint nextLocation = 0;
	GLuint nextOffset = 0;

public:
	Vao(GLuint _stride) : stride(_stride) {
		glGenVertexArrays(1, &id);
	}

	void bind() const {
		glBindVertexArray(id);
	}

	void attrib(GLuint count, GLenum type, GLuint typeSize) {
		glVertexAttribPointer(nextLocation, count, type, GL_FALSE, stride, (void*)nextOffset);
		glEnableVertexAttribArray(nextLocation);
		nextLocation++;
		nextOffset += (count * typeSize);
	}
};
