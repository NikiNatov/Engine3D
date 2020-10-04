#pragma once

#include "Engine3D\Renderer\Model.h"

#include <glm\glm.hpp>

namespace E3D
{
	class MeshFactory
	{
	public:
		static Ref<Model> CreatePlane(float width, float height, const Ref<Material>& material);
		static Ref<Model> CreatePlane(const glm::vec2& size, const Ref<Material>& material);
				   
		static Ref<Model> CreateCube(float size, const Ref<Material>& material);
		static Ref<Model> CreateGrid(uint32_t numVerticesX, uint32_t numVerticesZ, float cellSize, const Ref<Material>& material);
		static Ref<Model> CreateGrid(const glm::vec2& size, float cellSize, const Ref<Material>& material);
				   
		static Ref<Model> CreateQuad(float x, float y, float width, float height, const Ref<Material>& material);
		static Ref<Model> CreateQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Material>& material);
	};
}