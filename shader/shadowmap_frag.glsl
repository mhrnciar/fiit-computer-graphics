#version 330
// A texture is expected as program attribute
uniform sampler2D ShadowMap;

// The vertex shader will feed this input
in vec2 texCoord;

// The final color
out vec4 FragmentColor;

void main() {
    float Depth = texture(ShadowMap, texCoord).x;
    Depth = 1.0 - (1.0 - Depth) * 25.0;
    FragmentColor = vec4(Depth);
}
