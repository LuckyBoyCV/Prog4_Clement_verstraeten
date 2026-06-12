#include "highScores.h"
#include <algorithm>
#include <fstream>

std::vector<qbert::ScoreEntry> qbert::loadHighScores(const std::filesystem::path& path)
{
	std::vector<ScoreEntry> entries;

	std::ifstream file{ path };
	if (!file)
		return entries;

	// Read each saved row and skip anything that looks broken
	std::string line;
	while (std::getline(file, line))
	{
		const auto comma = line.rfind(',');
		if (comma == std::string::npos)
			continue;

		try
		{
			ScoreEntry entry{ line.substr(0, comma), std::stoi(line.substr(comma + 1)) };
			if (!entry.name.empty())
				entries.push_back(std::move(entry));
		}
		catch (...)
		{
			// one malformed row should not take the whole board down with it
		}
	}

	// Show the best scores first
	std::sort(entries.begin(), entries.end(),
		[](const ScoreEntry& a, const ScoreEntry& b) { return a.score > b.score; });
	return entries;
}

void qbert::saveHighScore(const std::filesystem::path& path, const std::string& name, int score)
{
	auto entries = loadHighScores(path);
	entries.push_back({ name, score });

	std::stable_sort(entries.begin(), entries.end(),
		[](const ScoreEntry& a, const ScoreEntry& b) { return a.score > b.score; });

	// Keep only the amount we want to display
	if (entries.size() > static_cast<size_t>(maxHighScores))
		entries.resize(static_cast<size_t>(maxHighScores));

	// Rewrite the file with the updated board
	std::ofstream file{ path, std::ios::trunc };
	for (const auto& entry : entries)
		file << entry.name << ',' << entry.score << '\n';
}
