#include "../Engine.h"

Model::Model()
{
	shader = new DefaultShader();
    isBufferCreated = false;
    isShadowRendering = false;
	renderElement = new RenderArrayElement(this);
    Engine::RegisterModel(renderElement);
}
Model::Model(Texture* texture, float* verts, float* uvs, float* normals, int count, GLushort* indices, int indicesCount, float Specularity)
{
	shader = new DefaultShader();
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (count / 3) * 2, uvs, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indicesCount, indices, GL_STATIC_DRAW);
	shader->Initialize(texture);
	shader->InitializeShadow();
}
void Model::SetUpEnviroment(const glm::mat4& Prespective, const glm::mat4& View,
                            const glm::vec3& AmbientLight, const glm::vec3& EyePos,
                            glm::vec3* LightColor, glm::vec3* LightPos, int LightCount)
{
    isShadowRendering = false;
    if(!isBufferCreated)
    {
        CreateBuffer();
    }
    glBindVertexArray(VAO);
	shader->SetUpEnviroment(Prespective, View, AmbientLight, EyePos, LightColor, LightPos, LightCount);
}
void Model::SetUpEnviroment(const glm::mat4& LightMVP)
{
    if(!isBufferCreated)
    {
        CreateBuffer();
    }
    glBindVertexArray(VAO);
    isShadowRendering = true;
	shader->SetUpShadowEnviroment(LightMVP);
}
void Model::BufferData(GameObject& obj, int i)
{
	obj.FlushBuffer();
    if(!isShadowRendering)
    {
		shader->BufferData(obj, obj.GetRenderingModelMatrix(), i);
    }
    else
    {
		shader->BufferShadowData(obj, obj.GetRenderingModelMatrix(), i);
    }
}
Texture* Model::GetTexture()
{
	return texture;
}
void Model::SetTexture(Texture* tex)
{
	texture = tex;
}
void Model::Draw(int count)
{
	glDrawElementsInstanced(GL_TRIANGLES, indicesCount, GL_UNSIGNED_SHORT, 0, count);
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