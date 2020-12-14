#include "pch.h"

#include "SkyBox.h"

#include "Engine3D\ResourceManager\TextureManager.h"
#include "Engine3D\Renderer\Shader.h"

#include "Engine3D\Renderer\RenderCommand.h"
#include "Engine3D\Renderer\Framebuffer.h"
#include "Engine3D\Renderer\MeshFactory.h"
#include "Engine3D\Renderer\Renderer.h"

#include <glm\gtc\matrix_transform.hpp>
#include <glad\glad.h>

namespace E3D
{
	Skybox::Skybox()
	{
		m_CubeVAO = MeshFactory::CreateCube(1.0f, CreateRef<Material>(ShaderLibrary::Get("StaticModelShader")))->GetVertexArray();
		
		m_ComputeShader = ShaderLibrary::Load("assets/shaders/SkyboxComputeShader.glsl");
		m_SkyboxShader = ShaderLibrary::Load("assets/shaders/SkyboxShader.glsl");
		m_IrradianceShader = ShaderLibrary::Load("assets/shaders/SkyboxIrradianceShader.glsl");
		m_PrefilterShader = ShaderLibrary::Load("assets/shaders/SkyboxPrefilterShader.glsl");
		m_BRDFShader = ShaderLibrary::Load("assets/shaders/SkyboxBRDFShader.glsl");

		// BRDF TEXTURE ////////////////////////////////////////////////////////////////////////////////
		m_BRDFTextureFB = Framebuffer::Create({ 512, 512, TextureTarget::Texture2D, TextureFormat::RG16F, 1 });

		m_BRDFShader->Bind();
		m_BRDFTextureFB->Bind();

		RenderCommand::ClearScreen();

		float quadVertices[] = {
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};

		uint32_t quadVAO, quadVBO;
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		m_BRDFTextureFB->Unbind();
	}

	void Skybox::SetTexture(const std::string& filepath)
	{
		m_HDRMap = TextureManager::LoadTexture(Texture2D::Create(filepath));
		CreateEnvironmentMap();
	}

	void Skybox::CreateEnvironmentMap()
	{
		m_CubeMapFB = Framebuffer::Create({ 1280, 1280, TextureTarget::CubeMapPositiveX, TextureFormat::RGB16F, 1 });
		m_IrradianceMapFB = Framebuffer::Create({ 32, 32, TextureTarget::CubeMapPositiveX, TextureFormat::RGB16F, 1 });
		m_PrefilterMapFB = Framebuffer::Create({ 128, 128, TextureTarget::CubeMapPositiveX, TextureFormat::RGB16F, 1 });

			
		// CUBE MAP ////////////////////////////////////////////////////////////////////////////////////////
		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 captureViews[] =
		{
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};

		m_ComputeShader->Bind();
		m_ComputeShader->SetInt("u_HDRMap", 0);
		m_ComputeShader->SetMat4("u_Projection", captureProjection);
		
		m_HDRMap->Bind(0);

		m_CubeMapFB->Bind();

		for (uint32_t i = 0; i < 6; i++)
		{
			m_ComputeShader->SetMat4("u_View", captureViews[i]);

			m_CubeMapFB->SetTextureTarget((TextureTarget)((uint32_t)TextureTarget::CubeMapPositiveX + i), 0);
			
			RenderCommand::ClearScreen();

			m_CubeVAO->Bind();
			RenderCommand::DrawIndexed(m_CubeVAO);
		}
		m_CubeMapFB->Unbind();
		m_CubeMapFB->GetColorAttachment()->GenerateMipMaps();

		// IRRADIANCE MAP ////////////////////////////////////////////////////////////////////////////////////////
		m_IrradianceShader->Bind();
		m_IrradianceShader->SetInt("u_SkyBox", 0);
		m_IrradianceShader->SetMat4("u_Projection", captureProjection);

		m_CubeMapFB->GetColorAttachment()->Bind(0);

		m_IrradianceMapFB->Bind();

		for (uint32_t i = 0; i < 6; i++)
		{
			m_IrradianceShader->SetMat4("u_View", captureViews[i]);

			m_IrradianceMapFB->SetTextureTarget((TextureTarget)((uint32_t)TextureTarget::CubeMapPositiveX + i), 0);

			RenderCommand::ClearScreen();

			m_CubeVAO->Bind();
			RenderCommand::DrawIndexed(m_CubeVAO);
		}
		m_IrradianceMapFB->Unbind();
		
		// PREFILTERMAP ////////////////////////////////////////////////////////////////////////////////////////
		m_PrefilterMapFB->GetColorAttachment()->GenerateMipMaps();

		m_PrefilterShader->Bind();
		m_PrefilterShader->SetInt("u_Skybox", 0);
		m_PrefilterShader->SetMat4("u_Projection", captureProjection);

		m_CubeMapFB->GetColorAttachment()->Bind(0);

		m_PrefilterMapFB->Bind();

		uint32_t maxMipLevels = 5;
		for (uint32_t mip = 0; mip < maxMipLevels; ++mip)
		{
			uint32_t mipWidth = 128 * std::pow(0.5, mip);
			uint32_t mipHeight = 128 * std::pow(0.5, mip);

			m_PrefilterMapFB->SetViewport(0, 0, mipWidth, mipHeight);

			float roughness = (float)mip / (float)(maxMipLevels - 1);
			m_PrefilterShader->SetFloat("u_Roughness", roughness);
			for (uint32_t i = 0; i < 6; ++i)
			{
				m_PrefilterShader->SetMat4("u_View", captureViews[i]);

				m_PrefilterMapFB->SetTextureTarget((TextureTarget)((uint32_t)TextureTarget::CubeMapPositiveX + i), mip);

				RenderCommand::ClearScreen();
				
				m_CubeVAO->Bind();
				RenderCommand::DrawIndexed(m_CubeVAO);
			}
		}
		
		m_PrefilterMapFB->Unbind();

		// BRDF ////////////////////////////////////////////////
		
	}
}