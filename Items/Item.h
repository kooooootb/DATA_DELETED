#ifndef LAB4_ITEM_H
#define LAB4_ITEM_H

#include "../AllClasses.h"

class Item {
protected:
	std::string name_;
	int weight_;
public:
	explicit Item(std::string &name, int weight = 10);
	
	virtual ~Item() = default;
	
	virtual ErrorCodes use(Creature *creature) = 0;
	int getWeight() const { return weight_; }
	void setWeight(int weight);
};

#include "Table.h"
#include "HealthKit.h"
#include "AmmoContainer.h"
#include "Gun.h"

#endif //LAB4_ITEM_H
