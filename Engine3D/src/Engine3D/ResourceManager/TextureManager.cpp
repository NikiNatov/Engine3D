#include "pch.h"
#include "TextureManager.h"

namespace E3D
{
	std::unordered_map<std::string, Ref<Texture2D>> TextureManager::m_LoadedTextures;

	Ref<Texture2D> TextureManager::LoadTexture(const Ref<Texture2D>& texture)
	{
		const std::string& name = texture->GetName();

		if (!Exists(name))
		{
			m_LoadedTextures[name] = texture;
			return texture;
		}
		else
		{
			E3D_CORE_LOG_WARNING("Texture with name \"{0}\" already exists", name);
			return m_LoadedTextures[name];
		}
	}

	Ref<Texture2D> TextureManager::GetTexture(const std::string& name)
	{
		E3D_CORE_ASSERT(Exists(name), "Material does not exist!");
		return m_LoadedTextures[name];
	}

	bool TextureManager::Exists(const std::string& name)
	{
		return m_LoadedTextures.find(name) != m_LoadedTextures.end();
	}
}