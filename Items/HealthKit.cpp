#include <SFML/Graphics.hpp>
#include "Item.h"
#include "Creature.h"

namespace nodata{
	HealthKit::HealthKit(std::string &name, int weight, int healAmount, int healTime) : Item(name, weight) , healAmount_(healAmount) , healTime_(healTime) {
		
	}
	
	ErrorCodes HealthKit::use(Creature *creature) {
		if(healTime_ > creature->getTimeCurrent()) return ERROR;//not enough time
		creature->heal(healAmount_);
		
		creature->spendTime(healTime_);
		return TODELETE;
	}
}
