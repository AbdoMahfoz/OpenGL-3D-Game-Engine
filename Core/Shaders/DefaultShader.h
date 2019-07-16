#include "Shader.h"

#ifndef DEFAULT_SHADER_CLASS
#define DEFAULT_SHADER_CLASS

class DefaultShader : public Shader
{
private:
	glm::mat4 VP;
	float Specularity;
	GLuint programID, LightMVPID, MVPID, ModelMatrixID, ColorID, EyeID, AmbientLightID, SpeculatiyID, texID,
		LightPosID, LightColorID, LightCountID;
	void Initialize(Texture*);
	void SetUpEnviroment(const glm::mat4& Prespective, const glm::mat4& View,
		const glm::vec3& AmbientLight, const glm::vec3& EyePos,
		glm::vec3* LightColor, glm::vec3* LightPos, int LightCount);
	void BufferData(GameObject&, const glm::mat4& ModelMatrix, int i);
};

#endif