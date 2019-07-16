#include "..//Engine.h"

void Shader::InitializeShadow()
{
	shadowProgramID = ShaderManager::GetShaders("Shaders/SVS.glsl", "Shaders/SFS.glsl");
	shadowMVPID = glGetUniformLocation(shadowProgramID, "MVP");
}
void Shader::SetUpShadowEnviroment(const glm::mat4& LightMVP)
{
	glUseProgram(shadowProgramID);
	ShadowMVP = &LightMVP;
	LightVP.push_back(LightMVP);
}
void Shader::BufferShadowData(GameObject& obj, const glm::mat4& ModelMatrix)
{
	glUniformMatrix4fv(shadowMVPID, 1, GL_FALSE, &((*ShadowMVP) * ModelMatrix)[0][0]);
}