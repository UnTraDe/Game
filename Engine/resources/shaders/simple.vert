#version 330 core

uniform mat4 MVP;

layout(location = 0) in vec3 position;

void main()
{
    gl_Position =  MVP * vec4(position, 1.0f);
}