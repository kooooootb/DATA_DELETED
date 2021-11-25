#include "Cell.h"

#include "../Parameters.h"

namespace nodata{
	Cell::Cell() : type_(FLOOR) {
	
	}
	
	void Cell::setType(CellType type, sf::Texture *textures) {
		type_ = type;
		sprite.setTexture(textures[type]);
		sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
		sprite.setScale(CELLSIZEX/sprite.getLocalBounds().width, CELLSIZEY/sprite.getLocalBounds().height);
	}
	
	void Cell::setType(CellType type) {
		type_ = type;
	}
	
	void Cell::setDrawPosition(float x, float y) {
		sprite.setPosition(x * CELLSIZEX, y * CELLSIZEY);
	}
	
	void Cell::drawCell(sf::RenderWindow &window) {
		window.draw(sprite);
	}
	
	void Cell::setTexture(sf::Texture &texture) {
		sprite.setTexture(texture);
		sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
		sprite.setScale(CELLSIZEX/sprite.getLocalBounds().width, CELLSIZEY/sprite.getLocalBounds().height);
	}
	
	bool Cell::walkAble() const {
		return (type_ == FLOOR || type_ == STORAGE);
	}
}
