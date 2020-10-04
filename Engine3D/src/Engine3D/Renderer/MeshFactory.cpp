#include "pch.h"
#include "MeshFactory.h"

#include "Engine3D\Renderer\Buffer.h"
#include "Engine3D\Renderer\VertexArray.h"

namespace E3D
{

		Ref<Model> MeshFactory::CreatePlane(float width, float height, const Ref<Material>& material)
		{
			std::vector<Vertex> vertices;
			vertices.resize(4);

			vertices[0].Position = glm::vec3(-width / 2.0f, 0.0f, -height / 2.0f);
			vertices[0].TextureCoord = glm::vec2(0.0f, 0.0f);
			vertices[0].Normal = glm::vec3(0.0f, 1.0f, 0.0f);
			vertices[0].Tangent = glm::vec3(1.0f, 0.0f, 0.0f);
			vertices[0].Bitangent = glm::vec3(0.0f, 0.0f, 1.0f);

			vertices[1].Position = glm::vec3(width / 2.0f, 0.0f, -height / 2.0f);
			vertices[1].TextureCoord = glm::vec2(1.0f, 0.0f);
			vertices[1].Normal = glm::vec3(0.0f, 1.0f, 0.0f);
			vertices[1].Tangent = glm::vec3(1.0f, 0.0f, 0.0f);
			vertices[1].Bitangent = glm::vec3(0.0f, 0.0f, 1.0f);

			vertices[2].Position = glm::vec3(width / 2.0f, 0.0f, height / 2.0f);
			vertices[2].TextureCoord = glm::vec2(1.0f, 1.0f);
			vertices[2].Normal = glm::vec3(0.0f, 1.0f, 0.0f);
			vertices[2].Tangent = glm::vec3(1.0f, 0.0f, 0.0f);
			vertices[2].Bitangent = glm::vec3(0.0f, 0.0f, 1.0f);

			vertices[3].Position = glm::vec3(-width / 2.0f, 0.0f, height / 2.0f);
			vertices[3].TextureCoord = glm::vec2(0.0f, 1.0f);
			vertices[3].Normal = glm::vec3(0.0f, 1.0f, 0.0f);
			vertices[3].Tangent = glm::vec3(1.0f, 0.0f, 0.0f);
			vertices[3].Bitangent = glm::vec3(0.0f, 0.0f, 1.0f);

			std::vector<uint32_t> indices = {
				0, 1, 2, 2, 3, 0
			};

			Ref<Model> model = CreateRef<Model>();
			model->AddMesh(CreateRef<Mesh>(vertices, indices, material));

			return model;
		}

		Ref<Model> MeshFactory::CreatePlane(const glm::vec2& size, const Ref<Material>& material)
		{
			return CreatePlane(size.x, size.y, material);
		}

