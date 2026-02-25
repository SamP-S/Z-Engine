#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 10) in vec2 aUV;

out vec3 vPosition;
out vec2 vUV;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    // apply transforms and assign
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);

    // propogate raw attributes to fragment shader
    vPosition = aPosition;
    vUV = aUV;
}