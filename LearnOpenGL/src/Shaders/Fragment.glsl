#version 330 core
out vec4 FragColor;

in vec3 v_Color;
in vec2 v_TexCoord;

uniform vec3 u_LightColor;

void main()
{
  FragColor = vec4(v_Color * u_LightColor, 1.0);
}