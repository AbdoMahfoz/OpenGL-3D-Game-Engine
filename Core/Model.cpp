#include "Engine.h"

int Model::univ_id = 0;

Model::Model(float* verts, int count)
{
    id = univ_id++;
    isBufferCreated = false;
    this->verts = new float[count];
    for(int i = 0; i < count; i++)
    {
        this->verts[i] = verts[i];
    }
    this->count = count;
    Engine::RegisterModel(this);
}
int Model::GetID() const
{
    return id;
}
void Model::SetUpEnviroment(const glm::mat4& Prespective, const glm::mat4& View)
{
    if(!isBufferCreated)
    {
        CreateBuffer();
    }
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    glUseProgram(ShaderProgram);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (char*)(sizeof(float) * 3));
    MVPMatrix = Prespective * View;
}
void Model::CreateBuffer()
{
    glCreateBuffers(1, &bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count, verts, GL_STATIC_DRAW);
    ShaderProgram = LoadShaders("Shaders/DefaultVertexShader.glsl", "Shaders/DefaultFragmentShader.glsl");
    mvpID = glGetUniformLocation(ShaderProgram, "MVP");
    colorID = glGetUniformLocation(ShaderProgram, "Color");
    isBufferCreated = true;
}
void Model::Draw(GameObject& obj)
{
    obj.BindTexture();
    glUniform3fv(colorID, 1, &(obj.GetColor()[0]));
    glUniformMatrix4fv(mvpID, 1, GL_FALSE, &(MVPMatrix * obj.GetModelMatrix())[0][0]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, count);
}
void Model::CleanUpEnviroment()
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
Model::~Model()
{
    delete[] verts;
}