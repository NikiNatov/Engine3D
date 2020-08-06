#pragma once

#include "Engine3D\Events\Event.h"
#include "Engine3D\Core\Timestep.h"

#include <string>

namespace E3D
{
	class Layer
	{
	public:
		Layer(const std::string& name);
		virtual ~Layer() = default;

		virtual void OnUpdate(Timestep ts) {};
		virtual void OnEvent(Event& event) {}

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnImGuiRender() {};

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}