#version 330 core

layout(location = 0) in vec3 MSPosition;
layout(location = 1) in vec3 MSNormal;
layout(location = 2) in vec2 UV;

uniform mat4 MVP;
uniform mat4 LightMVP;
uniform mat4 ModelMatrix;

out vec2 uv;
out vec3 WSPosition;
out vec3 WSNomral;
out vec4 LightPrepPos;

void main()
{
    gl_Position = MVP * vec4(MSPosition, 1);
    WSPosition = vec3(ModelMatrix * vec4(MSPosition, 1));
    WSNomral = vec3(normalize(ModelMatrix * vec4(MSNormal, 0)));
    LightPrepPos = LightMVP * vec4(WSPosition, 1);
    uv = UV;
}