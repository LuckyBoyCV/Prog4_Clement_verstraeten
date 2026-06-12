#pragma once
#include <memory>

namespace qbert
{
	class CoilyComponent;

	class CoilyState
	{
	public:
		virtual ~CoilyState() = default;
		virtual std::unique_ptr<CoilyState> Update(CoilyComponent& coily, float deltaTime) = 0;
		virtual void onEnter(CoilyComponent&) {}
		virtual void onExit(CoilyComponent&) {}
		virtual bool isJumping() const { return false; }
		// Coily kills qbert on a shared tile in every form  only off when respawning off-screen
		virtual bool isDangerous() const { return true; }
	};
}