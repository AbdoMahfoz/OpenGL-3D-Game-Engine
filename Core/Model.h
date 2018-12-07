#ifndef MODEL_CLASS
#define MODEL_CLASS

#include <GL/glew.h>
#include "AutoClean.h"

class GameObject;

class Model : public AutoClean
{
private:
    static int univ_id;
    int id;
    float* verts;
    int count;
    GLuint bufferID;
    GLuint ShaderProgram;
    GLuint mvpID, colorID;
    bool isBufferCreated;
    glm::mat4 MVPMatrix;
public:
    Model(float* verts, int count);
    int GetID() const;
    virtual void SetUpEnviroment(const glm::mat4& Prespective, const glm::mat4& View);
    virtual void Draw(GameObject&);
    virtual void CleanUpEnviroment();
    virtual ~Model();
};

#endif