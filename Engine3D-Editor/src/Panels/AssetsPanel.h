#pragma once

#include "Engine3D\ResourceManager\ModelManager.h"
#include "Engine3D\ResourceManager\TextureManager.h"
#include "Engine3D\ResourceManager\MaterialManager.h"

namespace E3D
{
	class AssetsPanel
	{
	public:
		AssetsPanel() = default;
		~AssetsPanel() = default;

		void OnImGuiRender();
	private:
	};
}