#version 330 core

// Interpolated values from the vertex shaders
in vec2 texCoord;
in vec3 Position_worldspace;
in vec3 normal;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

// Ouput data
out vec4 FragmentColor;

// Values that stay constant for the whole mesh.
uniform sampler2D Texture;
uniform vec3 LightPosition;
uniform vec3 LightColor;
uniform vec3 LightEmit;
uniform float LightPower;

void main(){
	// Material properties
	vec3 MaterialDiffuseColor = texture(Texture, texCoord).rgb;
	vec3 MaterialAmbientColor = vec3(0.1, 0.1, 0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3, 0.3, 0.3);

	// Distance to the light
	float distance = length(LightPosition - Position_worldspace);

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
	
	FragmentColor.xyz =
		// Ambient : simulates indirect lighting
		LightEmit + MaterialAmbientColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * LightColor * LightPower * cosAlpha / (distance*distance);
}
