#include "pch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>

namespace E3D
{
	GLenum TextureFormatToOpenGLInternalFormat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::R8:				return GL_R8;
		case TextureFormat::RGBA8:			return GL_RGBA8;
		case TextureFormat::RGB8:			return GL_RGB8;
		case TextureFormat::RG8:			return GL_RG8;
		case TextureFormat::RGB16F:			return GL_RGB16F;
		case TextureFormat::RG16F:			return GL_RG16F;
		case TextureFormat::Depth_Stencil:	return GL_DEPTH24_STENCIL8;
		}

		E3D_CORE_ASSERT(false, "Texture format not supported!");
		return 0;
	}

	GLenum TextureFormatToOpenGLDataFormat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::R8:				return GL_RED;
		case TextureFormat::RGBA8:			return GL_RGBA;
		case TextureFormat::RGB8:			return GL_RGB;
		case TextureFormat::RG8:			return GL_RG;
		case TextureFormat::RGB16F:			return GL_RGB;
		case TextureFormat::RG16F:			return GL_RG;
		case TextureFormat::Depth_Stencil:	return GL_DEPTH_STENCIL;
		}

		E3D_CORE_ASSERT(false, "Texture format not supported!");
		return 0;
	}

	GLenum TextureWrapToOpenGLWrap(TextureWrap wrap)
	{
		switch (wrap)
		{
		case TextureWrap::Clamp_To_Edge:		return GL_CLAMP_TO_EDGE;
		case TextureWrap::Clamp_To_Border:		return GL_CLAMP_TO_BORDER;
		case TextureWrap::Repeat:				return GL_REPEAT;
		case TextureWrap::Mirrored_Repeat:		return GL_MIRRORED_REPEAT;
		}

		E3D_CORE_ASSERT(false, "Wrap mode not supporterd!");
		return 0;
	}

	GLenum FormatDataType(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::R8:				return GL_UNSIGNED_BYTE;
		case TextureFormat::RGBA8:			return GL_UNSIGNED_BYTE;
		case TextureFormat::RGB8:			return GL_UNSIGNED_BYTE;
		case TextureFormat::RG8:			return GL_UNSIGNED_BYTE;
		case TextureFormat::RGB16F:			return GL_FLOAT;
		case TextureFormat::RG16F:			return GL_FLOAT;
		case TextureFormat::Depth_Stencil:	return GL_UNSIGNED_INT_24_8;
		}

		E3D_CORE_ASSERT(false, "Texture format not supported!");
		return 0;
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, const TextureSpecification& spec)
		: m_Width(width), m_Height(height), m_TextureSpec(spec)
	{

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_TextureSpec.Filter == TextureFilter::Linear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_TextureSpec.Filter == TextureFilter::Linear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TextureWrapToOpenGLWrap(m_TextureSpec.Wrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TextureWrapToOpenGLWrap(m_TextureSpec.Wrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, TextureWrapToOpenGLWrap(m_TextureSpec.Wrap));

		glTexImage2D(GL_TEXTURE_2D, 0, TextureFormatToOpenGLInternalFormat(m_TextureSpec.Format), m_Width, m_Height, 0, TextureFormatToOpenGLDataFormat(m_TextureSpec.Format), FormatDataType(m_TextureSpec.Format), nullptr);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath, const TextureSpecification& spec)
		: m_TextureSpec(spec)
	{		
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		void* data;

		bool isHDR = stbi_is_hdr(filepath.c_str());

		if (isHDR)
			data = stbi_loadf(filepath.c_str(), &width, &height, &channels, 0);
		else
			data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

		E3D_CORE_ASSERT(data, "Failed to load texture!");

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		if (channels == 2)
		{
			m_TextureSpec.Format = isHDR ? TextureFormat::RG16F : TextureFormat::RG8;

			GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_GREEN };
			glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
		}
		else if (channels == 3)
		{
			m_TextureSpec.Format = isHDR ? TextureFormat::RGB16F : TextureFormat::RGB8;

			GLint swizzleMask[] = { GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA };
			glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
		}
		else if (channels == 4)
		{
			m_TextureSpec.Format = TextureFormat::RGBA8;

			GLint swizzleMask[] = { GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA };
			glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
		}
		else if (channels == 1)
		{
			m_TextureSpec.Format = TextureFormat::R8;

			GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_RED };
			glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
		}
		else
			E3D_CORE_ASSERT(false, "Format not supported!");

		m_Width = width;
		m_Height = height;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_TextureSpec.Filter == TextureFilter::Linear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_TextureSpec.Filter == TextureFilter::Linear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TextureWrapToOpenGLWrap(m_TextureSpec.Wrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TextureWrapToOpenGLWrap(m_TextureSpec.Wrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, TextureWrapToOpenGLWrap(m_TextureSpec.Wrap));

		glTexImage2D(GL_TEXTURE_2D, 0, TextureFormatToOpenGLInternalFormat(m_TextureSpec.Format), m_Width, m_Height, 0, TextureFormatToOpenGLDataFormat(m_TextureSpec.Format), FormatDataType(m_TextureSpec.Format), data);

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

	void OpenGLTexture2D::SetData(const void* data, uint32_t size)
	{
		uint32_t bytesPerPixel = 0;

		switch (m_TextureSpec.Format)
		{
			case TextureFormat::RGBA8:		bytesPerPixel = 4; break;
			case TextureFormat::RGB8:		bytesPerPixel = 3; break;
			case TextureFormat::RGB16F:		bytesPerPixel = 3; break;
			case TextureFormat::RG8:		bytesPerPixel = 2; break;
			case TextureFormat::RG16F:		bytesPerPixel = 2; break;
			default:						E3D_CORE_ASSERT(false, "Format not supported!");
		}
		
		E3D_CORE_ASSERT(size == m_Width * m_Height * bytesPerPixel, "Size must fill the whole texture!");
		
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, TextureFormatToOpenGLDataFormat(m_TextureSpec.Format), FormatDataType(m_TextureSpec.Format), data);
	}

	void OpenGLTexture2D::GenerateMipMaps()
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_TextureSpec.Filter == TextureFilter::Linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST);
		glGenerateMipmap(GL_TEXTURE_2D);
	}


	OpenGLTextureCubeMap::OpenGLTextureCubeMap(uint32_t size, const TextureSpecification& spec)
		: m_Size(size), m_TextureSpec(spec)
	{

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, TextureWrapToOpenGLWrap(m_TextureSpec.Wrap));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, TextureWrapToOpenGLWrap(m_TextureSpec.Wrap));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, TextureWrapToOpenGLWrap(m_TextureSpec.Wrap));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_TextureSpec.Filter == TextureFilter::Linear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_TextureSpec.Filter == TextureFilter::Linear ? GL_LINEAR : GL_NEAREST);

		for (uint32_t i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, TextureFormatToOpenGLInternalFormat(m_TextureSpec.Format), m_Size, m_Size, 0, TextureFormatToOpenGLDataFormat(m_TextureSpec.Format), FormatDataType(m_TextureSpec.Format), nullptr);
		}
	}

	OpenGLTextureCubeMap::~OpenGLTextureCubeMap()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void OpenGLTextureCubeMap::Bind(uint32_t slot)
	{
		glBindTextureUnit(slot, m_TextureID);
	}

	void OpenGLTextureCubeMap::Unbind(uint32_t slot)
	{
		glBindTextureUnit(slot, 0);
	}

	void OpenGLTextureCubeMap::GenerateMipMaps()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_TextureSpec.Filter == TextureFilter::Linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}

	
}