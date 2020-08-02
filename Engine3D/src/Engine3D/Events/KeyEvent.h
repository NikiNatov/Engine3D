#pragma once

#include "Event.h"
#include <sstream>

namespace E3D
{
	class KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode)
		{
		}

		int m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount)
		{
		}

		virtual std::string ToString() const override
		{
			std::stringstream ss;

			ss << "KeyPressed: " << m_KeyCode << "(" << m_RepeatCount << ")";

			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount;
	};


	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode)
		{
		}

		virtual std::string ToString() const override
		{
			std::stringstream ss;

			ss << "KeyReleased: " << m_KeyCode;

			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)

	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode)
		{
		}

		std::string ToString() const override
		{
			std::stringstream ss;

			ss << "KeyTyped: " << m_KeyCode;

			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};

	inline std::ostream& operator<<(std::ostream& stream, const Event& event)
	{
		stream << event.ToString();
		return stream;
	}
}