#include "Item.h"

#include "../Parameters.h"

namespace nodata{
	Item::Item(std::string &name, int weight, const Point &point) : name_(name) , weight_(weight) , coord_(point) {}
	
	void Item::setWeight(int weight) {
		weight_ = weight;
	}
	
	void Item::setDrawPosition(float x, float y) {
		sprite.setPosition(x * CELLSIZEX + ((float)CELLSIZEX/2), y * CELLSIZEY + ((float)CELLSIZEY/2));
	}
	
	void Item::setPosition(const Point &point) {
		coord_ = point;
	}
	
	void Item::setTexture(sf::Texture &texture) {
		sprite.setTexture(texture);
		sprite.setScale(CELLSIZEX/sprite.getLocalBounds().width, CELLSIZEY/sprite.getLocalBounds().height);
		
		srand(time(nullptr) * weight_);
		sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);
		sprite.rotate(rand());
	}
	
	void Item::drawItem(sf::RenderWindow &window) {
		window.draw(sprite);
	}
}
