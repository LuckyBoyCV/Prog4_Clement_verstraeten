#pragma once
#include "Component.h"
#include "Subject.h"
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

		int getRound() const { return m_round; }
		int getLevel() const { return m_level; }

		// Wiring for round progression (set in loadGame)
		void setPyramid(PyramidComponent* pyramid) { m_pPyramid = pyramid; }
		void addQbert(QbertComponent* qbert) { if (qbert) m_qberts.push_back(qbert); }

		dae::Subject m_subject;

	private:
		int m_round{ 1 };
		int m_level{ 1 };

		PyramidComponent* m_pPyramid{ nullptr };
		std::vector<QbertComponent*> m_qberts;
	};
}