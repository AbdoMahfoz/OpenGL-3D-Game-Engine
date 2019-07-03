#include "Engine.h"
#include <math.h>

#define PI 3.14159265

std::vector<GameObject*> objs;
bool GameObject::EnableCollision = false;

bool IsCollided(GameObject* o1, GameObject* o2)
{
	glm::vec3 Position_A = o1->GetPosition();
	glm::vec3 Scale_A = o1->GetScale();
	glm::vec3 Position_B = o2->GetPosition();
	glm::vec3 Scale_B = o2->GetScale();
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
	this->BufferToBeFlushed = -1;
	this->bufferDirty[0] = this->bufferDirty[1] = 0;
	this->lastBufferUsed = -1;
	this->dirty = false;
    this->CallBack = nullptr;
    this->texture = texture;
    this->m_color = glm::vec3(1.0f, 1.0f, 1.0f);
    this->model = model;
	this->position = glm::vec3(0.0f);
	this->rotation = glm::vec3(0.0f);
	this->index = 0;
    this->scale = glm::vec3(1.0f);
    ModelMatrix = Buffer[0] = Buffer[1] = glm::mat4(1.0f);
    if(model != 0)
        Engine::RegisterGameObject(this);
    objs.push_back(this);
}
void GameObject::FlushBuffer()
{
	int i = Engine::GetFrameNumber();
	MatLock.lock();
	if (lastBufferUsed != i && bufferDirty[i])
	{
		Buffer[!i] = Buffer[!i] * Buffer[i];
		Buffer[i] = glm::mat4(1);
		bufferDirty[i] = false;
		bufferDirty[!i] = true;
	}
	MatLock.unlock();
	if (bufferDirty[!i])
	{
		if (BufferToBeFlushed == i)
		{
			MatLock.lock();
			ModelMatrix = Buffer[!i] * Buffer[i] * ModelMatrix;
			Buffer[i] = glm::mat4(1);
			bufferDirty[i] = false;
			MatLock.unlock();
		}
		else
		{
			ModelMatrix = Buffer[!i] * ModelMatrix;
		}
		Buffer[!i] = glm::mat4(1);
		bufferDirty[!i] = false;
	}
	BufferToBeFlushed = i;
}
void GameObject::UpdateModelMatrix(const glm::mat4& m)
{
	int i = Engine::GetFrameNumber();
	MatLock.lock();
	if (lastBufferUsed != i && bufferDirty[i])
	{
		Buffer[!i] = Buffer[!i] * Buffer[i];
		Buffer[i] = glm::mat4(1);
		bufferDirty[i] = false;
	}
	MatLock.unlock();
	Buffer[i] = m * Buffer[i];
	lastBufferUsed = i;
	bufferDirty[i] = true;
}
void GameObject::updatePosRot()
{
	if (dirty)
	{
		dirty = false;
		const glm::mat4& m = GetModelMatrix();
		position.x = m[3][0];
		position.y = m[3][1];
		position.z = m[3][2];
		glm::vec3 xDirection = glm::vec3(1.0f, 0.0f, 0.0f) - (glm::vec3)(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) * m);
		glm::vec3 yDirection = glm::vec3(0.0f, 1.0f, 0.0f) - (glm::vec3)(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) * m);
		glm::vec3 zDirection = glm::vec3(0.0f, 0.0f, 1.0f) - (glm::vec3)(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) * m);
		rotation.x = atan2(xDirection.z, xDirection.y);
		rotation.y = atan2(xDirection.z, xDirection.x);
		rotation.z = atan2(xDirection.y, xDirection.x);
	}
}
void GameObject::Translate(const glm::vec3& position)
{
	updatePosRot();
	UpdateModelMatrix(glm::translate(position));
	this->position += position;
    while(EnableCollision && TestCollision(this))
    {
		UpdateModelMatrix(glm::translate(-position * 0.1f));
		this->position += position;
    }
}
void GameObject::Translate(float distance, const glm::vec3& direction)
{
	updatePosRot();
	const glm::mat4& m = GetModelMatrix();
	UpdateModelMatrix(glm::translate((glm::vec3)(glm::normalize(m[2])) * distance));
	this->position.x = m[3][0];
	this->position.y = m[3][1];
	this->position.z = m[3][2];
	while (EnableCollision && TestCollision(this))
	{
		UpdateModelMatrix(glm::translate(-position * 0.1f));
		this->position.x = m[3][0];
		this->position.y = m[3][1];
		this->position.z = m[3][2];
	}
}
void GameObject::Rotate(const glm::vec3& rotation)
{
	updatePosRot();
	UpdateModelMatrix(glm::translate(position) *
		glm::rotate(rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::rotate(rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::translate(-position));
	this->rotation += rotation;
}
void GameObject::RotateAround(const glm::vec3& rotation, const glm::vec3& rotationPoint)
{
	updatePosRot();
	UpdateModelMatrix(glm::translate(position - rotationPoint) *
		glm::rotate(rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::rotate(rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::translate(rotationPoint - position));
	dirty = true;
}
void GameObject::Scale(const glm::vec3& scale)
{
    UpdateModelMatrix(glm::scale(scale));
    this->scale *= scale;
}
void GameObject::ScaleWithRespectTo(const glm::vec3& scale, const glm::vec3& scalingPoint)
{
	UpdateModelMatrix(glm::translate(position - scalingPoint) *
		glm::scale(scale) *
		glm::translate(scalingPoint - position));
	throw std::exception("Scale deduction from ModelMatrix is not yet implmented");
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
	updatePosRot();
	return position;
}
const glm::vec3& GameObject::GetRotation()
{
	updatePosRot();
	return rotation;
}
const glm::vec3& GameObject::GetScale()
{
	return scale;
}
const glm::vec3& GameObject::GetColor() const
{
    return m_color;
}
glm::mat4 GameObject::GetModelMatrix()
{
	int i = Engine::GetFrameNumber(); 
	MatLock.lock();
	if (lastBufferUsed != i && bufferDirty[i])
	{
		Buffer[!i] = Buffer[i] * Buffer[!i];
		Buffer[i] = glm::mat4(1);
		bufferDirty[i] = false;
		bufferDirty[!i] = true;
	}
	MatLock.unlock();
    return Buffer[i] * Buffer[!i] * ModelMatrix;
}
const glm::mat4& GameObject::GetRenderingModelMatrix()
{
	return ModelMatrix;
}
GameObject::~GameObject()
{
    if(model != 0)
        Engine::UnRegisterGameObject(this);
    objs.erase(find(objs.begin(), objs.end(), this));
}