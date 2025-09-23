#include "game.hpp"
#include "tileMap.hpp"
#include "player.hpp"

#include <print>

sf::Vector2u TILE_SIZE(16, 16);
unsigned int WIDTH = 8;
unsigned int HEIGHT = 8;
float SCALE = 4.f;

sf::Vector2f mapSize = sf::Vector2f(TILE_SIZE.x * WIDTH * SCALE,
									TILE_SIZE.y * HEIGHT * SCALE);

const int level[] = {
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 0, 0, 0, 0, 0, 0, 5,
	5, 0, 0, 0, 0, 0, 0, 5,
	5, 0, 0, 0, 0, 0, 0, 5,
	5, 0, 0, 0, 0, 0, 0, 5,
	5, 0, 0, 0, 0, 0, 0, 5,
	5, 0, 0, 0, 0, 0, 0, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
};

Game::Game()
{
	m_window.create(sf::VideoMode({ 512, 512 }), "Vector Bound", sf::Style::Close);
	m_view = sf::View(sf::Vector2f(0.f,0.f), sf::Vector2f(m_window.getSize()));
	m_window.setView(m_view);


	run();
}

int Game::run()
{
	TileMap map;
	map.setScale(sf::Vector2f(SCALE, SCALE));
	map.setPosition(sf::Vector2f(-mapSize.x / 2.f, -mapSize.y / 2.f));
	if (!map.load(TILE_SIZE, level, WIDTH, HEIGHT))
	{
		return -1;
	}



	m_blockTileset = std::make_shared<sf::Texture>();
	if (!m_blockTileset->loadFromFile("assets/textures/tileset.png"))
	{
		throw std::runtime_error("Failed to load tileset");
	}
	m_blocks.push_back(Block(m_blockTileset, sf::Vector2f(1.f, 0.f), 3, 0, SCALE, TILE_SIZE.x));
	m_blocks.push_back(Block(m_blockTileset, sf::Vector2f(-1.f, 0.f), 1, 0, SCALE, TILE_SIZE.x));

	Player player(SCALE, TILE_SIZE.x, 0, 0, map, WIDTH, HEIGHT, m_blocks);

	sf::Clock clock;
	sf::Time deltaTime;

	while (m_window.isOpen())
	{
		while (auto event = m_window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				m_window.close();
			player.onEvent(event);
		}

		deltaTime = clock.restart();

		player.update(deltaTime);

		m_window.clear(sf::Color(200, 150, 200));

		m_window.draw(map);
		player.draw(m_window);
		for (auto& block : m_blocks)
			block.draw(m_window);

		m_window.display();
	}
	return 0;
}