#pragma once
#include "Component.h"
#include "Observer.h"
#include "Subject.h"
#include "../Q-bert/GameEvent.h"

namespace dae
{
	class GameObject;
	class AchievementComponent final : public Component, public Observer
	{
	public:
		explicit AchievementComponent(GameObject* pGameObject);
		virtual ~AchievementComponent() noexcept override = default;

		AchievementComponent(const AchievementComponent& other) = delete;
		AchievementComponent(AchievementComponent&& other) = delete;
		AchievementComponent& operator=(const AchievementComponent& other) = delete;
		AchievementComponent& operator= (AchievementComponent&& other) = delete;

		virtual void OnNotify(GameEvent event, GameObject* pGameObject) override;
	private:
		void UnlockAchievement();

		bool m_achieved{ false };
	};
}