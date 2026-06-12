#pragma once
#include "Component.h"
#include "Observer.h"
#include "Enemy.h"
#include "Subject.h"
#include "SlickSamState.h"
#include <memory>

namespace qbert
{
	class PyramidComponent;
	class QbertComponent;

	class SlickSamComponent final : public dae::Component, public dae::Observer, public Enemy
	{
	public:
		explicit SlickSamComponent(dae::GameObject* pOwner, PyramidComponent* pyramid, QbertComponent* qbert);
		~SlickSamComponent() override = default;
		SlickSamComponent(const SlickSamComponent&) = delete;
		SlickSamComponent(SlickSamComponent&&) = delete;
		SlickSamComponent& operator=(const SlickSamComponent&) = delete;
		SlickSamComponent& operator=(SlickSamComponent&&) = delete;

		void Update(float deltaTime) override;

		// Enemy activation contract
		void activate() override;
		void deactivate() override;
		bool isActive() const override { return m_active; }

		bool isJumping() const;

		// Called by JumpingState after landing; reverses the tile and returns true if qbert was on it
		bool onTile();
		void OnNotify(dae::GameEvent event, dae::GameObject* pOwner) override;

		dae::Subject m_subject;

		// Accessors for state objects
		PyramidComponent* getPyramid()        const { return m_pPyramid; }
		QbertComponent*   getQbert()          const { return m_pQbert; }
		dae::GameObject*  getOwner()          const { return m_Owner; }
		int  getRow()          const { return m_row; }
		int  getCol()          const { return m_col; }
		int  getFutureRow()    const { return m_futureRow; }
		int  getFutureCol()    const { return m_futureCol; }
		void setRow(int r)           { m_row = r; }
		void setCol(int c)           { m_col = c; }
		void setFutureRow(int r)     { m_futureRow = r; }
		void setFutureCol(int c)     { m_futureCol = c; }
		float getSpriteOffsetX()  const { return m_spriteOffsetX; }
		float getSpriteOffsetY()  const { return m_spriteOffsetY; }
		float getJumpDuration()   const { return m_jumpDuration; }
		float getJumpInterval()   const { return m_jumpInterval; }
		float getRespawnDuration() const { return m_respawnDuration; }

	private:
		void SetState(std::unique_ptr<SlickSamState> newState);

		PyramidComponent* m_pPyramid;
		QbertComponent*   m_pQbert;
		int   m_row{ 0 };
		int   m_col{ 0 };
		int   m_futureRow{ 0 };
		int   m_futureCol{ 0 };

		float m_jumpDuration{ 0.2f };
		float m_jumpInterval{ 1.0f };
		float m_respawnDuration{ 3.f };
		float m_spriteOffsetY{ 25.f };
		float m_spriteOffsetX{ 30.f };

		// Pending respawn set by OnNotify(PlayerDied) to avoid transitioning mid-Update
		bool  m_pendingRespawn{ false };
		float m_respawnDelay{ 0.f };
		int   m_respawnRow{ 0 };
		int   m_respawnCol{ 0 };
		bool  m_active{ false };

		std::unique_ptr<SlickSamState> m_pState;
	};
}
