#version 330 core

in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
	float ambientStrength = 0.2f;
	vec3 ambientLight = objectColor * ambientStrength;

	vec3 lightDir = normalize(lightPos - fragPos);
	float diffCos = max(0.0f, dot(normal, lightDir));
	vec3 diffLight = diffCos * lightColor;

	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float specCos = pow(max(dot(reflectDir, viewDir), 0.0f), 32);
	vec3 specLight = specularStrength * specCos * lightColor;

	vec3 result = (ambientLight + diffLight + specLight) * objectColor;

	fragColor = vec4(result, 1.0f);
}