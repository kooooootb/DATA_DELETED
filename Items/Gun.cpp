#include "Header_Items.h"
#include "Creature.h"

Gun::Gun(std::string &name, int weight, int damage, int shootTime, int reloadTime, Ammunition ammoType, int ammoMax, int accuracy) :
		Item(name, weight) , damage_(damage) , shootTime_(shootTime) , reloadTime_(reloadTime) , ammoType_(ammoType) ,
		ammoMax_(ammoMax) , accuracy_(accuracy) {
	ammoCurrent_ = ammoMax_;
}

void Gun::setAmmoCurrent(int ammoCurrent) {
	ammoCurrent_ = ammoCurrent;
}

void Gun::receiveDamage(Level *level, int damage, Operative *operative) {
	operative->receiveDamage(level, damage);
}

ErrorCodes Gun::use(Creature *creature) {
	auto *operative = dynamic_cast<Operative*>(creature);
	if(operative == nullptr) return ERROR;
	
	Gun *activeGun = operative->getActiveGun();
	
	if(activeGun != nullptr){
		operative->getTable().addItem(activeGun);
	}
	operative->setActiveGun(this);
	
	return TOREMOVE;
}

int Gun::calcAmountByType() const{
	switch(ammoType_){
		case LARGE_CALIBER: return AMOUNTLARGE;
		case MEDIUM_CALIBER: return AMOUNTMEDIUM;
		case SMALL_CALIBER: return AMOUNTSMALL;
		default: throw std::exception();
	}
}

int Gun::calcAmmoWeightByType(int amount) const {
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

void Gun::shoot(Level *level, Creature *victim, float hitsMultipl) {
	int amount;
	if((amount = calcAmountByType()) < ammoCurrent_) amount = ammoCurrent_;
	int hits;
	if(calcAmountByType() == 1)
		hits = hitsMultipl > 0.7 ? 1 : 0;
	else hits = (int)((float)amount * hitsMultipl) + 1;
	victim->receiveDamage(level, damage_ * hits);
	
	ammoCurrent_ -= amount;
	weight_ -= calcAmmoWeightByType(amount);
}

float Gun::countHits(int operAccuracy) const {
	return (float)(operAccuracy + accuracy_) / 200;
}
