#include "pch.h"

#include "Material.h"

namespace E3D
{
	Material::Material(const Ref<Shader>& shader)
		: m_Shader(shader)
	{
	}

	void Material::Bind()
	{
		m_Shader->Bind();
		m_Shader->SetFloat3("u_Material.Ambient", m_Properties.Ambient);
		m_Shader->SetFloat3("u_Material.Diffuse", m_Properties.Diffuse);
		m_Shader->SetFloat3("u_Material.Specular", m_Properties.Specular);

		m_Shader->SetFloat("u_Material.Shininess", m_Properties.Shininess);
		m_Shader->SetFloat("u_Material.Transparency", m_Properties.Transparency);

		m_Shader->SetInt("u_Material.DiffuseTexture", 0);
		m_Shader->SetInt("u_Material.SpecularTexture", 1);
		m_Shader->SetInt("u_Material.NormalMap", 2);

		m_Shader->SetBool("u_HasDiffuseTexture", m_HasDiffuseTexture);
		m_Shader->SetBool("u_HasSpecularTexture", m_HasSpecularTexture);
		m_Shader->SetBool("u_UseDiffuseTexture", m_UseDiffuseTex);
		m_Shader->SetBool("u_UseSpecularTexture", m_UseSpecularTex);
		m_Shader->SetBool("u_UseNormalMap", m_UseNormalMap);

		if (m_Properties.DiffuseTexture)
			m_Properties.DiffuseTexture->Bind(0);
		if (m_Properties.SpecularTexture)
			m_Properties.SpecularTexture->Bind(1);
		if (m_Properties.NormalMap)
			m_Properties.NormalMap->Bind(2);
	}

	void Material::SetName(const std::string& name)
	{
		m_Properties.Name = name;
	}

	void Material::SetAmbientColor(const glm::vec3& ambient)
	{
		m_Properties.Ambient = ambient;
	}

	void Material::SetDiffuseColor(const glm::vec3& diffuse)
	{
		m_Properties.Diffuse = diffuse;
	}

	void Material::SetSpecularColor(const glm::vec3& specular)
	{
		m_Properties.Specular = specular;
	}

	void Material::SetShininess(float shininess)
	{
		m_Properties.Shininess = shininess;
	}

	void Material::SetTransparency(float transparency)
	{
		m_Properties.Transparency = transparency;
	}

	void Material::SetDiffuseTexture(const Ref<Texture>& texture)
	{
		m_Properties.DiffuseTexture = texture;
		SetHasDiffuseTexture(true);
		UseDiffuseTexture(true);
	}

	void Material::SetSpecularTexture(const Ref<Texture>& texture)
	{
		m_Properties.SpecularTexture = texture;
		SetHasSpecularTexture(true);
		UseSpecularTexture(true);
	}

	void Material::SetNormalMap(const Ref<Texture>& texture)
	{
		m_Properties.NormalMap = texture;
	}

	void Material::SetHasDiffuseTexture(bool state)
	{
		m_HasDiffuseTexture = state;
	}

	void Material::SetHasSpecularTexture(bool state)
	{
		m_HasSpecularTexture = state;
	}

}