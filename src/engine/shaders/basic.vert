#version 330 core

layout(location=0) in vec3 inPosition;
layout(location=1) in vec3 inColor;
layout(location=2) in vec2 inTexCoords;

out vec3 verColor;
out vec2 texCoords;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP*vec4(inPosition, 1.0);
    verColor=inColor;
    texCoords=inTexCoords;
}