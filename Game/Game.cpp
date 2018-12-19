#include "Engine.h"

GameObject *Cube, *Plane;

void func()
{
    Cube->Rotate(glm::vec3(0.0f, 1.0f, 0.0f));
}
void Engine::Start()
{
    Texture* tex = new Texture("uvtemplate.bmp", 0);
    CubeModel* c = new CubeModel();
    Cube = new GameObject(c, tex);
    Plane = new GameObject(c, tex);
    Plane->Scale(glm::vec3(100.0f, 0.2f, 100.0f));
    Plane->Translate(glm::vec3(0.0f, -5.0f, 0.0f));
    Engine::GetCurrentCamera().Walk(-4.0f);
    Engine::RegisterRoutine(func, false);
}