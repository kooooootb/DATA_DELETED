#ifndef LAB4_SENTIENT_H
#define LAB4_SENTIENT_H

#include "Gun.h"
#include "Creature.h"

class Sentient : public Creature {
private:
	Gun* activeGun_;
	int accuracy_;
public:
	Sentient(std::string &name, Point &coord, int accuracy);
	
	~Sentient() override;
	
	void receiveGun(Gun*);
	void throwGun(Level *level);
	void shoot(Level *level, Creature *victim);
};


#endif //LAB4_SENTIENT_H
