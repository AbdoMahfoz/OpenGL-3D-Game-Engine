#include "Engine.h"
#include <math.h>

#define PI 3.14159265

std::vector<GameObject*> objs;
bool GameObject::EnableCollision = false;

bool IsCollided(GameObject* o1, GameObject* o2)
{
	//Get Position Vector of The First Object..
	glm::vec3 Position_A = o1->GetPosition();

	//Get Scale Vector of The First Object..
	glm::vec3 Scale_A = o1->GetScale();

	//Get Position Vector of The Second Object..
	glm::vec3 Position_B = o2->GetPosition();

	//Get Scale Vector of The Second Object..
	glm::vec3 Scale_B = o2->GetScale();

    //Apply AABB Algorithm (Axis Aligned Bounding Box)..
    if(   abs(Position_A.x-Position_B.x)<Scale_A.x+Scale_B.x
       && abs(Position_A.y-Position_B.y)<Scale_A.y+Scale_B.y 
       && abs(Position_A.z-Position_B.z)<Scale_A.z+Scale_B.z )
    {
        if(o1->CallBack != nullptr)
            o1->CallBack(o2);
        if(o2->CallBack != nullptr)
            o2->CallBack(o1);
        return true;
    }
    return false;
}

bool TestCollision(GameObject* o)
{
    for(auto i : objs)
    {
        if(o != i && IsCollided(o, i))
        {
            return true;
        }
    }
    return false;
}

GameObject::GameObject(Model* model, Texture* texture)
{
    this->CallBack = nullptr;
    this->texture = texture;
    this->m_color = glm::vec3(1.0f, 1.0f, 1.0f);
    this->model = model;
    this->scale = glm::vec3(1.2f);
    ModelMatrix = glm::mat4(1.0f);
    if(model != 0)
        Engine::RegisterGameObject(this);
    objs.push_back(this);
} 
void GameObject::Translate(const glm::vec3& position)
{
    //Apply the Translation ..
    ModelMatrix *= glm::translate(position);
    //Update Position Vector ..
    this->position.x = ModelMatrix[3][0];
    this->position.y = ModelMatrix[3][1];
    this->position.z = ModelMatrix[3][2];
    while(EnableCollision && TestCollision(this))
    {
        ModelMatrix *= glm::translate(-position * 0.1f);
        //Update Position Vector ..
        this->position.x = ModelMatrix[3][0];
        this->position.y = ModelMatrix[3][1];
        this->position.z = ModelMatrix[3][2];
    }
}
void GameObject::Rotate(const glm::vec3& rotation)
{
    //RotateAround(rotation, position);
    //Apply the Rotation ..
    glm::mat4 m = glm::mat4(1);
    m *= glm::rotate(-rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    m *= glm::rotate(-rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    m *= glm::rotate(-rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    this->ModelMatrix *= m;
    //Update Rotation Vector ..
    this->rotation += rotation;
}
void GameObject::RotateAround(const glm::vec3& rotation, const glm::vec3& rotationPoint)
{
    /*
    //Translate to Origin ..
    this->ModelMatrix*=glm::translate(-rotationPoint);

    //Apply Rotation ..
    this->ModelMatrix*=glm::rotate(rotation);


    //Update Rotation Vector ..
    this->rotation+=rotation;

    //Translate Back ..
    this->ModelMatrix*=glm::translate(rotationPoint);
    */
}
void GameObject::Scale(const glm::vec3& scale)
{
    //Aplly Scaling ..
    this->ModelMatrix*=glm::scale(scale);

    //Update Scale Vector  ..
    this->scale = scale;
}
void GameObject::ScaleWithRespectTo(const glm::vec3& scale, const glm::vec3& scalingPoint)
{
    
    //Translate to Origin ..
    this->ModelMatrix*=glm::translate(-scalingPoint);

    //Apply Scaling ..
    this->ModelMatrix*=glm::scale(scale);

    //Update Scale Vector ..
    this->scale =scale;
    
    //Translate Back ..
    this->ModelMatrix*=glm::translate(scalingPoint);
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
const glm::vec3& GameObject::GetPosition()
{
    this->position.x = ModelMatrix[3][0];
    this->position.y = ModelMatrix[3][1];
    this->position.z = ModelMatrix[3][2];
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
    objs.erase(find(objs.begin(), objs.end(), this));
}