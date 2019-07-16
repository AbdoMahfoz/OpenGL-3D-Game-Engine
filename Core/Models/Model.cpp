#include "../Engine.h"

Model::Model()
{
    isBufferCreated = false;
    isShadowRendering = false;
	renderElement = new RenderArrayElement(this);
    Engine::RegisterModel(renderElement);
}
Model::Model(float* verts, float* uvs, float* normals, int count, GLushort* indices, int indicesCount, float Specularity)
{
	renderElement = new RenderArrayElement(this);
    isBufferCreated = false;
    isShadowRendering = false;
    this->count = count;
    this->indicesCount = indicesCount;
    this->verts = new float[count];
    this->normals = new float[count];
    this->uvs = new float[(count/3) * 2];
    this->indices = new GLushort[indicesCount];
    this->Specularity = Specularity;
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
    Engine::RegisterModel(renderElement);
}
RenderArrayElement* Model::GetRenderElement() const
{
    return renderElement;
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
    EyeID = glGetUniformLocation(programID, "WSEye");
    AmbientLightID = glGetUniformLocation(programID, "AmbientLight");
    SpeculatiyID = glGetUniformLocation(programID, "Specularity");
    texID = glGetUniformLocation(programID, "InputTexture");
    LightPosID = glGetUniformLocation(programID, "WSLight");
    LightColorID = glGetUniformLocation(programID, "LightColor");
    LightCountID = glGetUniformLocation(programID, "lightCount");
}
void Model::SetUpEnviroment(const glm::mat4& Prespective, const glm::mat4& View,
                            const glm::vec3& AmbientLight, const glm::vec3& EyePos,
                            glm::vec3* LigthColor, glm::vec3* LightPos, int LightCount)
{
    isShadowRendering = false;
    if(!isBufferCreated)
    {
        CreateBuffer();
    }
    MVP = Prespective * View;
    glBindVertexArray(VAO);
    glUseProgram(programID);
    glUniform3fv(EyeID, 1, &EyePos[0]);
    glUniform3fv(AmbientLightID, 1, &AmbientLight[0]);
    glUniform1fv(SpeculatiyID, 1, &Specularity);
    glUniform3fv(LightPosID, LightCount, &LightPos[0][0]);
    glUniform3fv(LightColorID, LightCount, &LigthColor[0][0]);
    glUniform1iv(LightCountID, 1, &LightCount);
    glUniformMatrix4fv(LightMVPID, LightCount, GL_FALSE, &LightVP[0][0][0]);
}
void Model::SetUpEnviroment(const glm::mat4& LightMVP)
{
    if(!isShadowRendering)
    {
        LightVP.clear();
    }
    isShadowRendering = true;
    if(!isBufferCreated)
    {
        CreateBuffer();
    }
    glBindVertexArray(VAO);
    glUseProgram(shadowProgramID);
    LightVP.push_back(LightMVP);
    ShadowMVP = LightMVP;
}
void Model::Draw(GameObject& obj)
{
	obj.FlushBuffer();
    if(!isShadowRendering)
    {
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(texID, 0);
        obj.BindTexture();
        glUniform3fv(ColorID, 1, &(obj.GetColor()[0]));
        glUniformMatrix4fv(MVPID, 1, GL_FALSE, &(MVP * obj.GetRenderingModelMatrix())[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &obj.GetRenderingModelMatrix()[0][0]);
    }
    else
    {
        glUniformMatrix4fv(ShadowMVPID, 1, GL_FALSE, &(ShadowMVP * obj.GetRenderingModelMatrix())[0][0]);
    }
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_SHORT, 0);
}
void Model::Delete()
{
	Engine::UnRegisterModel(this->renderElement);
}
Model::~Model()
{
	renderElement->verts = verts;
	renderElement->uvs = uvs;
	renderElement->normals = normals;
	renderElement->indices = indices;
	for (auto i : renderElement->buff)
	{
		delete i;
	}
	renderElement->deleteBuff.clear();
	Engine::UnRegisterModel(renderElement);
}