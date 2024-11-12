#pragma once

#include "glfw/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
private:
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float yaw = -90.0f;
	float pitch = 0.0f;

	float lastMouseX = 800 / 2;
	float lastMouseY = 600 / 2;

	float fov = 45.0f;

	bool firstMouse = true;

public:
	void processKeyboard(GLFWwindow* window, float deltaTime) {
		const float cameraSpeed = 2.5f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
	}

	void processMouse(GLFWwindow* window, float x, float y) {
		if (firstMouse) {
			lastMouseX = x;
			lastMouseY = y;
			firstMouse = false;
		}

		float deltaMouseX = x - lastMouseX;
		float deltaMouseY = lastMouseY - y;
		lastMouseX = x;
		lastMouseY = y;

		const float sensitivity = 0.1f;
		deltaMouseX *= sensitivity;
		deltaMouseY *= sensitivity;

		yaw += deltaMouseX;
		pitch += deltaMouseY;

		pitch = std::min(pitch, 89.99f);
		pitch = std::max(pitch, -89.99f);

		glm::vec3 direction{};
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		cameraFront = glm::normalize(direction);
	}

	void processScroll(GLFWwindow* window, float y) {
		fov -= y;
		fov = std::min(fov, 140.0f);
		fov = std::max(fov, 30.0f);
	}

	glm::mat4 getViewMat() const {
		return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

	glm::mat4 getPerspective(float windowWidth, float windowHeight) const {
		return glm::perspective(glm::radians(fov), windowWidth / windowHeight, 0.1f, 100.0f);
	}
};
