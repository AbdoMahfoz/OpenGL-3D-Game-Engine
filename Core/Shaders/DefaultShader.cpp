#include "..//Engine.h"

void DefaultShader::Initialize(Texture* tex)
{
	Specularity = 10;
	programID = ShaderManager::GetShaders("Shaders/LVS.glsl", "Shaders/LFS.glsl");
	LightMVPID = glGetUniformLocation(programID, "LightMVP");
	AmbientLightID = glGetUniformLocation(programID, "AmbientLight");
	SpeculatiyID = glGetUniformLocation(programID, "Specularity");
	texID = glGetUniformLocation(programID, "InputTexture");
	LightPosID = glGetUniformLocation(programID, "WSLight");
	LightColorID = glGetUniformLocation(programID, "LightColor");
	LightCountID = glGetUniformLocation(programID, "lightCount");
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(texID, 0);
	tex->Bind();
}
void DefaultShader::SetUpEnviroment(const glm::mat4& Prespective, const glm::mat4& View,
	const glm::vec3& AmbientLight, const glm::vec3& EyePos,
	glm::vec3* LightColor, glm::vec3* LightPos, int LightCount)
{
	VP = Prespective * View;
	glUseProgram(programID);
	glUniform3fv(EyeID, 1, &EyePos[0]);
	glUniform3fv(AmbientLightID, 1, &AmbientLight[0]);
	glUniform1fv(SpeculatiyID, 1, &Specularity);
	glUniform3fv(LightPosID, LightCount, &LightPos[0][0]);
	glUniform3fv(LightColorID, LightCount, &LightColor[0][0]);
	glUniform1iv(LightCountID, 1, &LightCount);
	glUniformMatrix4fv(LightMVPID, LightCount, GL_FALSE, &LightVP[0][0][0]);
	LightVP.clear();
}
void DefaultShader::BufferData(GameObject& obj, const glm::mat4& ModelMatrix, int i)
{
	std::string x = "[" + std::to_string(i) + "]";
	ColorID = glGetUniformLocation(programID, ("Color" + x).c_str());
	MVPID = glGetUniformLocation(programID, ("MVP" + x).c_str());
	ModelMatrixID = glGetUniformLocation(programID, ("ModelMatrix" + x).c_str());
	glUniform3fv(ColorID, 1, &(obj.GetColor()[0]));
	glUniformMatrix4fv(MVPID, 1, GL_FALSE, &(VP * ModelMatrix)[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
}