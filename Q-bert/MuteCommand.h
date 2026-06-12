#pragma once
#include "Command.h"
#include "ServiceLocator.h"

namespace qbert
{
	// F2: toggle mute on the active sound system
	class MuteCommand final : public dae::Command
	{
	public:
		void Execute() override
		{
			auto& sound = dae::ServiceLocator::GetSoundSystem();
			sound.SetMuted(!sound.IsMuted());
		}
	};
}
