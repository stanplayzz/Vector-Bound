#include "tileMap.hpp"

#include <print>
#include <exception>
#include <string>

TileMap::TileMap(sf::Vector2u tilesize, sf::Vector2i size, int Scale) {
	auto path = std::string(ASSETS_DIR) + "/textures/tileset.png";
	if (!m_tileset.loadFromFile(path)) {
		throw std::runtime_error("Failed to load tileset at: " + path);
	}
	m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
	m_vertices.resize(size.x * size.y * 6);

	width = size.x;
	height = size.y;
	tileSize = tilesize.x;
	scale = Scale;
}

bool TileMap::load(std::vector<int> tiles) {
	m_tiles = tiles;
	for (unsigned int i = 0; i < width; i++) {
		for (unsigned int j = 0; j < height; j++) {
			int tileNumber = tiles[i + j * width];

			int tu = tileNumber % (m_tileset.getSize().x / tileSize);
			int tv = tileNumber / (m_tileset.getSize().x / tileSize);

			sf::Vertex* triangles = &m_vertices[(i + j * width) * 6];

			triangles[0].position = sf::Vector2f(i * tileSize, j * tileSize);
			triangles[1].position = sf::Vector2f((i + 1) * tileSize, j * tileSize);
			triangles[2].position = sf::Vector2f(i * tileSize, (j + 1) * tileSize);
			triangles[3].position = sf::Vector2f(i * tileSize, (j + 1) * tileSize);
			triangles[4].position = sf::Vector2f((i + 1) * tileSize, j * tileSize);
			triangles[5].position = sf::Vector2f((i + 1) * tileSize, (j + 1) * tileSize);

			triangles[0].texCoords = sf::Vector2f(tu * tileSize, tv * tileSize);
			triangles[1].texCoords = sf::Vector2f((tu + 1) * tileSize, tv * tileSize);
			triangles[2].texCoords = sf::Vector2f(tu * tileSize, (tv + 1) * tileSize);
			triangles[3].texCoords = sf::Vector2f(tu * tileSize, (tv + 1) * tileSize);
			triangles[4].texCoords = sf::Vector2f((tu + 1) * tileSize, tv * tileSize);
			triangles[5].texCoords = sf::Vector2f((tu + 1) * tileSize, (tv + 1) * tileSize);
		}
	}

	return true;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	states.texture = &m_tileset;
	target.draw(m_vertices, states);
}

int TileMap::getTileAt(sf::Vector2i position) const {
	if (position.x <= width && position.y <= height) {
		return m_tiles[position.x + position.y * width];
	}

	return 0;
}