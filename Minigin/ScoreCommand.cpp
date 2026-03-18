#include "ScoreCommand.h"
#include "GameObject.h"
#include "PlayerComponent.h"

void dae::ScoreCommand::Execute()
{
    auto* player = GetOwner()->GetComponent<PlayerComponent>();
    if (player) player->AddScore(m_Points);
}