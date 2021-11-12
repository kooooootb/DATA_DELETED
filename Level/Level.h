#ifndef LAB4_LEVEL_H
#define LAB4_LEVEL_H

#include "../AllClasses.h"

#include "Item.h"
#include "Creature.h"
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
	
	unsigned int C_OPERATIVES = 0;
	unsigned int C_SENTIENTS = 0;
	unsigned int C_WILDS = 0;
	unsigned int C_FORAGERS = 0;
	unsigned int CELLS_HORIZ = 0, CELLS_VERT = 0;
public:
	Level();//constructor loads creatures and cells from default files
	~Level() { delete cells_; }
	
	Cell **getCells() const { return cells_; }
	Map<Item*> getItemMap() const { return itemMap; }
	Map<Creature*> getCreatureMap() const { return creatureMap; }
	
	void setCell(int x, int y, Cell &cell);
	
	void dropItem(Point &point, Item *item);
	void addItem(Point &point);
	
	void loadCells(const char *);
	
	//spawn all creatures from file
	void loadOperative(const char *);
	void loadSentient(const char *);
	void loadWild(const char *);
	void loadForager(const char *);
	
	//killCreature == remove it from creatureAr_; should be called by creature->kill()
	void killOperative(Creature*);
	void killSentient(Creature*);
	void killWild(Creature*);
	void killForager(Creature*);
};


#endif //LAB4_LEVEL_H
