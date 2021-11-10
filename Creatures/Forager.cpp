#include "Forager.h"

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
