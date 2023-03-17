#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoordBox;
layout (location = 2) in vec2 aTexCoordSmile;

uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoordBox;
out vec2 TexCoordSmile;
void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0f);
   
   TexCoordBox = aTexCoordBox;
   TexCoordSmile = aTexCoordSmile;
}