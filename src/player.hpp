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

struct Move
{
	sf::Vector2f oldPlayerPos;
	sf::Vector2f newPlayerPos;
	std::vector<sf::Vector2f> oldBlockPositions;
	std::vector<sf::Vector2f> newBlockPositions;
};

class Player
{
public:
	Player(Level& level);
	void onEvent(sf::Event event, Level& level);
	void update(sf::Time deltaTime, Level& level);
	void draw(sf::RenderWindow& window);

private:
	bool canMove(sf::Vector2i dir, TileMap const& map);
	void changeSprite(int col, int row, Level& level);
	void animate(sf::Time deltaTime, Level& level);
	bool push(sf::Vector2i targetPos, Level& level);
	void undo(Level& level);

	// player
	sf::Texture m_playerTexture;
	sf::Sprite m_player;

	// movement
	float m_movementSpeed = 300.f;
	sf::Vector2f m_position{0.f,0.f};
	sf::Vector2f m_currentPosition{0.f, 0.f};
	sf::Vector2f m_targetPosition{ 0.f, 0.f };
	Direction m_currentDirection = Direction::None;
	bool m_moving = false;
	std::vector<Move> m_moveHistory;

	// animation
	int m_stages = 3; // goes back in reverse order
	float m_animationSpeed = 0.75f; // time between frames in seconds
	int m_currentFrame = 0;
	sf::Time m_elapsedTime;
	bool m_animationRight = true;
};