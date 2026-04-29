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


void dae::RenderComponent::Render() 
{
    if (m_pTexture)
    {
        const glm::vec3 pos = m_Owner->GetWorldPosition();
        const glm::vec2 size = m_pTexture->GetSize();

        SDL_FRect destination{
            pos.x, pos.y,
            (m_hasSrcRect ? m_sourceRect.w : size.x) * m_scale,
            (m_hasSrcRect ? m_sourceRect.h : size.y) * m_scale
        };

        const SDL_FRect* src = m_hasSrcRect ? &m_sourceRect : nullptr;
        SDL_RenderTexture(dae::Renderer::GetInstance().GetSDLRenderer(),
            m_pTexture->GetSDLTexture(), src, &destination);
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

void dae::RenderComponent::SetSourceRect(int x, int y, int width, int height)
{
	m_sourceRect = { static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height) };
	m_hasSrcRect = true;
}
