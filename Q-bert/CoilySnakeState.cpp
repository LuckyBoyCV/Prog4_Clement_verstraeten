#include "CoilySnakeState.h"
#include "CoilyComponent.h"

qbert::CoilyState* qbert::CoilySnakeState::Update(CoilyComponent& coily, float deltaTime)
{
	if (coily.isJumping())
	{
		return nullptr;
	}

	m_JumpTime += deltaTime;

	int rowDiff = coily.getQbertRow() - coily.getRow();
	int colDiff = coily.getQbertCol() - coily.getCol();

	int rowStep;
	if (rowDiff > 0)
		rowStep = 1;
	else if (rowDiff < 0) 
		rowStep = -1;
	else                  
		rowStep = 0;

	int colStep;
	if (colDiff > 0) 
		colStep = 1;
	else if (colDiff < 0) 
		colStep = -1;
	else                  
		colStep = 0;

	if (rowStep == 1 && colStep != 0 && colStep != 1)  colStep = 0;
	if (rowStep == -1 && colStep != 0 && colStep != -1) colStep = 0;

	coily.Move(rowStep, colStep);
	return nullptr;
}

void qbert::CoilySnakeState::onEnter(CoilyComponent& coily)
{
	coily.SetSprite(0, 48, 16, 32);
}
