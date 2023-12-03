#version 430 core
layout(location = 0) in vec3 apos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
    TexCoords = apos;
    vec4 pos = projection * view * vec4(apos, 1.0);
    gl_Position = pos.xyww;
}