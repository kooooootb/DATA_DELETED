#include <SFML/Graphics.hpp>
#include "Table.h"
#include "Creature.h"
#include "Level.h"


Wild::Wild(std::string &name, Point coord, int accuracy, int damage) : Creature(name, coord) ,
accuracy_(accuracy) , damage_(damage) {

}

void Wild::attack(Creature *victim, Level *level) {
	int damage = damage_ * (int)((float)accuracy_ / 100);
	victim->receiveDamage(level, damage);
}

void Wild::kill(Level *level) {
	level->killWild(this);
}

void Wild::drawCell(sf::RectangleShape &shape) {

}

Wild::~Wild() = default;//nothing to clear
