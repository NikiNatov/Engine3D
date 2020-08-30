#pragma once

#include "Engine3D\Scene\Entity.h"
#include "Engine3D\Scene\Components.h"

namespace E3D
{
	struct ComponentData
	{
		std::string Name;
		void(*DrawInfo)(Entity);
		void(*AddComponent)(Entity);
		void(*RemoveComponent)(Entity);
	};

	class EntityInspector
	{
	public:
		EntityInspector();

		void OnImGuiRender(Entity entity);
	private:
		template<typename T>
		void RegisterComponent(const std::string& name)
		{
			ComponentData component;
			component.Name = name;
			component.AddComponent = [](Entity entity) { entity.AddComponent<T>(); };
			component.RemoveComponent = [](Entity entity) { entity.RemoveComponent<T>(); };

			auto componentID = entt::type_info<T>::id();
			auto [iterator, result] = m_RegisteredComponents.insert_or_assign(componentID, component);
			E3D_CORE_ASSERT(result, "Component already registered!");
		}

	private:
		using ComponentID = ENTT_ID_TYPE;
		std::unordered_map<ComponentID, ComponentData> m_RegisteredComponents;
	};
}
