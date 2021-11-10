#ifndef LAB4_WILD_H
#define LAB4_WILD_H

#include "Creature.h"

class Wild : public Creature {
private:
	int accuracy_;
	int damage_;
public:
	Wild(std::string &name, Point coord, int accuracy, int damage);
	
	~Wild() override;
	
	void attack(Creature *victim, Level *level);
};


#endif //LAB4_WILD_H
