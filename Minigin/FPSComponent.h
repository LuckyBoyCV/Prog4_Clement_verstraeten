#pragma once
#include "Component.h"

namespace dae
{
	class FPSComponent final : public Component
	{
	public:
		FPSComponent(GameObject* owner);
		~FPSComponent() override = default;
		void Update(float deltaTime) override;
	private:
		float m_accumulatedTime{ 0.f };
		int   m_frameCount{ 0 };
		float m_updateInterval{ 0.5f };
	};
}