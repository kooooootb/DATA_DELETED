#include "Header_Creatures.h"
#include "Item.h"
#include "Level.h"
#include "Operative.h"


Operative::Operative(std::string &name, Point &coord, float reloadTime, int force, int accuracy) : Creature(name, coord) ,
		reloadTime_(reloadTime), force_(force) , accuracy_(accuracy) {
	activeGun_ = nullptr;
	//...
}

Operative::~Operative() {
	//activeGun should be nullptr here if operative was killed
	delete activeGun_;
}

//void Operative::switchGun(int index) {
//	Item *item = itemTable_.getItem(index);
//	if(item == nullptr) return;
//
//	item->use(this);
//}
//useItem == switchGun

ErrorCodes Operative::receiveItem(Item *item) {
	if(item->getWeight() + itemTable_.getWeight() + activeGun_->getWeight() > force_) return ERROR;//too heavy
	
	itemTable_.addItem(item);
	
	return OK;
}

void Operative::throwItem(Level *level, int index) {
	Item *item = itemTable_.deleteItem(index);
	if(item == nullptr) return;
	
	level->dropItem(coord_, item);
}

void Operative::useItem(int index) {
	Item *item = itemTable_.getItem(index);
	if(item == nullptr) return;
	
	ErrorCodes status = item->use(this);
	
	switch(status){
		case TODELETE:
			delete itemTable_.deleteItem(index);
			break;
		case TOREMOVE:
			itemTable_.deleteItem(index);
			break;
		default:
			break;
	}
}

void Operative::shoot(Level *level, Creature *victim) {
	if(activeGun_ == nullptr) return;
	
	activeGun_->shoot(level, victim, activeGun_->countHits(accuracy_));
}

void Operative::setActiveGun(Gun *gun) {
	activeGun_ = gun;
}

void Operative::kill(Level *level) {
	level->killOperative(this);
}
