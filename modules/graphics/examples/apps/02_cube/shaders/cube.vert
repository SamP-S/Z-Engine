#version 330 core

layout(location = 0) in vec3 aPosition;

out vec3 vPosition;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    // apply transforms and assign
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);

    // propogate untransformed positions to fragment shader
    vPosition = aPosition;
}