#include "pch.h"
#include "Scene.h"

#include "Engine3D\Scene\Entity.h"
#include "Engine3D\Scene\Components.h"

#include "Engine3D\Renderer\Renderer.h"

#include <glm\gtc\type_ptr.hpp>

#include <imgui.h>
#include <ImGuizmo.h>

#include "Engine3D\Core\Input.h"
#include "Engine3D\Core\KeyCodes.h"

namespace E3D
{
	Scene::Scene()
	{

	}

	Scene::Scene(const Ref<Skybox>& skybox)
		: m_Skybox(skybox)
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity(m_Registry.create(), this);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<NameComponent>(name);
		entity.AddComponent<SceneNodeComponent>();

		return entity;
	}


	void Scene::OnSceneStart()
	{
		m_Registry.view<ScriptComponent>().each([=](auto entity, auto& scriptComponent) {

			if (!scriptComponent.Instance)
			{
				scriptComponent.Instance = scriptComponent.CreateScript();
				scriptComponent.Instance->m_Entity = CreateRef<Entity>(entity, this);
				scriptComponent.Instance->OnCreate();
			}
		});
	}

	void Scene::OnUpdate(Timestep ts)
	{
		m_Registry.view<ScriptComponent>().each([=](auto entity, auto& scriptComponent) {
			scriptComponent.Instance->OnUpdate(ts);
		});
	}

	void Scene::OnSceneEnd()
	{
		m_Registry.view<ScriptComponent>().each([=](auto entity, auto& scriptComponent) {

			if (scriptComponent.Instance)
			{
				scriptComponent.Instance->OnDestroy();
				scriptComponent.DestroyScript(&scriptComponent);
			}
		});
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		auto view = m_Registry.view<CameraComponent>();

		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

	void Scene::OnEditRender()
	{
		Renderer::BeginScene(m_CameraController.GetCamera(), m_Skybox);

		auto view = m_Registry.view<TransformComponent, MeshComponent, SceneNodeComponent>();

		for (auto entity : view)
		{
			auto [transform, mesh, node] = view.get<TransformComponent, MeshComponent, SceneNodeComponent>(entity);

			if (node.Parent)
			{
				auto& parentTransform = node.Parent.GetComponent<TransformComponent>().Transform;

				mesh.Mesh->Draw(parentTransform * transform.Transform);
			}
			else
				mesh.Mesh->Draw(transform.Transform);

		}
	}

	void Scene::OnRunningRender()
	{
		auto view = m_Registry.view<CameraComponent, TransformComponent>();

		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;

		for (auto entity : view)
		{
			auto [camera, transform] = view.get<CameraComponent, TransformComponent>(entity);

			if (camera.Primary)
			{
				mainCamera = &camera.Camera;
				cameraTransform = &transform.Transform;
				break;
			}
		}

		if (mainCamera)
		{
			Renderer::BeginScene(*mainCamera, *cameraTransform, m_Skybox);

			auto view = m_Registry.view<TransformComponent, MeshComponent, SceneNodeComponent>();

			for (auto entity : view)
			{
				auto [transform, mesh, node] = view.get<TransformComponent, MeshComponent, SceneNodeComponent>(entity);

				if (node.Parent)
				{
					auto& parentTransform = node.Parent.GetComponent<TransformComponent>().Transform;

					mesh.Mesh->Draw(parentTransform * transform.Transform);
				}
				else
					mesh.Mesh->Draw(transform.Transform);

			}
		}
	}
	
}