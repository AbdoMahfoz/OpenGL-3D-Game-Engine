#ifndef LIGHTSOURCE_CLASS
#define LIGHTSOURCE_CLASS

#define DEFAULT_SHADOW_WIDTH 2048
#define DEFAULT_SHADOW_HEIGHT 2048
#define MAX_LIGHT_COUNT 30

#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "EulerCamera.h"

class LightSource
{
protected:
    glm::mat4 ViewMatrix, ProjectionMatrix, VP;
    GLuint dmFrameBuffer, dmTexture;
    glm::vec3 LightColor, Position, LookAt;
    bool IsModified;
    int width, height;
    void InitializeBufferTexture(int width, int height);
public:
    LightSource(const glm::vec3& InitialPosition);
    LightSource(const glm::vec3& InitialPosition, int width, int height);
    virtual void SetUpEnviroment();
    void SetLightColor(const glm::vec3& LightColor);
    void BindDepthMap();
    const glm::mat4& GetLightVP();
    const glm::vec3& GetLightColor();
    const glm::vec3& GetPosition();
    ~LightSource();
};

#endif