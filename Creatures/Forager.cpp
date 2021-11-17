#include <SFML/Graphics.hpp>
#include "Table.h"
#include "Creature.h"
#include "Item.h"
#include "Level.h"


Forager::Forager(Level *level, std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, int force) : Creature(level, name, coord, healthMax, timeMax, walkTime, viewRadius) , force_(force) {

}

ErrorCodes Forager::receiveItem(Item *item) {
	if(item->getWeight() + itemTable_.getWeight() > force_) return ERROR;//too heavy
	
	itemTable_.addItem(item);
	
	return OK;
}

ErrorCodes Forager::throwItem(int index) {
	Item *item = itemTable_.getItem(index);
	if(item == nullptr) return ERROR;
	
	level_.dropItem(coord_, item);
	
	return OK;
}

void Forager::kill() {
	dropAllItems();
	level_.killForager(this);
}

void Forager::dropAllItems() {
	itemTable_.dropAll(level_, coord_);
}

void Forager::drawCell(sf::RectangleShape &shape) {

}
