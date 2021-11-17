#include <SFML/Graphics.hpp>
#include "Item.h"
#include "Creature.h"
#include "../Parameters.h"

AmmoContainer::AmmoContainer(std::string &name, int weight, Ammunition ammoType, int ammoMax) :
Item(name, weight + calcAmmoWeightByType(ammoMax)) , ammoType_(ammoType) , ammoMax_(ammoMax) {
	ammoCurrent_ = ammoMax_;
}

void AmmoContainer::setAmmoType(Ammunition ammoType) {
	ammoType_ = ammoType;
}

void AmmoContainer::setAmmoCurrent(int ammoCurrent) {
	ammoCurrent_ = ammoCurrent;
}

void AmmoContainer::setAmmoMax(int ammoMax) {
	ammoMax_ = ammoMax;
}

ErrorCodes AmmoContainer::use(Creature *creature) {//reload activeGun using container
	auto *operative = dynamic_cast<Operative*>(creature);
	if(operative == nullptr) return ERROR;
	
	Gun *activeGun = operative->getActiveGun();
	
	if(activeGun->getAmmoType() != ammoType_)//different types
		return ERROR;
	
	int magSize = activeGun->getAmmoMax();
	int oddAmmo = activeGun->getAmmoCurrent();
	
	operative->spendTime((int)((float)activeGun->getReloadTime() * operative->getReloadTimeMultipl()));
	
	if(ammoCurrent_ + oddAmmo > magSize){//full mag -> container isn't empty
		activeGun->setAmmoCurrent(magSize);
		ammoCurrent_ = ammoCurrent_ - magSize + oddAmmo;
		
		int diff = calcAmmoWeightByType(magSize - oddAmmo);//negative
		weight_ -= diff;
		operative->getTable().changeWeight(-1 * diff);
		activeGun->setWeight(activeGun->getWeight() + diff);
		
		return OK;
	}else{//empty container
		activeGun->setAmmoCurrent(ammoCurrent_ + oddAmmo);
		
		int diff = calcAmmoWeightByType(ammoCurrent_);
		weight_ -= diff;
		operative->getTable().changeWeight(-1 * diff);
		activeGun->setWeight(activeGun->getWeight() + diff);
		
		return TODELETE;
	}
}

int AmmoContainer::calcAmmoWeightByType(int amount) {
	switch(ammoType_){
		case LARGE_CALIBER:
			return amount * WEIGHTLARGE;
		case MEDIUM_CALIBER:
			return amount * WEIGHTMEDIUM;
		case SMALL_CALIBER:
			return amount * WEIGHTSMALL;
		default:
			throw std::exception();
	}
}

void AmmoContainer::drawCell(sf::RectangleShape &shape) {

}
