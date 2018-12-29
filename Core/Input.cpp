#include "Input.h"

#include <map>

std::map<int, bool> KeyMap;
GLFWwindow* CurrentWindow;
glm::vec2 delta;

void Input::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	KeyMap[key] = (action != GLFW_RELEASE);
}
bool Input::IsKeyPressed(int key)
{
	return KeyMap[key];
}
void Input::CalculateDelta()
{
	double x, y;
	glfwGetCursorPos(CurrentWindow, &x, &y);
	glfwSetCursorPos(CurrentWindow, 500.0, 500.0);
	delta = glm::vec2(x - 500.0, y - 500.0);
}
glm::vec2 Input::GetMouseDelta()
{
	return delta;
}
void Input::SetWindow(GLFWwindow* window)
{
	CurrentWindow = window;
}