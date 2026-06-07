#pragma once
#include "Component.h"
#include "Observer.h"
#include <vector>

namespace qbert
{
	class GameStateComponent;
	class Enemy;

	// Owns which enemies are active each round. Coily is present every round and
	// counts toward the on-screen cap, so at most one descending enemy (red ball /
	// Slick / Sam) is active alongside it. Descenders are paced in one at a time
	// and rotated for variety when a slot frees up (timeout or eaten).
	class enemySpawnerComponent final : public dae::Component, public dae::Observer
	{
	public:
		enemySpawnerComponent(dae::GameObject* owner, GameStateComponent* gameState,
			Enemy* coily, Enemy* redBall, Enemy* slick, Enemy* sam);
		~enemySpawnerComponent() override = default;
		enemySpawnerComponent(const enemySpawnerComponent&) = delete;
		enemySpawnerComponent(enemySpawnerComponent&&) = delete;
		enemySpawnerComponent& operator=(const enemySpawnerComponent&) = delete;
		enemySpawnerComponent& operator=(enemySpawnerComponent&&) = delete;

		void Update(float deltaTime) override;
		void OnNotify(dae::GameEvent event, dae::GameObject* pOwner) override;

	private:
		void gateForRound();

		GameStateComponent* m_pGameState;
		Enemy* m_pCoily;
		Enemy* m_pRedBall;
		Enemy* m_pSlick;
		Enemy* m_pSam;

		std::vector<Enemy*> m_eligible;   // descenders allowed this round
		Enemy*  m_pCurrent{ nullptr };    // the one active descender (if any)
		size_t  m_rotationIndex{ 0 };     // cycles through m_eligible for variety
		float   m_freeTimer{ 0.f };       // counts up while the descender slot is empty
		float   m_lifeTimer{ 0.f };       // counts up while a descender is active
		bool    m_gated{ false };         // first-round gate runs on the first Update

		static constexpr int   kMaxConcurrent   = 2;   // Coily + one descender
		static constexpr float kFirstSpawnDelay = 3.f; // before the first descender of a round
		static constexpr float kDescenderLife   = 8.f; // how long a descender stays before rotating
		static constexpr float kRotationGap     = 2.f; // gap between descenders
	};
}
