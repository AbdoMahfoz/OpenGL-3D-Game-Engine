#include "Engine.h"

GameObject *Cube, *Plane;

//float mode = 1.0f;

void func()
{
    /*if(g->GetColor().x - 0.01f < 0.0f || g->GetColor().x + 0.01f > 1.0f)
    {
        mode *= -1;
    }
    g->SetColor(g->GetColor() + (0.01f * mode));*/
    //Plane->Translate(glm::vec3(0.01f, 0.0f, 0.0f));
    Cube->Rotate(glm::vec3(0.0f, 1.0f, 0.0f));
}
void Engine::Start()
{
    /*float verts[] = 
    {
        +0.1f, +0.1f, +0.0f,
        +1.0f, +1.0f,
        +0.1f, -0.1f, +0.0f,
        +1.0f, +0.0f,
        -0.1f, -0.1f, +0.0f,
        +0.0f, +0.0f,
        -0.1f, +0.1f, +0.0f,
        +0.0f, +1.0f,
    };*/
    Texture* tex = new Texture("uvtemplate.bmp", 0);
    Cube = new GameObject(new CubeModel(), tex);
    //Plane = new GameObject(new Model(verts, sizeof(verts) / sizeof(verts[0])), tex);
    //Plane->Translate(glm::vec3(-1.0f, 0.0f, 4.0f));
    //g->Rotate(glm::vec3(0.0f, 95.0f, 0.0f));
    Engine::GetCurrentCamera().Walk(-4.0f);
    Engine::RegisterRoutine(func, false);
}