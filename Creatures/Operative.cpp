#include <SFML/Graphics.hpp>
#include "Table.h"
#include "Creature.h"
#include "Item.h"
#include "Level.h"


Operative::Operative(Level *level, std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, float reloadTime, int force, float accuracy) : Creature(level, name, coord, healthMax, timeMax, walkTime, viewRadius) , reloadTimeMultipl_(reloadTime) , force_(force) , accuracyMultipl_(accuracy) {
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
	int sumWeight = item->getWeight() + itemTable_.getWeight();
	if(activeGun_ != nullptr)  sumWeight += activeGun_->getWeight();
	if(sumWeight > force_) return ERROR;//too heavy
	
	itemTable_.addItem(item);
	
	return OK;
}

void Operative::throwItem(int index) {
	Item *item = itemTable_.deleteItem(index);
	if(item == nullptr) return;
	
	level_.dropItem(coord_, item);
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

void Operative::shoot(Creature *victim) {
	if(activeGun_ == nullptr || victim == this) return;
	
	activeGun_->shoot(victim, this, activeGun_->countHitsMultipl(accuracyMultipl_));
}

void Operative::setActiveGun(Gun *gun) {
	activeGun_ = gun;
}

void Operative::kill() {
	dropAllItems();
	level_.killOperative(this);
}

void Operative::dropAllItems() {
	itemTable_.dropAll(level_, coord_);
	if(activeGun_ != nullptr) level_.addItem(coord_, activeGun_);
	activeGun_ = nullptr;
}

void Operative::drawCell(sf::RectangleShape &shape) {
	shape.setFillColor(sf::Color(10,0,115));
	
	sf::Texture texture;
	
	
	
}
