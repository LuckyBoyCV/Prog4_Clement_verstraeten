#include "coilyJumpCommand.h"
#include "CoilyComponent.h"
#include "PyramidComponent.h"
#include "GameObject.h"

void qbert::coilyJumpCommand::Execute()
{
	auto* coily = GetOwner()->GetComponent<qbert::CoilyComponent>();
	if (!coily || !coily->getIsSnake())
		return;

	int targetRow = coily->getRow() + m_destRow;
	int targetCol = coily->getCol() + m_destCol;

	if (targetRow < 0 || targetRow >= qbert::PyramidComponent::Rows
		|| targetCol < 0 || targetCol > targetRow)
		return;

	coily->Move(targetRow, targetCol);
}
