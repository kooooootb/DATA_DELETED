#ifndef LAB4_EDITTOOL_H
#define LAB4_EDITTOOL_H

#include <SFML/Graphics.hpp>
#include "Parameters.h"

#include "Level.h"

using namespace nodata;

namespace edittool{
	const int VIEWSTEP = 5;
	
	enum EditTips{
		E_CELL = 0,
		E_COUNT
	};
	
	class EditTool {
	private:
		Level level;
		Point coord;
		sf::RenderWindow window;
//	std::array<std::array<sf::RectangleShape, WINDOWHEIGHT_AMOUNT>, WINDOWWIDTH_AMOUNT> wCells;
		sf::RectangleShape **wCells;
		sf::Font font;
		
		std::vector<sf::Text> mesTips;
		
		unsigned int cellsVert, cellsHoriz;
		CellType celltype = FLOOR;
		bool mousePressed = false;
		int getIntFromWindow(int amount);
		void redrawWindow();
	public:
		EditTool();
		
		~EditTool();
		
		//refresh moves and changes shapes
		void refreshMap();
		void refreshInterface();
		
		//draw shapes
		void drawMap();
		
		void start();
		
	};
}


#endif //LAB4_EDITTOOL_H
