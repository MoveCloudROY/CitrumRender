#version 330 core
in vec2 TexCoordBox;

out vec4 FragColor;

uniform sampler2D texture_box;

void main()
{
    vec4 color_box = texture(texture_box, TexCoordBox);

    FragColor = color_box;
}