#ifndef LAB4_CELLIT_H
#define LAB4_CELLIT_H

#include "../AllClasses.h"

namespace nodata{
	class CellIt {
	private:
		Cell *cur, **cells;
		Point begin, end, point;
		int index;
		bool steep;
		int dx, dy;
		int signX = 1, signY = 1;
		
		int vertCells, horizCells;
	public:
		CellIt(const Point &Begin, const Point &End, const Level &level);
		CellIt(const Level &level, const Point &Begin, const Point &End);
		
		const Point &getPoint(){ return point; }
		
		CellIt & operator++();
		Cell & operator*() const;
		bool operator!=(const CellIt &it) const;
	};
}


#endif //LAB4_CELLIT_H