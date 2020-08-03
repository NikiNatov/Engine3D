#pragma once

#include "Engine3D\Layer.h"

namespace E3D
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnUpdate() override;
		virtual void OnEvent(Event& event) override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
	};
}