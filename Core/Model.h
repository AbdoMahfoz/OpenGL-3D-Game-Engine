#include "Engine.h"

class Model
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
public:
    Model(float* verts, int count);
    int GetID() const;
    virtual void SetUpEnviroment();
    virtual void Draw(const GameObject&);
    virtual void CleanUpEnviroment();
    virtual ~Model();
};