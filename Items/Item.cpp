#include "Item.h"

#include "../Parameters.h"

namespace nodata{
	Item::Item(std::string &name, int weight) : name_(name) , weight_(weight) {}
	
	void Item::setWeight(int weight) {
		weight_ = weight;
	}
	
	void Item::setDrawPosition(float x, float y) {
		sprite.setPosition(x * CELLSIZEX, y * CELLSIZEY);
	}
	
	void Item::setTexture(sf::Texture &texture) {
		sprite.setTexture(texture);
		sprite.setScale(CELLSIZEX/sprite.getLocalBounds().width, CELLSIZEY/sprite.getLocalBounds().height);
	}
	
	void Item::drawItem(sf::RenderWindow &window) {
		window.draw(sprite);
	}
}
