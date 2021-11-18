#include <SFML/Graphics.hpp>
#include "Item.h"
#include "Creature.h"
#include "../Parameters.h"

namespace nodata{
	Gun::Gun(std::string &name, int weight, int damage, int shootTime, int reloadTime, Ammunition ammoType, int ammoMax, float accuracy,
			 int switchTime) :
			Item(name, 0) , damage_(damage) , shootTime_(shootTime) , reloadTime_(reloadTime) , ammoType_(ammoType) ,
			ammoMax_(ammoMax) , accuracy_(accuracy) , switchTime_(switchTime) {
		setWeight(weight + calcAmmoWeightByType(ammoMax));
		ammoCurrent_ = ammoMax_;
	}
	
	void Gun::setAmmoCurrent(int ammoCurrent) {
		ammoCurrent_ = ammoCurrent;
	}
	
	void Gun::receiveDamage(int damage, Operative *operative) {
		operative->receiveDamage(damage);
	}
	
	ErrorCodes Gun::use(Creature *creature) {
		auto *operative = dynamic_cast<Operative*>(creature);
		if(operative == nullptr) return ERROR;
		
		if(switchTime_ > creature->getTimeCurrent()) return ERROR;//not enough time
		
		Gun *activeGun = operative->getActiveGun();
		
		if(activeGun != nullptr){
			operative->getTable().addItem(activeGun);
		}
		operative->setActiveGun(this);
		
		creature->spendTime(switchTime_);
		
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
	
	void Gun::shoot(Creature *victim, Creature *shooter, double hitsMultipl) {
		int amount;
		if((amount = calcAmountByType()) > ammoCurrent_) amount = ammoCurrent_;//not enough bullets
		if(shooter->getTimeCurrent() < shootTime_ * amount) amount = shooter->getTimeCurrent() / shootTime_;//not enough time
		int hits;
		if(amount == 1)
			hits = hitsMultipl > 0.7 ? 1 : 0;
		else hits = (int)((float)amount * hitsMultipl);
		victim->receiveDamage(damage_ * hits);
		
		ammoCurrent_ -= amount;
		shooter->spendTime(shootTime_ * amount);
		weight_ -= calcAmmoWeightByType(amount);
	}
	
	double Gun::countHitsMultipl(double crAccuracy, double dist) const {
		double res = crAccuracy * accuracy_ / (dist - 7);
		if(res > 1.0 || res < 0) res = 1.0;
		return res;
	}
	
	void Gun::drawCell(sf::RectangleShape &shape) {
		shape.setFillColor(sf::Color(0,100,5));
	}
}
