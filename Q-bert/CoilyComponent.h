#pragma once
#include "Component.h"
#include "RenderComponent.h"
#include <memory>
#include "CoilyState.h"

namespace qbert
{
	class PyramidComponent;
	class QbertComponent;
	class CoilyState;

	class CoilyComponent final : public dae::Component
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
		void SetSprite(int x, int y, int w, int h);

		int getRow() const { return m_row; }
		int getCol() const { return m_col; }

		int getQbertRow() const;
		int getQbertCol() const;

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

		void SetState(CoilyState* newState);
	};





}