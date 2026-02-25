#version 330 core

uniform vec4 uColour;

out vec4 FragColour;

void main()
{
    // use colour from uniform
    FragColour = uColour;
}