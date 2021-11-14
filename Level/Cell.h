#ifndef LAB4_CELL_H
#define LAB4_CELL_H

#include "../AllClasses.h"

class Cell {
private:
	CellType type_;
public:
	Cell() : type_(FLOOR) {}
	
	CellType getType() const { return type_; }
	
	void setType(CellType type);
};


#endif //LAB4_CELL_H
