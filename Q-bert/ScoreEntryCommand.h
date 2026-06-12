#pragma once
#include "Command.h"
#include "ScoreEntryComponent.h"

namespace qbert
{
	class letterCycleCommand final : public dae::Command
	{
	public:
		letterCycleCommand(ScoreEntryComponent* entry, int dir) : m_entry(entry), m_dir(dir) {}
		void Execute() override { m_entry->cycleLetter(m_dir); }

	private:
		ScoreEntryComponent* m_entry;
		int m_dir;
	};

	class slotMoveCommand final : public dae::Command
	{
	public:
		slotMoveCommand(ScoreEntryComponent* entry, int dir) : m_entry(entry), m_dir(dir) {}
		void Execute() override { m_entry->moveSlot(m_dir); }

	private:
		ScoreEntryComponent* m_entry;
		int m_dir;
	};

	class submitCommand final : public dae::Command
	{
	public:
		explicit submitCommand(ScoreEntryComponent* entry) : m_entry(entry) {}
		void Execute() override { m_entry->submit(); }

	private:
		ScoreEntryComponent* m_entry;
	};
}
