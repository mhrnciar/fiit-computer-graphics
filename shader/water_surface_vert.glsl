#version 330
// The inputs will be fed by the vertex buffer objects
layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
//layout(location = 2) in vec3 Normal;

// Matrices as program attributes
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

uniform vec3 cameraPosition;

// This will be passed to the fragment shader
out vec2 texCoord;
out vec3 toCameraVector;
//out vec3 normalVector;

void main() {
    // Copy the input to the fragment shader
    texCoord = TexCoord;
    //normalVector = Normal;

    vec4 worldPosition = ModelMatrix * vec4(Position.x, 0.0f, Position.z, 1.0f);
    toCameraVector = cameraPosition - worldPosition.xyz;
    // Calculate the final position on screen
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0);
}
