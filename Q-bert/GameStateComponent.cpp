#include "GameStateComponent.h"
#include "GameObject.h"
#include "GameEvent.h"
#include "PyramidComponent.h"
#include "QbertComponent.h"
#include <algorithm>
#include <cstdlib>
#include <utility>

qbert::GameStateComponent::GameStateComponent(dae::GameObject* pOwner)
	: Component(pOwner)
{
}

void qbert::GameStateComponent::Update(float)
{
	if (m_gameOver)
		return;

	// Round is complete once every tile is flipped. Finishing RoundsPerLevel rounds
	// advances the level (resetting to round 1); clearing the final round of the last
	// level ends the game. Otherwise we just bump the round. refreshBoard() then clears
	// the pyramid so IsStepped() is false again next frame (no double-trigger).
	if (m_pPyramid && m_pPyramid->isStepped())
	{
		if (m_round >= m_config.roundsPerLevel)
		{
			if (m_level >= static_cast<int>(m_config.levels.size()))
			{
				endGame();
				return;
			}
			advanceLevel();
		}
		else
		{
			advanceRound();
		}

		refreshBoard();
	}
}

void qbert::GameStateComponent::advanceRound()
{
	++m_round;
	m_subject.Notify(dae::GameEvent::roundChanged, m_Owner);
}

void qbert::GameStateComponent::advanceLevel()
{
	++m_level;
	m_round = 1;
	applyCurrentLevel(); // switch tiles to the new level's flip rule (+ fixed colour)
	m_subject.Notify(dae::GameEvent::levelChanged, m_Owner);
}

void qbert::GameStateComponent::skipLevel()
{
	// F1 debug shortcut: jump straight to the next level's fresh board. Past the last
	// level there's nowhere to go, so it ends the game just like clearing it would.
	if (m_gameOver)
		return;

	if (m_level >= static_cast<int>(m_config.levels.size()))
	{
		endGame();
		return;
	}

	advanceLevel();
	refreshBoard();
}

void qbert::GameStateComponent::refreshBoard()
{
	if (!m_pPyramid)
		return;

	// Colour for the new round: a level may pin a fixed colour set, otherwise pick a fresh
	// random one (always different from the current). Tiles and the CHANGE TO swatch follow it.
	const levelData& level = currentLevel();
	if (level.colorSet >= 0)
	{
		m_pPyramid->setColorSet(level.colorSet);
	}
	else
	{
		const int count   = PyramidComponent::colorSetCount;
		const int current = m_pPyramid->getColorSet();
		m_pPyramid->setColorSet((current + 1 + std::rand() % (count - 1)) % count);
	}

	m_pPyramid->reset();
	m_pPyramid->placeDisks();   // fresh discs for the new round
	for (auto* qbert : m_qberts)
	{
		qbert->CancelRide();    // nobody stays stuck mid-ride across a refresh
		qbert->Respawn();
	}
}

void qbert::GameStateComponent::endGame()
{
	m_gameOver = true;
	m_subject.Notify(dae::GameEvent::GameOver, m_Owner);
}

void qbert::GameStateComponent::setConfig(LevelConfig config)
{
	// Installed once during loadGame (after the pyramid is wired). Applying it here makes the
	// first level's flip rule/colour take effect immediately, before round 1 starts.
	m_config = std::move(config);
	applyCurrentLevel();
}

void qbert::GameStateComponent::applyCurrentLevel()
{
	if (!m_pPyramid)
		return;

	const levelData& level = currentLevel();
	m_pPyramid->setFlipRule(level.flipRule);
	if (level.colorSet >= 0)
		m_pPyramid->setColorSet(level.colorSet);
}

const qbert::levelData& qbert::GameStateComponent::currentLevel() const
{
	// m_level is 1-based; clamp so a short or malformed level table can never index out of bounds.
	static const levelData fallback{};
	if (m_config.levels.empty())
		return fallback;

	const int last = static_cast<int>(m_config.levels.size()) - 1;
	const int idx  = std::clamp(m_level - 1, 0, last);
	return m_config.levels[static_cast<size_t>(idx)];
}