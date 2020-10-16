#pragma once

#include "Engine3D\Renderer\Texture.h"
#include "Engine3D\Renderer\Shader.h"
#include "Engine3D\Renderer\VertexArray.h"

#include "Engine3D\Renderer\Framebuffer.h"

namespace E3D
{
	class Skybox
	{
	public:
		Skybox();

		void SetTexture(const std::string& filepath);

		void CreateEnvironmentMap();

		inline const Ref<VertexArray>& GetVertexArray() const { return m_CubeVAO; }
		inline const Ref<Shader>& GetShader() const { return m_SkyboxShader; }
		inline const Ref<Shader>& GetBRDFShader() const { return m_BRDFShader; }
		inline const Ref<Texture2D>& GetHDRMap() const { return m_HDRMap; }
		inline const Ref<Texture>& GetCubeMap() const { return m_CubeMapFB->GetColorAttachment(); }
		inline const Ref<Texture>& GetIrradianceMap() const { return m_IrradianceMapFB->GetColorAttachment(); }
		inline const Ref<Texture>& GetPrefilterMap() const { return m_PrefilterMapFB->GetColorAttachment(); }
		inline const Ref<Texture>& GetBRDFTexture() const { return m_BRDFTextureFB->GetColorAttachment(); }
		inline float& GetExposure() { return m_Exposure; }
		inline float& GetLOD() { return m_LOD; }
	private:
		float m_Exposure = 3.0f;
		float m_LOD = 0.0f;
		Ref<Shader> m_ComputeShader;
		Ref<Shader> m_SkyboxShader;
		Ref<Shader> m_IrradianceShader;
		Ref<Shader> m_PrefilterShader;
		Ref<Shader> m_BRDFShader;
		Ref<VertexArray> m_CubeVAO;
		Ref<Texture2D> m_HDRMap;

		Ref<Framebuffer> m_CubeMapFB;
		Ref<Framebuffer> m_IrradianceMapFB;
		Ref<Framebuffer> m_PrefilterMapFB;
		Ref<Framebuffer> m_BRDFTextureFB;
	};
}