#pragma once
#include "Engine3D\Renderer\GraphicsContext.h"

#include <GLFW\glfw3.h>

namespace E3D
{
	class OpenGLGraphicsContext : public GraphicsContext
	{
	public:
		OpenGLGraphicsContext(GLFWwindow* handle);
		~OpenGLGraphicsContext() = default;


		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}