#include "Shader.h"

static char* readFile(const char* filePath) {
	FILE* fp{};
	fopen_s(&fp, filePath, "r");
	assert(fp && "file pointer is null");
	assert(fseek(fp, 0, SEEK_END) == 0);
	size_t len = ftell(fp);
	assert(fseek(fp, 0, SEEK_SET) == 0);
	char* buf = (char*)malloc(len + 1);
	assert(buf);
	assert(memset(buf, 0, len + 1));
	fread(buf, 1, len, fp);
	buf[len] = 0;
	assert(!ferror(fp));
	fclose(fp);
	return buf;
}

Shader::Shader(const char* vertPath, const char* fragPath) {
	const char* vertSrc = readFile(vertPath);
	const char* fragSrc = readFile(fragPath);
	GLuint vert = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
	int status{};
	char infoLog[1024]{};

	glShaderSource(vert, 1, &vertSrc, nullptr);
	glCompileShader(vert);
	glGetShaderiv(vert, GL_COMPILE_STATUS, &status);
	if (!status) {
		std::cout << "error compiling vertex shader:\n";
		glGetShaderInfoLog(vert, 1024 - 1, nullptr, infoLog);
		std::cout << infoLog << '\n';
	}

	glShaderSource(frag, 1, &fragSrc, nullptr);
	glCompileShader(frag);
	glGetShaderiv(frag, GL_COMPILE_STATUS, &status);
	if (!status) {
		std::cout << "error compiling fragment shader:\n";
		glGetShaderInfoLog(frag, 1024 - 1, nullptr, infoLog);
		std::cout << infoLog << '\n';
	}

	id = glCreateProgram();
	glAttachShader(id, vert);
	glAttachShader(id, frag);
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &status);
	if (!status) {
		std::cout << "failed to link the program:\n";
		glGetProgramInfoLog(id, 1024 - 1, nullptr, infoLog);
		std::cout << infoLog << '\n';
	}

	glDeleteShader(vert);
	glDeleteShader(frag);
	delete vertSrc;
	delete fragSrc;
}

void Shader::use() const {
	glUseProgram(id);
}

void Shader::setInt(const std::string& name, int value) const {
	const GLuint uni = glGetUniformLocation(id, name.c_str());
	glUniform1i(uni, value);
}

void Shader::setFloat(const std::string& name, float value) const {
	const GLuint uni = glGetUniformLocation(id, name.c_str());
	glUniform1f(uni, value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
	const GLuint uni = glGetUniformLocation(id, name.c_str());
	glUniformMatrix4fv(uni, 1, GL_FALSE, glm::value_ptr(value));
}
