#ifndef LAB4_FORAGER_H
#define LAB4_FORAGER_H

#include "Table.h"
#include "Creature.h"

class Forager : public Creature {
private:
	Table itemTable_;
	int force_;
public:
	Forager(std::string &name, Point &coord, int force);
	
	~Forager() override	= default;
	
	ErrorCodes receiveItem(Item *item);
	ErrorCodes throwItem(Level *level, int index);
};


#endif //LAB4_FORAGER_H
