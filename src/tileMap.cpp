#include "tileMap.hpp"

#include <print>

bool TileMap::load(sf::Vector2u tileSize, const int* tiles, unsigned int mapWidth, unsigned int mapHeight)
{
	if (!m_tileset.loadFromFile("assets/textures/tileset.png"))
	{
		return false;
	}

	m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
	m_vertices.resize(mapWidth * mapHeight * 6);

	m_tiles = tiles;
	width = mapWidth;
	height = mapHeight;

	for (unsigned int i = 0; i < mapWidth; i++)
	{
		for (unsigned int j = 0; j < mapHeight; j++)
		{
			int tileNumber = tiles[i + j * mapWidth];

			int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
			int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

			sf::Vertex* triangles = &m_vertices[(i + j * mapWidth) * 6];

			triangles[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
			triangles[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
			triangles[2].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);
			triangles[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);
			triangles[4].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
			triangles[5].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);

			triangles[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
			triangles[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
			triangles[2].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
			triangles[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
			triangles[4].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
			triangles[5].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
		}
	}

	return true;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &m_tileset;
	target.draw(m_vertices, states);
}

int TileMap::getTileAt(sf::Vector2i position) const
{
	std::println("{} {} {} {}", position.x, position.y, width, height);
	if (position.x <= width && position.y <= height)
	{
		return m_tiles[position.x + position.y * width];
	}

	return 0;
}