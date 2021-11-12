#ifndef LAB4_TABLE_H
#define LAB4_TABLE_H

#include "../AllClasses.h"

class Table {
private:
	std::vector<Item *> items_;
	int weight_;
public:
	Table() : weight_(0) {}
	
	~Table();
	
	int getAmount() const;
	Item *getItem(int);
	int getWeight() const;
	
	void setItem(int,Item*);
	
	void addItem(Item*);
	void calcWeight();
	void changeWeight(int diff);
	Item *deleteItem(int index);
	void dropAll(Level *level, Point &point);
};


#endif //LAB4_TABLE_H
