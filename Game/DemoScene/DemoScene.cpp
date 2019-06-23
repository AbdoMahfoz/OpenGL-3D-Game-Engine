#include "../GameScenes.h"

GameObject *StaticCube, *Plane, *Player;
LightSource *light, *light2;

void MouseLook()
{
    StaticCube->Rotate(glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec2 delta = Input::GetMouseDelta();
    Engine::GetCurrentCamera().Pitch(-delta.y * 0.005f);
    Engine::GetCurrentCamera().Yaw(-delta.x * 0.005f);
    Player->Rotate(glm::vec3(0.0f, -delta.x * 0.28656f, 0.0f));
    float temp = Engine::GetCurrentCamera().mAngleY;
    Engine::GetCurrentCamera().mAngleY = 0;
    Engine::GetCurrentCamera().UpdateViewMatrix();
    Engine::GetCurrentCamera().SetEyePosition(Player->GetPosition() + 1.3f * Engine::GetCurrentCamera().mDirection);
    Engine::GetCurrentCamera().mAngleY = temp;
    Engine::GetCurrentCamera().UpdateViewMatrix();
}
void PlayerMovment()
{
    if(Input::IsKeyPressed(GLFW_KEY_W))
    {
        Player->Translate(glm::vec3(0.0f, 0.0f, -0.1f));
    }
    if(Input::IsKeyPressed(GLFW_KEY_S))
    {
        Player->Translate(glm::vec3(0.0f, 0.0f, 0.1f));
    }
    if(Input::IsKeyPressed(GLFW_KEY_A))
    {
        Player->Translate(glm::vec3(-0.1f, 0.0f, 0.0f));
    }
    if(Input::IsKeyPressed(GLFW_KEY_D))
    {
        Player->Translate(glm::vec3(0.1f, 0.0f, 0.0f));
    }
}
void GameScenes::DemoScene()
{
    light = new LightSource(glm::vec3(3.0f, 3.0f, 3.0f));
    //light2 = new LightSource(glm::vec3(-2.0f, 3.0f, -2.0f));
    //light->SetLightColor(glm::vec3(1.0f, 0.0f, 0.0f));
    //light2->SetLightColor(glm::vec3(0.0f, 1.0f, 0.0f));
    Texture* tex = new Texture("uvtemplate.bmp", 0);
    CubeModel* c = new CubeModel();
    StaticCube = new GameObject(c, tex);
    Plane = new GameObject(c, tex);
    Player = new GameObject(c, tex);
    StaticCube->Translate(glm::vec3(0.0f, 1.0f, 0.0f));
    Plane->Scale(glm::vec3(100.0f, 0.2f, 100.0f));
    Plane->Translate(glm::vec3(0.0f, -5.0f, 0.0f));
    Player->Translate(glm::vec3(0.0f, 1.0f, -6.0f));
    Engine::RegisterRoutine(PlayerMovment, false);
    Engine::RegisterRoutine(MouseLook, false);
    GameObject::EnableCollision = true;
}