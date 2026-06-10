#include "levelConfig.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <string>

namespace
{
	// "once"/"twice"/"toggle" -> the tile flip rule used by PyramidComponent::StepOn.
	// Anything unrecognised falls back to "once" (a single hop flips the tile).
	TileType FlipRuleFromString(const std::string& rule)
	{
		if (rule == "twice")  return TileType::twice;
		if (rule == "toggle") return TileType::reverse;
		return TileType::normal; // "once" and any unknown value
	}

	// Built-in fallback used when the file is missing or invalid. Mirrors the game's
	// original hardcoded behaviour: 3 levels, 4 rounds each, random colours.
	qbert::levelConfig DefaultConfig()
	{
		qbert::levelConfig config;
		config.roundsPerLevel = 4;
		config.levels = {
			{ TileType::normal,  -1 },
			{ TileType::twice,   -1 },
			{ TileType::reverse, -1 },
		};
		return config;
	}
}

qbert::levelConfig qbert::loadLevelConfig(const std::filesystem::path& jsonPath)
{
	std::ifstream file(jsonPath);
	if (!file.is_open())
	{
		std::cerr << "loadLevelConfig: could not open " << jsonPath.string()
			<< ", using defaults\n";
		return DefaultConfig();
	}

	try
	{
		const nlohmann::json root = nlohmann::json::parse(file);

		levelConfig config;
		config.roundsPerLevel = root.value("roundsPerLevel", 4);
		if (config.roundsPerLevel < 1)
			config.roundsPerLevel = 1;

		for (const auto& levelJson : root.at("levels"))
		{
			levelData data;
			data.flipRule = FlipRuleFromString(levelJson.value("flipRule", std::string{ "once" }));

			// "color" is either an integer colour-set index or the string "random" (or absent).
			const auto colorIt = levelJson.find("color");
			if (colorIt != levelJson.end() && colorIt->is_number_integer())
				data.colorSet = colorIt->get<int>();
			else
				data.colorSet = -1;

			config.levels.push_back(data);
		}

		if (config.levels.empty())
		{
			std::cerr << "loadLevelConfig: no levels in " << jsonPath.string()
				<< ", using defaults\n";
			return DefaultConfig();
		}

		return config;
	}
	catch (const std::exception& e)
	{
		std::cerr << "loadLevelConfig: failed to parse " << jsonPath.string()
			<< " (" << e.what() << "), using defaults\n";
		return DefaultConfig();
	}
}
