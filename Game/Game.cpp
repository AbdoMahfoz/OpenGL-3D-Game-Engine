#include "Engine.h"

GameObject *Cube, *Plane, *Zombo;
LightSource *light, *light2;

void func()
{
    //Cube->Rotate(glm::vec3(0.0f, 1.0f, 0.0f));
    light->GetCam().Reset(light->GetCam().GetEyePosition(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    light->GetCam().Strafe(-0.01f);
    light->GetCam().Reset(light->GetCam().GetEyePosition(), Zombo->GetPosition(), glm::vec3(0.0f, 1.0f, 0.0f));
}
void Engine::Start()
{
    light = new LightSource();
    light->GetCam().SetPerspectiveProjection(75.0f, (float)DEFAULT_SHADOW_WIDTH/DEFAULT_SHADOW_HEIGHT, 1.0f, 7.5f);
    light->GetCam().Reset(glm::vec3(2.0f, 1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    light->GetCam().UpdateViewMatrix();
    Texture* tex = new Texture("uvtemplate.bmp", 0);
    Zombo = new GameObject(new BlenderModel("Zombie_1.obj"), tex);
    Zombo->Scale(glm::vec3(0.1f, 0.1f, 0.1f));
    Zombo->Translate(glm::vec3(0.0f, -4.0f, 20.0f));
    CubeModel* c = new CubeModel();
    //Cube = new GameObject(c, tex);
    Plane = new GameObject(c, tex);
    Plane->Scale(glm::vec3(100.0f, 0.2f, 100.0f));
    Plane->Translate(glm::vec3(0.0f, -5.0f, 0.0f));
    Engine::GetCurrentCamera().Walk(-4.0f);
    Engine::GetCurrentCamera().Strafe(4.0f);
    Engine::GetCurrentCamera().Yaw(45.0f);
    Engine::RegisterRoutine(func, false);
}