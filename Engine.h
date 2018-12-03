#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <iostream>


namespace Engine
{
    void FireEngine();
    void Start();
    void RegisterRoutine(void (*func)(), bool shouldCheck = false);
    void UnRegisterRoutine(void (*func)());
}

