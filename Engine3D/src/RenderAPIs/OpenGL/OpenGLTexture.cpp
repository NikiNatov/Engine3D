#include "pch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>
#include <glad\glad.h>

namespace E3D
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

		E3D_CORE_ASSERT(data, "Failed to load texture!");

		GLenum internalFormat = 0;
		GLenum format = 0;

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		if (channels == 2)
		{
			internalFormat = GL_RG8;
			format = GL_RG;

			GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_GREEN };
			glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			format = GL_RGB;

			GLint swizzleMask[] = { GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA };
			glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
		}
		else if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			format = GL_RGBA;

			GLint swizzleMask[] = { GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA };
			glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
		}
		
		m_Width = width;
		m_Height = height;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot)
	{
		glBindTextureUnit(slot, m_TextureID);
	}

	void OpenGLTexture2D::Unbind(uint32_t slot)
	{
		glBindTextureUnit(slot, 0);
	}
}