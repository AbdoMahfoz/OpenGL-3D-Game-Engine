#include "Engine.h"

GameObject *Cube, *Plane, *Zombo;
LightSource *light, *light2;

void func()
{
    Cube->Rotate(glm::vec3(0.0f, 1.0f, 0.0f));
}
void Engine::Start()
{
    light = new LightSource(glm::vec3(2.0f, 1.0f, 2.0f));
    light2 = new LightSource(glm::vec3(-2.0f, 3.0f, -2.0f));
    Texture* tex = new Texture("uvtemplate.bmp", 0);
    CubeModel* c = new CubeModel();
    Cube = new GameObject(c, tex);
    Plane = new GameObject(c, tex);
    Plane->Scale(glm::vec3(100.0f, 0.2f, 100.0f));
    Plane->Translate(glm::vec3(0.0f, -5.0f, 0.0f));
    Engine::GetCurrentCamera().Walk(-4.0f);
    Engine::GetCurrentCamera().Strafe(4.0f);
    Engine::GetCurrentCamera().Yaw(45.0f);
    Engine::RegisterRoutine(func, false);
    AudioManager::PlayBackGroundMusic();
}