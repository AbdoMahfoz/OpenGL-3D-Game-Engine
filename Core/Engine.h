#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "GameObject.h"
#include "AutoClean.h"
#ifdef _WIN32
	#include "ImportedAssets/EulerCamera.h"
	#include "ImportedAssets/texture.h"
	#include "Models/Cube.h"
	#include "Lighting/LightSource.h"
	#include "Models/BlenderModel.h"
	#include "Models/ObjectLoader.h"
#else
	#include "EulerCamera.h"
	#include "texture.h"
	#include "Cube.h"
	#include "LightSource.h"
	#include "BlenderModel.h"
	#include "ObjectLoader.h"
#endif
#include "ShaderManager.h"
#include "Input.h"
#include "AudioManager.h"

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
    void Terminate();
}