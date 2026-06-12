#pragma once
#include "Component.h"
#include "Observer.h"
#include <functional>
#include <vector>

namespace qbert
{
	class QbertComponent;

	// Watches for the first GameOver event from either player or the game state, reads the
	// top score off the players and hands it to the callback that brings up the entry screen.
	class gameOverComponent final : public dae::Component, public dae::Observer
	{
	public:
		gameOverComponent(dae::GameObject* pOwner, std::vector<QbertComponent*> qberts,
			std::function<void(int)> onGameOver);
		~gameOverComponent() override = default;
		gameOverComponent(const gameOverComponent&) = delete;
		gameOverComponent(gameOverComponent&&) = delete;
		gameOverComponent& operator=(const gameOverComponent&) = delete;
		gameOverComponent& operator=(gameOverComponent&&) = delete;

		void OnNotify(dae::GameEvent event, dae::GameObject* pOwner) override;

	private:
		std::vector<QbertComponent*> m_qberts;
		std::function<void(int)> m_onGameOver;
		bool m_triggered{ false };
	};
}