		Ref<Model> MeshFactory::CreateCube(float size, const Ref<Material>& material)
		{
			std::vector<Vertex> vertices;
			vertices.reserve(24);
			
			glm::vec3 positions[8];
			glm::vec3 normals[6];
			glm::vec2 texCoords[14];

			positions[0] = glm::vec3( size / 2.0f,  size / 2.0f, -size / 2.0f);
			positions[1] = glm::vec3( size / 2.0f, -size / 2.0f, -size / 2.0f);
			positions[2] = glm::vec3( size / 2.0f,  size / 2.0f,  size / 2.0f);
			positions[3] = glm::vec3( size / 2.0f, -size / 2.0f,  size / 2.0f);
			positions[4] = glm::vec3(-size / 2.0f,  size / 2.0f, -size / 2.0f);
			positions[5] = glm::vec3(-size / 2.0f, -size / 2.0f, -size / 2.0f);
			positions[6] = glm::vec3(-size / 2.0f,  size / 2.0f,  size / 2.0f);
			positions[7] = glm::vec3(-size / 2.0f, -size / 2.0f,  size / 2.0f);

			normals[0] = glm::vec3( 0.0f,  1.0f,  0.0f);
			normals[1] = glm::vec3( 0.0f,  0.0f,  1.0f);
			normals[2] = glm::vec3(-1.0f,  0.0f,  0.0f);
			normals[3] = glm::vec3( 0.0f, -1.0f,  0.0f);
			normals[4] = glm::vec3( 1.0f,  0.0f,  0.0f);
			normals[5] = glm::vec3( 0.0f,  0.0f, -1.0f);

			
			texCoords[0] =	glm::vec2(0.875f, 0.500f);
			texCoords[1] =	glm::vec2(0.625f, 0.750f);
			texCoords[2] =	glm::vec2(0.625f, 0.500f);
			texCoords[3] =	glm::vec2(0.375f, 1.000f);
			texCoords[4] =	glm::vec2(0.375f, 0.750f);
			texCoords[5] =	glm::vec2(0.625f, 0.000f);
			texCoords[6] =	glm::vec2(0.375f, 0.250f);
			texCoords[7] =	glm::vec2(0.375f, 0.000f);
			texCoords[8] =	glm::vec2(0.375f, 0.500f);
			texCoords[9] =	glm::vec2(0.125f, 0.750f);
			texCoords[10] = glm::vec2(0.125f, 0.500f);
			texCoords[11] = glm::vec2(0.625f, 0.250f);
			texCoords[12] = glm::vec2(0.875f, 0.750f);
			texCoords[13] = glm::vec2(0.625f, 1.000f);
			
			vertices.push_back({ positions[4], texCoords[0], normals[0] });
			vertices.push_back({ positions[2], texCoords[1], normals[0] });
			vertices.push_back({ positions[0], texCoords[2], normals[0] });
			vertices.push_back({ positions[2], texCoords[1], normals[1] });
			vertices.push_back({ positions[7], texCoords[3], normals[1] });
			vertices.push_back({ positions[3], texCoords[4], normals[1] });
			vertices.push_back({ positions[6], texCoords[5], normals[2] });
			vertices.push_back({ positions[5], texCoords[6], normals[2] });
			vertices.push_back({ positions[7], texCoords[7], normals[2] });
			vertices.push_back({ positions[1], texCoords[8], normals[3] });
			vertices.push_back({ positions[7], texCoords[9], normals[3] });
			vertices.push_back({ positions[5], texCoords[10], normals[3] });

			vertices.push_back({ positions[0], texCoords[2], normals[4] });
			vertices.push_back({ positions[3], texCoords[4], normals[4] });
			vertices.push_back({ positions[1], texCoords[8], normals[4] });
			vertices.push_back({ positions[4], texCoords[11], normals[5] });
			vertices.push_back({ positions[1], texCoords[8], normals[5] });
			vertices.push_back({ positions[5], texCoords[6], normals[5] });
			//4,0,0                                                        
			vertices.push_back({ positions[6], texCoords[12], normals[0] });
				//2,1,0                                                        
				//2,1,1                                                        
			vertices.push_back({ positions[6], texCoords[13], normals[1] });
			//7,3,1                                                        
			//6,5,2                                                        
			vertices.push_back({ positions[4], texCoords[11], normals[2] });
			//5,6,2                                                        
			//1,8,3                                                        
			vertices.push_back({ positions[3], texCoords[4], normals[3] });
			//7,9,3                                                        
			//0,2,4                                                        
			vertices.push_back({ positions[2], texCoords[1], normals[4] });
			//3,4,4                                                        
			//4,11,5                                                       
			vertices.push_back({ positions[0], texCoords[2], normals[5] });
			//1,8,5                                                        

			std::vector<uint32_t> indices =
			{
				0, 1, 2,
				3, 4, 5,
				6, 7, 8,
				9, 10, 11,
				12, 13, 14,
				15, 16, 17,
				0, 18, 1,
				3, 19, 4,
				6, 20, 7,
				9, 21, 10,
				12, 22, 13,
				15, 23, 16
			};

			Ref<Model> model = CreateRef<Model>();
			model->AddMesh(CreateRef<Mesh>(vertices, indices, material));

			return model;
		}

