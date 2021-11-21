#include <SFML/Graphics.hpp>
#include "Item.h"
#include "Creature.h"
#include "../Parameters.h"
#include "Level.h"

#include <iostream>

namespace nodata{
	Gun::Gun(std::string &name, int weight, int damage, int shootTime, int reloadTime, Ammunition ammoType, int ammoMax, float accuracy,
			 int switchTime) :
			Item(name, 0) , damage_(damage) , shootTime_(shootTime) , reloadTime_(reloadTime) , ammoType_(ammoType) ,
			ammoMax_(ammoMax) , accuracy_(accuracy) , switchTime_(switchTime) {
		setWeight(weight + calcAmmoWeightByType(ammoMax, ammoType));
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
	
	int Gun::calcAmmoWeightByType(int amount, Ammunition ammoType) {
		switch(ammoType){
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
	
	void Gun::shoot(Creature *victim, Creature *shooter) {
		victim->receiveDamage(damage_);
		
//		ammoCurrent_ -= 1;
//		shooter->spendTime(shootTime_);
//		weight_ -= calcAmmoWeightByType(1, ammoType_);
	}
	
	void Gun::shoot(Level &level, Point coord, Creature *shooter, int hitsMultipl) {
		if (ammoCurrent_ < 1 || shooter->getTimeCurrent() < shootTime_) return;
		
		srand(time(nullptr));
		if (hitsMultipl != 0) {
			coord.x += rand() % hitsMultipl;
			coord.y += rand() % hitsMultipl;
		}
		
		//make ray
		level.setRay(shooter->getPosition(), coord);
		for (Level::Iterator it = level.begin(), endIt = level.end(); it != endIt; ++it) {
			CellType type = (*it).getType();
			Point curPoint = it.getPoint();
			if (type != FLOOR || level.getCreatureMap()[curPoint] != nullptr) {
				coord = curPoint;
				break;
			}
		}
		
		//shoot
		const std::vector<Creature *> *creatures = level.getCreatureMap()[coord];
		Cell cell;
		ErrorCodes status = level.getCell(coord, cell);
		if (creatures != nullptr) {//shoot creature
			int amount = (int) (*creatures).size();
			srand(time(nullptr));
			shoot((*creatures)[rand() % amount], shooter);
		} else if (status != ERROR) {//shoot cell
			if ((cell.getType() == GLASS || cell.getType() == PARTITION)) level.setCell(coord, FLOOR);
		}
		
		ammoCurrent_ -= 1;
		shooter->spendTime(shootTime_);
		weight_ -= calcAmmoWeightByType(1, ammoType_);
	}
	
	int Gun::countAccuracy(double crAccuracy, double dist) const {
		double res = (dist - 2) / (crAccuracy * accuracy_);
		if(res < 0) res = 0;
		res = sqrt(res);
		return (int)res;
	}
	
	void Gun::drawCell(sf::RectangleShape &shape) {
		shape.setFillColor(sf::Color(0,100,5));
	}
}
