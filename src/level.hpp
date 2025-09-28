#pragma once
#include "tileMap.hpp"
#include "block.hpp"

#include <djson/json.hpp>

struct Level {
	TileMap map;
	BlockManager blockManager;

	Level(int tileSize, int scale, sf::Vector2i size);
	void loadLevel(int index);
	void update(sf::Time deltaTime);
	void draw(sf::RenderWindow& window);

	dj::Json levelsJson;

	bool hasWon = false;
};