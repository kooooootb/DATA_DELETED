#ifndef LAB4_GAME_H
#define LAB4_GAME_H

#include <SFML/Graphics.hpp>
#include "Parameters.h"

#include "Level.h"

enum Tips{
	T_ACTCREATURE = 0,
	T_TAKE,
	T_ENDT,//end turn
	T_USE,
	T_THROW,
	T_ERROR,
	T_INTMES,
	T_WIN,
	T_TURN,
	T_INPUT,
	T_COUNT
};

enum Stats{
	S_TIME = 0,
	S_HEALTH,
	S_AMMO,
	S_WEIGHT,
	S_COUNT
};

namespace nodata{
	class Game {
	private:
		Level level;
		sf::RenderWindow window;
		sf::RectangleShape interfaceWindow, invWindow;
		sf::Font font;
		
		Ptr<Item*> nearItems;//items in creature's position
		
		std::vector<sf::Text> mesTips;//0 - active creature, 1 - taking items, 2 - shoot, 3 - use
		std::vector<sf::Text> mesInv;
		std::vector<sf::Text> mesStats;
		
		Creature *cr;
		
		sf::Clock clock;
		sf::Vertex line[2];
		sf::Time period;
		
		sf::Event event;
		
		std::vector<Cell*> cellsOnScreen;
		std::vector<Creature*> creaturesOnScreen;
		std::vector<Item*> itemsOnScreen;
		
		int getIntFromWindow(int amount);
		std::string getStringFromWindow();
		void redrawWindow();
		void drawCell(int x, int y);
		void drawRay(const Point &point);
		
		void refreshActcreature();
		void refreshTake();
		void refreshEndt();
		void refreshUse();
		void refreshThrow();
		void refreshTime();
		void refreshHealth();
		void refreshAmmo();
		void refreshWeight();
		
		void cleanTips(int amount);
		
		void initGame();
		
		int getInt(const char *mes, int amount);
		std::string getString(const char *mes);
		void clearError();
		
		void saveGame();
		ErrorCodes makeMove(int &prevAmount, int all);
	public:
		Game(std::string &cells_cfg, std::string &items_cfg, std::string &creatures_cfg);
		explicit Game(std::string &save);
		
		~Game();
		
		//refresh moves and changes shapes
		void refreshMap();
		void refreshInterface();
		
		//draw shapes
		void drawMap();
		void drawInterface();
		void showError(const std::string &mes);
		void setIntmes(const std::string &mes);
		void clearIntmes();
		
		void switchInt();
		void takeInt();
		void shootInt();
		void useInt();
		ErrorCodes endInt();
		void throwInt();
		
		void setLine(const Point &from, const Point &to);
		
		void start();
	};
}


#endif //LAB4_GAME_H
