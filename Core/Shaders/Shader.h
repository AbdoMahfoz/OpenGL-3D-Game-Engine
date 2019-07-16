#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#ifndef SHADER_CLASS
#define SHADER_CLASS

class GameObject;

class Shader
{
private:
	GLuint shadowProgramID, shadowMVPID;
	const glm::mat4* ShadowMVP;
protected:
	friend class Model;
	std::vector<const glm::mat4&> LightVP;
	virtual void Initialize() = 0;
	virtual void InitializeShadow();
	virtual void SetUpShadowEnviroment(const glm::mat4& LightMVP);
	virtual void SetUpEnviroment(const glm::mat4& Prespective, const glm::mat4& View,
		const glm::vec3& AmbientLight, const glm::vec3& EyePos,
		glm::vec3* LigthColor, glm::vec3* LightPos, int LightCount) = 0;
	virtual void BufferShadowData(GameObject&, const glm::mat4& ModelMatrix);
	virtual void BufferData(GameObject&, const glm::mat4& ModelMatrix) = 0;
	Shader() {};
	virtual ~Shader() {}
};

#endif