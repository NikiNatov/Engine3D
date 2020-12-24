#pragma once

#include "Engine3D\Renderer\Mesh.h"

namespace E3D
{
	class AnimatedMesh : public Mesh
	{
	public:
		AnimatedMesh(const Ref<VertexArray>& vertexArray, const Ref<Material>& material);
		AnimatedMesh(const std::vector<AnimatedVertex>& vertices, const std::vector<uint32_t>& indices, const Ref<Material>& material);

		virtual const Ref<VertexArray>& GetVertexArray() const override { return m_VertexArray; }

		virtual Ref<Material>& GetMaterial() override { return m_Material; }
		virtual void SetMaterial(const Ref<Material>& material) override { m_Material = material; }

		virtual const std::string& GetName() const { return m_Name; }
		virtual void SetName(const std::string& name) override { m_Name = name; }

		virtual void Render() override;
	private:
		Ref<VertexArray> m_VertexArray;
		Ref<Material> m_Material;
		std::string m_Name;
	};
}