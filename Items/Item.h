#ifndef LAB4_ITEM_H
#define LAB4_ITEM_H

#include "../AllClasses.h"

#include <string>

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


#endif //LAB4_ITEM_H
