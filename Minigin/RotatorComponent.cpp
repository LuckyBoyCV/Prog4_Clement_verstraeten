#include "RotatorComponent.h"
#include "GameObject.h"
#include <cmath>

dae::RotatorComponent::RotatorComponent(GameObject* owner, float radius, float speed)
	: Component(owner)
	, m_radius(radius)
	, m_speed(speed)
{
}

void dae::RotatorComponent::Update(float deltaTime)
{
	m_angle += m_speed * deltaTime;
	const float x = m_radius * cos(m_angle);
	const float y = m_radius * sin(m_angle);
	m_Owner->SetPosition(x, y);
}
