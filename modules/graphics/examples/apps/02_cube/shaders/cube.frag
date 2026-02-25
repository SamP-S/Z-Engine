#version 330 core

in vec3 vPosition;

out vec4 FragColour;

uniform vec4 uColour;

void main()
{
    // normalize position from [-1,1] to [0,1]
    vec4 colourNormalised = vec4((vPosition * 0.5) + 0.5, 1.0);

    // blend 50/50 of uniform colour and vertex position
    vec4 mixedColour = 0.5 * uColour + 0.5 * colourNormalised;

    // use colour from uniform
    FragColour = mixedColour;
}