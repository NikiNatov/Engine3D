#pragma once

#include "Engine3D\Renderer\Texture.h"

#include <glad\glad.h>

namespace E3D
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height, const TextureSpecification& spec, bool genMips);
		OpenGLTexture2D(const std::string& filepath, const TextureSpecification& spec, bool genMips);
		~OpenGLTexture2D();

		virtual void Bind(uint32_t slot) override;
		virtual void Unbind(uint32_t slot) override;

		virtual void SetData(const void* data, uint32_t size) override;
		virtual void GenerateMipMaps() override;

		virtual uint32_t GetTextureID() const override { return m_TextureID; }
		virtual const std::string& GetName() const { return m_Name; }

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual bool operator==(const Texture& other) override
		{
			return m_TextureID == ((OpenGLTexture2D&)other).m_TextureID;
		}
	private:
		std::string m_Name = "";
		uint32_t m_TextureID;
		uint32_t m_Width, m_Height;
		TextureSpecification m_TextureSpec;
	};

	class OpenGLTextureCubeMap : public TextureCubeMap
	{
	public:
		OpenGLTextureCubeMap(uint32_t size, const TextureSpecification& spec, bool genMips);
		~OpenGLTextureCubeMap();

		virtual void Bind(uint32_t slot) override;
		virtual void Unbind(uint32_t slot) override;

		virtual void GenerateMipMaps() override;

		virtual uint32_t GetTextureID() const override { return m_TextureID; }

		virtual uint32_t GetSize() const override { return m_Size; }

		virtual bool operator==(const Texture& other) override
		{
			return m_TextureID == ((OpenGLTextureCubeMap&)other).m_TextureID;
		}
	private:
		uint32_t m_TextureID;
		uint32_t m_Size;
		TextureSpecification m_TextureSpec;

	};
}