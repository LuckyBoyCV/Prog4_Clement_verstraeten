#pragma once
#include "Command.h"

namespace dae
{
    class DamageCommand final : public GameActorCommand
    {
    public:
        explicit DamageCommand(GameObject* pOwner) : GameActorCommand(pOwner) {}
        ~DamageCommand() override = default;
        void Execute() override;
    };
}