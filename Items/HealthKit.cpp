#include "HealthKit.h"

HealthKit::HealthKit(std::string &name, int weight, int healAmount, int healTime) : Item(name, weight) ,
healAmount_(healAmount) , healTime_(healTime) {

}

ErrorCodes HealthKit::use(Creature *creature) {
	creature->heal(healAmount_);
	
	creature->spendTime(healTime_);
	return TODELETE;
}
