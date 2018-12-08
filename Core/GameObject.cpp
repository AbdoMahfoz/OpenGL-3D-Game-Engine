#include "Engine.h"
#include <math.h>
#define PI 3.14159265
using namespace glm;
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
    //throw std::logic_error("function not implmented yet!");
    RotateAround(rotation, position);
}
void GameObject::RotateAround(const glm::vec3& rotation, const glm::vec3& rotationPoint)
{
    //throw std::logic_error("function not implmented yet!");
    mat4 rot_z=mat4(1);
    mat4 rot_y=mat4(1);
    mat4 rot_x=mat4(1);

    //Translate to origin ..
    this->ModelMatrix*=translate(-rotationPoint);

    //Apply Rotation ..
    if (rotationPoint[0]!=0)
    {
        //Rotation Matrix OF x-axis..
        rot_x[2][2]=rot_z[1][1]=cos(rotation[0]*PI/180);
        rot_x[1][2]=sin(-rotation[0]*PI/180);
        rot_x[2][1]=sin(rotation[0]*PI/180); 
        this->ModelMatrix*=rot_x;
    }
    if (rotationPoint[1]!=0)
    {
        //Rotation Matrix OF y-axis..
        rot_y[0][0]=rot_z[2][2]=cos(rotation[1]*PI/180);
        rot_y[2][0]=sin(-rotation[1]*PI/180);
        rot_y[0][2]=sin(rotation[1]*PI/180); 
        this->ModelMatrix*=rot_y;
    }
    if (rotationPoint[2]!=0)
    {
        //Rotation Matrix OF Z-axis..
        rot_z[0][0]=rot_z[1][1]=cos(rotation[2]*PI/180);  
        rot_z[0][1]=sin(-rotation[2]*PI/180);
        rot_z[1][0]=sin(rotation[2]*PI/180); 
        this->ModelMatrix*=rot_z;
    }
    //Translate Back ..
    this->ModelMatrix*=translate(rotationPoint);
    this->position=vec3(this->ModelMatrix[0][3],this->ModelMatrix[1][3],this->ModelMatrix[2][3]);
}
void GameObject::Scale(const glm::vec3& scale)
{
    //throw std::logic_error("function not implmented yet!");
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