#include "RenderComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"


dae::RenderComponent::RenderComponent(GameObject* pOwner)
	: Component(pOwner)
{
}


dae::RenderComponent::RenderComponent(GameObject* pOwner, const std::string& filename)
	:Component(pOwner)
{
	SetTexture(filename);
}


void dae::RenderComponent::Render() const
{
	if (m_pTexture)
	{
		const auto& pos = m_Owner->GetTransform().GetPosition();
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
	}
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::RenderComponent::SetTexture(std::shared_ptr<Texture2D> texture)
{
	m_pTexture = texture;
}
