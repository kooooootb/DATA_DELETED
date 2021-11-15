#ifndef LAB4_GAME_H
#define LAB4_GAME_H

#include <SFML/Graphics.hpp>
#include "Parameters.h"

#include "Level.h"

class Game {
private:
	Level level;
	sf::RenderWindow window;
	std::array<std::array<sf::RectangleShape, WINDOWHEIGHT_AMOUNT>, WINDOWWIDTH_AMOUNT> wCells;
	sf::RectangleShape interfaceWindow;
	sf::Text textActCreature, textItems;
	sf::Font font;
	
	std::vector<Item*> *nearItems;
	
	Creature *cr;
	
	int getIntFromWindow(int amount);
public:
	Game();
	
	~Game() = default;
	
	//refresh moves and changes shapes
	void refreshMap();
	void refreshInterface();
	
	//draw draws shapes
	void drawMap();
	void drawInterface();
	
	void switchCreature();
	void takeItem();
	
	void start();
	
};


#endif //LAB4_GAME_H
