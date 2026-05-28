#include "CoilyEggState.h"
#include "CoilyComponent.h"
#include "CoilySnakeState.h"
#include "CoilyJumpingState.h"
#include "CoilyPlayerState.h"
#include "PyramidComponent.h"
#include <cstdlib>

std::unique_ptr<qbert::CoilyState> qbert::CoilyEggState::Update(CoilyComponent& coily, float deltaTime)
{
	m_JumpTime += deltaTime;
	if (m_JumpTime < m_JumpInterval)
		return nullptr;

	m_JumpTime = 0.f;

	int destRow = coily.getRow() + 1;
	int destCol = coily.getCol() + (std::rand() % 2);

	if (destRow >= PyramidComponent::Rows)
	{
		if (coily.isPlayerControlled())
			return std::make_unique<coilyPlayerState>();
		return std::make_unique<CoilySnakeState>();
	}

	return std::make_unique<CoilyJumpingState>(destRow, destCol, std::make_unique<CoilyEggState>());
}

void qbert::CoilyEggState::onEnter(CoilyComponent& coily)
{
	coily.setSprite(64, 16, 16, 16);
	coily.spriteOffset(-15.f);
	coily.setIsSnake(false);
}
