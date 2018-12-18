#version 330 core

in vec3 WSPosition;
in vec3 WSNomral;
in vec2 uv;
in vec4 LightPrepPos;

uniform vec3 WSLight;
uniform vec3 WSEye;
uniform vec3 LightColor;
uniform vec3 AmbientLight;
uniform vec3 Color;
uniform float Specularity;
uniform sampler2D InputTexture;
uniform sampler2D shadowMap;

out vec4 Res;

float CalculateShadow(vec3 LightVector)
{
    vec3 ProjectedCoordinates = LightPrepPos.xyz / LightPrepPos.w;
    ProjectedCoordinates = ProjectedCoordinates * 0.5 + 0.5;
    if(ProjectedCoordinates.z > 1.0)
    {
        return 0.0;
    }
    float closestDepth = texture(shadowMap, ProjectedCoordinates.xy).r;
    float currentDepth = ProjectedCoordinates.z;
    float bias = max(0.05 * (1.0 - dot(WSNomral, LightVector)), 0.005);
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, ProjectedCoordinates.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    return shadow;
}

void main()
{
    float dist = distance(WSPosition, WSLight);
    //Diffuse
    vec3 LightVector = normalize(WSLight - WSPosition);
    float diffuse = dot(LightVector, WSNomral);
    vec3 diffuseLight = clamp(vec3(diffuse, diffuse, diffuse), 0, 1);
    //Specular
    vec3 ReflectionVector = reflect(-LightVector, WSNomral);
    vec3 EyeVector = normalize(WSEye - WSPosition);
    float Specular = pow(clamp(dot(EyeVector, ReflectionVector), 0, 1), Specularity);
    vec3 specularLight = clamp(vec3(Specular, Specular, Specular), 0, 1);
    //Result
    vec4 PixelColor = (texture(InputTexture, uv) * vec4(Color, 1));
    vec4 TotalLight = vec4(((1.0 - CalculateShadow(LightVector)) * (diffuseLight + specularLight)) + AmbientLight, 1) * vec4(LightColor * 1.7, 1);
    float Attenuation = 1.0 / (1.0 + 0.1 * dist + 0.01 * dist * dist);
    Res = PixelColor * TotalLight * Attenuation;
}