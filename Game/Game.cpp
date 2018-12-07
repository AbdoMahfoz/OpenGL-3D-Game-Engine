#include "Engine.h"

Model* m;
GameObject* g;

float mode = 1.0f;

void func()
{
    if(g->GetColor().x - 0.01f < 0.0f || g->GetColor().x + 0.01f > 1.0f)
    {
        mode *= -1;
    }
    g->SetColor(g->GetColor() + (0.01f * mode));
    g->Translate(glm::vec3(0.0001f, 0.0f, 0.0f));
}
void Engine::Start()
{
    float verts[] = 
    {
        +0.1f, +0.1f, +0.0f,
        +0.1f, -0.1f, +0.0f,
        -0.1f, -0.1f, +0.0f,
        -0.1f, +0.1f, +0.0f,
    };
    m = new Model(verts, 12);
    g = new GameObject(m);
    g->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
    Engine::RegisterRoutine(func, false);
}