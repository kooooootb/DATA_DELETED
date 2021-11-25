#include "CellIt.h"
#include "Level.h"

namespace nodata{
	CellIt::CellIt(const Point &Begin, const Point &End, const Level &Level) : level(Level) , cells(Level.getCells()) , begin(Begin) , end(End){//init algorithm
		vertCells = level.getVertCells();
		horizCells = level.getHorizCells();
		
		dx = End.x - Begin.x;
		dy = End.y - Begin.y;
		if (dx < 0){
			signX = -1;
			dx = -dx;
		}
		if (dy < 0){
			signY = -1;
			dy = -dy;
		}
		steep = dy > dx;
		dx += 1;
		dy += 1;
		
		index = 1;
		
		point.x = begin.x + (steep ? (index * dx) / dy : index) * signX;
		point.y = begin.y + (steep ? index : (index * dy) / dx) * signY;
		if(point.x < 0 || point.y < 0 || point.x >= horizCells || point.y >= vertCells) point = end;
		cur = cells[point.x] + point.y;
	}
	
	CellIt::CellIt(const Level &Level, const Point &Begin, const Point &End) : level(Level) , cells(Level.getCells()) , begin(Begin) , end(End){//init end
		vertCells = level.getVertCells();
		horizCells = level.getHorizCells();
		
		dx = End.x - Begin.x;
		dy = End.y - Begin.y;
		if (dx < 0){
			signX = -1;
			dx = -dx;
		}
		if (dy < 0){
			signY = -1;
			dy = -dy;
		}
		steep = dy > dx;
		dx += 1;
		dy += 1;
		
		index = steep ? dy : dx;
		
		point.x = begin.x + (steep ? (index * dx) / dy : index) * signX;
		point.y = begin.y + (steep ? index : (index * dy) / dx) * signY;
		
		if(point.x < 0 || point.y < 0 || point.x >= horizCells || point.y >= vertCells) point = end;
	}
	
	CellIt &CellIt::operator++() {
		point.x = begin.x + (steep ? (index * dx) / dy : index) * signX;
		point.y = begin.y + (steep ? index : (index * dy) / dx) * signY;
		
		if(point.x < 0 || point.y < 0 || point.x >= horizCells || point.y >= vertCells) point = end;
		
		cur = cells[point.x] + point.y;
		index += 1;
		
		return *this;
	}
	
	CellIt CellIt::operator++(int) {
		CellIt res(begin, end, level);
		res.point = point;
		res.cur = cur;
		
		point.x = begin.x + (steep ? (index * dx) / dy : index) * signX;
		point.y = begin.y + (steep ? index : (index * dy) / dx) * signY;
		
		if(point.x < 0 || point.y < 0 || point.x >= horizCells || point.y >= vertCells) point = end;
		
		cur = cells[point.x] + point.y;
		index += 1;
		
		return res;
	}
	
	Cell &CellIt::operator*() const {
		return *cur;
	}
	
	bool CellIt::operator!=(const CellIt &it) const {
		return point != it.point;
	}
}
