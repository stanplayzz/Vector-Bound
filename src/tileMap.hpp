#pragma once

#include <SFML/Graphics.hpp>

#include <string>

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
	bool load(sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height);
	const int getTileAt(int x, int y);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
	const int* m_tiles{};
	unsigned int m_width{};
	unsigned int m_height{};
};