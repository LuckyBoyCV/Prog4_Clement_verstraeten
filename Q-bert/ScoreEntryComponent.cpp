#include "scoreEntryComponent.h"
#include "TextComponent.h"
#include "highScores.h"
#include <SDL3/SDL.h>
#include <algorithm>
#include <utility>

qbert::ScoreEntryComponent::ScoreEntryComponent(dae::GameObject* pOwner, std::vector<dae::TextComponent*> slots,
	int score, std::filesystem::path csvPath, std::function<void()> onSubmitted)
	: Component(pOwner)
	, m_slots(std::move(slots))
	, m_csvPath(std::move(csvPath))
	, m_onSubmitted(std::move(onSubmitted))
	, m_score(score)
{
	updateDisplay();
}

void qbert::ScoreEntryComponent::cycleLetter(int dir)
{
	if (m_done)
		return;

	int letter = m_letters[m_index] - 'A';
	letter = (letter + dir + 26) % 26;   // wrap inside A to Z both ways
	m_letters[m_index] = static_cast<char>('A' + letter);
	updateDisplay();
}

void qbert::ScoreEntryComponent::moveSlot(int dir)
{
	if (m_done)
		return;

	m_index = std::clamp(m_index + dir, 0, 2);
	updateDisplay();
}

void qbert::ScoreEntryComponent::submit()
{
	if (m_done)
		return;

	finalize();
}

void qbert::ScoreEntryComponent::finalize()
{
	m_done = true;
	updateDisplay();   

	const std::string name{ m_letters[0], m_letters[1], m_letters[2] };
	saveHighScore(m_csvPath, name, m_score);

	if (m_onSubmitted)
		m_onSubmitted();
}

void qbert::ScoreEntryComponent::updateDisplay()
{
	for (int i = 0; i < static_cast<int>(m_slots.size()); ++i)
	{
		m_slots[i]->SetText(std::string(1, m_letters[i]));
		const bool active = (i == m_index) && !m_done;
		m_slots[i]->SetColor(active ? SDL_Color{ 255, 165, 0, 255 } : SDL_Color{ 180, 180, 180, 255 });
	}
}
