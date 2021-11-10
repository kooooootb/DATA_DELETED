#ifndef LAB4_HEALTHKIT_H
#define LAB4_HEALTHKIT_H

#include "Item.h"
#include "Operative.h"

class HealthKit : public Item {
private:
	int healAmount_;
	int healTime_;
public:
	HealthKit(std::string &name, int weight, int healAmount, int healTime);
	
	~HealthKit() override = default;
	
	ErrorCodes use(Creature *creature) override;
};


#endif //LAB4_HEALTHKIT_H
