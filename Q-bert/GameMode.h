#pragma once

namespace qbert
{
	// Which game variant the player picked from the main menu
	enum class gameMode
	{
		singlePlayer, // one Q*bert, normal enemies
		Coop,         // two Q*berts, normal enemies
		Versus        // one Q*bert, second player controls Coily
	};
}
