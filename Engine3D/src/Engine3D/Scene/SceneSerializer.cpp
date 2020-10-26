#include "pch.h"
#include "SceneSerializer.h"

#include "Engine3D\Scene\Components.h"
#include "Engine3D\Scene\Entity.h"

#include "Engine3D\ResourceManager\MeshManager.h"

#include <yaml-cpp\yaml.h>

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}


namespace E3D
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << (uint32_t)entity;

		if (entity.HasComponent<NameComponent>())
		{
			out << YAML::Key << "NameComponent";
			out << YAML::BeginMap;

			auto& name = entity.GetComponent<NameComponent>().Name;
			out << YAML::Key << "Name" << YAML::Value << name;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<MeshComponent>())
		{
			out << YAML::Key << "MeshComponent";
			out << YAML::BeginMap;

			auto& mc = entity.GetComponent<MeshComponent>();
			out << YAML::Key << "Mesh" << YAML::Value << mc.Mesh->GetName();

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			out << YAML::Key << "Camera" << YAML::Value;

			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNear();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFar();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNear();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFar();
			out << YAML::EndMap;

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SceneNodeComponent>())
		{
			out << YAML::Key << "SceneNodeComponent";
			out << YAML::BeginMap;

			auto& sceneNodeComponent = entity.GetComponent<SceneNodeComponent>();
			auto parent = sceneNodeComponent.Parent;
			auto firstChild = sceneNodeComponent.FirstChild;
			auto nextSibling = sceneNodeComponent.NextSibling;
			auto previousSibling = sceneNodeComponent.PreviousSibling;

			out << YAML::Key << "Parent" << YAML::Value << (uint32_t)parent;
			out << YAML::Key << "FirstChild" << YAML::Value << (uint32_t)firstChild;
			out << YAML::Key << "NextSibling" << YAML::Value << (uint32_t)nextSibling;
			out << YAML::Key << "PreviousSibling" << YAML::Value << (uint32_t)previousSibling;

			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		m_Scene->m_Registry.each([&](auto entityID)
		{
			Entity entity = { entityID, m_Scene.get() };
			if (!entity)
				return;

			SerializeEntity(out, entity);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		E3D_CORE_ASSERT(false, "Not implemented");
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;

		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		E3D_CORE_LOG_TRACE("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];

		if (entities)
		{
			for (int64_t it = entities.size() - 1; it >= 0; it--)
			{
				uint64_t uuid = entities[it]["Entity"].as<uint64_t>();

				std::string name;
				auto nameComponent = entities[it]["NameComponent"];
				if (nameComponent)
					name = nameComponent["Name"].as<std::string>();

				E3D_CORE_LOG_TRACE("Deserializing entity with ID = '{0}', name = {1}", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntity(name);

				auto transformComponent = entities[it]["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto meshComponent = entities[it]["MeshComponent"];
				if (meshComponent)
				{
					auto& mc = deserializedEntity.AddComponent<MeshComponent>();
					auto mesh = MeshManager::GetMesh(meshComponent["Mesh"].as<std::string>());
					mc.Mesh = CreateRef<Mesh>(mesh->GetVertexArray(), CreateRef<Material>(mesh->GetMaterial()->GetShader())); // TODO: Material Instances
					mc.Mesh->SetName(mesh->GetName());
				}

				auto cameraComponent = entities[it]["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();
					auto cameraProperties = cameraComponent["Camera"];

					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProperties["ProjectionType"].as<int>());

					cc.Camera.SetPerspectiveFOV(cameraProperties["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectiveNear(cameraProperties["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFar(cameraProperties["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthographicSize(cameraProperties["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNear(cameraProperties["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFar(cameraProperties["OrthographicFar"].as<float>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
				}

				auto sceneNodeComponent = entities[it]["SceneNodeComponent"];
				if (sceneNodeComponent)
				{
					auto& snc = deserializedEntity.GetComponent<SceneNodeComponent>();

					auto id1 = sceneNodeComponent["Parent"].as<uint32_t>();
					auto id2 = sceneNodeComponent["FirstChild"].as<uint32_t>();
					auto id3 = sceneNodeComponent["NextSibling"].as<uint32_t>();
					auto id4 = sceneNodeComponent["PreviousSibling"].as<uint32_t>();

					snc.Parent = Entity(sceneNodeComponent["Parent"].as<uint32_t>(), m_Scene.get());
					snc.FirstChild = Entity(sceneNodeComponent["FirstChild"].as<uint32_t>(), m_Scene.get());
					snc.NextSibling = Entity(sceneNodeComponent["NextSibling"].as<uint32_t>(), m_Scene.get());
					snc.PreviousSibling = Entity(sceneNodeComponent["PreviousSibling"].as<uint32_t>(), m_Scene.get());
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		E3D_CORE_ASSERT(false, "Not implemented");
		return false;
	}
}