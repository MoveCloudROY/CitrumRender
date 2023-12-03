#version 430 core
in vec2 TexCoordBox;

uniform vec3 lightColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(lightColor, 1.0);
}