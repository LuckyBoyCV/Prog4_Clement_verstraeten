#pragma once


namespace dae
{
	class GameObject;

	class Component
	{
	public:
		explicit Component(GameObject* owner) : m_Owner(owner) {}
		virtual ~Component() = default;

		virtual void Update(float) {};
		virtual void Render() const {}

		Component(const Component&) = delete;
		Component(Component&&) = delete;
		Component& operator=(const Component&) = delete;
		Component& operator=(Component&&) = delete;

	protected:
		GameObject*  const m_Owner;
	};
}