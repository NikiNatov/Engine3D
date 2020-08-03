#include "pch.h"
#include "OpenGLGraphicsContext.h"

#include "Engine3D\Core\Config.h"

#include <glad\glad.h>

namespace E3D
{
	OpenGLGraphicsContext::OpenGLGraphicsContext(GLFWwindow* handle)
		: m_WindowHandle(handle)
	{
		E3D_CORE_ASSERT(m_WindowHandle, "Window is null");
	}

	void OpenGLGraphicsContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		E3D_CORE_ASSERT(status, "Failed to initialize Glad!");

		E3D_CORE_LOG_INFO("OpenGL Info:");
		E3D_CORE_LOG_INFO("Vendor: {0}", glGetString(GL_VENDOR));
		E3D_CORE_LOG_INFO("Renderer: {0}", glGetString(GL_RENDERER));
		E3D_CORE_LOG_INFO("Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLGraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}