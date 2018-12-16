#include "Engine.h"

bool isInitalized = false;
GLFWwindow* MainWindow;
std::vector<void(*)()> routines, exitFuncs;
std::vector<std::pair<Model*, std::vector<GameObject*>>> RenderArray;
std::thread *LogicThread, *RenderingThread;
std::mutex LogicMutex, RenderingMutex, LogicStarted, RenderStarted;
GLuint VertexArrayID;
FPCamera MainCamera, *CurrentCamera;

//------------Start of Private functions(Inaccessable outside of this file)---------
GLFWwindow* CreateWindow(int width, int height, const char* title)
{   
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
    /*while(glfwWindowShouldClose(MainWindow) == 0)
    {
        LogicMutex.unlock();
        LogicStarted.lock();
        LogicMutex.lock();
        LogicStarted.unlock();*/
        for(auto i : routines)
        {
            i();
        }
    //}
}
void Rendering()
{
    /*glfwMakeContextCurrent(MainWindow);
    while(glfwWindowShouldClose(MainWindow) == 0)
    {
        RenderingMutex.unlock();
        RenderStarted.lock();
        RenderingMutex.lock();
        RenderStarted.unlock();*/
        CurrentCamera->UpdateViewMatrix();
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        for(auto i : RenderArray)
        {
            i.first->SetUpEnviroment(CurrentCamera->GetProjectionMatrix(), CurrentCamera->GetViewMatrix(),
                                     glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, -4.0f), 
                                     glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.1f, 0.1f, 0.1f), 50);
            for(auto j : i.second)
            {
                i.first->Draw(*j);
            }
        }
    //}
}
void MainLoop()
{
    Engine::Start();
    //glfwSwapInterval(60);
    do
    {
        /*LogicMutex.unlock();
        RenderingMutex.unlock();
        LogicStarted.lock();
        RenderStarted.lock();
        LogicMutex.lock();
        RenderingMutex.lock();
        LogicStarted.unlock();
        RenderStarted.unlock();*/
	    glfwPollEvents();
        Logic();
        Rendering();
        glfwSwapBuffers(MainWindow);
    }
    while(glfwWindowShouldClose(MainWindow) == 0);
    /*LogicMutex.unlock();
    RenderingMutex.unlock();
    LogicThread->join();
    RenderingThread->join();
    delete LogicThread;
    delete RenderingThread;*/
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
            /*LogicMutex.lock();
            RenderingMutex.lock();
            LogicThread = new std::thread(Logic);
            RenderingThread = new std::thread(Rendering);*/
            if(glewInit() == GLEW_OK)
            {
                MainCamera.Walk(-0.5f);
                MainCamera.SetPerspectiveProjection(75.0f, 800.0f/600.0f, 0.1f, 1000.0f);
                MainCamera.UpdateViewMatrix();
                CurrentCamera = &MainCamera;
                glewExperimental = true;
                glCreateVertexArrays(1, &VertexArrayID);
                glBindVertexArray(VertexArrayID);
                SetClearColor(glm::vec3(1.0f, 0.5f, 1.0f));
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LESS);
                MainLoop();
            }
        }
    }
}
void Engine::SetClearColor(const glm::vec3& color)
{
    glClearColor(color.x, color.y, color.z, 1.0f);
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
FPCamera& Engine::GetCurrentCamera()
{
    return *CurrentCamera;
}
void SetCurrentCamera(FPCamera* Camera)
{
    CurrentCamera = Camera;
    CurrentCamera->SetPerspectiveProjection(75.0f, 800.0f/600.0f, 0.1f, 1000.0f);
}
void Engine::SetMainCameraAsCurrent()
{
    CurrentCamera = &MainCamera;
}