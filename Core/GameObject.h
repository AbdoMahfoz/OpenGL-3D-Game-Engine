#ifndef GAMEOBJECT_CLASS
#define GAMEOBJECT_CLASS

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <mutex>
#ifdef _WIN32
	#include "Models/Model.h"
	#include "ImportedAssets/texture.h"
#else
	#include "Model.h"
	#include "texture.h"
#endif
#include "AutoClean.h"

class GameObject
{
protected:
    Model* model;
    Texture* texture;
    glm::vec3 position, rotation, scale, m_color;
    glm::mat4 ModelMatrix, Buffer[2];
	std::mutex MatLock;
	bool bufferDirty[2];
	bool dirty;
    int index;
	int lastBufferUsed, BufferToBeFlushed;
	void updatePosRot();
public:
	void FlushBuffer();
	void UpdateModelMatrix(const glm::mat4&);
    void (*CallBack)(GameObject*);
    static bool EnableCollision;
    GameObject(Model* model, Texture* texture);
    void Translate(const glm::vec3& position);
	void Translate(float distance, const glm::vec3& direction);
	void TranslateLeft(float distance);
    void Rotate(const glm::vec3& rotation);
    void RotateAround(const glm::vec3& rotation, const glm::vec3& rotationPoint);
    void Scale(const glm::vec3& scale);
    void ScaleWithRespectTo(const glm::vec3& scale, const glm::vec3& scalingPoint);
    void SetColor(const glm::vec3& m_color);
    void BindTexture();
    const glm::vec3& GetPosition();
    const glm::vec3& GetRotation();
    const glm::vec3& GetScale();
    const glm::vec3& GetColor() const;
    glm::mat4 GetModelMatrix();
	const glm::mat4& GetRenderingModelMatrix();
    Model* GetModel() const;
	virtual void Delete();
	virtual ~GameObject() {}
};

#endif