		Ref<Model> MeshFactory::CreateGrid(uint32_t numVerticesX, uint32_t numVerticesZ, float cellSize, const Ref<Material>& material)
		{
			std::vector<Vertex> vertices;
			vertices.reserve(numVerticesX * numVerticesZ);

			for (uint32_t i = 0; i < numVerticesZ; i++)
			{
				for (uint32_t j = 0; j < numVerticesX; j++)
				{
					Vertex vertex;

					float x = ((float)j / (float)numVerticesX - 0.5f) * cellSize;
					float y = 0.0f;
					float z = ((float)i / (float)numVerticesZ - 0.5f) * cellSize;

					vertex.Position = { x, y, z };
					vertex.Normal = { 0.0f, 1.0f, 0.0f };
					vertex.TextureCoord = { ((float)j / (float)numVerticesX) , ((float)i / (float)numVerticesZ) };
					vertex.Tangent = { 1.0f, 0.0f, 0.0f };
					vertex.Bitangent = { 0.0f, 0.0f, 1.0f };

					vertices.push_back(vertex);
				}
			}

			std::vector<uint32_t> indices;
			uint32_t numIndices = (numVerticesX - 1) * (numVerticesZ - 1) * 8;
			indices.reserve(numIndices);

			for (uint32_t i = 0; i < numVerticesZ - 1; i++)
			{
				for (uint32_t j = 0; j < numVerticesX - 1; j++)
				{
					indices.push_back(j + i * numVerticesX);
					indices.push_back(j + 1 + i * numVerticesX);
					indices.push_back(j + 1 + i * numVerticesX);
					indices.push_back(j + 1 + (i + 1) * numVerticesX);
					indices.push_back(j + 1 + (i + 1) * numVerticesX);
					indices.push_back(j + (i + 1) * numVerticesX);
					indices.push_back(j + (i + 1) * numVerticesX);
					indices.push_back(j + i * numVerticesX);
				}
			}

			Ref<Model> model = CreateRef<Model>();
			model->AddMesh(CreateRef<Mesh>(vertices, indices, material));

			return model;
		}

		Ref<Model> MeshFactory::CreateGrid(const glm::vec2& size, float cellSize, const Ref<Material>& material)
		{
			return CreateGrid(size.x, size.y, cellSize, material);
		}

		Ref<Model> MeshFactory::CreateQuad(float x, float y, float width, float height, const Ref<Material>& material)
		{
			/*struct QuadVertex
			{
				glm::vec3 Position;
				glm::vec2 TextureCoord;
			};*/

			std::vector<Vertex> vertices;
			vertices.resize(4);

			vertices[0].Position = glm::vec3(x, y, 0.0f);
			vertices[0].TextureCoord = glm::vec2(0.0f, 0.0f);

			vertices[1].Position = glm::vec3(x + width, y, 0.0f);
			vertices[1].TextureCoord = glm::vec2(1.0f, 0.0f);

			vertices[2].Position = glm::vec3(x + width, y + height, 0.0f);
			vertices[2].TextureCoord = glm::vec2(1.0f, 1.0f);

			vertices[3].Position = glm::vec3(x, y + height, 0.0f);
			vertices[3].TextureCoord = glm::vec2(0.0f, 1.0f);

			std::vector<uint32_t> indices = {
				0, 1, 2, 2, 3, 0
			};

			Ref<Model> model = CreateRef<Model>();
			model->AddMesh(CreateRef<Mesh>(vertices, indices, material));

			return model;
		}

		Ref<Model> MeshFactory::CreateQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Material>& material)
		{
			return CreateQuad(position.x, position.y, size.x, size.y, material);
		}

}