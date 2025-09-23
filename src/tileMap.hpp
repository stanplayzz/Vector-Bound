#pragma once

#include <SFML/Graphics.hpp>

#include <string>

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
	bool load(sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height);
	int getTileAt(sf::Vector2i position) const;
	unsigned int width{};
	unsigned int height{};

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
	const int* m_tiles{};
};