#pragma once
#include "Component.h"
#include "Subject.h"

namespace qbert
{

	class GameStateComponent final : public dae::Component
	{
	public:

		explicit GameStateComponent(dae::GameObject* owner);
		~GameStateComponent() override = default;
		GameStateComponent(const GameStateComponent&) = delete;
		GameStateComponent(GameStateComponent&&) = delete;
		GameStateComponent& operator=(const GameStateComponent&) = delete;
		GameStateComponent& operator=(GameStateComponent&&) = delete;

		void advanceRound();
		void advanceLevel();

		int getRound() const { return m_round; }
		int getLevel() const { return m_level; }


		dae::Subject m_subject;

	private:
		int m_round{ 1 };
		int m_level{ 1 };
	};
}