#pragma once
#include "level.hpp"

#include <SFML/Graphics.hpp>

enum class Direction
{
	None,
	Up,
	Down,
	Left,
	Right
};

class Player
{
public:
	Player(float scale, int tileSize);
	void onEvent(sf::Event event, Level& level);
	void update(sf::Time deltaTime);
	void draw(sf::RenderWindow& window);

private:
	bool canMove(sf::Vector2i dir, TileMap const& map);
	void changeSprite(int col, int row);
	void animate(sf::Time deltaTime);
	bool push(sf::Vector2i targetPos, Level& level);

	// player
	float m_scale;
	int m_tileSize;
	sf::Texture m_playerTexture;
	sf::Sprite m_player;

	// movement
	float m_movementSpeed = 300.f;
	sf::Vector2f m_position;
	sf::Vector2f m_currentPosition;
	sf::Vector2f m_targetPosition;
	Direction m_currentDirection = Direction::None;
	bool m_moving = false;

	// animation
	int m_stages = 3; // goes back in reverse order
	float m_animationSpeed = 0.75f; // time between frames in seconds
	int m_currentFrame = 0;
	sf::Time m_elapsedTime;
	bool m_animationRight = true;
};