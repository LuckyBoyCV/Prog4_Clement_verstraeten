#pragma once
#include "Component.h"
#include "Subject.h"
#include "levelConfig.h"
#include <vector>

namespace qbert
{
	class PyramidComponent;
	class QbertComponent;

	class GameStateComponent final : public dae::Component
	{
	public:

		explicit GameStateComponent(dae::GameObject* owner);
		~GameStateComponent() override = default;
		GameStateComponent(const GameStateComponent&) = delete;
		GameStateComponent(GameStateComponent&&) = delete;
		GameStateComponent& operator=(const GameStateComponent&) = delete;
		GameStateComponent& operator=(GameStateComponent&&) = delete;

		void Update(float) override;

		void advanceRound();
		void advanceLevel(); // also resets round back to 1
		void skipLevel();    // F1 debug shortcut jump to the next level

		int getRound() const { return m_round; }
		int getLevel() const { return m_level; }

		// Wiring for round progression (set in loadGame)
		void setPyramid(PyramidComponent* pyramid) { m_pPyramid = pyramid; }
		void addQbert(QbertComponent* qbert) { if (qbert) m_qberts.push_back(qbert); }
		void setConfig(LevelConfig config); // installs the level table and applies level 1 (call after setPyramid)

		dae::Subject m_subject;

	private:
		void awardDiskBonus();      // 50 points per unused flying disc when a stage is cleared
		void refreshBoard();        // re-colour, clear the pyramid, send players back to start
		void endGame();             // mark the game over and fire GameOver once
		void applyCurrentLevel();   // push the current level's flip rule (+ fixed colour) to the pyramid
		const levelData& currentLevel() const; // current level's rules, clamped (safe if config unset)

		int m_round{ 1 };
		int m_level{ 1 };
		bool m_gameOver{ false };

		LevelConfig m_config;       // level table from levels.json; level count = m_config.levels.size()

		PyramidComponent* m_pPyramid{ nullptr };
		std::vector<QbertComponent*> m_qberts;
	};
}