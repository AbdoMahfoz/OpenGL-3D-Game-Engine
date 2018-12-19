#include "Engine.h"

int Model::univ_id = 0;

Model::Model()
{
    id = univ_id++;
    isBufferCreated = false;
    isShadowRendering = false;
    Engine::RegisterModel(this);
}
Model::Model(float* verts, float* uvs, float* normals, int count, GLushort* indices, int indicesCount)
{
    id = univ_id++;
    isBufferCreated = false;
    isShadowRendering = false;
    this->count = count;
    this->indicesCount = indicesCount;
    this->verts = new float[count];
    this->normals = new float[count];
    this->uvs = new float[(count/3) * 2];
    this->indices = new GLushort[indicesCount];
    for(int i = 0; i < std::max(count, indicesCount); i++)
    {
        if(i < count)
        {
            this->verts[i] = verts[i];
            this->normals[i] = normals[i];
        }
        if(i < (count/3) * 2)
        {
            this->uvs[i] = uvs[i];
        }
        if(i < indicesCount)
        {
            this->indices[i] = indices[i];
        }
    }
    Engine::RegisterModel(this);
}
int Model::GetID() const
{
    return id;
}
void Model::CreateBuffer()
{
    isBufferCreated = true;
    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VertexID);
    glGenBuffers(1, &NormalID);
    glGenBuffers(1, &UVID);
    glGenBuffers(1, &IndicesID);
    glBindBuffer(GL_ARRAY_BUFFER, VertexID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count, verts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    glBindBuffer(GL_ARRAY_BUFFER, NormalID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count, normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    glBindBuffer(GL_ARRAY_BUFFER, UVID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (count/3)*2, uvs, GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndicesID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indicesCount, indices, GL_STATIC_DRAW);
    programID = ShaderManager::GetShaders("Shaders/LVS.glsl", "Shaders/LFS.glsl");
    shadowProgramID = ShaderManager::GetShaders("Shaders/SVS.glsl", "Shaders/SFS.glsl");
    ShadowMVPID = glGetUniformLocation(shadowProgramID, "MVP");
    LightMVPID = glGetUniformLocation(programID, "LightMVP");
    MVPID = glGetUniformLocation(programID, "MVP");
    ModelMatrixID = glGetUniformLocation(programID, "ModelMatrix");
    ColorID = glGetUniformLocation(programID, "Color");
    LightPosID = glGetUniformLocation(programID, "WSLight");
    EyeID = glGetUniformLocation(programID, "WSEye");
    LightColorID = glGetUniformLocation(programID, "LightColor");
    AmbientLightID = glGetUniformLocation(programID, "AmbientLight");
    SpeculatiyID = glGetUniformLocation(programID, "Specularity");
    texID = glGetUniformLocation(programID, "InputTexture");
}
void Model::SetUpEnviroment(const glm::mat4& Prespective, const glm::mat4& View,
                                 const glm::vec3& LightPos, const glm::vec3& EyePos,
                                 const glm::vec3& LigthColor, const glm::vec3& AmbientLight,
                                 float Specularity)
{
    isShadowRendering = false;
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
    glUniformMatrix4fv(LightMVPID, 1, GL_FALSE, &ShadowMVP[0][0]);
}
void Model::SetUpEnviroment(const glm::mat4& Prespective, const glm::mat4& View)
{
    isShadowRendering = true;
    if(!isBufferCreated)
    {
        CreateBuffer();
    }
    glBindVertexArray(VAO);
    glUseProgram(shadowProgramID);
    ShadowMVP = Prespective * View;
}
void Model::Draw(GameObject& obj)
{
    if(!isShadowRendering)
    {
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(texID, 0);
        obj.BindTexture();
        glUniform3fv(ColorID, 1, &(obj.GetColor()[0]));
        glUniformMatrix4fv(MVPID, 1, GL_FALSE, &(MVP * obj.GetModelMatrix())[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &obj.GetModelMatrix()[0][0]);
    }
    else
    {
        glUniformMatrix4fv(ShadowMVPID, 1, GL_FALSE, &(ShadowMVP * obj.GetModelMatrix())[0][0]);
    }
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_SHORT, 0);
}
Model::~Model()
{
    delete[] verts, uvs, normals, indices;
}