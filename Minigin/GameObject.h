#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <type_traits>
#include "Transform.h"
#include "Component.h"

namespace dae
{
	class GameObject final
	{
	public:
		void Update(float deltaTime);
		void Render() const;

		template<typename T, typename... Args>
		T* AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
			auto comp = std::make_unique<T>(this, std::forward<Args>(args)...);
			T* raw = comp.get();
			m_components.push_back(std::move(comp));
			return raw;
		}

		template<typename T>
		T* GetComponent() const
		{
			static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
			for (const auto& c : m_components)
				if (auto* ptr = dynamic_cast<T*>(c.get()))
					return ptr;
			return nullptr;
		}

		template<typename T>
		bool HasComponent() const { return GetComponent<T>() != nullptr; }

		template<typename T>
		void RemoveComponent()
		{
			static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
			m_components.erase(
				std::remove_if(m_components.begin(), m_components.end(),
					[](const std::unique_ptr<Component>& c)
					{ return dynamic_cast<T*>(c.get()) != nullptr; }),
				m_components.end());
		}

		Transform& GetTransform() { return m_transform; }
		const Transform& GetTransform() const { return m_transform; }
		void SetPosition(float x, float y) { m_transform.SetPosition(x, y, 0.f); }

		GameObject() = default;
		~GameObject() = default;
		GameObject(const GameObject&) = delete;
		GameObject(GameObject&&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject& operator=(GameObject&&) = delete;

	private:
		Transform m_transform{};
		std::vector<std::unique_ptr<Component>> m_components;
	};
}