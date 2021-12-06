#version 330 core

// Interpolated values from the vertex shaders
in vec2 texCoord;

// Ouput data
out vec4 FragmentColor;

uniform sampler2D Texture;

void main(){
    // Output color = color of the texture at the specified UV
    FragmentColor = texture( Texture, texCoord );
}