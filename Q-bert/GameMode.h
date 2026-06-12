#pragma once

namespace qbert
{
	// Which game variant the player picked from the main menu
	enum class gameMode
	{
		singlePlayer, // one qbert, normal enemies
		Coop,         // two qberts, normal enemies
		Versus        // one qbert, second player controls Coily
	};
}
