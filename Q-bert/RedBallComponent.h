#pragma once
#include "Component.h"
#include "Observer.h"
#include "RenderComponent.h"
#include "RedBallState.h"
#include <memory>

namespace qbert
{
	class PyramidComponent;
	class QbertComponent;

	class redBallComponent final : public dae::Component, public dae::Observer
	{
	public:
		explicit redBallComponent(dae::GameObject* pOwner, PyramidComponent* pyramid, QbertComponent* qbert);
		~redBallComponent() override = default;
		redBallComponent(const redBallComponent&) = delete;
		redBallComponent(redBallComponent&&) = delete;
		redBallComponent& operator=(const redBallComponent&) = delete;
		redBallComponent& operator=(redBallComponent&&) = delete;

		void Update(float deltaTime) override;
		bool isJumping() const;
		void setSprite(int x, int y, int w, int h);

		void OnNotify(dae::GameEvent event, dae::GameObject* pOwner) override;
		// Called by JumpingState after landing; kills Q-bert if on the same tile
		void onTile();

		// Accessors for state objects
		PyramidComponent* getPyramid()       const { return m_pPyramid; }
		QbertComponent*   getQbert()         const { return m_pQbert; }
		dae::GameObject*  getOwner()         const { return m_Owner; }
		int  getRow()         const { return m_row; }
		int  getCol()         const { return m_col; }
		int  getFutureRow()   const { return m_futureRow; }
		int  getFutureCol()   const { return m_futureCol; }
		void setRow(int r)          { m_row = r; }
		void setCol(int c)          { m_col = c; }
		void setFutureRow(int r)    { m_futureRow = r; }
		void setFutureCol(int c)    { m_futureCol = c; }
		float getSpriteOffsetX() const { return m_spriteOffsetX; }
		float getSpriteOffsetY() const { return m_spriteOffsetY; }
		float getJumpDuration()  const { return m_jumpDuration; }
		float getJumpInterval()  const { return m_jumpInterval; }

	private:
		void SetState(std::unique_ptr<RedBallState> newState);

		PyramidComponent* m_pPyramid;
		QbertComponent*   m_pQbert;
		int   m_row{ 0 };
		int   m_col{ 0 };
		int   m_futureRow{ 0 };
		int   m_futureCol{ 0 };

		float m_jumpDuration{ 0.2f };
		float m_jumpInterval{ 0.8f };
		float m_spriteOffsetY{ 15.f };
		float m_spriteOffsetX{ 30.f };

		// Pending respawn set by OnNotify(PlayerDied) to avoid transitioning mid-Update
		bool  m_pendingRespawn{ false };
		float m_respawnDelay{ 0.f };
		int   m_respawnRow{ 0 };
		int   m_respawnCol{ 0 };

		std::unique_ptr<RedBallState> m_pState;
	};
}
