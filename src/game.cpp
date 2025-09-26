#include "game.hpp"
#include "player.hpp"
#include "level.hpp"

#include <print>
#include <exception>

int TILE_SIZE = 16;
unsigned int WIDTH = 8;
unsigned int HEIGHT = 8;
float SCALE = 4.f;

sf::Vector2f mapSize = sf::Vector2f(TILE_SIZE * WIDTH * SCALE,
									TILE_SIZE * HEIGHT * SCALE);

Game::Game()
{
	m_window.create(sf::VideoMode({ 512, 512 }), "Vector Bound", sf::Style::Close);
	m_view = sf::View(sf::Vector2f(0.f,0.f), sf::Vector2f(m_window.getSize()));
	m_window.setView(m_view);

	run();
}

int Game::run()
{
	m_blockTileset = std::make_shared<sf::Texture>();
	if (!m_blockTileset->loadFromFile("assets/textures/tileset.png"))
	{
		throw std::runtime_error("Failed to load tileset");
	}

	Level level(TILE_SIZE, SCALE, sf::Vector2i(WIDTH, HEIGHT));
	
	Player player(level);

	sf::Clock clock;
	sf::Time deltaTime;

	while (m_window.isOpen())
	{
		while (auto event = m_window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				m_window.close();
			player.onEvent(*event, level);
		}

		deltaTime = clock.restart();

		player.update(deltaTime, level);
		level.update(deltaTime);

		m_window.clear(sf::Color(70, 70, 70));

		level.draw(m_window);
		player.draw(m_window);

		m_window.display();
	}
	return 0;
}