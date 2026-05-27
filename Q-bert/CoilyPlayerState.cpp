#include "coilyPlayerState.h"
#include "CoilyComponent.h"

std::unique_ptr<qbert::CoilyState> qbert::coilyPlayerState::Update(CoilyComponent&, float)
{
	return nullptr; 
}

void qbert::coilyPlayerState::onEnter(CoilyComponent& coily)
{
	coily.setSprite(80, 32, 16, 32);
	coily.spriteOffset(-40.f);
	coily.setIsSnake(true);
}
