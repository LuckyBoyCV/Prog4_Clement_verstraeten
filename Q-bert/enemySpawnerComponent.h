#pragma once
#include "Component.h"
#include "Observer.h"
#include <vector>

namespace qbert
{
	class GameStateComponent;
	class Enemy;

	// Owns which enemies are active each round, Coily is present every round and
	// counts toward the on screen cap, so at most one other enemy is active alongside
	// it: a descender red ball / Slick / Sam, or an upward climber Ugg / Wrongway
	// The pool is paced in one at a time and rotated for variety when a slot frees up
	// after a timeout or when eaten
	class EnemySpawnerComponent final : public dae::Component, public dae::Observer
	{
	public:
		EnemySpawnerComponent(dae::GameObject* owner, GameStateComponent* gameState,
			Enemy* coily, Enemy* redBall, Enemy* slick, Enemy* sam, Enemy* ugg, Enemy* wrongway);
		~EnemySpawnerComponent() override = default;
		EnemySpawnerComponent(const EnemySpawnerComponent&) = delete;
		EnemySpawnerComponent(EnemySpawnerComponent&&) = delete;
		EnemySpawnerComponent& operator=(const EnemySpawnerComponent&) = delete;
		EnemySpawnerComponent& operator=(EnemySpawnerComponent&&) = delete;

		void Update(float deltaTime) override;
		void OnNotify(dae::GameEvent event, dae::GameObject* pOwner) override;

	private:
		void setupRoundEnemies();

		GameStateComponent* m_pGameState;
		Enemy* m_pCoily;
		Enemy* m_pRedBall;
		Enemy* m_pSlick;
		Enemy* m_pSam;
		Enemy* m_pUgg;
		Enemy* m_pWrongway;

		std::vector<Enemy*> m_roundDescenders;   // enemies eligible to rotate in this round
		Enemy* m_pCurrent{ nullptr };    // the one active rotating enemy if any
		size_t m_rotationIndex{ 0 };     // cycles through m_eligible for variety
		float m_descenderCooldownTimer{ 0.f };      // counts up while the descender slot is empty
		float m_lifeTimer{ 0.f };       // counts up while a descender is active
		bool m_gated{ false };         // first-round gate runs on the first Update

		static constexpr int maxSimultaneousEnemies = 2;   // Coily + one descender
		static constexpr float firstDescenderDelay = 3.f; // before the first descender of a round
		static constexpr float descenderLife   = 8.f; // how long a descender stays before rotating
		static constexpr float descenderSpawnInterval = 5.f; // gap between descenders
	};
}
