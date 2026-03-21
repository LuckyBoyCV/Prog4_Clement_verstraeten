#pragma once
#include "Component.h"
#include "Observer.h"

namespace dae
{
    class ScoreDisplayComponent final : public Component, public Observer
    {
    public:
        explicit ScoreDisplayComponent(GameObject* owner);
        ~ScoreDisplayComponent() override = default;

        ScoreDisplayComponent(const ScoreDisplayComponent&) = delete;
        ScoreDisplayComponent(ScoreDisplayComponent&&) = delete;
        ScoreDisplayComponent& operator=(const ScoreDisplayComponent&) = delete;
        ScoreDisplayComponent& operator=(ScoreDisplayComponent&&) = delete;

        void OnNotify(GameEvent event, GameObject* pOwner) override;
    };
}