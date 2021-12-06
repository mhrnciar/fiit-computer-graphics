#version 330 core

layout(location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;

// Output data ; will be interpolated for each fragment.
out vec2 texCoord;

// Values that stay constant for the whole mesh.
uniform vec3 CameraRight;
uniform vec3 CameraUp;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec3 BillboardPos; // Position of the center of the billboard
uniform vec2 BillboardSize; // Size of the billboard, in world units (probably meters)

void main(){
    vec3 vertexPosition = BillboardPos
    + CameraRight * Position.x * BillboardSize.x
    + CameraUp * Position.y * BillboardSize.y;

    // Output position of the vertex
    gl_Position = ProjectionMatrix * ViewMatrix * vec4(vertexPosition, 1.0f);

    // UV of the vertex. No special space for this one.
    texCoord = TexCoord;
}

