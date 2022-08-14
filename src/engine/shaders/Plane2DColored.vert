#version 330 core

layout(location=0) in vec3 inPosition;

out vec4 verColor;

uniform mat4 MVP;
uniform vec4 COLOR;

void main()
{
    gl_Position = MVP*vec4(inPosition, 1.0);
    verColor=COLOR;
}