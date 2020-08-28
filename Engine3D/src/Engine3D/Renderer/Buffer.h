#pragma once

#include "Engine3D\Core\Config.h"

#include <string>
#include <vector>

namespace E3D
{
	enum class DataType
	{
		None = 0,
		Int, Int2, Int3, Int4,
		Float, Float2, Float3, Float4,
		Mat4,
		Bool
	};

	struct BufferElement
	{
		std::string Name;
		DataType Type;
		bool Normalized;
		uint32_t Offset;

		BufferElement(const std::string& name, DataType type, bool normalized)
			: Name(name), Type(type), Normalized(normalized)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case DataType::Int:		return 1;
				case DataType::Int2:	return 2;
				case DataType::Int3:	return 3;
				case DataType::Int4:	return 4;
				case DataType::Float:	return 1;
				case DataType::Float2:	return 2;
				case DataType::Float3:	return 3;
				case DataType::Float4:	return 4;
				case DataType::Mat4:	return 4 * 4;
				case DataType::Bool:	return 1;
			}

			E3D_CORE_ASSERT(false, "Unknown Data Type!");
			return 0;
		}

		uint32_t GetElementSize() const
		{
			switch (Type)
			{
				case DataType::Int:		return 1 * 4;
				case DataType::Int2:	return 2 * 4;
				case DataType::Int3:	return 3 * 4;
				case DataType::Int4:	return 4 * 4;
				case DataType::Float:	return 1 * 4;
				case DataType::Float2:	return 2 * 4;
				case DataType::Float3:	return 3 * 4;
				case DataType::Float4:	return 4 * 4;
				case DataType::Mat4:	return 4 * 4 * 4;
				case DataType::Bool:	return 1;
			}

			E3D_CORE_ASSERT(false, "Unknown Data Type!");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		~BufferLayout() = default;

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline uint32_t GetStride() const { return m_Stride; }

		void CalculateOffsetsAndStride()
		{
			uint32_t offset = 0;

			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.GetElementSize();
				m_Stride += element.GetElementSize();
			}
		}

		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		static Ref<VertexBuffer> Create(const void* data, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(const uint32_t* data, uint32_t count);
	};
}