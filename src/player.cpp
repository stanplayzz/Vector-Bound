#include "player.hpp"

#include <exception>
#include <print>
#include <cmath>

Player::Player(float scale, int tileSize, int col, int row, TileMap map, float mapWidth, float mapHeight, std::vector<Block>& blocks) : m_player(m_playerTexture), m_blocks(blocks)
{
	if (!m_playerTexture.loadFromFile("assets/textures/player.png"))
	{
		throw std::runtime_error("Failed to load playerset");
	}

	m_map = map;
	m_mapWidth = mapWidth;
	m_mapHeight = mapHeight;
	m_blocks = blocks;

	m_scale = scale;
	m_tileSize = tileSize;

	changeSprite(col, row);
	m_player.setScale({ scale, scale });
}

void Player::changeSprite(int col, int row)
{
	m_player.setTextureRect(sf::IntRect({ col * m_tileSize, row * m_tileSize }, { m_tileSize, m_tileSize }));
}

bool Player::canMove(sf::Vector2f dir)
{
	if (m_map.getTileAt(dir.x + m_mapWidth / 2.f, dir.y + m_mapHeight / 2.f) != 5)
	{
		return true;
	}
	return false;
}

bool Player::push(sf::Vector2f targetPos)
{
	sf::Vector2f targetPosition = targetPos * static_cast<float>(m_tileSize) * m_scale;
	for (auto& block : m_blocks)
	{
		if (block.getPosition() == targetPosition)
		{
			sf::Vector2f currentPos = block.getPosition();
			if (m_currentDirection == Direction::Up)
			{
				sf::Vector2f newPos = currentPos + sf::Vector2f(0, -static_cast<float>(m_tileSize) * m_scale);
				if (!canMove({ targetPos.x, targetPos.y - 1 }))
					return false;
				if (!push({ targetPos.x, targetPos.y - 1 }))
					return false;
				block.moveTo(newPos);
			}
			if (m_currentDirection == Direction::Down)
			{
				sf::Vector2f newPos = currentPos + sf::Vector2f(0, static_cast<float>(m_tileSize) * m_scale);
				if (!canMove({ targetPos.x, targetPos.y + 1 }))
					return false;
				if (!push({ targetPos.x, targetPos.y + 1 }))
					return false;
				block.moveTo(newPos);
			}
			if (m_currentDirection == Direction::Left)
			{
				sf::Vector2f newPos = currentPos + sf::Vector2f(-static_cast<float>(m_tileSize) * m_scale, 0);
				if (!canMove({ targetPos.x - 1, targetPos.y }))
					return false;
				if (!push({ targetPos.x - 1 , targetPos.y}))
					return false;
				block.moveTo(newPos);
			}
			if (m_currentDirection == Direction::Right)
			{
				sf::Vector2f newPos = currentPos + sf::Vector2f(static_cast<float>(m_tileSize) * m_scale, 0);
				if (!canMove({ targetPos.x + 1, targetPos.y }))
					return false;
				if (!push({ targetPos.x + 1, targetPos.y }))
					return false;
				block.moveTo(newPos);
			}
		}
	}
	return true;
}

void Player::onEvent(std::optional<sf::Event> event)
{
	if (m_moving)
		return;

	sf::Vector2f newPos = m_position;

	if (auto key = event->getIf<sf::Event::KeyPressed>())
	{
		if (key->scancode == sf::Keyboard::Scancode::W)
		{
			if (canMove(sf::Vector2f(m_position.x, m_position.y -1)))
			{
				m_currentDirection = Direction::Up;
				newPos.y += -1;
			}
		}
		if (key->scancode == sf::Keyboard::Scancode::S)
		{
			if (canMove(sf::Vector2f(m_position.x, m_position.y + 1)))
			{
				m_currentDirection = Direction::Down;
				newPos.y += 1;
			}
		}
		if (key->scancode == sf::Keyboard::Scancode::A)
		{
			if (canMove(sf::Vector2f(m_position.x - 1, m_position.y)))
			{
				newPos.x += -1;
				if (m_currentDirection == Direction::Right)
					changeSprite(m_currentFrame, 1);
				m_currentDirection = Direction::Left;
			}
		}
		if (key->scancode == sf::Keyboard::Scancode::D)
		{
			if (canMove(sf::Vector2f(m_position.x + 1, m_position.y)))
			{
				newPos.x += 1;
				if (m_currentDirection == Direction::Left)
					changeSprite(m_currentFrame, 0);
				m_currentDirection = Direction::Right;
			}
		}
	}

	if (push(newPos))
	{
		m_position = newPos;
		m_targetPosition = sf::Vector2f(m_position * static_cast<float>(m_tileSize) * m_scale);
		if (m_targetPosition != m_currentPosition)
		{
			m_moving = true;
		}
	}
	
}

void Player::update(sf::Time deltaTime)
{
	// movement
	if (m_moving)
	{
		sf::Vector2f dir = m_targetPosition - m_currentPosition;
		float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

		if (len > 0.1f)
		{
			sf::Vector2f norm = dir / len;
			float moveDistance = m_movementSpeed * deltaTime.asSeconds();
			if (moveDistance > len)
				moveDistance = len;
			m_currentPosition += norm * moveDistance;
		}
		else
		{
			m_currentPosition = m_targetPosition;
			m_moving = false;
		}
		m_player.setPosition(m_currentPosition);
	}

	for (auto& block : m_blocks)
	{
		block.update(deltaTime);
	}

	// animating
	animate(deltaTime);
}

void Player::animate(sf::Time deltaTime)
{
	m_elapsedTime += deltaTime;
	if (m_elapsedTime.asSeconds() >= m_animationSpeed)
	{
		m_elapsedTime -= sf::seconds(m_animationSpeed);

		m_currentFrame += m_animationRight ? 1 : -1;
		if (m_currentFrame >= m_stages)
		{
			m_currentFrame = m_stages - 2;
			m_animationRight = false;
		}
		else if (m_currentFrame < 0)
		{
			m_currentFrame = 1;
			m_animationRight = true;
		}

		int row = 0;
		if (m_currentDirection == Direction::Left)
			row = 1;
		changeSprite(m_currentFrame, row);
		if (m_currentFrame == 0)
			m_elapsedTime -= sf::seconds(m_animationSpeed * 4);
	}
}


void Player::draw(sf::RenderWindow& window)
{
	window.draw(m_player);
}