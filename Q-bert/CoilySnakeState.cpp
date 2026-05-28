#include "CoilySnakeState.h"
#include "CoilyJumpingState.h"
#include "CoilyComponent.h"

std::unique_ptr<qbert::CoilyState> qbert::CoilySnakeState::Update(CoilyComponent& coily, float deltaTime)
{
	m_JumpTime += deltaTime;
	if (m_JumpTime < m_JumpInterval)
		return nullptr;

	m_JumpTime = 0.f;

	int row  = coily.getRow();
	int col  = coily.getCol();
	int qRow = coily.getQbertRow();
	int qCol = coily.getQbertCol();

	int destRow;
	int destCol;

	if (qRow > row)
	{
		destRow = row + 1;
		destCol = (qCol > col) ? col + 1 : col;
	}
	else
	{
		destRow = row - 1;
		destCol = (qCol >= col) ? col : col - 1;
	}

	return std::make_unique<CoilyJumpingState>(destRow, destCol, std::make_unique<CoilySnakeState>());
}

void qbert::CoilySnakeState::onEnter(CoilyComponent& coily)
{
	coily.setSprite(80, 32, 16, 32);
	coily.spriteOffset(-40.f);
	coily.setIsSnake(true);
}
