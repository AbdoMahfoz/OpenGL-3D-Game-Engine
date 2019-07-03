#include "../GameScenes.h"
#include "PathFinding/PathFinidng.h"
#include "Actor/Zombie.h"
#include "../../Core/Engine.h"

LightSource* l;
Cube *MainCube;
Zombie* z[4];
namespace CubeShooter
{
    Cube *Plane;
    void CameraMovment()
    {
        glm::vec2 delta = Input::GetMouseDelta();
        MainCube->Rotate(glm::vec3(0.0f, delta.x, 0.0f));
    }
    void PlayerMovment()
    {
        if(Input::IsKeyPressed(GLFW_KEY_UP))
        {
            MainCube->Translate(glm::vec3(0.0f, 0.0f, 0.1f));
        }
        if(Input::IsKeyPressed(GLFW_KEY_DOWN))
        {
            MainCube->Translate(glm::vec3(0.0f, 0.0f, -0.1f));
        }
        if(Input::IsKeyPressed(GLFW_KEY_LEFT))
        {
            MainCube->Translate(glm::vec3(0.1f, 0.0f, 0.0f));
        }
        if(Input::IsKeyPressed(GLFW_KEY_RIGHT))
        {
            MainCube->Translate(glm::vec3(-0.1f, 0.0f, 0.0f));
        }
    }
    void CleanUp()
    {
        delete l;
        delete MainCube;
        for(int i = 0; i < 4; i++)
        {
            delete z[i];
        }
    }
    void ZombieHitMe(GameObject* o)
    {
        for(int i = 0; i < 4; i++)
        {
            if(o == z[i])
            {
                Engine::Terminate();
            }
        }
    }
}
void GameScenes::CubeShooter()
{
    //AudioManager::PlayBackGroundMusic();
    GameObject::EnableCollision = false;
    l = new LightSource(glm::vec3(0.0f, 5.0f, 0.0f));
    l->SetLightColor(glm::vec3(1.0f) * 100.0f);
    MainCube = new Cube();
    MainCube->CallBack = CubeShooter::ZombieHitMe;
    MainCube->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
    CubeShooter::Plane = new Cube();
    CubeShooter::Plane->Scale(glm::vec3(10000.0f, 0.1f, 10000.0f));
    CubeShooter::Plane->Translate(glm::vec3(0.0f, -1.0f, 0.0f));
    z[0] = new Zombie(glm::vec3(0.0f, 0.0f, 15.0f), MainCube);
	//z[0]->SetDebugging(true);
    z[1] = new Zombie(glm::vec3(0.0f, 0.0f, 10.0f), MainCube);
	//z[1]->SetDebugging(true);
    /*
    z[2] = new Zombie(glm::vec3(-10.0f, 0.0f, -10.0f), MainCube);
    z[3] = new Zombie(glm::vec3(10.0f, 0.0f, -10.0f), MainCube);
    */
    Engine::GetCurrentCamera().Fly(60.0f);
    Engine::GetCurrentCamera().Pitch(glm::radians(-89.0f));
    Engine::GetCurrentCamera().Roll(glm::radians(45.0f));
    //Engine::RegisterRoutine(CubeShooter::CameraMovment, false);
    Engine::RegisterRoutine(CubeShooter::PlayerMovment, false);
    Engine::RegisterOnExit(CubeShooter::CleanUp);
}