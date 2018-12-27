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
#include "EulerCamera.h"
#include "texture.h"
#include "CubeModel.h"
#include "ShaderManager.h"
#include "LightSource.h"
#include "BlenderModel.h"
#include "ObjectLoader.h"

namespace Engine
{
    void FireEngine();
    void Start();
    void SetClearColor(const glm::vec3& color);
    void RegisterRoutine(void (*func)(), bool shouldCheck);
    void UnRegisterRoutine(void (*func)());
    void RegisterOnExit(void (*func)());
    void UnRegisterOnExit(void (*func)());
    void RegisterLight(LightSource* light);
    void UnRegisterLight(LightSource* light);
    void RegisterModel(Model* model);
    void RegisterGameObject(GameObject* obj);
    void UnRegisterGameObject(GameObject* obj);
    EulerCamera& GetCurrentCamera();
    void SetCurrentCamera(EulerCamera* Camera);
    void SetMainCameraAsCurrent();
}