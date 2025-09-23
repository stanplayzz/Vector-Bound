#include "player.hpp"

#include <exception>
#include <print>
#include <cmath>

Player::Player(float scale, int tileSize) : m_player(m_playerTexture)
{
	if (!m_playerTexture.loadFromFile("assets/textures/player.png"))
	{
		throw std::runtime_error("Failed to load playerset");
	}

	m_scale = scale;
	m_tileSize = tileSize;

	changeSprite(0, 0);
	m_player.setScale({ scale, scale });
}

void Player::changeSprite(int col, int row)
{
	m_player.setTextureRect(sf::IntRect({ col * m_tileSize, row * m_tileSize }, { m_tileSize, m_tileSize }));
}

bool Player::canMove(sf::Vector2i dir, TileMap const& map)
{
	if (map.getTileAt(sf::Vector2i(dir.x + map.width / 2.f, dir.y + map.height / 2.f)) == 0)
	{
		return true;
	}
	return false;
}

bool Player::push(sf::Vector2i targetPos, Level& level)
{
	sf::Vector2f targetPosition = sf::Vector2f(targetPos) * static_cast<float>(m_tileSize) * m_scale;
	for (auto& block : level.blocks)
	{
		std::println("{} {} {} {}", block.getPosition().x, block.getPosition().y, targetPosition.x, targetPosition.y);
		if (block.getPosition() == targetPosition)
		{
			sf::Vector2f currentPos = block.getPosition();
			sf::Vector2i gridOffset;
			switch (m_currentDirection)
			{
			case Direction::Up:    gridOffset = { 0, -1 }; break;
			case Direction::Down:  gridOffset = { 0,  1 }; break;
			case Direction::Left:  gridOffset = { -1, 0 }; break;
			case Direction::Right: gridOffset = {  1, 0 }; break;
			default: return false;
			}

			sf::Vector2f offset = sf::Vector2f(gridOffset) * static_cast<float>(m_tileSize) * m_scale;
			sf::Vector2i nextGridPos = targetPos + gridOffset;
			if (!canMove(nextGridPos, level.map) || !push(nextGridPos, level))
				return false;

			block.moveTo(block.getPosition() + offset);
		}
	}
	return true;
}

void Player::onEvent(sf::Event event, Level& level)
{
	if (m_moving)
		return;

	if (auto key = event.getIf<sf::Event::KeyPressed>())
	{
		sf::Vector2i gridOffset(0, 0);

		switch (key->scancode)
		{
		case sf::Keyboard::Scancode::W:
			gridOffset = { 0, -1 };
			m_currentDirection = Direction::Up;
			break;
		case sf::Keyboard::Scancode::S:
			gridOffset = { 0, 1 };
			m_currentDirection = Direction::Down;
			break;
		case sf::Keyboard::Scancode::A:
			gridOffset = { -1, 0 };
			changeSprite(m_currentFrame, 1);
			m_currentDirection = Direction::Left;
			break;
		case sf::Keyboard::Scancode::D:
			gridOffset = { 1, 0 };
			changeSprite(m_currentFrame, 0);
			m_currentDirection = Direction::Right;
			break;
		default: break;
		}
		if (gridOffset != sf::Vector2i(0,0))
		{
			sf::Vector2i newGridPos = sf::Vector2i(m_position) + sf::Vector2i(gridOffset);
			if (canMove(newGridPos, level.map) && push(newGridPos, level))
			{
				m_position = sf::Vector2f(newGridPos);
				m_targetPosition = m_position * static_cast<float>(m_tileSize) * m_scale;
				if (m_targetPosition != m_currentPosition)
					m_moving = true;
			}
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