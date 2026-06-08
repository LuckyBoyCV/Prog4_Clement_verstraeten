#include "ChangeToComponent.h"
#include "PyramidComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

qbert::ChangeToComponent::ChangeToComponent(dae::GameObject* pOwner, PyramidComponent* pyramid)
	: Component(pOwner)
	, m_pPyramid(pyramid)
{
	m_spriteSheet = dae::ResourceManager::GetInstance().LoadTexture("sprites_Qbert.png");
}

void qbert::ChangeToComponent::Render()
{
	if (!m_spriteSheet) return;

	const int set = m_pPyramid ? m_pPyramid->getColorSet() : 0;
	const SDL_FRect src{ static_cast<float>(swatchBaseX + set * swatchStride),
						 swatchY, swatchW, swatchH };

	// Drawn just below the "CHANGE TO:" text label that shares this GameObject.
	const glm::vec3 pos = m_Owner->GetWorldPosition();
	constexpr float scale = 4.f;
	const SDL_FRect dst{ pos.x, pos.y + 28.f, swatchW * scale, swatchH * scale };

	SDL_RenderTexture(dae::Renderer::GetInstance().GetSDLRenderer(),
		m_spriteSheet->GetSDLTexture(), &src, &dst);
}
