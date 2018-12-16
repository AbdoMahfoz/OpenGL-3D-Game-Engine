#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "GameObject.h"
#include "AutoClean.h"
#include "FPCamera.h"
#include "texture.h"
#include "CubeModel.h"
#include "ShaderManager.h"
#include "LightModel.h"

namespace Engine
{
    void FireEngine();
    void Start();
    void SetClearColor(const glm::vec3& color);
    void RegisterRoutine(void (*func)(), bool shouldCheck);
    void UnRegisterRoutine(void (*func)());
    void RegisterOnExit(void (*func)());
    void UnRegisterOnExit(void (*func)());
    void RegisterModel(Model* model);
    void RegisterGameObject(GameObject* obj);
    void UnRegisterGameObject(GameObject* obj);
    FPCamera& GetCurrentCamera();
    void SetCurrentCamera(FPCamera* Camera);
    void SetMainCameraAsCurrent();
}