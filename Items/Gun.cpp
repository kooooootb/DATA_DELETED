#include <SFML/Graphics.hpp>
#include "Item.h"
#include "Creature.h"
#include "../Parameters.h"
#include "Level.h"

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
		srand(time(nullptr));
		if(rand() % 100 < hitsMultipl * 100) victim->receiveDamage(damage_);
		
		ammoCurrent_ -= 1;
		shooter->spendTime(shootTime_);
		weight_ -= calcAmmoWeightByType(1);
	}
	
	void Gun::shoot(Level &level, const Point &coord, Creature *shooter, double hitsMultipl) {
		if(ammoCurrent_ < 1 || shooter->getTimeCurrent() < shootTime_) return;
		const std::vector<Creature*>* creatures = level.getCreatureMap()[coord];
		Cell cell;
		ErrorCodes status = level.getCell(coord, cell);
		if(creatures != nullptr){//shoot creature
			int amount = (int)(*creatures).size();
			if(amount == 1){
				shoot((*creatures)[0], shooter, hitsMultipl);
			}
			else{
				srand(time(nullptr));
				shoot((*creatures)[rand() % amount], shooter, hitsMultipl);
			}
		}
		else if(status != ERROR){//shoot cell
			srand(time(nullptr));
			if(cell.getType() == GLASS && rand() % 100 < (int)(hitsMultipl * 100)) level.setCell(coord, FLOOR);
			ammoCurrent_ -= 1;
			shooter->spendTime(shootTime_);
		}

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
