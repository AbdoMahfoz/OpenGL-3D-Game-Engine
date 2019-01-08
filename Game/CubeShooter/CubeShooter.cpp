#include "GameScenes.h"
#include "PathFinidng.h"

LightSource* l;
Cube* MainCube;
glm::vec3 *LastPath1 = nullptr, *LastPath2 = nullptr;
std::vector<Cube*> LastCubes1, LastCubes2;
int count1 = 0, count2 = 0;
namespace CubeShooter
{
    void CameraMovment()
    {
        glm::vec2 delta = Input::GetMouseDelta();
        Engine::GetCurrentCamera().Pitch(-delta.y * 0.005f);
        Engine::GetCurrentCamera().Yaw(-delta.x * 0.005f);
        if(Input::IsKeyPressed(GLFW_KEY_W))
        {
            Engine::GetCurrentCamera().Walk(0.3f);
        }
        if(Input::IsKeyPressed(GLFW_KEY_S))
        {
            Engine::GetCurrentCamera().Walk(-0.3f);
        }
        if(Input::IsKeyPressed(GLFW_KEY_A))
        {
            Engine::GetCurrentCamera().Strafe(-0.3f);
        }
        if(Input::IsKeyPressed(GLFW_KEY_D))
        {
            Engine::GetCurrentCamera().Strafe(0.3f);
        }
    }
    void CallBack1(glm::vec3* arr, int count)
    {
        for(auto i : LastCubes1)
        {
            delete i;
        }
        LastCubes1.clear();
        delete[] LastPath1;
        LastPath1 = arr;
        count1 = count;
        for(int i = 0; i < count; i++)
        {
            Cube* c = new Cube();
            c->SetColor(glm::vec3(0.0f, 0.0f, 1.0f));
            c->Translate(arr[i]);
            LastCubes1.push_back(c);
        }
    }
    void CallBack2(glm::vec3* arr, int count)
    {
        for(auto i : LastCubes2)
        {
            delete i;
        }
        LastCubes2.clear();
        delete[] LastPath2;
        LastPath2 = arr;
        count2 = count;
        for(int i = 0; i < count; i++)
        {
            Cube* c = new Cube();
            c->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));                                              
            c->Translate(arr[i]);
            LastCubes2.push_back(c);
        }
    }
    glm::vec3 LastPos;
    void PlayerMovment()
    {
        bool changed = false;
        if(Input::IsKeyPressed(GLFW_KEY_UP))
        {
            changed = true;
            MainCube->Translate(glm::vec3(0.0f, 0.0f, 0.1f));
        }
        if(Input::IsKeyPressed(GLFW_KEY_DOWN))
        {
            changed = true;
            MainCube->Translate(glm::vec3(0.0f, 0.0f, -0.1f));
        }
        if(Input::IsKeyPressed(GLFW_KEY_LEFT))
        {
            changed = true;
            MainCube->Translate(glm::vec3(0.1f, 0.0f, 0.0f));
        }
        if(Input::IsKeyPressed(GLFW_KEY_RIGHT))
        {
            changed = true;
            MainCube->Translate(glm::vec3(-0.1f, 0.0f, 0.0f));
        }
        if(MainCube->GetPosition().x / 2 != LastPos.x ||
           MainCube->GetPosition().z / 2 != LastPos.z)
        {
            LastPos.x = MainCube->GetPosition().x / 2;
            LastPos.z = MainCube->GetPosition().z / 2;
            PathFinidng::RequestPath(glm::vec3(5.0f, 5.0f, 5.0f), MainCube->GetPosition(),
                LastPath1, count1, CubeShooter::CallBack1);
            PathFinidng::RequestPath(glm::vec3(8.0f, 5.0f, 3.0f), MainCube->GetPosition(),
                LastPath2, count2, CubeShooter::CallBack2);
        }
    }
}
void GameScenes::CubeShooter()
{
    l = new LightSource(glm::vec3(0.0f, 5.0f, 0.0f));
    MainCube = new Cube();
    Engine::GetCurrentCamera().Fly(5.0f);
    PathFinidng::RequestPath(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f),
        nullptr, 0, CubeShooter::CallBack1);
    PathFinidng::RequestPath(glm::vec3(8.0f, 5.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
        nullptr, 0, CubeShooter::CallBack2);
    Engine::RegisterRoutine(CubeShooter::CameraMovment, false);
    Engine::RegisterRoutine(CubeShooter::PlayerMovment, false);
}