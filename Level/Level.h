#ifndef LAB4_LEVEL_H
#define LAB4_LEVEL_H

#include "../AllClasses.h"

#include "Creature.h"
#include <vector>
#include "Cell.h"

class Level {
private:
	std::vector<Operative *> operativeAr_;
	std::vector<Wild *> wildAr_;
	std::vector<Sentient *> sentientAr_;
	std::vector<Forager *> foragerAr_;
	Cell **cells_;
	int x_, y_;
	Creature *activeCreature;
public:
	Level(int x, int y);
	~Level() { delete cells_; }
	
	void setX();
	void setY();
	void setCell();
	int getX() const { return x_; }
	int getY() const { return y_; }
	Cell getCell(int x,int y) const { return cells_[x][y]; }
	
	void killActive();
	void dropItem(Point point, Item *item);
};


#endif //LAB4_LEVEL_H
