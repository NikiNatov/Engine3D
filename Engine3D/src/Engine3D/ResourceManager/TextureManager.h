#pragma once

#include "Engine3D\Renderer\Texture.h"

namespace E3D
{
	class TextureManager
	{
	public:
		TextureManager(const TextureManager& other) = delete;
		TextureManager& operator=(const TextureManager& other) = delete;

		static Ref<Texture2D> LoadTexture(const Ref<Texture2D>& texture);

		static Ref<Texture2D> GetTexture(const std::string& name);
		static const std::unordered_map<std::string, Ref<Texture2D>>& GetLoadedTextures() { return m_LoadedTextures; }
		static bool Exists(const std::string& name);
	private:
		TextureManager() {};
	private:
		static std::unordered_map<std::string, Ref<Texture2D>> m_LoadedTextures;
	};
}