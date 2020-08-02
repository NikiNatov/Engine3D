#pragma once

#include "Event.h"

#include <sstream>

namespace E3D
{
	class WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(uint32_t width, uint32_t height)
			: m_Width(width), m_Height(height)
		{
		}

		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;

			ss << "WindowResized: " << m_Width << ", " << m_Height;

			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResized)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		uint32_t m_Width;
		uint32_t m_Height;
	};

	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent()
		{
		}

		EVENT_CLASS_TYPE(WindowClosed)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowMovedEvent : public Event
	{
	public:

	private:
	};
}