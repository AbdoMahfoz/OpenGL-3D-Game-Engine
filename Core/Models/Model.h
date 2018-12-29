#ifndef Model_CLASS
#define Model_CLASS

#include <GL/glew.h>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

class GameObject;

class Model
{
protected:
    static int univ_id;
    int id;
    float *verts, *uvs, *normals, Specularity;
    GLushort *indices;
    int count, indicesCount;
    bool isBufferCreated, isShadowRendering;
    GLuint programID, shadowProgramID;
    GLuint VAO, VertexID, NormalID, UVID, IndicesID;
    GLuint LightMVPID, ShadowMVPID, MVPID, ColorID, ModelMatrixID, LightPosID, EyeID, LightColorID, AmbientLightID, SpeculatiyID, texID, LightCountID;
    glm::mat4 MVP, ShadowMVP;
    std::vector<glm::mat4> LightVP;
    Model();
public:
    Model(float* verts, float* uvs, float* normals, int count, GLushort* indices, int indicesCount, float Speculatiry);
    int GetID() const;
    virtual void SetUpEnviroment(const glm::mat4& LightMVP);
    virtual void SetUpEnviroment(const glm::mat4& Prespective, const glm::mat4& View,
                                 const glm::vec3& AmbientLight, const glm::vec3& EyePos,
                                 glm::vec3* LigthColor, glm::vec3* LightPos, int LightCount);
    virtual void Draw(GameObject&);
    virtual void CreateBuffer();
    virtual ~Model();
};

#endif