#version 330 core

layout(location = 0) in vec3 MSPosition;
layout(location = 1) in vec3 MSNormal;
layout(location = 2) in vec2 UV;

uniform mat4 MVP[50];
uniform vec3 Color[50];
uniform mat4 LightMVP[30];
uniform mat4 ModelMatrix[50];
uniform int lightCount;

flat out int LightCount;
out vec3 color;
out vec2 uv;
out vec3 WSPosition;
out vec3 WSNomral;
out vec4 LightPrepPos[5];

void main()
{
    gl_Position = MVP[gl_InstanceID] * vec4(MSPosition, 1);
    WSPosition = vec3(ModelMatrix[gl_InstanceID] * vec4(MSPosition, 1));
    WSNomral = vec3(normalize(ModelMatrix[gl_InstanceID] * vec4(MSNormal, 0)));
    LightCount = lightCount;
    for(int i = 0; i < lightCount; i++)
    {
        LightPrepPos[i] = LightMVP[i] * vec4(WSPosition, 1);
    }
    uv = UV;
	color = Color[gl_InstanceID];
}