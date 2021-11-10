#ifndef LAB4_CELL_H
#define LAB4_CELL_H

enum CellType{
	GLASS,
	WALL,
	FLOOR,
	PARTITION
};

class Cell {
private:
	int type_;
public:
	int getType() const { return type_; }
};


#endif //LAB4_CELL_H
