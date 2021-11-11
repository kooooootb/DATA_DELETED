#ifndef LAB4_LEVEL_H
#define LAB4_LEVEL_H

#include "../AllClasses.h"

#include "Header_Items.h"
#include "Header_Creatures.h"
#include "Cell.h"
#include "Map.h"

class Level {
private:
	std::vector<Operative *> operativeAr_;
	std::vector<Wild *> wildAr_;
	std::vector<Sentient *> sentientAr_;
	std::vector<Forager *> foragerAr_;
	Cell **cells_;
	Creature *activeCreature;
	Map<Item*> itemMap;
	Map<Creature*> creatureMap;
	
public:
	Level();
	~Level() { delete cells_; }
	
	Cell **getCells() const { return cells_; }
	Map<Item*> getItemMap() const { return itemMap; }
	Map<Creature*> getCreatureMap() const { return creatureMap; }
	
	void setCell(int x, int y, Cell &cell);
	
	void dropItem(Point &point, Item *item);
	
	void spawnOperative(std::string &);
	void spawnSentient(std::string &);
	void spawnWild(std::string &);
	void spawnForager(std::string &);
	
	//killCreature == remove it from creatureAr_; should be called by creature->kill()
	void killOperative(Creature*);
	void killSentient(Creature*);
	void killWild(Creature*);
	void killForager(Creature*);
};


#endif //LAB4_LEVEL_H
