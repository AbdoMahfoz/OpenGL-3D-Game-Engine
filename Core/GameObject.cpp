#include "Engine.h"
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
    ModelMatrix *= glm::translate(position);
    this->position += position;
}
void GameObject::Rotate(const glm::vec3& rotation)
{
    //RotateAround(rotation, position);
    //this->ModelMatrix*=glm::rotate(rotation);
}
void GameObject::RotateAround(const glm::vec3& rotation, const glm::vec3& rotationPoint)
{
    //position = glm::vec3(ModelMatrix[3][0], ModelMatrix[3][1], ModelMatrix[3][2]);
    //std::cout << this->position[0] << ' ' << this->position[1] << ' ' << this->position[2] << '\n';
    ModelMatrix *= glm::translate(-rotationPoint);
    if(rotation.y != 0)
        ModelMatrix *= glm::rotate(rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    if(rotation.x != 0)
        ModelMatrix *= glm::rotate(rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    if(rotation.z != 0)
        ModelMatrix *= glm::rotate(rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    ModelMatrix *= glm::translate(rotationPoint);
    position = glm::vec3(ModelMatrix[3].x, ModelMatrix[3].y, ModelMatrix[3].z);
    //std::cout << this->position[0] << ' ' << this->position[1] << ' ' << this->position[2] << '\n';
}
void GameObject::Scale(const glm::vec3& scale)
{
    this->ModelMatrix*=glm::scale(scale);
}
void GameObject::ScaleWithRespectTo(const glm::vec3& scale, const glm::vec3& scalingPoint)
{
    throw std::logic_error("function not implmented yet!");
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