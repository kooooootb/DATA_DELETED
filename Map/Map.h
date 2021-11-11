#ifndef LAB4_MAP_H
#define LAB4_MAP_H

#include "../AllClasses.h"

template<class T>
class Map {
private:
	struct HorCell{
		int x;
		std::vector<T> items;
		
		HorCell(int X, T *item) : x(X) {
			items.push_back(item);
		}
	};
	struct VertCell{
		int y;
		int amountX;
		HorCell *row;
	};
	VertCell *vertAr;
	int amountY;
	
	template<class CellT>
	CellT* createCell(int index, CellT *&ar, int &amount);
	template<class CellT>
	CellT* removeCell(int index, CellT *&ar, int &amount);
public:
	Map();
	
	~Map();
	
	void addItem(int x, int y, T *item);
	void removeItem(int x, int y, Item *item);
	
	std::vector<T> ** getLine(int y, int begin, int end);
	
	void print(std::ostream &s);
};


#endif //LAB4_MAP_H
