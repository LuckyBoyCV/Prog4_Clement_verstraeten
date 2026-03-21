#include "AchievementComponent.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#if USE_STEAMWORKS
#pragma warning(push)
#pragma warning(disable:4996)
#include <steam_api.h>
#pragma warning(pop)
#endif

dae::AchievementComponent::AchievementComponent(GameObject* pGameObject)
	: Component(pGameObject)
{

}

void dae::AchievementComponent::OnNotify(GameEvent event, GameObject* pGameObject)
{
	constexpr int maxPoints{ 500 };
	if (m_achieved) return;
	if (event != GameEvent::PlayerPickedUp) return;
	auto* score = pGameObject->GetComponent<PlayerComponent>();
	if (score && score->GetScore() >= maxPoints)
	{
		UnlockAchievement();
	}
}

void dae::AchievementComponent::UnlockAchievement()
{
#if USE_STEAMWORKS
	SteamUserStats()->SetAchievement("ACH_WIN_ONE_GAME");
	SteamUserStats()->StoreStats();
#endif
	m_achieved = true;
}