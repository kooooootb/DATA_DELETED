#include <SFML/Graphics.hpp>
#include "Table.h"
#include "Creature.h"
#include "Item.h"
#include "Level.h"


Forager::Forager(std::string &name, Point &coord, int force) : Creature(name, coord) , force_(force) {

}

ErrorCodes Forager::receiveItem(Item *item) {
	if(item->getWeight() + itemTable_.getWeight() > force_) return ERROR;//too heavy
	
	itemTable_.addItem(item);
	
	return OK;
}

ErrorCodes Forager::throwItem(Level *level, int index) {
	Item *item = itemTable_.getItem(index);
	if(item == nullptr) return ERROR;
	
	level->dropItem(coord_, item);
	
	return OK;
}

void Forager::kill(Level *level) {
	dropAllItems(level);
	level->killForager(this);
}

void Forager::dropAllItems(Level *level) {
	itemTable_.dropAll(level, coord_);
}

void Forager::drawCell(sf::RectangleShape &shape) {

}
