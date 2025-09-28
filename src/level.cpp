#include "level.hpp"

#include <string>
#include <print>
#include <exception>

BlockType stringToBlockType(std::string string) {
	if (string == "Red")    return BlockType::Red;
	if (string == "Blue")   return BlockType::Blue;
	if (string == "Green")  return BlockType::Green;
	if (string == "Yellow") return BlockType::Yellow;
	if (string == "Unbreakable") return BlockType::Unbreakable;
	throw std::runtime_error("Unknown block type: " + string);
}

Level::Level(int tileSize, int scale, sf::Vector2i gridSize) : map{ sf::Vector2u(tileSize, tileSize), gridSize, scale } {
	sf::Vector2i size(map.width * tileSize * scale, map.height * tileSize * scale);
	map.setScale(sf::Vector2f(scale, scale));
	map.setPosition(sf::Vector2f(-size.x / 2.f, -size.y / 2.f));

	auto path = std::string(ASSETS_DIR) + "/level/levels.json";
	auto jsonRes = dj::Json::from_file(path);
	if (!jsonRes)
		std::println("Failed to parse JSON: ", path, "\n");
	levelsJson = jsonRes.value();
}

void Level::loadLevel(int index) {
	auto levelsArray = levelsJson["Levels"].as_array();

	if (index < 0 || index >= levelsArray.size())
		throw std::runtime_error("Invalid level index: " + index);

	auto const& levelData = levelsArray[index];

	auto level = std::vector<int>{};
	auto const& in_level = levelData["Level"].as_array();
	level.reserve(in_level.size());
	for (auto const& in_int : in_level)
		level.push_back(in_int.as<int>());
	if (!map.load(level)) {
		throw std::runtime_error("Failed to load level: " + index);
	}

	for (auto const& block : levelData["Blocks"].as_array()) {
		blockManager.addBlock(
			stringToBlockType(block["Type"].as_string()),
			sf::Vector2i(block["Position"][0].as<int>(), block["Position"][1].as<int>()),
			sf::Vector2i(block["Target"][0].as<int>(), block["Target"][1].as<int>()),
			*this);
	}
}

void Level::update(sf::Time deltaTime) {
	blockManager.update(deltaTime, *this);
}

void Level::draw(sf::RenderWindow& window) {
	window.draw(map);
	blockManager.draw(window);
}