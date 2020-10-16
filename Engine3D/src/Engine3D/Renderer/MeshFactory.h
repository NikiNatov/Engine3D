#pragma once

#include "Engine3D\Renderer\Mesh.h"

#include <glm\glm.hpp>

namespace E3D
{
	class MeshFactory
	{
	public:
		static Ref<Mesh> CreatePlane(float width, float height, const Ref<Material>& material);
		static Ref<Mesh> CreatePlane(const glm::vec2& size, const Ref<Material>& material);
				   
		static Ref<Mesh> CreateCube(float size, const Ref<Material>& material);
		static Ref<Mesh> CreateGrid(uint32_t numVerticesX, uint32_t numVerticesZ, float cellSize, const Ref<Material>& material);
		static Ref<Mesh> CreateGrid(const glm::vec2& size, float cellSize, const Ref<Material>& material);
				   
		static Ref<Mesh> CreateQuad(float x, float y, float width, float height, const Ref<Material>& material);
		static Ref<Mesh> CreateQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Material>& material);
	};
}