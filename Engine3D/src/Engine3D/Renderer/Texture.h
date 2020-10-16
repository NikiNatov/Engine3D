#pragma once

#include "Engine3D\Core\Config.h"

#include <string>

namespace E3D
{
	enum class TextureFormat
	{
		None = 0, RGBA8, RGB8, RG8, R8, RGB16F, RG16F, Depth_Stencil
	};

	enum class TextureFilter
	{
		None = 0, Nearest, Linear
	};

	enum class TextureWrap
	{
		None = 0, Clamp_To_Edge, Clamp_To_Border, Repeat, Mirrored_Repeat
	};

	struct TextureSpecification
	{
		TextureFormat Format;
		TextureFilter Filter;
		TextureWrap Wrap;

		TextureSpecification() 
			: Format(TextureFormat::RGBA8), Filter(TextureFilter::Linear), Wrap(TextureWrap::Clamp_To_Edge)
		{
		}

		TextureSpecification(TextureFormat format, TextureFilter filter, TextureWrap wrap)
			: Format(format), Filter(filter), Wrap(wrap)
		{
		}
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void Bind(uint32_t slot) = 0;
		virtual void Unbind(uint32_t slot) = 0;

		virtual void GenerateMipMaps() = 0;

		virtual uint32_t GetTextureID() const = 0;
		

		virtual bool operator==(const Texture& other) = 0;
	};

	class Texture2D : public Texture
	{
	public:
		virtual void SetData(const void* data, uint32_t size) = 0;
		virtual const std::string& GetName() const = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		static Ref<Texture2D> Create(uint32_t width, uint32_t height, const TextureSpecification& spec = TextureSpecification(), bool genMips = false);
		static Ref<Texture2D> Create(const std::string& filepath, const TextureSpecification& spec = TextureSpecification(), bool genMips = false);
	};

	class TextureCubeMap : public Texture
	{
	public:
		virtual uint32_t GetSize() const = 0;

		static Ref<TextureCubeMap> Create(uint32_t size, const TextureSpecification& spec = TextureSpecification(), bool genMips = false);
	};
}