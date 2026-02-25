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
		const glm::vec3 pos = m_Owner->GetWorldPosition();
		const glm::vec2 size = m_pTexture->GetSize();
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y, size.x * m_scale, size.y * m_scale);
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
