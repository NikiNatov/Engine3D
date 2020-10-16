#pragma once

#include "Engine3D\Renderer\Texture.h"
#include "Engine3D\Renderer\Shader.h"

#include <glm\glm.hpp>

namespace E3D
{
	struct MaterialProperties
	{
		std::string Name;

		glm::vec3 Albedo { 0.8f };
		float Roughness = 1.0f;
		float Metalness = 0.5f;

		Ref<Texture> AlbedoMap;
		Ref<Texture> NormalMap;
		Ref<Texture> RoughnessMap;
		Ref<Texture> MetalnessMap;
	};

	class Material
	{
		friend class MaterialInspector;
	public:
		Material(const Ref<Shader>& shader);
		virtual ~Material() = default;

		void Bind();
		void Unbind();
		void SetName(const std::string& name);

		void SetAlbedoColor(const glm::vec3& color);
		void SetRoughness(float value);
		void SetMetalness(float value);

		void SetAlbedoMap(const Ref<Texture>& texture);
		void SetNormalMap(const Ref<Texture>& texture);
		void SetRoughnessMap(const Ref<Texture>& texture);
		void SetMetalnessMap(const Ref<Texture>& texture);


		void UseAlbedoMap(bool state);
		void UseNormalMap(bool state);
		void UseRoughnessMap(bool state);
		void UseMetalnessMap(bool state);

		inline const Ref<Shader>& GetShader() const { return m_Shader; }
		inline std::string& GetName() { return m_Properties.Name; }
		inline const Ref<Texture>& GetAlbedoMap() const { return m_Properties.AlbedoMap; }
		inline const Ref<Texture>& GetNormalMap() const { return m_Properties.NormalMap; }
		inline const Ref<Texture>& GetRoughnessMap() const { return m_Properties.RoughnessMap; }
		inline const Ref<Texture>& GetMetalnessMap() const { return m_Properties.MetalnessMap; }
		inline const glm::vec3& GetAlbedoColor() const { return m_Properties.Albedo; }
		inline const float GetRoughness() const { return m_Properties.Roughness; }
		inline const float GetMetalness() const { return m_Properties.Metalness; }

	protected:
		MaterialProperties m_Properties;

		bool m_UseAlbedoMap = false;
		bool m_UseNormalMap = false;
		bool m_UseRoughnessMap = false;
		bool m_UseMetalnessMap = false;

		Ref<Shader> m_Shader;
	};
}