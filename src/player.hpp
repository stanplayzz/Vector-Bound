#pragma once
#include "tileMap.hpp"

#include <SFML/Graphics.hpp>

class Player
{
public:
	Player(float scale, int tileSize, int col, int row, TileMap map, float mapWidth, float mapHeight);
	void onEvent(std::optional<sf::Event> event);
	void update(sf::Time deltaTime);
	void draw(sf::RenderWindow& window);

private:
	bool canMove(sf::Vector2f dir);

	float m_scale;
	float m_tileSize;
	sf::Texture m_playerTexture;
	sf::Sprite m_player;

	TileMap m_map;
	float m_mapWidth;
	float m_mapHeight;

	sf::Vector2f m_position;
	sf::Vector2f m_currentPosition;
	sf::Vector2f m_targetPosition;
	float m_movementSpeed = 500.f;
	bool m_moving = false;
};