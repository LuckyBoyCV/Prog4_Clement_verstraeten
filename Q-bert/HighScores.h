#pragma once
#include <filesystem>
#include <string>
#include <vector>

namespace qbert
{
	struct ScoreEntry
	{
		std::string name;
		int score{ 0 };
	};

	inline constexpr int maxHighScores{ 10 };

	// sorted high to low, empty when the file does not exist yet
	std::vector<ScoreEntry> loadHighScores(const std::filesystem::path& path);

	// inserts the entry, keeps the top maxHighScores and rewrites the file
	void saveHighScore(const std::filesystem::path& path, const std::string& name, int score);
}
