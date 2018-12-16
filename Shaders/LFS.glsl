#version 330 core

in vec3 WSPosition;
in vec3 WSNomral;
in vec2 uv;

uniform vec3 WSLight;
uniform vec3 WSEye;
uniform vec3 LightColor;
uniform vec3 AmbientLight;
uniform vec3 Color;
uniform float Specularity;
uniform sampler2D InputTexture;

out vec4 Res;

void main()
{
    //Diffuse
    vec3 LightVector = normalize(WSLight - WSPosition);
    float diffuse = dot(LightVector, WSNomral);
    vec3 diffuseLight = clamp(vec3(diffuse, diffuse, diffuse) * LightColor, 0, 1);
    //Specular
    vec3 ReflectionVector = reflect(-LightVector, WSNomral);
    vec3 EyeVector = normalize(WSEye - WSPosition);
    float Specular = pow(dot(ReflectionVector, EyeVector), Specularity);
    vec3 specularLight = clamp(vec3(Specular, Specular, Specular) * LightColor, 0, 1);
    //Result
    Res = texture(InputTexture, uv) * vec4(diffuseLight + specularLight + AmbientLight, 1) * vec4(Color, 1);
}