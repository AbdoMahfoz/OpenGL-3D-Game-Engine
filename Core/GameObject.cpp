#include "Engine.h"

GameObject::GameObject(Model* model)
{
    this->model = model;
    ModelMatrix = glm::mat4(1);
    Engine::RegisterGameObject(this);
}
void GameObject::Translate(const glm::vec3& position)
{
    ModelMatrix *= glm::translate(position);
    this->position += position;
}
void GameObject::Rotate(const glm::vec3& rotation)
{
    throw std::logic_error("function not implmented yet!");
}
void GameObject::RotateAround(const glm::vec3& rotation, const glm::vec3& rotationPoint)
{
    throw std::logic_error("function not implmented yet!");
}
void GameObject::Scale(const glm::vec3& scale)
{
    throw std::logic_error("function not implmented yet!");
}
void GameObject::ScaleWithRespectTo(const glm::vec3& scale, const glm::vec3& scalingPoint)
{
    throw std::logic_error("function not implmented yet!");
}
void GameObject::SetColor(const glm::vec3& m_color)
{
    this->m_color = m_color;
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
    Engine::UnRegisterGameObject(this);
}