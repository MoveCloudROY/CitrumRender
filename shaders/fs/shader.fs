#version 330 core
in vec2 TexCoordBox;
in vec2 TexCoordSmile;

out vec4 FragColor;

uniform sampler2D texture_box;
uniform sampler2D texture_smile;

uniform float alpha;

void main()
{
    vec4 color_box = texture(texture_box, TexCoordBox);
    vec4 color_smile = texture(texture_smile, TexCoordSmile);
    // FragColor = texture(texture_box, TexCoordBox);
    // FragColor = vec4(ourColor, 1.0f);

    //**** A Bugfix ? ****//
    // 原来第三个参数使用 alpha，但是笑脸透明部分为黑边
    // 修改为 color_smile.a * alpha 则正常
    // TODO: 仍然不知道为什么...
    FragColor = mix(color_box, color_smile, color_smile.a * alpha);

    //**** HomeWork 1 ****//
    // FragColor = mix(texture(texture_box, TexCoordBox),texture(texture_smile, vec2(1.0 - TexCoordBox.x, TexCoordBox.y)),0.2);
}