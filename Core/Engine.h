#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <thread>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "GameObject.h"
#include "AutoClean.h"
#include "ImportedAssets/EulerCamera.h"
#include "ImportedAssets/texture.h"
#include "ImportedAssets/Semaphore.h"
#include "Models/Cube.h"
#include "Lighting/LightSource.h"
#include "Models/BlenderModel.h"
#include "Models/ObjectLoader.h"
#include "ShaderManager.h"
#include "Input.h"
#include "AudioManager.h"

#ifndef RENDERARRAYELEMENT
#define RENDERARRAYELEMENT

struct RenderArrayElement
{
public:
	Model* model;
	std::vector<GameObject*> objs, buff;
	std::set<GameObject*> deleteBuff;
	float *verts, *uvs, *normals;
	GLushort* indices;
	RenderArrayElement(Model* model)
	{
		this->model = model;
		verts = uvs = normals = nullptr;
		indices = nullptr;
	}
	~RenderArrayElement()
	{
		delete[] verts, uvs, normals, indices;
	}
};

#endif

#ifndef ENGINE
#define ENGINE

class Engine
{
private:
	friend class Model;
	friend class GameObject;
	friend class LightSource;
	Engine();
	static GLFWwindow* CreateWindow(int width, int height, const char* title);
	static void FlushBuffers();
	static void Logic();
	static void Rendering();
	static void MainLoop();
	static int GetFrameNumber();
	static void RegisterLight(LightSource* light);
	static void UnRegisterLight(LightSource* light);
	static void RegisterModel(RenderArrayElement* element);
	static void UnRegisterModel(RenderArrayElement* element);
	static void RegisterGameObject(GameObject* obj);
	static void UnRegisterGameObject(GameObject* obj);
public:
	static void FireEngine();
	static void Start();
	static void SetClearColor(const glm::vec3& color);
	static void RegisterRoutine(void (*func)(), bool shouldCheck);
	static void UnRegisterRoutine(void (*func)());
	static void RegisterOnExit(void (*func)());
	static void UnRegisterOnExit(void (*func)());
	static void SetAmbientLight(const glm::vec3& light);
	static void HideCursor();
	static void ShowCursor();
	static EulerCamera& GetCurrentCamera();
	static void SetCurrentCamera(EulerCamera* Camera);
	static void SetMainCameraAsCurrent();
	static void Terminate();
};

#endif