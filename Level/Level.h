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
	std::vector<std::vector<Cell>> cells_;
	Creature *activeCreature;
	Map<Item*> itemMap;
	Map<Creature*> creatureMap;
	CreatType turn;
	
	unsigned int C_OPERATIVES = 0;
	unsigned int C_SENTIENTS = 0;
	unsigned int C_WILDS = 0;
	unsigned int C_FORAGERS = 0;
	unsigned int CELLS_HORIZ = 0, CELLS_VERT = 0;
public:
	Level();//constructor loads creatures and cells from default files
	~Level() = default;
	
	std::vector<std::vector<Cell>> &getCells() { return cells_; }
	ErrorCodes getCell(int x, int y, Cell &cell) const;//return pointer to cell or nullptr if indexes were invalid
	Map<Item*> &getItemMap() { return itemMap; }
	const Map<Creature*> &getCreatureMap() const { return creatureMap; }
	Creature *getActiveCreature() const { return activeCreature; }
	unsigned int getHorizCells() const { return CELLS_HORIZ; }
	unsigned int getVertCells() const { return CELLS_VERT; }
	const std::vector<std::vector<Cell>> &getCells() const { return cells_; }
	const std::vector<Operative *> &getOperativeArray() const { return operativeAr_; }
	
	void setCell(int x, int y, CellType type);
	void setActive(int i);
	
	void dropItem(Point &point, Item *item);
	void addItem(Point &point, Item *item);
	void moveCreature(int x, int y, Creature *creature, Direction direction);
	
	void loadCells(const char *);
	
	//spawn all creatures from file
	void loadOperative(const char *);
	void loadSentient(const char *);
	void loadWild(const char *);
	void loadForager(const char *);
	void loadHKits(const char *);
	void loadAConts(const char *);
	void loadGuns(const char *);
	
	//killCreature == remove it from creatureAr_; should be called by creature->kill()
	void killOperative(Creature*);
	void killSentient(Creature*);
	void killWild(Creature*);
	void killForager(Creature*);
};


#endif //LAB4_LEVEL_H
