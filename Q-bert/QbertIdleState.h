#pragma once
#include "QbertState.h"

namespace qbert
{
	// Q-bert is standing on a tile, waiting for jump input
	class QbertIdleState final : public QbertState
	{
	public:
		std::unique_ptr<QbertState> Update(QbertComponent&, float) override;
	};
}
