#pragma once

#include "Event.h"

#include <sstream>

namespace E3D
{
	class MouseButtonEvent : public Event
	{
	public:
		inline int GetButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
	protected:
		MouseButtonEvent(int button)
			: m_Button(button)
		{
		}

		int m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button)
		{
		}

		std::string ToString() const override
		{
			std::stringstream ss;

			ss << "MouseButtonPressed: " << m_Button;

			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button)
		{
		}

		std::string ToString() const override
		{
			std::stringstream ss;

			ss << "MouseButtonReleased: " << m_Button;

			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float xPos, float yPos)
			: m_XPos(xPos), m_YPos(yPos)
		{
		}

		inline float GetXPosition() const { return m_XPos; }
		inline float GetYPosition() const { return m_YPos; }

		std::string ToString() const override
		{
			std::stringstream ss;

			ss << "MouseMoved: " << m_XPos << ", " << m_YPos;

			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
	private:
		float m_XPos;
		float m_YPos;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset)
		{
		}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;

			ss << "MouseScrolled: " << m_XOffset << ", " << m_YOffset;

			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
	private:
		float m_XOffset;
		float m_YOffset;
	};
}