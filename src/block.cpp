#include "block.hpp"
#include "level.hpp"

#include <exception>
#include <print>

BlockManager::BlockManager()
{
	m_tileset = std::make_shared<sf::Texture>();
	if (!m_tileset->loadFromFile("assets/textures/tileset.png"))
	{
		throw std::runtime_error("Failed to load assets/textures/tileset.png");
	}
}

void BlockManager::addBlock(BlockType type, sf::Vector2i gridPosition, sf::Vector2i targetGridPosition, Level& level)
{
	sf::Sprite sprite(*m_tileset);
	sprite.setTextureRect(sf::IntRect({ static_cast<int>(type) * level.map.tileSize, 0 }, { level.map.tileSize, level.map.tileSize }));
	sprite.setScale(sf::Vector2f(level.map.scale, level.map.scale));
	sprite.setPosition(sf::Vector2f(gridPosition * level.map.tileSize * level.map.scale));

	sf::Sprite targetSprite(*m_tileset);
	targetSprite.setTextureRect(sf::IntRect({ static_cast<int>(type) * level.map.tileSize, 0 }, { level.map.tileSize, level.map.tileSize }));
	targetSprite.setScale(sf::Vector2f(level.map.scale, level.map.scale));
	targetSprite.setPosition(sf::Vector2f(targetGridPosition * level.map.tileSize * level.map.scale));
	targetSprite.setColor(sf::Color(255, 255, 255, 128));

	Block block{
		type,
		std::move(sprite),
		std::move(targetSprite),
		sf::Vector2f(gridPosition * level.map.tileSize * level.map.scale),
		false,
		targetGridPosition
	};

	blocks.push_back(block);
}

void BlockManager::moveTo(Block& block, sf::Vector2f position)
{
	if (block.moving)
		return;
	block.target = position;
	block.moving = true;
}

void BlockManager::update(sf::Time deltaTime, Level& level)
{
	bool blocksAtFinalPosition = true;
	for (auto& block : blocks)
	{
		if (block.moving)
		{
			sf::Vector2f currentPos = block.sprite.getPosition();
			sf::Vector2f dir = block.target - currentPos;
			float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

			if (dist <= m_pushSpeed * deltaTime.asSeconds())
			{
				block.sprite.setPosition(block.target);
				block.moving = false;
			}
			else
			{
				block.sprite.move(dir / dist * m_pushSpeed * deltaTime.asSeconds());
			}
		}
		if (block.sprite.getPosition() != sf::Vector2f(block.finalTargetGridPosition * level.map.scale * level.map.tileSize))
		{
			blocksAtFinalPosition = false;
		}
	}

	if (blocksAtFinalPosition && !level.hasWon)
	{
		level.hasWon = true;
		std::println("You won :p");
	}
	
}


void BlockManager::draw(sf::RenderWindow& window)
{
	for (auto& block : blocks)
	{
		window.draw(block.targetSprite);
		window.draw(block.sprite);
	}
}