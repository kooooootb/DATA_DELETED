#include <SFML/Graphics.hpp>
#include "Item.h"
#include "Creature.h"

namespace nodata{
	HealthKit::HealthKit(std::string &name, int weight, const Point &point, int healAmount, int healTime) : Item(name, weight, point) , healAmount_(healAmount) , healTime_(healTime) {
		
	}
	
	ErrorCodes HealthKit::use(Creature *creature) {
		if(healTime_ > creature->getTimeCurrent()) return ERROR;//not enough time
		creature->heal(healAmount_);
		
		creature->spendTime(healTime_);
		return TODELETE;
	}
	
	void HealthKit::saveFile(std::ofstream &fs) {
		fs << name_ << std::endl;
		fs << HKIT << ' ' << coord_.x << ' ' << coord_.y << ' ' << weight_ << ' ' << healAmount_ << ' ' << healTime_ << std::endl;
	}
}
