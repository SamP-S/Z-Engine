#version 330 core

layout(location = 0) in vec3 aPosition;

out vec3 vPosition;

void main()
{
    // set fragment position
    gl_Position = vec4(aPosition, 1.0);
    
    // propogate to fragment shader
    vPosition = aPosition;
}