#pragma once
#include "Component.h"
#include "Subject.h"
#include "QbertState.h"
#include <glm/vec2.hpp>
#include <memory>

namespace qbert
{
	class PyramidComponent;

	class QbertComponent final : public dae::Component
	{
	public:
		explicit QbertComponent(dae::GameObject* pOwner, PyramidComponent* pyramid, int startRow, int startCol);
		~QbertComponent() override = default;
		QbertComponent(const QbertComponent&) = delete;
		QbertComponent(QbertComponent&&) = delete;
		QbertComponent& operator=(const QbertComponent&) = delete;
		QbertComponent& operator=(QbertComponent&&) = delete;

		void Update(float deltaTime) override;
		// Initiates a jump by delta (called by JumpCommand)
		bool Move(int deltaRow, int deltaCol);

		void loseLife();
		void kill();
		void Respawn();
		void addScore(int points);
		void CancelRide();   // force back to idle (used when the board refreshes mid-ride)

		bool isJumping() const;
		bool isFalling() const;
		bool isRidingDisk() const;

		int getRow()       const { return m_row; }
		int getCol()       const { return m_col; }
		int getFutureRow() const { return m_futureRow; }
		int getFutureCol() const { return m_futureCol; }
		int getLives()     const { return m_Lives; }
		int getScore()     const { return m_Score; }

		void setRow(int r) { m_row = r; }
		void setCol(int c) { m_col = c; }

		PyramidComponent*  getPyramid()      const { return m_pPyramid; }
		dae::GameObject*   getOwner()        const { return m_Owner; }
		float              getJumpDuration() const { return m_jumpDuration; }
		float              getRespawnDuration() const { return m_respawnDuration; }

		dae::Subject m_subject;

	private:
		void SetState(std::unique_ptr<QbertState> newState);

		PyramidComponent* m_pPyramid;
		int m_Lives;
		int m_Score{ 0 };
		int m_row;
		int m_col;
		int m_startRow{ 0 };
		int m_startCol{ 0 };
		int m_futureRow{ 0 };
		int m_futureCol{ 0 };

		static constexpr float m_jumpDuration{ 0.2f };
		static constexpr float m_respawnDuration{ 2.f };

		std::unique_ptr<QbertState> m_pState;
	};
}
