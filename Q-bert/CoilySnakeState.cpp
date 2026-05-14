#include "CoilySnakeState.h"
#include "CoilyComponent.h"

qbert::CoilyState* qbert::CoilySnakeState::Update(CoilyComponent& coily, float deltaTime)
{
	if (coily.isJumping())
	{
		return nullptr;
	}

    m_JumpTime += deltaTime;
    if (m_JumpTime < m_JumpInterval)
        return nullptr;

    m_JumpTime = 0.f;

    int row = coily.getRow();
    int col = coily.getCol();
    int qRow = coily.getQbertRow();
    int qCol = coily.getQbertCol();

    int destRow;
    int destCol;

    if (qRow > row)
    {
        destRow = row + 1;
        if (qCol > col)
            destCol = col + 1;
        else
            destCol = col;
    }
    else
    {
        destRow = row - 1;
        if (qCol >= col)
            destCol = col;
        else
            destCol = col - 1;
    }
	coily.Move(destRow, destCol);
	return nullptr;
}

void qbert::CoilySnakeState::onEnter(CoilyComponent& coily)
{
	coily.SetSprite(80, 32, 16, 32);
}
