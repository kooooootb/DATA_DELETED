#include <SFML/Graphics.hpp>
#include "Item.h"
#include "Creature.h"
#include "../Parameters.h"
#include "Level.h"

#include <iostream>

namespace nodata{
	Gun::Gun(std::string &name, int weight, const Point &point, int damage, int shootTime, int reloadTime, Ammunition ammoType, int ammoMax, float accuracy, int switchTime) :
			Item(name, 0, point) , damage_(damage) , shootTime_(shootTime) , reloadTime_(reloadTime) , ammoType_(ammoType) ,
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
	
	void Gun::shoot(Creature *victim) const {
		victim->receiveDamage(damage_);
	}
	
	ErrorCodes Gun::shoot(Level &level, Point &coord, Creature *shooter, int hitsMultipl, int randVar) {
		if (ammoCurrent_ <= 0) {//no bullets
			coord = shooter->getPosition();
			return WARNING;
		}
		if (shooter->getTimeCurrent() < shootTime_) {//no time
			coord = shooter->getPosition();
			return ERROR;
		}
		
		if(coord == shooter->getPosition()){//if shoot point and creature's coord are the same
			Ptr<Creature*> nearCreatures = level.getCreatureMap()[coord];
			Creature *victim = nullptr;
			
			for(int i = 0;i < nearCreatures.amount;++i){//look for victim
				if(nearCreatures.ptr[i] != shooter){
					victim = nearCreatures.ptr[i];
					break;
				}
			}
			delete [] nearCreatures.ptr;
			
			if(victim == nullptr){//nobody near you, can't shoot yourself
				coord = shooter->getPosition();
				return OK;
			}
			
			shoot(victim);
		}
		else{
			//count accuracy
			srand(randVar);
			if (hitsMultipl != 0) {
				coord.x += rand() % (2 * hitsMultipl) - hitsMultipl;
				coord.y += rand() % (2 * hitsMultipl) - hitsMultipl;
			}
			
			//make ray
			level.setRay(shooter->getPosition(), coord);
			for (const CellIt &it : level) {
				Point curPoint = it.getPoint();
				Ptr<Creature*> nearCr = level.getCreatureMap()[curPoint];
				if (!it.getCell()->walkAble() || nearCr.ptr != nullptr) {
					delete [] nearCr.ptr;
					coord = curPoint;
					break;
				}
			}
			
			//shoot
			Ptr<Creature *> creatures = level.getCreatureMap()[coord];
			Cell *cell;
			ErrorCodes status = level.getCell(coord, cell);
			if (creatures.ptr != nullptr) {//shoot creature
				int amount = (int) creatures.amount;
				srand(randVar);
				shoot(creatures.ptr[rand() % amount]);
				delete [] creatures.ptr;
			} else if (status != ERROR) {//shoot cell
				if ((cell->getType() == GLASS || cell->getType() == PARTITION)) level.setCell(coord, FLOOR);
			}
		}
		
		ammoCurrent_ -= 1;
		shooter->spendTime(shootTime_);
		weight_ -= calcAmmoWeightByType(1, ammoType_);
		
		return OK;
	}
	
	int Gun::countAccuracy(double crAccuracy, double dist) const {
		double res = (dist - 2) / (crAccuracy * accuracy_);
		if(res < 0) res = 0;
		res = sqrt(res);
		return (int)res;
	}
	
	void Gun::saveFile(std::ofstream &fs) {
		fs << name_ << std::endl;
		fs << GUN << ' ' << coord_.x << ' ' << coord_.y << ' ' << weight_ - calcAmmoWeightByType(ammoMax_, ammoType_) << ' ' << damage_ << ' ' << shootTime_ << ' ' << reloadTime_ << ' ' << ammoType_ << ' ' << ammoMax_ << ' ' << accuracy_ << ' ' << switchTime_ << std::endl;
	}
}
