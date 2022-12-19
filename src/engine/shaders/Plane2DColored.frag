#version 330 core

layout(location = 0) out vec4 f_out_color;

in vec3 v_out_normal;
in vec3 v_out_fragPos;

uniform vec3 u_objectColor;
uniform vec3 u_ambientLightColor;
uniform vec3 u_difLightPos;
uniform vec3 u_difLightColor;

void main()
{
    vec3 f3Norm = normalize(v_out_normal);
    vec3 f3LightDir = normalize(u_difLightPos - v_out_fragPos);
    float fDiff = max(dot(f3Norm, f3LightDir), 0.0);
    vec3 v3diffusedLightColor = fDiff * u_difLightColor;

    f_out_color = vec4((u_ambientLightColor + v3diffusedLightColor) * u_objectColor, 1.0);
};