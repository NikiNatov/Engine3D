#pragma once

#include "Engine3D\Core\Config.h"

#include <string>
#include <functional>

#include "Engine3D\Events\Event.h"

namespace E3D {

	struct WindowProperties
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		bool VSync;

		WindowProperties(const std::string& title = "Engine3D", uint32_t width = 1280, uint32_t height = 720, bool vSync = false)
			: Title(title), Width(width), Height(height), VSync(vSync)
		{
		}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window() = default;
		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual void Init(const WindowProperties& properties) = 0;
		virtual const std::string& GetTitle() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual bool IsVSyncOn() const = 0;
		virtual void SetVSync(bool state) = 0;
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProperties& properties = WindowProperties());
	};
}