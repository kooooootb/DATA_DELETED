#ifndef LAB4_ALLCLASSES_H
#define LAB4_ALLCLASSES_H

#include <string>
#include <string>
#include <vector>
#include <typeinfo>
#include <fstream>
#include <stdexcept>
#include <map>


enum CellType{
	GLASS,
	WALL,
	FLOOR,
	PARTITION
};

enum Ammunition{
	LARGE_CALIBER,
	MEDIUM_CALIBER,
	SMALL_CALIBER
};

enum ErrorCodes{
	ERROR = -1,
	OK,
	TODELETE,
	TOREMOVE
};

enum Direction{
	LEFT,
	UP,
	RIGHT,
	DOWN
};

struct Point{
	int x,y;
	
	explicit Point(int X = 0, int Y = 0) : x(X), y(Y) {}
};

class Creature;
class Forager;
class Operative;
class Sentient;
class Wild;
class AmmoContainer;
class Gun;
class HealthKit;
class Item;
class Table;
class Cell;
class Level;

//#include "Creature.h"
//#include "Forager.h"
//#include "Operative.h"
//#include "Sentient.h"
//#include "Wild.h"
//#include "Item.h"
//#include "Table.h"
//#include "Cell.h"
//#include "Level.h"


#endif //LAB4_ALLCLASSES_H
