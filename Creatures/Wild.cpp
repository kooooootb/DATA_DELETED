#include <SFML/Graphics.hpp>
#include "Table.h"
#include "Creature.h"
#include "Level.h"

namespace nodata{
	Wild::Wild(Level *level, std::string &name, Point coord, int healthMax, int timeMax, int walkTime, int viewRadius, int accuracy, int damage) : Creature(level, name, coord, healthMax, timeMax, walkTime, viewRadius) ,
																																				   accuracy_(accuracy) , damage_(damage) {
		
	}
	
	void Wild::attack(Creature *victim) {
		if(abs(coord_.x - victim->getPosition().x) > 1 || abs(coord_.y - victim->getPosition().y) > 1) return;//can't reach target
		if(timeCurrent_ < attackTime_) return;//not enough time
		
		spendTime(attackTime_);
		
		int damage = damage_ * (int)((float)accuracy_ / 100);
		victim->receiveDamage(damage);
	}
	
	void Wild::kill() {
		level_.killWild(this);
	}
	
	void Wild::drawCell(sf::RectangleShape &shape) {
	
	}
}
