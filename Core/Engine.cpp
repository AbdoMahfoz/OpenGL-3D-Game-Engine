#include "Engine.h"

bool isInitalized = false;
GLFWwindow* MainWindow;
std::vector<void(*)()> routines, exitFuncs;
std::vector<std::pair<Model*, std::vector<GameObject*>>> RenderArray;
std::vector<LightSource*> Lights;
GLuint VertexArrayID, shadowMap[MAX_LIGHT_COUNT];
EulerCamera MainCamera, *CurrentCamera;
glm::vec3 AmbientLight = glm::vec3(0.5f, 0.5f, 0.5f);
int SCREEN_WIDTH, SCREEN_HEIGHT, FrameNumber = 0;
bool ProgramTerminated = false;
Semaphore logic, render;

//------------Start of Private functions(Inaccessable outside of this file)---------
GLFWwindow* CreateWindow(int width, int height, const char* title)
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
void Logic()
{
    while(!ProgramTerminated)
    {
		logic.wait();
        for(auto i : routines)
        {
            i();
        }
		render.notify();
    }
}
void Rendering()
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
            i.first->SetUpEnviroment(m);
            for(auto j : i.second)
            {
                i.first->Draw(*j);
            }
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    CurrentCamera->UpdateViewMatrix();
    for(auto i : RenderArray)
    {
        i.first->SetUpEnviroment(CurrentCamera->GetProjectionMatrix(), CurrentCamera->GetViewMatrix(),
                                    AmbientLight, CurrentCamera->GetEyePosition(), 
                                    &LightColor[0], &LightPosition[0], LightColor.size());
        for(auto j : i.second)
        {
            for(int l = 0; l < Lights.size(); l++)
            {
                glActiveTexture(GL_TEXTURE1 + l);
                glUniform1i(shadowMap[l], l + 1);
                Lights[l]->BindDepthMap();
            }
            i.first->Draw(*j);
        }
    }
}
void MainLoop()
{
	std::thread* LogicThread = new std::thread(Logic);
    Engine::Start();
	Engine::HideCursor();
    do
    {
	    glfwPollEvents();
        Input::CalculateDelta();
		logic.notify();
        Rendering();
		render.wait();
        glfwSwapBuffers(MainWindow);
		FrameNumber = !FrameNumber;
    }
    while(glfwWindowShouldClose(MainWindow) == 0);
	ProgramTerminated = true;
	Engine::ShowCursor();
	logic.notify();
    LogicThread->join();
    delete LogicThread;
    for(auto i : exitFuncs)
    {
        i();
    }
    glfwTerminate();
}
//------------End of Private functions(Inaccessable outside of this file)---------
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
    for(int i = 0; i < routines.size(); i++)
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
    for(int i = 0; i < exitFuncs.size(); i++)
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
    assert(Lights.size() != 5);
    Lights.push_back(light);
}
void Engine::UnRegisterLight(LightSource* light)
{
    for(int i = 0; i < Lights.size(); i++)
    {
        if(Lights[i] == light)
        {
            Lights.erase(Lights.begin() + i);
            break;
        }
    }
}
void Engine::RegisterModel(Model* model)
{
    RenderArray.push_back({model, std::vector<GameObject*>()});
}
void Engine::RegisterGameObject(GameObject* obj)
{
    RenderArray[obj->GetModel()->GetID()].second.push_back(obj);
}
void Engine::UnRegisterGameObject(GameObject* obj)
{
    std::vector<GameObject*>& arr = RenderArray[obj->GetModel()->GetID()].second;
    for(int i = 0; i < arr.size(); i++)
    {
        if(arr[i] == obj)
        {
            arr.erase(arr.begin() + i);
            return;
        }
    }
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