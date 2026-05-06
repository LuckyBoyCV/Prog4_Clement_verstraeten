#include "CoilyEggState.h"
#include "CoilyComponent.h"
#include "CoilySnakeState.h"
#include "PyramidComponent.h"


qbert::CoilyState* qbert::CoilyEggState::Update(CoilyComponent& coily, float deltaTime)
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
        return new CoilySnakeState();

    coily.Move(destRow, destCol);
    return nullptr;
}

void qbert::CoilyEggState::onEnter(CoilyComponent& coily)
{
    coily.SetSprite(64, 16, 16, 16);
}
