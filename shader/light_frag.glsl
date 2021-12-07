#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

struct PointLight {
	vec3 position;
	vec3 color;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
#define MAX_LIGHTS 30
uniform PointLight pointLights[MAX_LIGHTS];
uniform float numLights;

uniform Material material;
uniform vec3 viewPos;

out vec4 FragmentColor;

in vec3 normal;
in vec3 FragPos;
in vec2 texCoord;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
	//properties
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	//phase 1: Directional lighting
	vec3 result = CalcDirLight(dirLight, norm, viewDir);

	//phase 2: Point lights
	for(int i = 0; i < numLights; i++)
	result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

	FragmentColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);

	//diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	//specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	//combine results
	vec3 ambient = light.ambient  * vec3(texture(material.diffuse, texCoord));
	vec3 diffuse = light.diffuse  * diff * vec3(texture(material.diffuse, texCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);

	//diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	//specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	//attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
	light.quadratic * (distance * distance));

	//combine results
	vec3 ambient = light.ambient * light.color * vec3(texture(material.diffuse, texCoord));
	vec3 diffuse = light.diffuse * light.color * diff * vec3(texture(material.diffuse, texCoord));
	vec3 specular = light.specular * light.color * spec * vec3(texture(material.specular, texCoord));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}
