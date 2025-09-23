#include "level.hpp"

const int level[] = {
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 0, 0, 0, 0, 0, 0, 5,
	5, 0, 0, 0, 0, 0, 0, 5,
	5, 0, 0, 0, 0, 0, 0, 5,
	5, 0, 0, 0, 0, 0, 0, 5,
	5, 0, 0, 0, 0, 0, 0, 5,
	5, 0, 0, 0, 0, 0, 0, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
};

Level::Level(int tileSize, int scale, sf::Vector2i gridSize)
{
	sf::Vector2i size(gridSize * tileSize * scale);
	map.setScale(sf::Vector2f(scale, scale));
	map.setPosition(sf::Vector2f(-size.x / 2.f, -size.y / 2.f));
	if (!map.load(sf::Vector2u(tileSize, tileSize), level, gridSize))
	{
		throw std::runtime_error("Failed to load map texture");
	}
}

void Level::update(sf::Time deltaTime)
{
	for (auto& block : blocks)
	{
		block.update(deltaTime);
	}
}

void Level::draw(sf::RenderWindow& window)
{
	window.draw(map);

	for (auto& block : blocks)
	{
		block.draw(window);
	}
}