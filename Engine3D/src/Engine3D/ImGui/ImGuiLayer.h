#pragma once

#include "Engine3D\Core\Layer.h"

namespace E3D
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& event) override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		inline void BlockEvents(bool block) { m_BlockEvents = block; }
	private:
		bool m_BlockEvents = true;
	};
}