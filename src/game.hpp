#pragma once
#include "block.hpp"

#include <SFML/Graphics.hpp>

#include <list>

class Game
{
public:
	Game();

private:
	sf::RenderWindow m_window;
	sf::View m_view;

	std::shared_ptr<sf::Texture> m_blockTileset;
	std::vector<Block> m_blocks;

	int run();
};