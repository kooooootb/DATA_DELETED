#include "Table.h"
#include "Creature.h"
#include "Level.h"

#include "../Parameters.h"

namespace nodata{
	Creature::Creature(Level *level, std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius) :
			name_(name) , coord_(coord) , healthMax_(healthMax) , timeMax_(timeMax) , walkTime_(walkTime) , viewRadius_(viewRadius) , level_(*level){
		healthCurrent_ = healthMax;
		timeCurrent_ = timeMax_;
	}
	
	Creature::~Creature() = default;//nothing to delete
	
	void Creature::walk(Direction direction) {
		if(timeCurrent_ < walkTime_) return;
		Cell **cells = level_.getCells();
		switch (direction) {
			case LEFT:
				if (coord_.x == 0) return;
				if (!cells[coord_.x - 1][coord_.y].walkAble()) return;
				level_.moveCreature(coord_.x, coord_.y, this, LEFT);
				coord_.x--;
				break;
			case UP:
				if (coord_.y == 0) return;
				if (!cells[coord_.x][coord_.y - 1].walkAble()) return;
				level_.moveCreature(coord_.x, coord_.y, this, UP);
				coord_.y--;
				break;
			case RIGHT:
				if (coord_.x == level_.getHorizCells() - 1) return;
				if (!cells[coord_.x + 1][coord_.y].walkAble()) return;
				level_.moveCreature(coord_.x, coord_.y, this, RIGHT);
				coord_.x++;
				break;
			case DOWN:
				if (coord_.y == level_.getVertCells() - 1) return;
				if (!cells[coord_.x][coord_.y + 1].walkAble()) return;
				level_.moveCreature(coord_.x, coord_.y, this, DOWN);
				coord_.y++;
				break;
		}
		
		spendTime(walkTime_);
		sprite.setRotation(direction * 90);
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
		if(healthMax_ < healthCurrent) healthMax_ = healthCurrent;
	}
	
	void Creature::setWalkTime(int walkTime) {
		walkTime_ = walkTime;
	}
	
	void Creature::setViewRadius(int viewRadius) {
		viewRadius_ = viewRadius;
	}
	
	void Creature::setTimeMax(int timeMax) {
		timeMax_ = timeMax;
		if(timeCurrent_ > timeMax) timeCurrent_ = timeMax;
	}
	
	void Creature::spendTime(int time) {
		timeCurrent_ -= time;
	}
	
	void Creature::resetTime() {
		timeCurrent_ = timeMax_;
	}
	
	void Creature::drawCreat(sf::RenderWindow &window) {
		window.draw(sprite);
	}
	
	void Creature::setDrawPosition(float x, float y) {
		sprite.setPosition(x * CELLSIZEX + ((float)CELLSIZEX/2), y * CELLSIZEY + ((float)CELLSIZEY/2));
	}
	
	void Creature::setTexture(sf::Texture &texture) {
		sprite.setTexture(texture);
		sprite.setScale(CELLSIZEX/sprite.getLocalBounds().width, CELLSIZEY/sprite.getLocalBounds().height);
		
		sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);
	}
}