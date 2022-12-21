#version 330 core

layout(location = 0) out vec4 f_out_color;


in vec3 v_out_normal;
in vec3 v_out_fragPos;
in vec2 v_out_texCoords;

uniform sampler2D u_baseColorTexture;
uniform vec3 u_ambientLightColor;
uniform vec3 u_pointLightPos;
uniform vec3 u_pointLightColor;
uniform vec3 u_viewPos;
uniform float u_specularStrength;

void main()
{
    // Diffuse lighting
    vec3 f3Norm = normalize(v_out_normal);
    vec3 f3LightDir = normalize(u_pointLightPos - v_out_fragPos);
    float fDiff = max(dot(f3Norm, f3LightDir), 0.0);
    vec3 f3diffusedLightColor = fDiff * u_pointLightColor;

    // Specular lighting
    vec3 f3ViewDir = normalize(u_viewPos - v_out_fragPos);
    vec3 f3ReflectDir = reflect(-f3LightDir, f3Norm);
    float fSpec = pow(max(dot(f3ViewDir, f3ReflectDir), 0.0), 32);
    vec3 f3SpecularLightColor = u_specularStrength * fSpec * u_pointLightColor;

    vec4 f4ObjectColor = texture(u_baseColorTexture, v_out_texCoords);
    f_out_color = vec4((u_ambientLightColor + f3diffusedLightColor + f3SpecularLightColor), 1.0) * f4ObjectColor;
};