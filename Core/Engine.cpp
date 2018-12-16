#include "Engine.h"

bool isInitalized = false;
GLFWwindow* MainWindow;
std::vector<void(*)()> routines, exitFuncs;
std::vector<std::pair<Model*, std::vector<GameObject*>>> RenderArray;
std::thread *LogicThread, *RenderingThread;
std::mutex LogicMutex, RenderingMutex, LogicStarted, RenderStarted;
GLuint VertexArrayID;
FPCamera MainCamera, *CurrentCamera;
GLuint depthMapFB, depthMapTX, depthMapID;
glm::vec3 LightPos;
int SHADOW_WIDTH, SHADOW_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT;

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
void SetUpShadowMap(int width, int height)
{
    SHADOW_WIDTH = width;
    SHADOW_HEIGHT = height;
    //Create frame buffer and texture
    glGenFramebuffers(1, &depthMapFB);
    glGenTextures(1, &depthMapTX);
    //Set up textures
    glBindTexture(GL_TEXTURE_2D, depthMapTX);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                 width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    //Bind frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFB);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTX, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
    depthMapID = glGetUniformLocation(ShaderManager::GetShaders("Shaders/LVS.glsl", "Shaders/LFS.glsl"), "shadowMap");

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
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFB);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        //glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
        glm::mat4 lightProjection = glm::perspective(75.0f, (float)SHADOW_WIDTH/SHADOW_HEIGHT, 1.0f, 7.5f);
        glm::mat4 lightView = glm::lookAt(LightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glClear( GL_DEPTH_BUFFER_BIT );
        for(auto i : RenderArray)
        {
            i.first->SetUpEnviroment(lightProjection, lightView);
            for(auto j : i.second)
            {
                i.first->Draw(*j);
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        CurrentCamera->UpdateViewMatrix();
        for(auto i : RenderArray)
        {
            i.first->SetUpEnviroment(CurrentCamera->GetProjectionMatrix(), CurrentCamera->GetViewMatrix(),
                                     LightPos, glm::vec3(0.0f, 0.0f, 4.0f), 
                                     glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.1f, 0.1f, 0.1f), 50);
            for(auto j : i.second)
            {
                glActiveTexture(GL_TEXTURE1);
                glUniform1i(depthMapID, 1);
                glBindTexture(GL_TEXTURE_2D, depthMapTX);
                i.first->Draw(*j);
            }
        }
    //}
}
void MainLoop()
{
    Engine::Start();
    SetUpShadowMap(2048, 2048);
    LightPos = glm::vec3(2.0f, 2.0f, 2.0f);
    glEnable(GL_TEXTURE_2D);
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
            MainWindow = CreateWindow(1280, 720, "Engine");
            /*LogicMutex.lock();
            RenderingMutex.lock();
            LogicThread = new std::thread(Logic);
            RenderingThread = new std::thread(Rendering);*/
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