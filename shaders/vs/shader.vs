#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoordBox;
layout (location = 3) in vec2 aTexCoordSmile;

uniform mat4 transform;

out vec2 TexCoordBox;
out vec2 TexCoordSmile;
void main()
{
   gl_Position = transform * vec4(aPos, 1.0f);
   
   TexCoordBox = aTexCoordBox;
   TexCoordSmile = aTexCoordSmile;
}