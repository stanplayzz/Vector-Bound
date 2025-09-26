#include "player.hpp"

#include <exception>
#include <print>
#include <cmath>

Player::Player(Level& level) : m_player(m_playerTexture)
{
	if (!m_playerTexture.loadFromFile("assets/textures/player.png"))
	{
		throw std::runtime_error("Failed to load playerset");
	}

	changeSprite(0, 0, level);
	m_player.setScale(sf::Vector2f(level.map.scale, level.map.scale));
}

void Player::changeSprite(int col, int row, Level& level)
{
	m_player.setTextureRect(sf::IntRect({ col * level.map.tileSize, row * level.map.tileSize }, { level.map.tileSize, level.map.tileSize }));
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
	sf::Vector2f targetPosition = sf::Vector2f(targetPos) * static_cast<float>(level.map.tileSize) * static_cast<float>(level.map.scale);
	for (auto& block : level.blockManager.blocks)
	{
		if (block.sprite.getPosition() == targetPosition)
		{
			sf::Vector2f currentPos = block.sprite.getPosition();
			sf::Vector2i gridOffset;
			switch (m_currentDirection)
			{
			case Direction::Up:    gridOffset = { 0, -1 }; break;
			case Direction::Down:  gridOffset = { 0,  1 }; break;
			case Direction::Left:  gridOffset = { -1, 0 }; break;
			case Direction::Right: gridOffset = {  1, 0 }; break;
			default: return false;
			}

			sf::Vector2f offset = sf::Vector2f(gridOffset) * static_cast<float>(level.map.tileSize) * static_cast<float>(level.map.scale);
			sf::Vector2i nextGridPos = targetPos + gridOffset;
			if (!canMove(nextGridPos, level.map) || !push(nextGridPos, level))
				return false;

			level.blockManager.moveTo(block, block.sprite.getPosition() + offset);
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
			changeSprite(m_currentFrame, 1, level);
			m_currentDirection = Direction::Left;
			break;
		case sf::Keyboard::Scancode::D:
			gridOffset = { 1, 0 };
			changeSprite(m_currentFrame, 0, level);
			m_currentDirection = Direction::Right;
			break;
		case sf::Keyboard::Scancode::Z:
			undo(level);
			break;
		default: break;
		}
		if (gridOffset != sf::Vector2i(0,0))
		{
			sf::Vector2i newGridPos = sf::Vector2i(m_position.x, m_position.y) + gridOffset;
			Move move;
			move.oldPlayerPos = m_currentPosition;
			for (auto& block : level.blockManager.blocks)
				move.oldBlockPositions.push_back(block.sprite.getPosition());

			if (canMove(newGridPos, level.map) && push(newGridPos, level))
			{
				m_position = sf::Vector2f(newGridPos);
				m_targetPosition = m_position * static_cast<float>(level.map.tileSize) * static_cast<float>(level.map.scale);

				move.newPlayerPos = m_targetPosition;
				for (auto& block : level.blockManager.blocks)
					move.newBlockPositions.push_back(block.target);

				m_moveHistory.push_back(move);

				if (m_targetPosition != m_currentPosition)
					m_moving = true;
			}
		}
	}
}

void Player::update(sf::Time deltaTime, Level& level)
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
	animate(deltaTime, level);
}

void Player::animate(sf::Time deltaTime, Level& level)
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
		changeSprite(m_currentFrame, row, level);
		if (m_currentFrame == 0)
			m_elapsedTime -= sf::seconds(m_animationSpeed * 4);
	}
}

void Player::undo(Level& level)
{
	if (m_moveHistory.empty())
		return;

	Move last = m_moveHistory.back();
	m_moveHistory.pop_back();

	m_currentPosition = last.oldPlayerPos;
	m_targetPosition = last.oldPlayerPos;
	m_player.setPosition(m_currentPosition);
	m_moving = false;

	m_position = sf::Vector2f(
		std::round(last.oldPlayerPos.x / (level.map.tileSize * level.map.scale)),
		std::round(last.oldPlayerPos.y / (level.map.tileSize * level.map.scale))
	);

	for (size_t i = 0; i < level.blockManager.blocks.size(); i++)
	{
		level.blockManager.blocks[i].sprite.setPosition(last.oldBlockPositions[i]);
		level.blockManager.blocks[i].target = last.oldBlockPositions[i];
		level.blockManager.blocks[i].moving = false;
	}
}


void Player::draw(sf::RenderWindow& window)
{
	window.draw(m_player);
}