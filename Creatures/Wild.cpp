#include <SFML/Graphics.hpp>
#include "Table.h"
#include "Creature.h"
#include "Level.h"
#include "../Parameters.h"

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
		Ptr<Creature*> nearCr = level_.getCreatureMap()[coord_];
		for(int i = 0; i < nearCr.amount; ++i){
			if(nearCr.ptr[i]->getType() == OPERATIVE){
				path_ = std::stack<Direction>();
				status = attack(nearCr.ptr[i]);
				delete [] nearCr.ptr;
				return status;
			}
		}
		delete [] nearCr.ptr;
		
		srand(randVar);
		if(penaltyMoves_ > 0){//previously bumped in wall
			auto direction = static_cast<Direction>(rand() % 4);
			status = walk(direction);
			penaltyMoves_--;
		}else if(!path_.empty()){//have path to target
			status = walk(path_.top());
			path_.pop();
			if(status == WARNING){//bumped into wall
				path_ = std::stack<Direction>();
				penaltyMoves_ = 5;
			}
		}else{
			Ptr<Point> nearCreatures = level_.getCreatureMap().getNear(coord_, viewRadius_);
			
			for(int pointId = 0;pointId < nearCreatures.amount;++pointId) {//for every point
				Ptr<Creature *> creatures = level_.getCreatureMap()[nearCreatures.ptr[pointId]];
				
				bool isOperative = false;
				for (int crId = 0; crId < creatures.amount; ++crId) {//for every creature in point
					if (creatures.ptr[crId]->getType() == OPERATIVE) {
						isOperative = true;
						break;
					}
				}
				
				if (isOperative) {//found operative
					bool canReach = true;
					level_.setRay(coord_, nearCreatures.ptr[pointId]);
					std::queue<Direction> savedPath;
					for (auto it : level_) {
						if(!it.getCell()->seeAble()){
							canReach = false;
							break;
						}
					}
					
					if(canReach){
						std::stack<Direction> newPath = level_.getPath(coord_, nearCreatures.ptr[pointId], [](const Cell &cell) -> bool {
							return cell.seeAble();
						});
						if(!newPath.empty()){//reachable
							path_ = std::move(newPath);
							delete [] creatures.ptr;
							delete [] nearCreatures.ptr;
							
							return OK;
						}
					}
				}
				delete [] creatures.ptr;
			}
			delete [] nearCreatures.ptr;
			
			auto direction = static_cast<Direction>(rand() % 4);
			status = walk(direction);
			return status;
		}
		
		if(status == ERROR){
			path_ = std::stack<Direction>();
			penaltyMoves_ = 0;
		}
		
		return status;
	}
	
	void Wild::saveFile(std::ofstream &fs){
		fs << name_ << std::endl;
		fs << WILD << ' ' << coord_.x << ' ' << coord_.y << ' ' << healthMax_ << ' ' << timeMax_ << ' ' << walkTime_ << ' ' << viewRadius_ << ' ' << accuracy_ << ' ' << damage_ << ' ' << attackTime_ << std::endl;
	}
	
	void Wild::saveCurrentState(std::ofstream &fs){
		fs << name_ << std::endl;
		fs << WILD << ' ' << coord_.x << ' ' << coord_.y << ' ' << healthMax_ << ' ' << timeMax_ << ' ' << walkTime_ << ' ' << viewRadius_ << ' ' << accuracy_ << ' ' << damage_ << ' ' << attackTime_ << ' ' << healthCurrent_ << ' ' << timeCurrent_ << std::endl;
	}
}
