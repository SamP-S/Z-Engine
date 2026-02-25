#version 330 core

in vec3 vPosition;
in vec2 vUV;

out vec4 FragColour;

uniform sampler2D uTexture;

void main()
{
    // sample colour texture
    vec4 texColour = texture(uTexture, vUV);
    vec4 uvColour = vec4(vUV, 0.0, 1.0);

    // use texture colour
    FragColour = texColour;
    //FragColour = mix(texColour, uvColour, 0.5);
}