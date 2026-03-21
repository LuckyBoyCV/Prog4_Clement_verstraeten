#pragma once
#include "Component.h"
#include "Subject.h"

namespace dae
{
    class PlayerComponent final : public Component
    {
    public:
        explicit PlayerComponent(GameObject* owner, int lives = 3);
        ~PlayerComponent() override = default;

        PlayerComponent(const PlayerComponent&) = delete;
        PlayerComponent(PlayerComponent&&) = delete;
        PlayerComponent& operator=(const PlayerComponent&) = delete;
        PlayerComponent& operator=(PlayerComponent&&) = delete;

        void LoseLife();
        void AddScore(int score);

        int GetLives() const { return m_Lives; }
        int GetScore() const { return m_Score; }

        Subject m_subject;

    private:
        int m_Lives{ 3 };
        int m_Score{ 0 };
    };
}