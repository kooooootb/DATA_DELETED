#ifndef LAB4_ALLCLASSES_H
#define LAB4_ALLCLASSES_H

const int WEIGHTLARGE = 10, WEIGHTMEDIUM = 5, WEIGHTSMALL = 2;//bullets' weight
const int AMOUNTLARGE = 1, AMOUNTMEDIUM = 5, AMOUNTSMALL = 4;//bullets per shot

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


#endif //LAB4_ALLCLASSES_H
