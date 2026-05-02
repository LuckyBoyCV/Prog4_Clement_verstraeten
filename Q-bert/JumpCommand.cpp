#include "JumpCommand.h"
#include "QbertComponent.h"
#include "GameObject.h"

// JumpCommand.cpp
void qbert::JumpCommand::Execute()
{
	if (auto* qbert = GetOwner()->GetComponent<QbertComponent>())
		qbert->Move(m_destRow, m_destCol);
}