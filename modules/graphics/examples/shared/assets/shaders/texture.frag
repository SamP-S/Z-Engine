#version 330 core

uniform sampler2D uTexture;

in vec2 vTexCoord;

out vec4 FragColour;

void main()
{
    // Sample Colour Texture
    vec4 texColour = texture(uTexture, vTexCoord);
    FragColour = texColour;
}