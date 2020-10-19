#pragma once

#include "Engine3D\Renderer\Texture.h"

#include "Engine3D\Scene\Entity.h"

namespace E3D
{
	class MaterialInspector
	{
	public:
		MaterialInspector();

		void OnImGuiRender(Entity entity);

	private:
		Ref<Texture> m_WhiteTexture;
		//Test
		Ref<Texture> m_Normal;
		Ref<Texture> m_Roughness;
		Ref<Texture> m_Metalness;

	};
}