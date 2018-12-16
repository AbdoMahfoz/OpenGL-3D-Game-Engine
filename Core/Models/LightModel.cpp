#include "Engine.h"

LightModel::LightModel()
{
    id = univ_id++;
    isBufferCreated = false;
    //Engine::RegisterModel(this);
}
LightModel::LightModel(float* verts, float* uvs, float* normals, int count, GLushort* indices, int indicesCount)
{
    id = univ_id++;
    isBufferCreated = false;
    this->count = count;
    this->indicesCount = indicesCount;
    this->verts = new float[count];
    this->uvs = new float[count];
    this->normals = new float[count];
    this->indices = new GLushort[indicesCount];
    for(int i = 0; i < count; i++)
    {
        this->verts[i] = verts[i];
        this->uvs[i] = uvs[i];
        this->normals[i] = normals[i];
    }
    for(int i = 0; i < indicesCount; i++)
    {
        this->indices[i] = indices[i];
    }
    //Engine::RegisterModel(this);
}
void LightModel::CreateBuffer()
{
    isBufferCreated = true;
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VertexID);
    glGenBuffers(1, &NormalID);
    glGenBuffers(1, &UVID);
    glGenBuffers(1, &IndicesID);
    glBindBuffer(GL_ARRAY_BUFFER, VertexID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count, verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    glBindBuffer(GL_ARRAY_BUFFER, NormalID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count, normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    glBindBuffer(GL_ARRAY_BUFFER, UVID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count, uvs, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndicesID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indicesCount, indices, GL_STATIC_DRAW);
    programID = ShaderManager::GetShaders("Shaders/LVS.glsl", "Shaders/LFS.glsl");
    MVPID = glGetUniformLocation(programID, "MVP");
    ModelMatrixID = glGetUniformLocation(programID, "ModelMatrix");
    ColorID = glGetUniformLocation(programID, "Color");
    LightPosID = glGetUniformLocation(programID, "WSLight");
    EyeID = glGetUniformLocation(programID, "WSEye");
    LightColorID = glGetUniformLocation(programID, "LightColor");
    AmbientLightID = glGetUniformLocation(programID, "AmbientLight");
    SpeculatiyID = glGetUniformLocation(programID, "Specularity");
}
void LightModel::SetUpEnviroment(const glm::mat4& Prespective, const glm::mat4& View,
                                 const glm::vec3& LightPos, const glm::vec3& EyePos,
                                 const glm::vec3& LigthColor, const glm::vec3& AmbientLight,
                                 float Specularity)
{
    if(!isBufferCreated)
    {
        CreateBuffer();
    }
    MVP = Prespective * View;
    glBindVertexArray(VAO);
    glUseProgram(programID);
    glUniform3fv(LightPosID, 1, &LightPos[0]);
    glUniform3fv(EyeID, 1, &EyePos[0]);
    glUniform3fv(LightColorID, 1, &LigthColor[0]);
    glUniform3fv(AmbientLightID, 1, &AmbientLight[0]);
    glUniform1fv(SpeculatiyID, 1, &Specularity);
}
void LightModel::Draw(GameObject& obj)
{
    obj.BindTexture();
    glUniform3fv(ColorID, 1, &(obj.GetColor()[0]));
    glUniformMatrix4fv(MVPID, 1, GL_FALSE, &(MVP * obj.GetModelMatrix())[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &obj.GetModelMatrix()[0][0]);
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_SHORT, 0);
}
LightModel::~LightModel()
{
    delete[] verts, uvs, normals, indices;
}