#version 330 core
in vec3 ourColor;
in vec2 TexCoordBox;
in vec2 TexCoordSmile;

out vec4 FragColor;

uniform sampler2D texture_box;
uniform sampler2D texture_smile;

uniform float alpha;

void main()
{
    // FragColor = texture(texture_box, TexCoordBox);
    // FragColor = vec4(ourColor, 1.0f);
    FragColor = mix(texture(texture_box, TexCoordBox),texture(texture_smile, TexCoordSmile),alpha);

    //**** HomeWork 1 ****//
    // FragColor = mix(texture(texture_box, TexCoordBox),texture(texture_smile, vec2(1.0 - TexCoordBox.x, TexCoordBox.y)),0.2);
}