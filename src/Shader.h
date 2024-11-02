#pragma once

#include "glad/glad.h"
#include "glfw/glfw3.h"
#include <string>
#include <cassert>
#include <iostream>

class Shader {
public:
	GLuint id;
	Shader(const char* vertPath, const char* fragPath);
	void use() const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};
