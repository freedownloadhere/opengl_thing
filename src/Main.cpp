#include "glad/glad.h"
#include "glfw/glfw3.h"

#include <iostream>
#include <cassert>

#include "Shader.h"

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

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, nullptr, nullptr);
	assert(window);
	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	gladLoadGL();

	GLuint vbo{}, ebo{}, vao{};
	Shader ourShader("src/default.vert", "src/default.frag");

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

		glBindVertexArray(vao);
		ourShader.use();

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}