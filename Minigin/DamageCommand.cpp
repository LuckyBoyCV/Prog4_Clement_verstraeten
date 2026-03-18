#include "DamageCommand.h"
#include "GameObject.h"
#include "PlayerComponent.h"

void dae::DamageCommand::Execute()
{
    auto* player = GetOwner()->GetComponent<PlayerComponent>();
    if (player) player->LoseLife();
}