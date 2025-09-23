#pragma once
#include "tileMap.hpp"
#include "block.hpp"

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
	Player(float scale, int tileSize, int col, int row, TileMap map, float mapWidth, float mapHeight, std::vector<Block>& blocks);
	void onEvent(std::optional<sf::Event> event);
	void update(sf::Time deltaTime);
	void draw(sf::RenderWindow& window);

private:
	bool canMove(sf::Vector2f dir);
	void changeSprite(int col, int row);
	void animate(sf::Time deltaTime);
	bool push(sf::Vector2f targetPos);

	// player
	float m_scale;
	int m_tileSize;
	sf::Texture m_playerTexture;
	sf::Sprite m_player;

	// map initializations
	TileMap m_map;
	float m_mapWidth;
	float m_mapHeight;
	std::vector<Block>& m_blocks;

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