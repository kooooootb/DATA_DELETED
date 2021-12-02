#include <SFML/Graphics.hpp>
#include "Table.h"
#include "Creature.h"
#include "Level.h"

#include <iostream>

namespace nodata{
	Wild::Wild(Level *level, std::string &name, Point coord, int healthMax, int timeMax, int walkTime, int viewRadius, int accuracy, int damage, int attackTime) : Creature(level, name, coord, healthMax, timeMax, walkTime, viewRadius) , accuracy_(accuracy) , damage_(damage) , attackTime_(attackTime) {}
	
	ErrorCodes Wild::attack(Creature *victim) {
		if(victim == this) return WARNING;
		if(timeCurrent_ < attackTime_) return ERROR;//not enough time
		
		spendTime(attackTime_);
		
		int damage = damage_ * (int)((float)accuracy_ / 100);
		victim->receiveDamage(damage);
		return OK;
	}
	
	void Wild::kill() {
		level_.killWild(this);
	}
	
	ErrorCodes Wild::move(int randVar) {
		ErrorCodes status;
		Ptr<Creature*> nearcr = level_.getCreatureMap()[coord_];
		if(nearcr.amount > 1){
			int i = 0;
			if(nearcr.ptr[i] == this) i += 1;
			status = attack(nearcr.ptr[i]);
			return status;
		}
		
		srand(randVar);
		if(penaltyMoves_ > 0){
			auto direction = static_cast<Direction>(rand() % 4);
			status = walk(direction);
			penaltyMoves_--;
		}else if(!path_.empty()){
			status = walk(path_.front());
			path_.pop();
			if(status == WARNING){
				path_ = std::queue<Direction>();
				penaltyMoves_ = 5;
			}
		}else{
			std::vector<Point> circle;
			createCircle(circle, viewRadius_);
			for(auto it : circle){
				level_.setRay(coord_, coord_ + it);
				for (const CellIt &cell : level_) {
					Ptr<Creature*> creatures = level_.getCreatureMap()[cell.getPoint()];
					if(creatures.ptr != nullptr){
						for(int i = 0;i < creatures.amount;++i){
							if(dynamic_cast<Operative*>(creatures.ptr[i]) != nullptr){
								Point point = coord_;
								level_.setRay(coord_, cell.getPoint());
								for(const CellIt &cellpath : level_){
									path_.emplace(point.getDirection(cellpath.getPoint()));
									point = cellpath.getPoint();
								}
								return OK;
							}
						}
					}
				}
			}
			
			auto direction = static_cast<Direction>(rand() % 4);
			status = walk(direction);
		}
		
		if(status == ERROR){
			path_ = std::queue<Direction>();
		}
		
		return status;
	}
	
	void Wild::saveFile(std::ofstream &fs){
		fs << name_ << std::endl;
		fs << WILD << ' ' << coord_.x << ' ' << coord_.y << ' ' << healthMax_ << ' ' << timeMax_ << ' ' << walkTime_ << ' ' << viewRadius_ << ' ' << accuracy_ << ' ' << damage_ << ' ' << attackTime_ << std::endl;
	}
}
