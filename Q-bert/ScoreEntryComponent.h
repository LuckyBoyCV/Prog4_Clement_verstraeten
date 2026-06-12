#pragma once
#include "Component.h"
#include <filesystem>
#include <functional>
#include <string>
#include <vector>

namespace dae { class TextComponent; }

namespace qbert
{
	// Arcade three letter name entry. Up/Down cycle the active letter, Left/Right move between
	// the three slots, submit saves the score and fires onSubmitted.
	class ScoreEntryComponent final : public dae::Component
	{
	public:
		ScoreEntryComponent(dae::GameObject* pOwner, std::vector<dae::TextComponent*> slots,
			int score, std::filesystem::path csvPath, std::function<void()> onSubmitted);
		~ScoreEntryComponent() override = default;
		ScoreEntryComponent(const ScoreEntryComponent&) = delete;
		ScoreEntryComponent(ScoreEntryComponent&&) = delete;
		ScoreEntryComponent& operator=(const ScoreEntryComponent&) = delete;
		ScoreEntryComponent& operator=(ScoreEntryComponent&&) = delete;

		void cycleLetter(int dir);
		void moveSlot(int dir);
		void submit();

	private:
		void updateDisplay();
		void finalize();

		std::vector<dae::TextComponent*> m_slots;
		std::filesystem::path m_csvPath;
		std::function<void()> m_onSubmitted;
		int m_score;
		int m_index{ 0 };
		bool m_done{ false };
		char m_letters[3]{ 'A', 'A', 'A' };
	};
}
