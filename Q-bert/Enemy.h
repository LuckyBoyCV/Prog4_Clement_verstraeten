#pragma once

namespace qbert
{
	// Activation contract used by enemySpawnerComponent to turn enemies on/off
	// without creating/destroying GameObjects. Inactive enemies park off-screen
	// and skip their state machine.
	class Enemy
	{
	public:
		virtual ~Enemy() = default;

		virtual void activate()   = 0; // place at spawn tile, start state machine
		virtual void deactivate() = 0; // park off-screen, stop updating
		virtual bool isActive() const = 0;
	};
}
