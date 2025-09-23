#pragma once

#include <SFML/Graphics.hpp>

#include <memory>

class Block
{
public:
	Block(std::shared_ptr<sf::Texture> tileset, sf::Vector2f position, int col, int row, float scale, int tileSize);
	sf::Vector2f getPosition() const;
	void moveTo(sf::Vector2f position);
	void update(sf::Time deltaTime);
	void draw(sf::RenderWindow& window);
private:
	float m_pushSpeed = 300.f;
	sf::Vector2f m_targetPos;
	bool m_moving = false;

	std::shared_ptr<sf::Texture> m_tileset;
	sf::Sprite m_block;
};