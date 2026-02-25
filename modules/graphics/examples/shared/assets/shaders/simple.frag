#version 330 core

uniform vec4 uColour;
uniform sampler2D uTexture;

in vec3 vPosition;
in vec2 vTexCoord;

out vec4 FragColour;

void main()
{
    // Normalize position from [-1,1] to [0,1]
    vec4 posColour = vec4((vPosition * 0.5) + 0.5, 1.0);
    // Sample Colour Texture
    vec4 texColour = texture(uTexture, vTexCoord);

    // Mix vertex normalised position, uniform colour, uniform texture sample
    FragColour = (uColour * 0.2 + posColour * 0.2 + texColour * 0.6);
}