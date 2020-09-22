#include "pch.h"
#include "Engine3D\Core\Input.h"
#include "Engine3D\Core\Application.h"

#include <GLFW\glfw3.h>

namespace E3D
{
	bool Input::IsKeyPressed(int keycode)
	{
		auto& app = Application::GetInstance();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		bool state = glfwGetKey(window, keycode);

		return (state == GLFW_PRESS || state == GLFW_REPEAT);
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		auto& app = Application::GetInstance();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		bool state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePosition()
	{
		auto& app = Application::GetInstance();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		return { (float)x, (float)y };
	}

	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}
}