#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Normal;

// Values that stay constant for the whole mesh.
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform vec3 LightPosition;
uniform vec3 LightEmit;

// Output data ; will be interpolated for each fragment.
out vec2 texCoord;
out vec3 normal;
out vec3 Position_worldspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;
out vec3 lightEmit;

void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0);
	
	// Position of the vertex, in worldspace : M * position
	Position_worldspace = (ModelMatrix * vec4(Position, 1)).xyz;
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 Position_cameraspace = (ViewMatrix * ModelMatrix * vec4(Position, 1)).xyz;
	EyeDirection_cameraspace = vec3(0, 0, 0) - Position_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPosition_cameraspace = (ViewMatrix * vec4(LightPosition, 1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
	
	// Normal of the the vertex, in camera space
	normal = normalize((ViewMatrix * ModelMatrix * vec4(Normal, 0)).xyz);
	
	// UV of the vertex. No special space for this one.
	texCoord = TexCoord;

	lightEmit = LightEmit;
}

