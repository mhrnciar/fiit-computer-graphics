#version 330
// A texture is expected as program attribute
uniform sampler2D Texture;

// (optional) Transparency
uniform float Transparency;

// (optional) Texture offset
uniform vec2 TextureOffset;

// The vertex shader will feed this input
in vec2 texCoord;
in vec3 toCameraVector;
//in vec3 normalVector;

// The final color
out vec4 FragmentColor;

void main() {

    vec3 viewVector = normalize(toCameraVector);
    float refractiveFactor = dot(viewVector, normalize(vec3(0.0f, toCameraVector.y, 0.0f)));
    refractiveFactor = pow(refractiveFactor, 0.4f);

    vec4 relfectionTexture = vec4 (1.0f, 1.0f, 1.0f, 1.0f);
    // Lookup the color in Texture on coordinates given by texCoord
    // NOTE: Texture coordinate is inverted vertically for compatibility with OBJ
    vec4 texColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset);
    
    FragmentColor = mix(relfectionTexture, texColor, refractiveFactor);
}