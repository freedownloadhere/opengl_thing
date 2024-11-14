#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	// maybe don't
	fragPos = vec3(model * vec4(aPos, 1.0f));
	gl_Position = projection * view * vec4(fragPos, 1.0f);
	// maybe don't
	normal = mat3(transpose(inverse(model))) * aNormal;
}