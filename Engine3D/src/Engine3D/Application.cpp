#include "Application.h"

namespace E3D {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{

		}
	}
}
