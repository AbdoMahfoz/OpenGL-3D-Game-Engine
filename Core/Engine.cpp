#include "Engine.h"
#include "GameObject.h"
#include "Model.h"

bool isInitalized = false;
GLFWwindow* MainWindow;
std::vector<void(*)()> routines;
std::vector<std::pair<Model*, std::vector<const GameObject*>>> RenderArray;
std::thread *LogicThread, *RenderingThread;
std::mutex LogicMutex, RenderingMutex, LogicStarted, RenderStarted;
GLuint VertexArrayID;

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
    while(glfwWindowShouldClose(MainWindow) == 0)
    {
        LogicStarted.lock();
        LogicMutex.lock();
        LogicStarted.unlock();
        for(auto i : routines)
        {
            i();
        }
        LogicMutex.unlock();
    }
}
void Rendering()
{
    while(glfwWindowShouldClose(MainWindow) == 0)
    {
        RenderStarted.lock();
        RenderingMutex.lock();
        RenderStarted.unlock();
        for(auto i : RenderArray)
        {
            i.first->SetUpEnviroment();
            for(auto j : i.second)
            {
                i.first->Draw(*j);
            }
            i.first->CleanUpEnviroment();
        }
        RenderingMutex.unlock();
    }
}
void MainLoop()
{
    Engine::Start();
    do
    {
        LogicMutex.unlock();
        RenderingMutex.unlock();
        LogicStarted.lock();
        LogicStarted.unlock();
        RenderStarted.lock();
        RenderStarted.unlock();
        LogicMutex.lock();
        RenderingMutex.lock();
        glfwSwapBuffers(MainWindow);
	    glfwPollEvents();
    }
    while(glfwWindowShouldClose(MainWindow) == 0);
    LogicMutex.unlock();
    RenderingMutex.unlock();
    LogicThread->join();
    RenderingThread->join();
    delete LogicThread;
    delete RenderingThread;
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
            LogicMutex.lock();
            RenderingMutex.lock();
            LogicThread = new std::thread(Logic);
            RenderingThread = new std::thread(Rendering);
            if(glewInit() == GLEW_OK)
            {
                glCreateVertexArrays(1, &VertexArrayID);
                glBindVertexArray(VertexArrayID);
                MainLoop();
            }
            glfwTerminate();
        }
    }
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
void Engine::RegisterModel(Model* model)
{
    RenderArray.push_back({model, std::vector<const GameObject*>()});
}
void Engine::RegisterGameObject(const GameObject* obj)
{
    RenderArray[obj->GetModel()->GetID()].second.push_back(obj);
}
void Engine::UnRegisterGameObject(const GameObject* obj)
{
    std::vector<const GameObject*>& arr = RenderArray[obj->GetModel()->GetID()].second;
    for(int i = 0; i < arr.size(); i++)
    {
        if(arr[i] == obj)
        {
            arr.erase(arr.begin() + i);
            return;
        }
    }
}