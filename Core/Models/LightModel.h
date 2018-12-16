#ifndef LIGHTMODEL_CLASS
#define LIGHTMODEL_CLASS

#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

class LightModel
{
protected:
    static int univ_id;
    int id;
    float *verts, *uvs, *normals;
    GLushort *indices;
    int count, indicesCount;
    bool isBufferCreated;
    GLuint programID;
    GLuint VAO, VertexID, NormalID, UVID, IndicesID;
    GLuint MVPID, ColorID, ModelMatrixID, LightPosID, EyeID, LightColorID, AmbientLightID, SpeculatiyID;
    glm::mat4 MVP;
    LightModel();
public:
    LightModel(float* verts, float* uvs, float* normals, int count, GLushort* indices, int indicesCount);
    int GetID() const;
    virtual void SetUpEnviroment(const glm::mat4& Prespective, const glm::mat4& View,
                                 const glm::vec3& LightPos, const glm::vec3& EyePos,
                                 const glm::vec3& LigthColor, const glm::vec3& AmbientLight,
                                 float Specularity);
    virtual void Draw(GameObject&);
    virtual void CreateBuffer();
    virtual ~LightModel();
};

#endif