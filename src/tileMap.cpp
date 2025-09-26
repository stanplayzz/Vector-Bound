#include "tileMap.hpp"

#include <print>

bool TileMap::load(sf::Vector2u tilesize, const int* tiles, sf::Vector2i size, int Scale)
{
	if (!m_tileset.loadFromFile("assets/textures/tileset.png"))
	{
		return false;
	}
	m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
	m_vertices.resize(size.x * size.y * 6);

	m_tiles = tiles;
	width = size.x;
	height = size.y;
	tileSize = tilesize.x;
	scale = Scale;

	for (unsigned int i = 0; i < size.x; i++)
	{
		for (unsigned int j = 0; j < size.y; j++)
		{
			int tileNumber = tiles[i + j * size.x];

			int tu = tileNumber % (m_tileset.getSize().x / tilesize.x);
			int tv = tileNumber / (m_tileset.getSize().x / tilesize.x);

			sf::Vertex* triangles = &m_vertices[(i + j * size.x) * 6];

			triangles[0].position = sf::Vector2f(i * tilesize.x, j * tilesize.y);
			triangles[1].position = sf::Vector2f((i + 1) * tilesize.x, j * tilesize.y);
			triangles[2].position = sf::Vector2f(i * tilesize.x, (j + 1) * tilesize.y);
			triangles[3].position = sf::Vector2f(i * tilesize.x, (j + 1) * tilesize.y);
			triangles[4].position = sf::Vector2f((i + 1) * tilesize.x, j * tilesize.y);
			triangles[5].position = sf::Vector2f((i + 1) * tilesize.x, (j + 1) * tilesize.y);

			triangles[0].texCoords = sf::Vector2f(tu * tilesize.x, tv * tilesize.y);
			triangles[1].texCoords = sf::Vector2f((tu + 1) * tilesize.x, tv * tilesize.y);
			triangles[2].texCoords = sf::Vector2f(tu * tilesize.x, (tv + 1) * tilesize.y);
			triangles[3].texCoords = sf::Vector2f(tu * tilesize.x, (tv + 1) * tilesize.y);
			triangles[4].texCoords = sf::Vector2f((tu + 1) * tilesize.x, tv * tilesize.y);
			triangles[5].texCoords = sf::Vector2f((tu + 1) * tilesize.x, (tv + 1) * tilesize.y);
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
	if (position.x <= width && position.y <= height)
	{
		return m_tiles[position.x + position.y * width];
	}

	return 0;
}