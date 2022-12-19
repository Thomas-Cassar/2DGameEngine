#version 330 core

layout(location=0) in vec3 v_in_position;
layout(location=1) in vec3 v_in_normal;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;
uniform mat3 u_normalMat;

out vec3 v_out_normal;
out vec3 v_out_fragPos;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(v_in_position, 1.0);
    v_out_normal = u_normalMat * v_in_normal;
    v_out_fragPos = vec3(u_model * vec4(v_in_position, 1.0));
}