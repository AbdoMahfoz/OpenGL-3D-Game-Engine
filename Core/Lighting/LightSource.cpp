#include "Engine.h"

//glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
//glm::mat4 lightProjection = glm::perspective(75.0f, (float)SHADOW_WIDTH/SHADOW_HEIGHT, 1.0f, 7.5f);
//glm::mat4 lightView = glm::lookAt(LightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

void LightSource::InitializeBufferTexture(int width, int height)
{
    glGenFramebuffers(1, &dmFrameBuffer);
    glGenTextures(1, &dmTexture);
    //Set up textures
    glBindTexture(GL_TEXTURE_2D, dmTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                 width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    //Bind frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, dmFrameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dmTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
LightSource::LightSource(const glm::vec3& InitialPosition)
{
    IsModified = true;
    LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    InitializeBufferTexture(DEFAULT_SHADOW_WIDTH, DEFAULT_SHADOW_HEIGHT);
    this->ProjectionMatrix = glm::perspective(75.0f, (float)DEFAULT_SHADOW_WIDTH/DEFAULT_SHADOW_HEIGHT, 1.0f, 7.5f);
    this->Position = InitialPosition;
    this->width = DEFAULT_SHADOW_WIDTH;
    this->height = DEFAULT_SHADOW_HEIGHT;
    Engine::RegisterLight(this);
}
LightSource::LightSource(const glm::vec3& InitialPosition, int width, int height)
{
    IsModified = true;
    LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    InitializeBufferTexture(width, height);
    this->ProjectionMatrix = glm::perspective(75.0f, (float)width/height, 1.0f, 7.5f);
    this->Position = InitialPosition;
    this->width = width;
    this->height = height;
    Engine::RegisterLight(this);
}
void LightSource::SetUpEnviroment()
{
    glBindFramebuffer(GL_FRAMEBUFFER, dmFrameBuffer);
    glViewport(0, 0, width, height);
    glClear(GL_DEPTH_BUFFER_BIT);
}
void LightSource::SetLightColor(const glm::vec3& LightColor)
{
    this->LightColor = LightColor;
}
void LightSource::BindDepthMap()
{
    glBindTexture(GL_TEXTURE_2D, dmTexture);
}
const glm::mat4& LightSource::GetLightVP()
{
    if(IsModified)
    {
        IsModified = false;
        ViewMatrix = glm::lookAt(Position, LookAt, glm::vec3(0.0f, 1.0f, 0.0f));
        VP = ProjectionMatrix * ViewMatrix;
    }
    return VP;
    //return cam.GetProjectionMatrix() * cam.GetViewMatrix();
    /*
    return glm::perspective(75.0f, (float)width/height, 1.0f, 7.5f) * 
           glm::lookAt(glm::vec3(2.0f, 1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    */
}
const glm::vec3& LightSource::GetLightColor()
{
    return LightColor;
}
const glm::vec3& LightSource::GetPosition()
{
    return Position;
}
LightSource::~LightSource()
{
    Engine::UnRegisterLight(this);
}