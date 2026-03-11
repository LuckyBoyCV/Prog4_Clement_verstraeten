#include "MoveCommand.h"
#include "GameObject.h"

void dae::MoveCommand::Execute()
{
	glm::vec3 newPos = GetOwner()->GetWorldPosition() + m_direction * m_speed;
	GetOwner()->SetLocalPostion(newPos);
}