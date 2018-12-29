#pragma once

#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

namespace Input
{
	void KeyCallBack(GLFWwindow*, int, int, int, int);
	bool IsKeyPressed(int key);
    void CalculateDelta();
	glm::vec2 GetMouseDelta();
	void SetWindow(GLFWwindow*);
}