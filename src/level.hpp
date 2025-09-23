#pragma once
#include "tileMap.hpp"
#include "block.hpp"


struct Level
{
	TileMap map;
	std::vector<Block> blocks;

	Level(int tileSize, int scale, sf::Vector2i size);
	void update(sf::Time deltaTime);
	void draw(sf::RenderWindow& window);
};