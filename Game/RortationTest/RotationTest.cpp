#include "..//GameScenes.h"

namespace RotationTest
{
	Cube* c;
	LightSource* light;
	void Cleanup()
	{
		delete c;
	}
	void Update()
	{
		c->Rotate(glm::vec3(0.1f, 0.1f, 0.1f));
		c->Translate(glm::vec3(0.01f, 0.0f, 0.0f));
	}
	void Initalize()
	{
		c = new Cube();
		light = new LightSource(glm::vec3(0.0f, 2.0f, 0.0f));
		Engine::SetAmbientLight(glm::vec3(1.0f, 1.0f, 1.0f));
		Engine::RegisterRoutine(Update, false);
		Engine::GetCurrentCamera().Walk(-25.0f);
		Engine::RegisterOnExit(Cleanup);
	}
}

void GameScenes::RotationTest()
{
	RotationTest::Initalize();
}