#pragma once
#include "Command.h"
#include "GameStateComponent.h"

namespace qbert
{
	// F1: jump straight to the next level (debug/demo shortcut).
	class SkipLevelCommand final : public dae::Command
	{
	public:
		explicit SkipLevelCommand(GameStateComponent* gameState) : m_pGameState(gameState) {}
		void Execute() override { if (m_pGameState) m_pGameState->skipLevel(); }

	private:
		GameStateComponent* m_pGameState;
	};
}
