#ifndef GAMEOBJECT_CLASS
#define GAMEOBJECT_CLASS

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "Model.h"
#include "AutoClean.h"

class GameObject : public AutoClean
{
protected:
    Model* model;
    Texture* texture;
    glm::vec3 position, rotation, scale, m_color;
    glm::mat4 ModelMatrix;
    int index;
public:
    GameObject(Model* model, Texture* texture);
    void Translate(const glm::vec3& position);
    void Rotate(const glm::vec3& rotation);
    void RotateAround(const glm::vec3& rotation, const glm::vec3& rotationPoint);
    void Scale(const glm::vec3& scale);
    void ScaleWithRespectTo(const glm::vec3& scale, const glm::vec3& scalingPoint);
    void SetColor(const glm::vec3& m_color);
    void BindTexture();
    const glm::vec3& GetPosition() const;
    const glm::vec3& GetRotation() const;
    const glm::vec3& GetScale() const;
    const glm::vec3& GetColor() const;
    const glm::mat4& GetModelMatrix() const;
    Model* GetModel() const;
    ~GameObject();
};

#endif