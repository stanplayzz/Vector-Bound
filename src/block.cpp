#include "block.hpp"


Block::Block(std::shared_ptr<sf::Texture> tileset, sf::Vector2f position, int col, int row, float scale, int tileSize) : m_tileset(tileset), m_block(*m_tileset)
{

	m_block.setTextureRect(sf::IntRect({ col * tileSize, row * tileSize }, { tileSize, tileSize }));
	m_block.setScale({ scale, scale });
	m_block.setPosition(position * static_cast<float>(tileSize) * scale);
}

sf::Vector2f Block::getPosition() const
{
	return m_block.getGlobalBounds().position;
}

void Block::moveTo(sf::Vector2f position)
{
	if (m_moving)
		return;
	m_targetPos = position;
	m_moving = true;
}

void Block::update(sf::Time deltaTime)
{
	if (m_moving)
	{
		sf::Vector2f currentPos = m_block.getPosition();
		sf::Vector2f dir = m_targetPos - currentPos;
		float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

		if (len > 0.1f)
		{
			sf::Vector2f moveDistance = (dir / len) * m_pushSpeed * deltaTime.asSeconds();
			if (std::sqrt(moveDistance.x * moveDistance.x + moveDistance.y * moveDistance.y) > len)
				moveDistance = dir;

			m_block.move(moveDistance);
		}
		else
		{
			m_block.setPosition(m_targetPos);
			m_moving = false;
			return;
		}
	}
}

void Block::draw(sf::RenderWindow& window)
{
	window.draw(m_block);
}