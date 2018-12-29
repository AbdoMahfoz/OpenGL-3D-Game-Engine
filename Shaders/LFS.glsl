#version 330 core

in vec3 WSPosition;
in vec3 WSNomral;
in vec2 uv;
in vec4 LightPrepPos[5];
flat in int LightCount;

uniform vec3 WSEye;
uniform vec3 AmbientLight;
uniform vec3 Color;
uniform float Specularity;
uniform sampler2D InputTexture;
uniform vec3 WSLight[30];
uniform vec3 LightColor[30];
uniform sampler2D shadowMap[30];

out vec4 Res;

float CalculateShadow(vec3 LightVector, vec4 LightPrepPos, sampler2D ss)
{
    vec3 ProjectedCoordinates = LightPrepPos.xyz / LightPrepPos.w;
    ProjectedCoordinates = ProjectedCoordinates * 0.5 + 0.5;
    if(ProjectedCoordinates.z > 1.0)
    {
        return 0.0;
    }
    float closestDepth = texture(ss, ProjectedCoordinates.xy).r;
    float currentDepth = ProjectedCoordinates.z;
    float bias = max(0.05 * (1.0 - dot(WSNomral, LightVector)), 0.005);
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(ss, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(ss, ProjectedCoordinates.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    return shadow;
}

void main()
{   
    vec4 PixelColor = (texture(InputTexture, uv) * vec4(Color, 1));
    vec4 TotalLight = vec4(AmbientLight, 1.0);
    float Attenuation = 0;
    for(int i = 0; i < LightCount; i++)
    {
        float dist = distance(WSPosition, WSLight[i]);
        Attenuation += 1.0 / (1.0 + 0.1 * dist + 0.01 * dist * dist);
        //Diffuse
        vec3 LightVector = normalize(WSLight[i] - WSPosition);
        float diffuse = dot(LightVector, WSNomral);
        vec3 diffuseLight = clamp(LightColor[i] * diffuse, 0, 1);
        //Specular
        vec3 ReflectionVector = reflect(-LightVector, WSNomral);
        vec3 EyeVector = normalize(WSEye - WSPosition);
        float Specular = pow(clamp(dot(EyeVector, ReflectionVector), 0, 1), Specularity);
        vec3 specularLight = clamp(LightColor[i] * Specular, 0, 1) * Specularity;
        //Result
        if(i == 0)
        {
            TotalLight += vec4(((1.0 - CalculateShadow(LightVector, LightPrepPos[i], shadowMap[0])) * (diffuseLight + specularLight)), 1);
        }
        if(i == 1)
        {
            TotalLight += vec4(((1.0 - CalculateShadow(LightVector, LightPrepPos[i], shadowMap[1])) * (diffuseLight + specularLight)), 1);
        }
        if(i == 2)
        {
            TotalLight += vec4(((1.0 - CalculateShadow(LightVector, LightPrepPos[i], shadowMap[2])) * (diffuseLight + specularLight)), 1);
        }
        if(i == 3)
        {
            TotalLight += vec4(((1.0 - CalculateShadow(LightVector, LightPrepPos[i], shadowMap[3])) * (diffuseLight + specularLight)), 1);
        }
        if(i == 4)
        {
            TotalLight += vec4(((1.0 - CalculateShadow(LightVector, LightPrepPos[i], shadowMap[4])) * (diffuseLight + specularLight)), 1);
        }
    }
    Res = PixelColor * TotalLight * Attenuation;
}