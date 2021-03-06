#ifndef GAMEOBJECT_CLASS
#define GAMEOBJECT_CLASS

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <mutex>
#include "Models/Model.h"
#include "ImportedAssets/texture.h"
#include "AutoClean.h"

class GameObject
{
private:
	friend class Engine;
	friend class Model;
	const glm::mat4& GetRenderingModelMatrix();
	void FlushBuffer();
    Model* model;
    glm::vec3 position, rotation, scale, m_color;
    glm::mat4 ModelMatrix, Buffer[2];
	std::mutex MatLock;
	bool bufferDirty[2];
	bool dirty;
    int index;
	int lastBufferUsed, BufferToBeFlushed;
	void updatePosRot();
protected:
	virtual ~GameObject() {}
public:
	void UpdateModelMatrix(const glm::mat4&);
    void (*CallBack)(GameObject*);
    static bool EnableCollision;
    GameObject(Model* model);
    void Translate(const glm::vec3& position);
	void Translate(float distance, const glm::vec3& direction);
	void TranslateLeft(float distance);
    void Rotate(const glm::vec3& rotation);
    void RotateAround(const glm::vec3& rotation, const glm::vec3& rotationPoint);
    void Scale(const glm::vec3& scale);
    void ScaleWithRespectTo(const glm::vec3& scale, const glm::vec3& scalingPoint);
    void SetColor(const glm::vec3& m_color);
    const glm::vec3& GetPosition();
    const glm::vec3& GetRotation();
    const glm::vec3& GetScale();
    const glm::vec3& GetColor() const;
    glm::mat4 GetModelMatrix();
    Model* GetModel() const;
	virtual void Delete();
};

#endif