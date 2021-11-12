#ifndef LAB4_CREATURE_H
#define LAB4_CREATURE_H

#include "../AllClasses.h"

class Creature {
protected:
	std::string name_;
	int healthCurrent_;
	int healthMax_;
	int timeCurrent_;
	int timeMax_;
	int walkTime_;
	int viewRadius_;
	Point coord_;
public:
	Creature(std::string &name, Point &coord, int healthMax = 100, int timeMax = 100, int walkTime = 5, int viewRadius = 5);
	
	virtual ~Creature();
	
	int getHealthCurrent() const { return healthCurrent_; }
	int getHealthMax() const { return healthMax_; }
	int getWalkTime() const { return walkTime_; }
	int getViewRadius() const { return viewRadius_; }
	int getTimeMax() const { return timeMax_; }
	int getTimeCurrent() const { return timeCurrent_; }
	const Point &getPosition() const { return coord_; }
	const std::string &getName() const { return name_; }
	
	void setHealthMax(int healthMax);
	void setHealthCurrent(int healthCurrent);
	void setWalkTime(int walkTime);
	void setViewRadius(int viewRadius);
	void setTimeMax(int timeMax);
	
	void heal(int healAmount);
	virtual void kill(Level *level) = 0;
	void spendTime(int time);
	void walk(Cell **cells, Direction direction);
	void receiveDamage(Level *level, int damage);
};

#include "Operative.h"
#include "Sentient.h"
#include "Wild.h"
#include "Forager.h"

#endif //LAB4_CREATURE_H
