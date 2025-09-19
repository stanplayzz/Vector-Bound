#pragma once
#include <SFML/Graphics.hpp>


class Game
{
public:
	Game();

private:
	sf::RenderWindow m_window;
	sf::View m_view;

	int run();
};