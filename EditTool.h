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
		E_ERROR = 1,
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
		int Rad = 9;
		int colLength = 4;
		int rowLength = 4;
		
		CreatType creatType = OPERATIVE;
		CellType celltype = FLOOR;
		bool drawingCell = true;
		bool mousePressed = false;
		
		void buildCell();
		void buildCreature();
		
		void buildOperative();
		void buildSentient();
		void buildWild();
		void buildForager();
		void clearCell();
		
		std::string getString(const char *mes);
		float getFloat(const char * mes, float amount);
		int getInt(const char * mes, int amount);
		void clearError();
		
		int getIntFromWindow(int amount);
		float getFloatFromWindow(float amount);
		std::string getStringFromWindow();
		void redrawWindow();
	public:
		EditTool();
		
		~EditTool();
		
		//refresh moves and changes shapes
		void refreshMap();
		void refreshInterface();
		
		void buildCircle();
		void buildColumn();
		void buildRow();
		void setCell(int x, int y);
		
		//draw shapes
		void drawMap();
		
		void start();
		
	};
}


#endif //LAB4_EDITTOOL_H
