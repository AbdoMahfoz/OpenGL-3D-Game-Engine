+#include "Engine.h"
#include <math.h>

#define PI 3.14159265

GameObject::GameObject(Model* model, Texture* texture)
{
    this->texture = texture;
    this->m_color = glm::vec3(1.0f, 1.0f, 1.0f);
    this->model = model;
    ModelMatrix = glm::mat4(1);
    if(model != 0)
        Engine::RegisterGameObject(this);
} 
void GameObject::Translate(const glm::vec3& position)
{
    //Apply the Translation ..
    ModelMatrix *= glm::translate(position);

    //Update Position Vector ..
    this->position += position;
}
void GameObject::Rotate(const glm::vec3& rotation)
{
    //RotateAround(rotation, position);
    //Apply the Rotation ..
    this->ModelMatrix*=glm::rotate(rotation);

    //Update Rotation Vector ..
    this->rotation+=rotation;
}
void GameObject::RotateAround(const glm::vec3& rotation, const glm::vec3& rotationPoint)
{
    //Translate to Origin ..
    this->ModelMatrix*=glm::translate(-rotationPoint);

    //Apply Rotation ..
    this->ModelMatrix*=glm::rotate(rotation);

    //Update Rotation Vector ..
    this->rotation+=rotation;

    //Translate Back ..
    this->ModelMatrix*=glm::Translate(rotationPoint);
}
void GameObject::Scale(const glm::vec3& scale)
{
    //Aplly Scaling ..
    this->ModelMatrix*=glm::scale(scale);

    //Update Scale Vector  ..
    this->scale+=scale;
}
void GameObject::ScaleWithRespectTo(const glm::vec3& scale, const glm::vec3& scalingPoint)
{
    
    //Translate to Origin ..
    this->ModelMatrix*=glm::translate(-scalingPoint);

    //Apply Scaling ..
    this->ModelMatrix*=glm::scale(scale);

    //Update Scale Vector ..
    this->scale+=scale;
    
    //Translate Back ..
    this->ModelMatrix*=glm::Translate(scalingPoint);
}
void GameObject::SetColor(const glm::vec3& m_color)
{
    this->m_color = m_color;
}
void GameObject::BindTexture()
{
    texture->Bind();
}
Model* GameObject::GetModel() const
{
    return model;
}
const glm::vec3& GameObject::GetPosition() const
{
    return position;
}
const glm::vec3& GameObject::GetRotation() const
{
    return rotation;
}
const glm::vec3& GameObject::GetScale() const
{
    return scale;
}
const glm::vec3& GameObject::GetColor() const
{
    return m_color;
}
const glm::mat4& GameObject::GetModelMatrix() const
{
    return ModelMatrix;
}
GameObject::~GameObject()
{
    if(model != 0)
        Engine::UnRegisterGameObject(this);
}