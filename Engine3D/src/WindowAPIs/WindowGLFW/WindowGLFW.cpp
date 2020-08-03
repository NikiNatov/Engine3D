#include "pch.h"
#include "WindowGLFW.h"

#include "Engine3D\Core\Config.h"

#include "Engine3D\Events\ApplicationEvent.h"
#include "Engine3D\Events\KeyEvent.h"
#include "Engine3D\Events\MouseEvent.h"

#include "RenderAPIs\OpenGL\OpenGLGraphicsContext.h"

namespace E3D {

	Window* Window::Create(const WindowProperties& properties)
	{
		return new WindowGLFW(properties);
	}

	WindowGLFW::WindowGLFW(const WindowProperties& properties)
	{
		Init(properties);
	}

	WindowGLFW::~WindowGLFW()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void WindowGLFW::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowGLFW::Init(const WindowProperties& properties)
	{
		m_WindowData.Width = properties.Width;
		m_WindowData.Height = properties.Height;
		m_WindowData.Title = properties.Title;
		m_WindowData.VSync = properties.VSync;

		int status = glfwInit();
		E3D_CORE_ASSERT(status, "Failed to initialize GLFW!");

		m_Window = glfwCreateWindow(m_WindowData.Width, m_WindowData.Height, m_WindowData.Title.c_str(), nullptr, nullptr);
		E3D_CORE_ASSERT(m_Window, "Window Creation Failed!");

		m_Context = new OpenGLGraphicsContext(m_Window);
		m_Context->Init();

		SetVSync(true);

		glfwSetWindowUserPointer(m_Window, &m_WindowData);

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int keycode, int scancode, int action, int mods)
		{
			WindowData& data = *((WindowData*)glfwGetWindowUserPointer(window));

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(keycode, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(keycode, 1);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(keycode);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}

		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizedEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowClosedEvent event;
			data.EventCallback(event);
		});
	}

	void WindowGLFW::SetVSync(bool state)
	{
		if (state == true)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_WindowData.VSync = state;
	}

	void WindowGLFW::SetEventCallback(const EventCallbackFn& callback)
	{
		m_WindowData.EventCallback = callback;
	}
}
