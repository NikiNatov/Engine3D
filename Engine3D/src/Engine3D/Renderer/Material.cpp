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
		m_Shader->SetFloat3("u_Material.Albedo", m_Properties.Albedo);

		m_Shader->SetFloat("u_Material.Roughness", m_Properties.Roughness);
		m_Shader->SetFloat("u_Material.Metalness", m_Properties.Metalness);

		m_Shader->SetInt("u_Material.AlbedoMap", 0);
		m_Shader->SetInt("u_Material.NormalMap", 1);
		m_Shader->SetInt("u_Material.RoughnessMap", 2);
		m_Shader->SetInt("u_Material.MetalnessMap", 3);

		m_Shader->SetBool("u_UseAlbedoMap", m_UseAlbedoMap);
		m_Shader->SetBool("u_UseNormalMap", m_UseNormalMap);
		m_Shader->SetBool("u_UseRoughnessMap", m_UseRoughnessMap);
		m_Shader->SetBool("u_UseMetalnessMap", m_UseMetalnessMap);

		if (m_Properties.AlbedoMap)
			m_Properties.AlbedoMap->Bind(0);
		if (m_Properties.NormalMap)
			m_Properties.NormalMap->Bind(1);
		if (m_Properties.RoughnessMap)
			m_Properties.RoughnessMap->Bind(2);
		if (m_Properties.MetalnessMap)
			m_Properties.MetalnessMap->Bind(3);
	}

	void Material::Unbind()
	{
		if (m_Properties.AlbedoMap)
			m_Properties.AlbedoMap->Unbind(0);
		if (m_Properties.NormalMap)
			m_Properties.NormalMap->Unbind(1);
		if (m_Properties.RoughnessMap)
			m_Properties.RoughnessMap->Unbind(2);
		if (m_Properties.MetalnessMap)
			m_Properties.MetalnessMap->Unbind(3);
	}

	void Material::SetName(const std::string& name)
	{
		m_Properties.Name = name;
	}

	void Material::SetAlbedoColor(const glm::vec3& color)
	{
		m_Properties.Albedo = color;
	}

	void Material::SetRoughness(float value)
	{
		m_Properties.Roughness = value;
	}

	void Material::SetMetalness(float value)
	{
		m_Properties.Metalness = value;
	}

	void Material::SetAlbedoMap(const Ref<Texture>& texture)
	{
		m_Properties.AlbedoMap = texture;
	}

	void Material::SetNormalMap(const Ref<Texture>& texture)
	{
		m_Properties.NormalMap = texture;
	}

	void Material::SetRoughnessMap(const Ref<Texture>& texture)
	{
		m_Properties.RoughnessMap = texture;
	}

	void Material::SetMetalnessMap(const Ref<Texture>& texture)
	{
		m_Properties.MetalnessMap = texture;
	}

	void Material::UseAlbedoMap(bool state)
	{
		m_UseAlbedoMap = state;
	}

	void Material::UseNormalMap(bool state)
	{
		m_UseNormalMap = state;
	}

	void Material::UseRoughnessMap(bool state)
	{
		m_UseRoughnessMap = state;
	}

	void Material::UseMetalnessMap(bool state)
	{
		m_UseMetalnessMap = state;
	}

}