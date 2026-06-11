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

	//null safety. If pyramid is null use default color
	const int colorSet = m_pPyramid ? m_pPyramid->getColorSet() : 0;

	// Show the tile's *final* target colour. Levels 1 and 3 finish on the visited
	// colour; level 2 needs two hops, so its swatch is the visitedTwice row (one
	// state lower on the sheet) rather than the intermediate single-hop colour.
	float srcY = swatchY;
	if (m_pPyramid && m_pPyramid->getTargetState() == TileState::visitedTwice)
		srcY += nextSwatchOffset;

	const SDL_FRect srcRect{ static_cast<float>(swatchBaseX + colorSet * swatchStride),
						 srcY, swatchW, swatchH };

	// Drawn just below the "CHANGE TO:" text label that shares this GameObject.
	const glm::vec3 pos = m_Owner->GetWorldPosition();
	constexpr float scale = 4.f;
	const SDL_FRect dst{ pos.x, pos.y + 28.f, swatchW * scale, swatchH * scale };

	SDL_RenderTexture(dae::Renderer::GetInstance().GetSDLRenderer(),
		m_spriteSheet->GetSDLTexture(), &srcRect, &dst);
}
