#version 330
// A texture is expected as program attribute
uniform sampler2D Texture;

// The vertex shader fill feed this input
in vec2 texCoord;

// The final color
out vec4 FragmentColor;

void main() {
    vec4 texColor = texture(Texture, texCoord);
    float gray = 0.21 * texColor.r + 0.71 * texColor.g + 0.07 * texColor.b;
    float colorFactor = 1.0;

    FragmentColor = vec4(texColor.rgb * (1.0 - colorFactor) + (gray * colorFactor), 1.0);
}
