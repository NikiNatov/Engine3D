#pragma once

#include "Engine3D\Renderer\Texture.h"
#include "Engine3D\Renderer\Mesh.h"

namespace E3D
{
	class ModelInspector
	{
	public:
		ModelInspector();

		void OnImGuiRender(Ref<Model>& model);
	private:
		Ref<Texture> m_WhiteTexture;
		//Test
		Ref<Texture> m_Normal;
		Ref<Texture> m_Roughness;
		Ref<Texture> m_Metalness;
	};
}