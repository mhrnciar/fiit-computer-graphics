#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec3 normal;
out vec3 FragPos;
out vec2 texCoord;

void main()
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0);

	FragPos = (ModelMatrix * vec4(Position, 1)).xyz;

	normal = Normal * mat3(transpose(inverse(ModelMatrix)));

	texCoord = TexCoord;
}