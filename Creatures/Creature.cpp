#include "Table.h"
#include "Header_Creatures.h"
#include "Level.h"

Creature::Creature(Level *level, std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius) :
	name_(name) , coord_(coord) , healthMax_(healthMax) , timeMax_(timeMax) , walkTime_(walkTime) , viewRadius_(viewRadius) , level_(*level){
	healthCurrent_ = healthMax;
	timeCurrent_ = timeMax_;
}

Creature::~Creature() = default;//nothing to delete

void Creature::walk(Direction direction) {
	const std::vector<std::vector<Cell>> &cells = level_.getCells();
	switch (direction){
		case LEFT:
			if(coord_.x == 0) return;
			if(cells[coord_.x - 1][coord_.y].getType() == FLOOR){
				level_.moveCreature(coord_.x, coord_.y, this, LEFT);
				coord_.x--;
			}
			break;
		case UP:
			if(coord_.y == 0) return;
			if(cells[coord_.x][coord_.y - 1].getType() == FLOOR){
				level_.moveCreature(coord_.x, coord_.y, this, UP);
				coord_.y--;
			}
			break;
		case RIGHT:
			if(coord_.x == level_.getHorizCells() - 1) return;
			if(cells[coord_.x + 1][coord_.y].getType() == FLOOR){
				level_.moveCreature(coord_.x, coord_.y, this, RIGHT);
				coord_.x++;
			}
			break;
		case DOWN:
			if(coord_.y == level_.getVertCells() - 1) return;
			if(cells[coord_.x][coord_.y + 1].getType() == FLOOR){
				level_.moveCreature(coord_.x, coord_.y, this, DOWN);
				coord_.y++;
			}
			break;
		default:
			throw std::exception();
	}
	
	spendTime(walkTime_);
}

void Creature::heal(int healAmount) {
	healthCurrent_ = healthCurrent_ + healAmount > healthMax_ ? healthMax_ : healthCurrent_ + healAmount ;
}

void Creature::receiveDamage(int damage) {
	if(damage >= healthCurrent_) kill();
	else healthCurrent_ -= damage;
}

void Creature::setHealthMax(int healthMax) {
	healthMax_ = healthMax;
}

void Creature::setHealthCurrent(int healthCurrent) {
	healthCurrent_ = healthCurrent;
}

void Creature::setWalkTime(int walkTime) {
	walkTime_ = walkTime;
}

void Creature::setViewRadius(int viewRadius) {
	viewRadius_ = viewRadius;
}

void Creature::setTimeMax(int timeMax) {
	timeMax_ = timeMax;
}

void Creature::spendTime(int time) {
	timeCurrent_ -= time;
}
