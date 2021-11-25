#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Normal;

// Values that stay constant for the whole mesh.
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

// Output data ; will be interpolated for each fragment.
out vec2 texCoord;
out vec3 normal;
out vec3 Position_worldspace;
out vec3 EyeDirection_cameraspace;

void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0);

	// Position of the vertex, in worldspace : M * position
	Position_worldspace = (ModelMatrix * vec4(Position, 1)).xyz;

	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 Position_cameraspace = (ViewMatrix * ModelMatrix * vec4(Position, 1)).xyz;
	EyeDirection_cameraspace = vec3(0, 0, 0) - Position_cameraspace;

	// Normal of the the vertex, in camera space
	normal = normalize((ViewMatrix * ModelMatrix * vec4(Normal, 0)).xyz);

	// UV of the vertex. No special space for this one.
	texCoord = TexCoord;
}

