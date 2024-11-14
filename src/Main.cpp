#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <cassert>

#include "Shader.h"
#include "Camera.h"
#include "Vao.h"
#include "Vbo.h"
#include "Ebo.h"
#include "Texture2D.h"

static const int windowWidth = 800;
static const int windowHeight = 600;
static const char* windowTitle = "window";

static float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

static float deltaTime = 0.0f;
static float lastFrame = 0.0f;

static Camera cam;

static void processKeys(GLFWwindow* window) {
	cam.processKeyboard(window, deltaTime);
}

static void mouseCallback(GLFWwindow* window, double x, double y) {
	cam.processMouse(window, x, y);
}

static void scrollCallback(GLFWwindow* window, double x, double y) {
	cam.processScroll(window, y);
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, nullptr, nullptr);
	assert(window);
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	gladLoadGL();

	Shader defShader("shaders/default.vert", "shaders/default.frag");
	defShader.use();
	defShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
	defShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

	Shader lightShader("shaders/default.vert", "shaders/light.frag");

	Vbo vbo;
	vbo.bind();
	vbo.data(vertices, sizeof(vertices));

	Vao vao(6 * sizeof(float));
	vao.bind();
	vbo.bind();
	vao.attrib(3, GL_FLOAT, sizeof(float));
	vao.attrib(3, GL_FLOAT, sizeof(float));

	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		processKeys(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		vao.bind();
		defShader.use();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = cam.getViewMat();
		glm::mat4 projection = cam.getPerspective(windowWidth, windowHeight);
		defShader.setMat4("model", model);
		defShader.setMat4("view", view);
		defShader.setMat4("projection", projection);
		defShader.setVec3("viewPos", cam.getPosition());
		glm::vec3 lightPos = glm::vec3(
			2.0f * cos((float)glfwGetTime()),
			1.0f,
			2.0f * sin((float)glfwGetTime())
		);
		defShader.setVec3("lightPos", lightPos);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightShader.use();
		model = glm::translate(model, lightPos);
		lightShader.setMat4("model", model);
		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}