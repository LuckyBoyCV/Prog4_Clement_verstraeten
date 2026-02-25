#pragma once
#include "Component.h"

namespace dae
{
	class RotatorComponent final : public Component
	{
	public:
		explicit RotatorComponent(GameObject* owner, float radius, float speed);
		~RotatorComponent() override = default;

		void Update(float deltaTime) override;

	private:
		float m_radius;
		float m_speed;
		float m_angle{ 0.f };
	};
}