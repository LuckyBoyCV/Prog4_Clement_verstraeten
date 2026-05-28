#include "QbertIdleState.h"
#include "QbertComponent.h"

std::unique_ptr<qbert::QbertState> qbert::QbertIdleState::Update(QbertComponent&, float)
{
	// Transitions happen in QbertComponent::Move() when a JumpCommand fires
	return nullptr;
}
