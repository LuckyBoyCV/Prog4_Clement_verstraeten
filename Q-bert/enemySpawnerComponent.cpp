#include "enemySpawnerComponent.h"
#include "GameStateComponent.h"
#include "Enemy.h"
#include "GameEvent.h"

qbert::EnemySpawnerComponent::EnemySpawnerComponent(dae::GameObject* owner, GameStateComponent* gameState,
	Enemy* coily, Enemy* redBall, Enemy* slick, Enemy* sam, Enemy* ugg, Enemy* wrongway)
	: Component(owner)
	, m_pGameState(gameState)
	, m_pCoily(coily)
	, m_pRedBall(redBall)
	, m_pSlick(slick)
	, m_pSam(sam)
	, m_pUgg(ugg)
	, m_pWrongway(wrongway)
{
}

void qbert::EnemySpawnerComponent::setupRoundEnemies()
{
	// Clear the field, then bring Coily back (always present from round 1).
	m_pRedBall->deactivate();
	m_pSlick->deactivate();
	m_pSam->deactivate();
	m_pUgg->deactivate();
	m_pWrongway->deactivate();
	m_pCurrent = nullptr;

	m_pCoily->activate();

	// Build the rotation pool for this round
	const int round = m_pGameState->getRound();
	m_roundDescenders.clear();
	if (round >= 2)
	{
		m_roundDescenders.push_back(m_pSlick);
		m_roundDescenders.push_back(m_pSam);
	}
	if (round >= 3)
	{
		m_roundDescenders.push_back(m_pRedBall);
	}
	if (round >= 4)
	{
		m_roundDescenders.push_back(m_pUgg);
		m_roundDescenders.push_back(m_pWrongway);
	}

	m_rotationIndex = 0;
	m_descenderCooldownTimer = 0.f;
	m_lifeTimer = 0.f;
}

void qbert::EnemySpawnerComponent::Update(float deltaTime)
{
	if (!m_gated)
	{
		setupRoundEnemies(); // round 1 gate (no roundChanged fires at start)
		m_gated = true;
	}

	if (m_roundDescenders.empty())
		return; // round 1: Coily only, nothing to pace

	// Coily already occupies one of the slots, so only kMaxConcurrent - 1 descenders fit.
	const int activeDescenders = (m_pCurrent != nullptr && m_pCurrent->isActive()) ? 1 : 0;
	if (activeDescenders == 0)
		m_pCurrent = nullptr;

	if (activeDescenders >= maxSimultaneousEnemies - 1)
	{
		// Slot is full: let the descender run for a while, then rotate it out.
		m_lifeTimer += deltaTime;
		if (m_lifeTimer >= descenderLife)
		{
			m_pCurrent->deactivate();
			m_pCurrent = nullptr;
			m_descenderCooldownTimer = 0.f;
		}
		return;
	}

	// Slot is free: bring in the next eligible descender once the delay elapses.
	m_descenderCooldownTimer += deltaTime;
	const float delay = (m_rotationIndex == 0) ? firstDescenderDelay : descenderSpawnInterval;
	if (m_descenderCooldownTimer >= delay)
	{
		Enemy* next = m_roundDescenders[m_rotationIndex % m_roundDescenders.size()];
		++m_rotationIndex;
		next->activate();
		m_pCurrent = next;
		m_lifeTimer = 0.f;
		m_descenderCooldownTimer = 0.f;
	}
}

void qbert::EnemySpawnerComponent::OnNotify(dae::GameEvent event, dae::GameObject*)
{
	if (event == dae::GameEvent::roundChanged || event == dae::GameEvent::levelChanged)
	{
		setupRoundEnemies();
	}
	else if (event == dae::GameEvent::EnemyDied)
	{
		// The active descender was eaten: free its slot so a different type rotates in.
		if (m_pCurrent)
		{
			m_pCurrent->deactivate();
			m_pCurrent = nullptr;
			m_descenderCooldownTimer = 0.f;
		}
	}
}
