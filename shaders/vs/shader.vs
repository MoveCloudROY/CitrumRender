#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
//** HomeWork T2 **//
uniform float bias;
out vec3 ourColor;
out vec2 TexCoord;
void main()
{
   // gl_Position = vec4(aPos, 1.0);

   //** HomeWork T1 **//
   // gl_Position = vec4(aPos.x, -aPos.y, aPos.z, 1.0);

   //** HomeWork T2 **//
   gl_Position = vec4(aPos.x+sin(bias)/2.0, aPos.y+cos(bias)/2.0, aPos.z, 1.0);
   
   ourColor = vec3(aColor.r+sin(bias)/2.0,aColor.g+cos(bias)/2.0, aColor.b);
   TexCoord = aTexCoord;
}