#version 330 core

in vec3 vPosition;

out vec4 FragColour;

void main()
{
    // normalize position from [-1,1] to [0,1]
    vec4 colourNormalised = vec4((vPosition * 0.5) + 0.5, 1.0);

    // set as output fragment colour
    FragColour = colourNormalised;
}