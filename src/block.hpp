#pragma once

#include <SFML/Graphics.hpp>

#include <memory>


enum class BlockType
{
	Red = 1,
	Blue = 2,
	Green = 3,
	Yellow = 4,
	Unbreakable = 5
};

struct Block
{
	BlockType type;
	sf::Sprite sprite;
	sf::Sprite targetSprite;
	sf::Vector2f target;
	bool moving = false;
	sf::Vector2i finalTargetGridPosition{};
};

class Level;

class BlockManager
{
public:
	BlockManager();
	void addBlock(BlockType type, sf::Vector2i gridPosition, sf::Vector2i targetGridPosition, Level& level);
	void moveTo(Block& block, sf::Vector2f position);
	void update(sf::Time deltaTime, Level& level);
	void draw(sf::RenderWindow& window);


	std::vector<Block> blocks;
private:
	float m_pushSpeed = 300.f;

	std::shared_ptr<sf::Texture> m_tileset;
};