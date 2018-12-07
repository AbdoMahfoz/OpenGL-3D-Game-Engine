#include "Model.h"
#include "GameObject.h"

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
void Model::SetUpEnviroment()
{
    if(!isBufferCreated)
    {
        glCreateBuffers(1, &bufferID);
        glBindBuffer(GL_ARRAY_BUFFER, bufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count, verts, GL_STATIC_DRAW);
        ShaderProgram = LoadShaders("Shaders/DefaultVertexShader.glsl", "Shaders/DefaultFragmentShader.glsl");
        mvpID = glGetUniformLocation(ShaderProgram, "MVP");
        colorID = glGetUniformLocation(ShaderProgram, "Color");
        isBufferCreated = true;
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    }
    glUseProgram(ShaderProgram);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
}
void Model::Draw(const GameObject& obj)
{
    glUniform3fv(colorID, 1, &(obj.GetColor()[0]));
    glUniformMatrix4fv(mvpID, 1, GL_FALSE, &(obj.GetModelMatrix()[0][0]));
    glDrawArrays(GL_TRIANGLE_FAN, 0, count);
}
void Model::CleanUpEnviroment()
{
    glDisableVertexAttribArray(0);
}
Model::~Model()
{
    delete[] verts;
}