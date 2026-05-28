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
		// Called by CoilyJumpCommand in versus mode
		bool Move(int destRow, int destCol);
		bool isJumping() const;

		void setSprite(int x, int y, int w, int h);
		void spriteOffset(float offset) { m_spriteOffsetY = offset; }
		void setIsSnake(bool isSnake) { m_isSnake = isSnake; }

		int getRow() const { return m_row; }
		int getCol() const { return m_col; }
		void setRow(int r) { m_row = r; }
		void setCol(int c) { m_col = c; }

		int getQbertRow() const;
		int getQbertCol() const;

		void setPlayerControlled(bool val) { m_playerControlled = val; }
		bool isPlayerControlled() const { return m_playerControlled; }
		bool getIsSnake() const { return m_isSnake; }

		PyramidComponent* getPyramid()      const { return m_pPyramid; }
		dae::GameObject*  getOwner()        const { return m_Owner; }
		float             getSpriteOffsetY() const { return m_spriteOffsetY; }

		void onTile();
		void OnNotify(dae::GameEvent event, dae::GameObject* pOwner) override;

	private:
		void SetState(std::unique_ptr<CoilyState> newState);
		void SnapToCurrentTile();

		PyramidComponent* m_pPyramid;
		QbertComponent*   m_pQbert;
		int   m_row{ 0 };
		int   m_col{ 0 };
		std::unique_ptr<CoilyState> m_pState;
		float m_spriteOffsetY{ -30.f };
		bool  m_isSnake{ false };
		bool  m_playerControlled{ false };
		bool  m_qbertWasJumping{ false };

		// Set by OnNotify; flushed at the start of the next Update to avoid mid-frame state changes
		bool  m_pendingRespawn{ false };
		float m_respawnDelay{ 0.f };
		int   m_respawnRow{ 0 };
		int   m_respawnCol{ 0 };

		// Set by Move() from CoilyJumpCommand; flushed the same way
		bool m_pendingJump{ false };
		int  m_pendingJumpRow{ 0 };
		int  m_pendingJumpCol{ 0 };
	};
}
