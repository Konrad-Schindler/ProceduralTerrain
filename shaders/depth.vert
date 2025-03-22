#version 460 core
layout (location = 0) in vec3 pos;

uniform mat4 lightMatrix;

void main()
{
    gl_Position = lightMatrix * vec4(pos, 1.0);
}