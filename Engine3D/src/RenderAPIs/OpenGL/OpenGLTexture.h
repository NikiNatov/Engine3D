#pragma once

#include "Engine3D\Renderer\Texture.h"

namespace E3D
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& filepath);
		~OpenGLTexture2D();

		virtual void Bind(uint32_t slot) override;
		virtual void Unbind(uint32_t slot) override;

		virtual uint32_t GetTextureID() const override { return m_TextureID; }

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
	private:
		uint32_t m_TextureID;
		uint32_t m_Width;
		uint32_t m_Height;
	};
}