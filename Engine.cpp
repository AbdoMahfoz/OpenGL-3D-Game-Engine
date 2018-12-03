#include "Engine.h"

bool isInitalized = false;
GLFWwindow* MainWindow;
std::vector<void(*)()> routines;
std::thread *LogicThread, *RenderingThread;

std::mutex LogicMutex, RenderingMutex, LogicStarted, RenderStarted;

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
        //Rendering goes here
        RenderingMutex.unlock();
    }
}

void Engine::RegisterRoutine(void (*ptr)(), bool shouldCheck)
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
    delete LogicThread;
    delete RenderingThread;
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
                MainLoop();
            }
            glfwTerminate();
        }
    }
}