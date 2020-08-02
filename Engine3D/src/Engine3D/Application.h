#pragma once

#include <iostream>

namespace E3D {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	public:
		inline static Application& GetInstance() { return *s_Instance; }
	private:
		bool m_Running = true;
	private:
		static Application* s_Instance;
	};

	static Application* Create();
}