#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightMatrix;

out vec3 position;
out vec3 normal;
out vec4 fragPosLightSpace;

void main()
{
    position = pos;
    normal = nor;
    fragPosLightSpace = lightMatrix * vec4(position, 1.0);
    gl_Position = projection * view * vec4(position, 1.0);
}