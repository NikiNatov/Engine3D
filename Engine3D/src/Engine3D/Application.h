#pragma once

#include "Engine3D\Window.h"
#include "Engine3D\Events\Event.h"
#include "Engine3D\Events\ApplicationEvent.h"

#include <memory>

namespace E3D {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnUpdate();
		void OnEvent(Event& event);
	private:
		bool OnWindowClosed(WindowClosedEvent& event);
	public:
		inline static Application& GetInstance() { return *s_Instance; }
	private:
		bool m_Running = true;

		std::unique_ptr<Window> m_Window;
	private:
		static Application* s_Instance;
	};

	static Application* Create();
}