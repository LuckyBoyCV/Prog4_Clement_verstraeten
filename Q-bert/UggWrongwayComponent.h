#pragma once
#include "Component.h"
#include "Observer.h"
#include "Enemy.h"
#include "UggWrongwayState.h"
#include <memory>

namespace qbert
{
	class PyramidComponent;
	class QbertComponent;

	// Which edge of the pyramid this enemy climbs (one shared class, mirrored side).
	enum class UggWrongwaySide { left, right };


	// ugg climbs the right edge, Wrongway the left.contact kills Q*bert

	class UggWrongwayComponent final : public dae::Component, public dae::Observer, public Enemy
	{
	public:
		explicit UggWrongwayComponent(dae::GameObject* pOwner, PyramidComponent* pyramid, QbertComponent* qbert, UggWrongwaySide side);
		~UggWrongwayComponent() override = default;
		UggWrongwayComponent(const UggWrongwayComponent&) = delete;
		UggWrongwayComponent(UggWrongwayComponent&&) = delete;
		UggWrongwayComponent& operator=(const UggWrongwayComponent&) = delete;
		UggWrongwayComponent& operator=(UggWrongwayComponent&&) = delete;

		void Update(float deltaTime) override;

		// Enemy activation 
		void activate() override;
		void deactivate() override;
		bool isActive() const override { return m_active; }

		bool isJumping() const;

		// Called by JumpingState after landing; kills Q*bert if he shares this tile (no tile flipping)
		void onTile();
		void OnNotify(dae::GameEvent event, dae::GameObject* pOwner) override;

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

		// Column delta applied each upward hop: 1 for the right edge (Ugg keeps col == row),
		// 0 for the left edge (Wrongway keeps col == 0). Keeps the climber on its edge.
		int  getColStep()      const { return m_side == UggWrongwaySide::right ? 1 : 0; }
		int  getSpawnRow()     const;
		int  getSpawnCol()     const;

	private:
		void SetState(std::unique_ptr<UggWrongwayState> newState);

		PyramidComponent* m_pPyramid;
		QbertComponent*   m_pQbert;
		UggWrongwaySide   m_side;
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

		std::unique_ptr<UggWrongwayState> m_pState;
	};
}
