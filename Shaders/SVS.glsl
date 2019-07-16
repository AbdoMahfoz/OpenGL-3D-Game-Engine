#version 330 core

layout(location = 0) in vec3 MSPosition;
layout(location = 1) in vec3 MSNormal;
layout(location = 2) in vec2 UV;

uniform mat4 MVP[50];

void main()
{
    gl_Position = MVP[gl_InstanceID] * vec4(MSPosition, 1);
}