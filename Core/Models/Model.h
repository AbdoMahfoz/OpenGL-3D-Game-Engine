#ifndef Model_CLASS
#define Model_CLASS

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class GameObject;
class Shader;
class Texture;
struct RenderArrayElement;

class Model
{
private:
	friend class Engine;
	bool isBufferCreated, isShadowRendering;
	Shader* shader;
	RenderArrayElement* renderElement;
	GLuint VAO, VertexID, NormalID, UVID, IndicesID;
	std::vector<glm::mat4> LightVP;
	RenderArrayElement* GetRenderElement() const;
	virtual void SetUpEnviroment(const glm::mat4& LightMVP);
	virtual void SetUpEnviroment(const glm::mat4& Prespective, const glm::mat4& View,
		const glm::vec3& AmbientLight, const glm::vec3& EyePos,
		glm::vec3* LightColor, glm::vec3* LightPos, int LightCount);
	virtual void BufferData(GameObject&);
	virtual void Draw(int count);
	virtual void CreateBuffer();
	virtual void BindTexture();
protected:
	Texture* texture;
    float *verts, *uvs, *normals, Specularity;
    GLushort *indices;
    int count, indicesCount;
	virtual ~Model();
    Model();
public:
    Model(Texture* texture, float* verts, float* uvs, float* normals, int count, GLushort* indices, int indicesCount, float Speculatiry);
	void Delete();
};

#endif