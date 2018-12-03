#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "shader.hpp"

class Model;
class GameObject;

namespace Engine
{
    void FireEngine();
    void Start();
    void RegisterRoutine(void (*func)(), bool shouldCheck);
    void UnRegisterRoutine(void (*func)());
    void RegisterModel(Model* model);
    void RegisterGameObject(const GameObject* obj);
    void UnRegisterGameObject(const GameObject* obj);
}

