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
    EulerCamera cam;
    GLuint dmFrameBuffer, dmTexture;
    glm::vec3 LightColor;
    int width, height;
    void InitializeBufferTexture(int width, int height);
public:
    LightSource();
    LightSource(int width, int height);
    virtual void SetUpEnviroment();
    void SetLightColor(const glm::vec3& LightColor);
    void BindDepthMap();
    glm::mat4 GetLightVP();
    const glm::vec3& GetLightColor();
    EulerCamera& GetCam();
    ~LightSource();
};

#endif