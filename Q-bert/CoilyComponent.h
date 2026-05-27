#pragma once
#include "Component.h"
#include "Observer.h"
#include "RenderComponent.h"
#include <memory>
#include "CoilyState.h"

namespace qbert
{
	class PyramidComponent;
	class QbertComponent;
	class CoilyState;

	class CoilyComponent final : public dae::Component, public dae::Observer
	{
	public:

		explicit CoilyComponent(dae::GameObject* pOwner, PyramidComponent* pyramid, QbertComponent* qbert);
		~CoilyComponent() override = default;
		CoilyComponent(const CoilyComponent&) = delete;
		CoilyComponent(CoilyComponent&&) = delete;
		CoilyComponent& operator=(const CoilyComponent&) = delete;
		CoilyComponent& operator=(CoilyComponent&&) = delete;


		void Update(float deltaTime) override;
		bool Move(int destRow, int destCol);
		bool isJumping() const { return m_isJumping; }
		void setSprite(int x, int y, int w, int h);
		void spriteOffset(float offset) { m_spriteOffsetY = offset; }
		void setIsSnake(bool isSnake) { m_isSnake = isSnake; }

		int getRow() const { return m_row; }
		int getCol() const { return m_col; }

		int getQbertRow() const;
		int getQbertCol() const;

		void setPlayerControlled(bool val) { m_playerControlled = val; }
		bool isPlayerControlled() const { return m_playerControlled; }
		bool getIsSnake() const { return m_isSnake; }

		void OnNotify(dae::GameEvent event, dae::GameObject* pOwner) override;

	private:
		PyramidComponent* m_pPyramid;
		QbertComponent* m_pQbert;
		int   m_row{ 0 };
		int   m_col{ 0 };
		bool  m_isJumping{ false };
		float m_jumpTime{ 0.f };
		float JumpDuration{ 0.2f };
		int   m_futureRow{ 0 };
		int   m_futureCol{ 0 };
		std::unique_ptr<CoilyState> m_pState;
		float m_spriteOffsetY{ -30.f };
		bool  m_isSnake{ false };
		bool  m_playerControlled{ false };
		bool  m_qbertWasJumping{ false };
		bool  m_waitingToRespawn{ false };
		float m_respawnTimer{ 0.f };
		float m_respawnDelay{ 0.f };
		int   m_respawnRow{ 0 };
		int   m_respawnCol{ 0 };

		void SetState(std::unique_ptr<CoilyState> newState);
		void SnapToCurrentTile();
		void onTile();
	};





}