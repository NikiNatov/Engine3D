#pragma once

#include "Engine3D\Core\Config.h"
#include "Engine3D\Renderer\Material.h"
#include "Engine3D\Renderer\VertexArray.h"

namespace E3D
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec2 TextureCoord;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
	};

	class Mesh
	{
	public:
		Mesh(const Ref<VertexArray>& vertexArray, const Ref<Material>& material);
		Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const Ref<Material>& material);
		Mesh(const Ref<Mesh>& other);
		~Mesh();

		void Render();

		inline const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }

		inline Ref<Material>& GetMaterial() { return m_Material; }
		inline void SetMaterial(const Ref<Material>& material) { m_Material = material; }

		inline const std::string& GetName() const { return m_Name; }
		inline void SetName(const std::string& name) { m_Name = name; }
	private:
		Ref<VertexArray> m_VertexArray;
		Ref<Material> m_Material;

		std::string m_Name = "";
	};	
}