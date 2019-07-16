#ifndef LIGHTSOURCE_CLASS
#define LIGHTSOURCE_CLASS

#define DEFAULT_SHADOW_WIDTH 2048
#define DEFAULT_SHADOW_HEIGHT 2048
#define MAX_LIGHT_COUNT 30

#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "../ImportedAssets/EulerCamera.h"

class LightSource
{
private:
	friend class Engine;
	void BindDepthMap();
	const glm::mat4& GetLightVP();
	virtual void SetUpEnviroment();
protected:
    glm::mat4 ViewMatrix, ProjectionMatrix, VP;
    GLuint dmFrameBuffer, dmTexture;
    glm::vec3 LightColor, Position, LookAt;
    bool IsModified;
    int width, height;
    void InitializeBufferTexture(int width, int height);
	virtual ~LightSource();
public:
    LightSource(const glm::vec3& InitialPosition);
    LightSource(const glm::vec3& InitialPosition, int width, int height);
    void SetLightColor(const glm::vec3& LightColor);
    void SetPosition(const glm::vec3&);
    void Translate(const glm::vec3&);
    void SetLookAt(const glm::vec3&);
    const glm::vec3& GetLightColor();
    const glm::vec3& GetPosition();
};

#endif