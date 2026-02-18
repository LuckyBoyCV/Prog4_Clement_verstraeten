#include "RenderComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::RenderComponent::RenderComponent(GameObject* pOwner, const std::string& filename)
	:Component(pOwner)
{
	SetTexture(filename);
}


void dae::RenderComponent::Render() const
{
	if (m_texture)
	{
		const auto& pos = m_Owner->GetTransform().GetPosition();
		Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
	}
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}
