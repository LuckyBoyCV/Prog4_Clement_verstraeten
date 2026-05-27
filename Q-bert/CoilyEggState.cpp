#include "CoilyEggState.h"
#include "CoilyComponent.h"
#include "CoilySnakeState.h"
#include "coilyPlayerState.h"
#include "PyramidComponent.h"
#include <cstdlib>
#include <ctime>


std::unique_ptr<qbert::CoilyState> qbert::CoilyEggState::Update(CoilyComponent& coily, float deltaTime)
{
    if (coily.isJumping())
        return nullptr;

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

    coily.Move(destRow, destCol);
    return nullptr;
}

void qbert::CoilyEggState::onEnter(CoilyComponent& coily)
{
    coily.setSprite(64, 16, 16, 16);
	coily.spriteOffset(-15.f);
}
