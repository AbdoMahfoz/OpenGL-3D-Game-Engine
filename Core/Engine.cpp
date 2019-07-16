#include "Engine.h"


bool isInitalized = false;
GLFWwindow* MainWindow;
std::set<RenderArrayElement*> invalidatedModels;
std::vector<void(*)()> routines, exitFuncs;
std::vector<RenderArrayElement*> RenderArray, RenderArrayBuffer, RenderArrayEraseBuffer;
std::vector<LightSource*> Lights, LightBuffer;
std::set<LightSource*> LightEraseBuffer;
GLuint VertexArrayID, shadowMap[MAX_LIGHT_COUNT];
EulerCamera MainCamera, *CurrentCamera;
glm::vec3 AmbientLight = glm::vec3(0.5f, 0.5f, 0.5f);
int SCREEN_WIDTH, SCREEN_HEIGHT, FrameNumber = 0;
bool ProgramTerminated = false;
Semaphore logic, render;

GLFWwindow* Engine::CreateWindow(int width, int height, const char* title)
{   
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    glfwMakeContextCurrent(window);
    return window;
}
void Engine::FlushBuffers()
{
	if (!RenderArrayBuffer.empty())
	{
		for (auto i : RenderArrayBuffer)
		{
			RenderArray.push_back(i);
		}
		RenderArrayBuffer.clear();
	}
	if (!invalidatedModels.empty())
	{
		for (auto i : invalidatedModels)
		{
			std::vector<GameObject*>& objs = i->objs;
			std::vector<GameObject*>& buff = i->buff;
			std::set<GameObject*>& deleteBuff = i->deleteBuff;
			if (buff.size() > 0)
			{
				objs.reserve(objs.size() + buff.size());
				for (auto j : buff)
				{
					objs.push_back(j);
				}
				buff.clear();
			}
			for (unsigned int k = 0; k < objs.size() && !deleteBuff.empty(); k++)
			{
				auto itr = deleteBuff.find(objs[k]);
				if (itr != deleteBuff.end())
				{
					delete objs[k];
					objs.erase(objs.begin() + k);
					deleteBuff.erase(itr);
					k--;
				}
			}
			deleteBuff.clear();
		}
		invalidatedModels.clear();
	}
	if (!LightBuffer.empty())
	{
		Lights.reserve(Lights.size() + LightBuffer.size());
		for (auto i : LightBuffer)
		{
			Lights.push_back(i);
		}
		LightBuffer.clear();
	}
	if (!LightEraseBuffer.empty())
	{
		for (unsigned int i = 0; i < Lights.size(); i++)
		{
			auto itr = LightEraseBuffer.find(Lights[i]);
			if (itr != LightEraseBuffer.end())
			{
				delete Lights[i];
				Lights.erase(Lights.begin() + i);
				LightEraseBuffer.erase(itr);
				i--;
			}
		}
	}
	if (!RenderArrayEraseBuffer.empty())
	{
		for (auto i : RenderArrayEraseBuffer)
		{
			for (auto j : i->objs)
			{
				delete j;
			}
			RenderArray.erase(find(RenderArray.begin(), RenderArray.end(), i));
			delete i->model;
			delete i;
		}
	}
}
void Engine::Logic()
{
    while(!ProgramTerminated)
    {
		logic.wait();
		if (ProgramTerminated) break;
        for(auto i : routines)
        {
            i();
        }
		render.notify();
    }
}
void Engine::Rendering()
{
    std::vector<glm::vec3> LightColor, LightPosition;
    for(auto l : Lights)
    {
        LightColor.push_back(l->GetLightColor());
        LightPosition.push_back(l->GetPosition());
        l->SetUpEnviroment();
        //l->GetCam().UpdateViewMatrix();
        const glm::mat4& m = l->GetLightVP();
        for(auto i : RenderArray)
        {
            i->model->SetUpEnviroment(m);
            for(int j = 0; j < (int)i->objs.size(); j++)
            {
                i->model->BufferData(*(i->objs[j]), j);
            }
			i->model->Draw(i->objs.size());
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    CurrentCamera->UpdateViewMatrix();
    for(auto i : RenderArray)
    {
        i->model->SetUpEnviroment(CurrentCamera->GetProjectionMatrix(), CurrentCamera->GetViewMatrix(),
                                    AmbientLight, CurrentCamera->GetEyePosition(), 
                                    &LightColor[0], &LightPosition[0], LightColor.size());
        for(unsigned int l = 0; l < Lights.size(); l++)
        {
            glActiveTexture(GL_TEXTURE1 + l);
            glUniform1i(shadowMap[l], l + 1);
            Lights[l]->BindDepthMap();
        }
		for (int j = 0; j < (int)i->objs.size(); j++)
		{
			i->model->BufferData(*(i->objs[j]), j);
		}
		i->model->Draw(i->objs.size());
    }
}
void Engine::MainLoop()
{
	std::thread* LogicThread = new std::thread(Logic);
    Engine::Start();
	glfwSwapInterval(1);
	logic.notify();
    do
    {
        Rendering();
		render.wait();
	    glfwPollEvents();
		FrameNumber = !FrameNumber;
		FlushBuffers();
		logic.notify();
        glfwSwapBuffers(MainWindow);
    }
    while(glfwWindowShouldClose(MainWindow) == 0);
	ProgramTerminated = true;
	logic.notify();
    LogicThread->join();
    delete LogicThread;
    for(auto i : exitFuncs)
    {
        i();
    }
    glfwTerminate();
}
void Engine::FireEngine()
{
    if(!isInitalized)
    {
        isInitalized = false;
        if(glfwInit())
        {
            MainWindow = CreateWindow(800, 600, "Engine");
            if(glewInit() == GLEW_OK)
            {
                MainCamera.Walk(-0.5f);
                MainCamera.SetPerspectiveProjection(75.0f, (float)SCREEN_WIDTH/SCREEN_HEIGHT, 0.1f, 1000.0f);
                MainCamera.UpdateViewMatrix();
                CurrentCamera = &MainCamera;
				glewExperimental = true;
                glCreateVertexArrays(1, &VertexArrayID);
                glBindVertexArray(VertexArrayID);
                //SetClearColor(glm::vec3(1.0f, 0.5f, 1.0f));
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LESS);
                glEnable(GL_TEXTURE_2D);
                GLuint pID = ShaderManager::GetShaders("Shaders/LVS.glsl", "Shaders/LFS.glsl");
                for(int i = 0; i < MAX_LIGHT_COUNT; i++)
                {
                    shadowMap[i] = glGetUniformLocation(pID, ("shadowMap[" + std::to_string(i) + "]").c_str());
                }
                glfwSetInputMode(MainWindow, GLFW_STICKY_KEYS, GL_TRUE);
                glfwSetKeyCallback(MainWindow, &Input::KeyCallBack);
                Input::SetWindow(MainWindow);
                MainLoop();
            }
        }
    }
}
int Engine::GetFrameNumber()
{
	return FrameNumber;
}
void Engine::SetClearColor(const glm::vec3& color)
{
    glClearColor(color.x, color.y, color.z, 1.0f);
}
void Engine::SetAmbientLight(const glm::vec3& light)
{
	AmbientLight = light;
}
void Engine::HideCursor()
{
	glfwSetInputMode(MainWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}
void Engine::ShowCursor()
{
	glfwSetInputMode(MainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
void Engine::RegisterRoutine(void (*ptr)(), bool shouldCheck = false)
{
    if(shouldCheck)
    {
        for(auto i : routines)
        {
            if(i == ptr)
            {
                return;
            }
        }
    }
    routines.push_back(ptr);
}
void Engine::UnRegisterRoutine(void (*ptr)())
{
    for(unsigned int i = 0; i < routines.size(); i++)
    {
        if(routines[i] == ptr)
        {
            routines.erase(routines.begin() + i);
            return;
        }
    }
}
void Engine::RegisterOnExit(void (*ptr)())
{
    exitFuncs.push_back(ptr);
}
void Engine::UnRegisterOnExit(void (*ptr)())
{
    for(unsigned int i = 0; i < exitFuncs.size(); i++)
    {
        if(exitFuncs[i] == ptr)
        {
            exitFuncs.erase(exitFuncs.begin() + i);
            return;
        }
    }
}
void Engine::RegisterLight(LightSource* light)
{
    assert(Lights.size() + LightBuffer.size() - LightEraseBuffer.size() != 5);
    LightBuffer.push_back(light);
}
void Engine::UnRegisterLight(LightSource* light)
{
	LightEraseBuffer.insert(light);
}
void Engine::RegisterModel(RenderArrayElement* element)
{
	RenderArrayBuffer.push_back(element);
}
void Engine::UnRegisterModel(RenderArrayElement* element)
{
	RenderArrayEraseBuffer.push_back(element);
}
void Engine::RegisterGameObject(GameObject* obj)
{
	RenderArrayElement* rend = obj->GetModel()->GetRenderElement();
	rend->buff.push_back(obj);
	invalidatedModels.insert(rend);
}
void Engine::UnRegisterGameObject(GameObject* obj)
{
	RenderArrayElement* rend = obj->GetModel()->GetRenderElement();
	rend->deleteBuff.insert(obj);
	invalidatedModels.insert(rend);
}
EulerCamera& Engine::GetCurrentCamera()
{
    return *CurrentCamera;
}
void Engine::SetCurrentCamera(EulerCamera* Camera)
{
    CurrentCamera = Camera;
    CurrentCamera->SetPerspectiveProjection(75.0f, 800.0f/600.0f, 0.1f, 1000.0f);
}
void Engine::SetMainCameraAsCurrent()
{
    CurrentCamera = &MainCamera;
}
void Engine::Terminate()
{
    glfwSetWindowShouldClose(MainWindow, 1);
}