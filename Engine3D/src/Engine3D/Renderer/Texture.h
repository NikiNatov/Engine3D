#pragma once

#include "Engine3D\Core\Config.h"

#include <string>

namespace E3D
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void Bind(uint32_t slot) = 0;
		virtual void Unbind(uint32_t slot) = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& filepath);
	};
}