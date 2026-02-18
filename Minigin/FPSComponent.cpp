#include "FPSComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include <sstream>
#include <iomanip>

dae::FPSComponent::FPSComponent(GameObject* owner)
	: Component(owner)
{
}

void dae::FPSComponent::Update(float deltaTime)
{
	++m_frameCount;
	m_accumulatedTime += deltaTime;

	if (m_accumulatedTime >= m_updateInterval)
	{
		const float fps = static_cast<float>(m_frameCount) / m_accumulatedTime;

		std::ostringstream oss;
		oss << std::fixed << std::setprecision(1) << fps << " FPS";

		if (auto* text = m_Owner->GetComponent<TextComponent>())
			text->SetText(oss.str());

		m_frameCount = 0;
		m_accumulatedTime = 0.f;
	}
}
