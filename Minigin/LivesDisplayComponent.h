#pragma once
#include "Component.h"
#include "Observer.h"

namespace dae
{
    class LivesDisplayComponent final : public Component, public Observer
    {
    public:
        explicit LivesDisplayComponent(GameObject* owner);
        ~LivesDisplayComponent() override = default;

        LivesDisplayComponent(const LivesDisplayComponent&) = delete;
        LivesDisplayComponent(LivesDisplayComponent&&) = delete;
        LivesDisplayComponent& operator=(const LivesDisplayComponent&) = delete;
        LivesDisplayComponent& operator=(LivesDisplayComponent&&) = delete;

        void OnNotify(GameEvent event, GameObject* pOwner) override;
    };
}