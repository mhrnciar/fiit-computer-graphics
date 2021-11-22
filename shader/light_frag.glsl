#version 330 core

// Interpolated values from the vertex shaders
in vec2 texCoord;
in vec3 Position_worldspace;
in vec3 normal;
in vec3 EyeDirection_cameraspace;

// Ouput data
out vec4 FragmentColor;

// Values that stay constant for the whole mesh.
uniform sampler2D Texture;
uniform mat4 ViewMatrix;

struct PointLight {
	vec3 position;
	vec3 color;
	float power;

	vec3 ambient;
	vec3 specular;
};
#define MAX_LIGHTS 10
uniform PointLight pointLights[MAX_LIGHTS];
uniform float numLights;

void main(){
	vec3 result = vec3(0.1, 0.1, 0.1);

	for (int i = 0; i < numLights; i++) {
		// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
		vec3 LightPosition_cameraspace = (ViewMatrix * vec4(pointLights[i].position, 1)).xyz;
		vec3 LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

		// Material properties
		vec3 MaterialDiffuseColor = texture(Texture, texCoord).rgb;
		vec3 MaterialAmbientColor = pointLights[i].ambient * MaterialDiffuseColor;
		vec3 MaterialSpecularColor = pointLights[i].specular;

		// Distance to the light
		float distance = length(pointLights[i].position - Position_worldspace);

		// Direction of the light (from the fragment to the light)
		vec3 l = normalize(LightDirection_cameraspace);

		// Cosine of the angle between the normal and the light direction,
		// clamped above 0
		//  - light is at the vertical of the triangle -> 1
		//  - light is perpendicular to the triangle -> 0
		//  - light is behind the triangle -> 0
		float cosTheta = clamp(dot(normal, l), 0, 1);

		// Eye vector (towards the camera)
		vec3 E = normalize(EyeDirection_cameraspace);

		// Direction in which the triangle reflects the light
		vec3 R = reflect(-l, normal);

		// Cosine of the angle between the Eye vector and the Reflect vector,
		// clamped to 0
		//  - Looking into the reflection -> 1
		//  - Looking elsewhere -> < 1
		float cosAlpha = clamp(dot(E, R), 0, 1);

		result +=
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * pointLights[i].color * pointLights[i].power * cosTheta / (distance*distance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * pointLights[i].color * pointLights[i].power * cosAlpha / (distance*distance);
	}

	FragmentColor = vec4(result, 1.0);
}
