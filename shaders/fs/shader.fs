#version 330 core
in vec3 ourColor;
in vec2 TexCoordBox;
in vec2 TexCoordSmile;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    // FragColor = texture(texture1, TexCoordBox);
    // FragColor = vec4(ourColor, 1.0f);
    FragColor = mix(texture(texture1, TexCoordBox),texture(texture2, TexCoordSmile),0.2);

    //**** HomeWork 1 ****//
    // FragColor = mix(texture(texture1, TexCoordBox),texture(texture2, vec2(1.0 - TexCoordBox.x, TexCoordBox.y)),0.2);
}