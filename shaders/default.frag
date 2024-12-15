#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

in vec3 normal;
in vec3 fragPos;
in vec2 texPos;

out vec4 fragColor;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() {
	vec3 lightDir = normalize(light.position - fragPos);
	float diffCos = max(0.0f, dot(normal, lightDir));

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float specCos = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);

	vec3 diffTex = texture(material.diffuse, texPos).rgb;
	vec3 specTex = texture(material.specular, texPos).rgb;

	vec3 ambient = light.ambient * diffTex;
	vec3 diffuse = light.diffuse * diffCos * diffTex;
	vec3 specular = light.specular * specCos * specTex;

	float d = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + d * light.linear + d * d * light.quadratic);

	vec3 result = ambient + diffuse + specular;
	result *= attenuation;

	fragColor = vec4(result, 1.0f);
}