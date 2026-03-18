#pragma once
#include "Command.h"

namespace dae
{
    class ScoreCommand final : public GameActorCommand
    {
    public:
        explicit ScoreCommand(GameObject* pOwner, int points)
            : GameActorCommand(pOwner), m_Points(points) {
        }
        ~ScoreCommand() override = default;
        void Execute() override;

    private:
        int m_Points;
    };
}