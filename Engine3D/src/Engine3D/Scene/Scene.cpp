#include "pch.h"
#include "Scene.h"

#include "Engine3D\Scene\Entity.h"
#include "Engine3D\Scene\Components.h"

#include "Engine3D\Renderer\Renderer.h"
#include "Engine3D\Renderer\Model.h"

#include <glm\gtc\type_ptr.hpp>

#include <imgui.h>
#include <ImGuizmo.h>

#include "Engine3D\Core\Input.h"
#include "Engine3D\Core\KeyCodes.h"

namespace E3D
{
	Scene::Scene()
		: m_Camera(glm::perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f))
	{
		
	}

	Scene::Scene(const Ref<Skybox>& skybox)
		: m_Camera(glm::perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f)), m_Skybox(skybox)
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

	Entity Scene::CreateFromModel(const Ref<Model>& model, const std::string& name)
	{
		Entity modelEntity = CreateEntity(name);
		modelEntity.GetComponent<SceneNodeComponent>().FirstChild = CreateEntity("Model Hierarchy");
		modelEntity.GetComponent<SceneNodeComponent>().FirstChild.GetComponent<SceneNodeComponent>().Parent = modelEntity;
		CreateFromModelNode(model->GetRootNode(), modelEntity.GetComponent<SceneNodeComponent>().FirstChild);
		return modelEntity;
	}

	void Scene::DeleteEntity(Entity entity)
	{
		entity.RemoveChildren();

		Entity parent = entity.GetComponent<SceneNodeComponent>().Parent;

		if (parent)
		{
			if (parent.GetComponent<SceneNodeComponent>().FirstChild == entity)
			{
				parent.GetComponent<SceneNodeComponent>().FirstChild = entity.GetComponent<SceneNodeComponent>().NextSibling;
				if(entity.GetComponent<SceneNodeComponent>().NextSibling)
					entity.GetComponent<SceneNodeComponent>().NextSibling.GetComponent<SceneNodeComponent>().PreviousSibling = {};
			}
			else
			{
				Entity prev = entity.GetComponent<SceneNodeComponent>().PreviousSibling;
				Entity next = entity.GetComponent<SceneNodeComponent>().NextSibling;

				if (prev)
					prev.GetComponent<SceneNodeComponent>().NextSibling = next;
				if (next)
					next.GetComponent<SceneNodeComponent>().PreviousSibling = prev;
			}
		}

		m_Registry.destroy((entt::entity)entity);
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
		m_ViewportWidth = width;
		m_ViewportHeight = height;

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
		Renderer::BeginScene(m_Camera, m_Skybox);

		Renderer::Submit(m_Skybox);

		auto view = m_Registry.view<TransformComponent, MeshComponent, SceneNodeComponent>();

		for (auto entity : view)
		{
			auto [tc, mc, sc] = view.get<TransformComponent, MeshComponent, SceneNodeComponent>(entity);

			if (sc.Parent)
			{
				auto accumulatedTransform = sc.Parent.GetComponent<TransformComponent>().GetTransform();
				Entity currentParent = sc.Parent;

				while (currentParent.GetComponent<SceneNodeComponent>().Parent)
				{
					currentParent = currentParent.GetComponent<SceneNodeComponent>().Parent;
					accumulatedTransform = currentParent.GetComponent<TransformComponent>().GetTransform() * accumulatedTransform;
				}

				Renderer::Submit(mc.Mesh, accumulatedTransform * tc.GetTransform());
			}
			else
				Renderer::Submit(mc.Mesh, tc.GetTransform());

		}
	}

	void Scene::OnRunningRender()
	{
		auto view = m_Registry.view<CameraComponent, TransformComponent, SceneNodeComponent>();

		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;

		for (auto entity : view)
		{
			auto [camera, transform, sc] = view.get<CameraComponent, TransformComponent, SceneNodeComponent>(entity);

			if (camera.Primary)
			{
				mainCamera = &camera.Camera;
				
				if (sc.Parent)
				{
					auto accumulatedTransform = sc.Parent.GetComponent<TransformComponent>().GetTransform();
					Entity currentParent = sc.Parent;

					while (currentParent.GetComponent<SceneNodeComponent>().Parent)
					{
						currentParent = currentParent.GetComponent<SceneNodeComponent>().Parent;
						accumulatedTransform = currentParent.GetComponent<TransformComponent>().GetTransform() * accumulatedTransform;
					}

					cameraTransform = accumulatedTransform * transform.GetTransform();
				}
				else
					cameraTransform = transform.GetTransform();
				break;
			}
		}

		if (mainCamera)
		{
			Renderer::BeginScene(*mainCamera, cameraTransform, m_Skybox);

			Renderer::Submit(m_Skybox);

			auto view = m_Registry.view<TransformComponent, MeshComponent, SceneNodeComponent>();

			for (auto entity : view)
			{
				auto [tc, mc, sc] = view.get<TransformComponent, MeshComponent, SceneNodeComponent>(entity);

				if (sc.Parent)
				{
					auto accumulatedTransform = sc.Parent.GetComponent<TransformComponent>().GetTransform();
					Entity currentParent = sc.Parent;

					while (currentParent.GetComponent<SceneNodeComponent>().Parent)
					{
						currentParent = currentParent.GetComponent<SceneNodeComponent>().Parent;
						accumulatedTransform = currentParent.GetComponent<TransformComponent>().GetTransform() * accumulatedTransform;
					}

					Renderer::Submit(mc.Mesh, accumulatedTransform * tc.GetTransform());

				}
				else
					Renderer::Submit(mc.Mesh, tc.GetTransform());

			}
		}
	}

	
	void Scene::CreateFromModelNode(const Ref<ModelNode>& node, Entity parentEntity)
	{
		Entity newParent;

		if (!node->Meshes.empty())
		{
			// TODO: Remove the for loop
			for (auto& mesh : node->Meshes)
			{
				Entity newEntity = CreateEntity(mesh->GetName());
				newEntity.AddComponent<MeshComponent>(mesh);

				glm::vec3 translation, rotation, scale;
				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(node->Transform), glm::value_ptr(translation), glm::value_ptr(rotation), glm::value_ptr(scale));

				newEntity.GetComponent<TransformComponent>().Translation = translation;
				newEntity.GetComponent<TransformComponent>().Rotation = glm::radians(rotation);
				newEntity.GetComponent<TransformComponent>().Scale = scale;

				newEntity.GetComponent<SceneNodeComponent>().Parent = parentEntity;

				parentEntity.AddChild(newEntity);

				newParent = newEntity;
			}
		}
		else
			newParent = parentEntity;

		for (auto& child : node->Children)
		{
			CreateFromModelNode(child, newParent);
		}
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<MeshComponent>(Entity entity, MeshComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NameComponent>(Entity entity, NameComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<SceneNodeComponent>(Entity entity, SceneNodeComponent& component)
	{
	}
}