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
	if (!map.load(sf::Vector2u(tileSize, tileSize), level, gridSize, scale))
	{
		throw std::runtime_error("Failed to load map texture");
	}


	blockManager.addBlock(BlockType::Green, sf::Vector2i(1, 0), sf::Vector2i(1, 2), *this);
	blockManager.addBlock(BlockType::Red, sf::Vector2i(0, -1), sf::Vector2i(1, -2), *this);

}

void Level::update(sf::Time deltaTime)
{
	blockManager.update(deltaTime, *this);
}

void Level::draw(sf::RenderWindow& window)
{
	window.draw(map);
	blockManager.draw(window);
}