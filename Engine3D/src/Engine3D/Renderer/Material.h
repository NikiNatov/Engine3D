#pragma once

#include "Engine3D\Renderer\Texture.h"
#include "Engine3D\Renderer\Shader.h"

#include <glm\glm.hpp>

namespace E3D
{
	struct MaterialProperties
	{
		std::string Name;

		glm::vec3 Ambient{ 0.0f };
		glm::vec3 Diffuse{ 1.0f };
		glm::vec3 Specular{ 1.0f };
		float Shininess = 35.0f;
		float Transparency = 1.0f;

		Ref<Texture> DiffuseTexture;
		Ref<Texture> SpecularTexture;
		Ref<Texture> NormalMap;
	};

	class Material
	{
	public:
		Material(const Ref<Shader>& shader);
		virtual ~Material() = default;

		void Bind();

		void SetName(const std::string& name);

		void SetAmbientColor(const glm::vec3& ambient);
		void SetDiffuseColor(const glm::vec3& diffuse);
		void SetSpecularColor(const glm::vec3& specular);

		void SetShininess(float shininess);
		void SetTransparency(float transparency);

		void SetDiffuseTexture(const Ref<Texture>& texture);
		void SetSpecularTexture(const Ref<Texture>& texture);
		void SetNormalMap(const Ref<Texture>& texture);

		inline bool HasDiffuseTexture() const { return m_HasDiffuseTexture; }
		inline bool HasSpecularTexture() const { return m_HasSpecularTexture; }

		inline const Ref<Shader>& GetShader() const { return m_Shader; }

		inline std::string& GetName() { return m_Properties.Name; }

		inline glm::vec3& GetAmbientColor() { return m_Properties.Ambient; }
		inline glm::vec3& GetDiffuseColor() { return m_Properties.Diffuse; }
		inline glm::vec3& GetSpecularColor() { return m_Properties.Specular; }

		inline Ref<Texture>& GetDiffuseTexture() { return m_Properties.DiffuseTexture; }
		inline Ref<Texture>& GetSpecularTexture() { return m_Properties.SpecularTexture; }
		inline float& GetShininess() { return m_Properties.Shininess; }
		inline float& GetTransparency() { return m_Properties.Transparency; }

		inline void UseDiffuseTexture(bool state) { m_UseDiffuseTex = state; m_Shader->SetBool("u_UseDiffuseTexture", m_UseDiffuseTex);
		}
		inline void UseSpecularTexture(bool state) { m_UseSpecularTex = state; m_Shader->SetBool("u_UseSpecularTexture", m_UseSpecularTex);
		}
	protected:
		void SetHasDiffuseTexture(bool state);
		void SetHasSpecularTexture(bool state);
	protected:
		MaterialProperties m_Properties;

		bool m_HasDiffuseTexture = false;
		bool m_HasSpecularTexture = false;

		bool m_UseDiffuseTex = false;
		bool m_UseSpecularTex = false;
		bool m_UseNormalMap = false;

		Ref<Shader> m_Shader;
	};
}