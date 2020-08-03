#pragma once

#include "Engine3D\Core\Window.h"
#include "Engine3D\Renderer\GraphicsContext.h"

#include <GLFW\glfw3.h>

namespace E3D {

	class WindowGLFW : public Window
	{
	public:
		WindowGLFW(const WindowProperties& properties);
		~WindowGLFW();

		virtual void OnUpdate() override;

		virtual void Init(const WindowProperties& properties) override;
		virtual const std::string& GetTitle() const override { return m_WindowData.Title; };
		virtual uint32_t GetWidth() const override { return m_WindowData.Width; }
		virtual uint32_t GetHeight() const override { return m_WindowData.Height; }
		virtual bool IsVSyncOn() const override { return m_WindowData.VSync; }
		virtual void SetVSync(bool state) override;
		virtual void SetEventCallback(const EventCallbackFn& callback) override;

		virtual void* GetNativeWindow() const override { return m_Window; }
	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		struct WindowData
		{
			std::string Title;
			uint32_t Width;
			uint32_t Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_WindowData;
	};
}