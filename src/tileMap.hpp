#pragma once

#include <SFML/Graphics.hpp>

#include <string>

class TileMap : public sf::Drawable, public sf::Transformable {
public:
	TileMap(sf::Vector2u tilesize, sf::Vector2i size, int Scale);
	bool load(std::vector<int> tiles);
	int getTileAt(sf::Vector2i position) const;
	int width{};
	int height{};
	int scale{};
	int tileSize{};

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
	std::vector<int> m_tiles{};
};