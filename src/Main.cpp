#include <iostream>
#include <cassert>
#include "glad/glad.h"
#include "glfw/glfw3.h"

const int windowWidth = 1280;
const int windowHeight = 720;
const char* windowTitle = "window";

const float vertices[] = {
	-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	
	 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
	 0.0f,	0.5f, 0.0f,		0.0f, 0.0f, 1.0f
};

const int indices[] = {
	0, 1, 2
};

static char* readFile(const char* filePath) {
	FILE* fp{};
	fopen_s(&fp, filePath, "r");
	assert(fp);
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

int main() {
	GLuint vert{}, frag{}, prog{}, vbo{}, ebo{}, vao{};
	GLint shaderStatus{};
	char infoLog[1024]{};
	const char* vertSource = readFile("src/default.vert");
	const char* fragSource = readFile("src/default.frag");

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, nullptr, nullptr);
	assert(window);
	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	gladLoadGL();

	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vertSource, nullptr);
	glCompileShader(vert);
	glGetShaderiv(vert, GL_COMPILE_STATUS, &shaderStatus);
	if (!shaderStatus) {
		glGetShaderInfoLog(vert, 1024 - 1, nullptr, infoLog);
		std::cerr << "error, vertex shader wasn't compiled properly" << std::endl;
		std::cerr << infoLog << std::endl;
		return 1;
	}

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragSource, nullptr);
	glCompileShader(frag);
	glGetShaderiv(frag, GL_COMPILE_STATUS, &shaderStatus);
	if (!shaderStatus) {
		glGetShaderInfoLog(frag, 1024 - 1, nullptr, infoLog);
		std::cerr << "error, fragment shader wasn't compiled properly" << std::endl;
		std::cerr << infoLog << std::endl;
		return 1;
	}

	prog = glCreateProgram();
	glAttachShader(prog, vert);
	glAttachShader(prog, frag);
	glLinkProgram(prog);
	glDeleteShader(vert);
	glDeleteShader(frag);
	glUseProgram(prog);
	delete vertSource;
	delete fragSource;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);

	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClearColor(0.0f, 0.3f, 0.45f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}