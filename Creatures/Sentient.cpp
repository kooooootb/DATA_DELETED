#include <SFML/Graphics.hpp>
#include "Table.h"
#include "Creature.h"
#include "Level.h"

#include <iostream>

namespace nodata{
	Sentient::Sentient(Level *level, std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, float accuracy) : Creature(level, name, coord, healthMax, timeMax, walkTime, viewRadius) ,
																																				  accuracyMultipl_(accuracy){
		activeGun_ = nullptr;
	}
	
	Sentient::~Sentient() {
		delete activeGun_;
	}
	
	void Sentient::receiveGun() {
		Ptr<Item*> nearItems = level_.getItemMap()[coord_];
		Gun *gun = nullptr;
		for(int i = 0;i < nearItems.amount;++i){
			if(nearItems.ptr[i]->getType() == GUN){
				gun = dynamic_cast<Gun*>(nearItems.ptr[i]);
				break;
			}
		}
		delete [] nearItems.ptr;
		
		if(gun != nullptr){
			activeGun_ = gun;
			level_.getItemMap().removeItem(coord_, gun);
			setTexture(WITH_GUN);
		}
	}
	
	void Sentient::throwGun() {
		if(activeGun_ == nullptr) return;
		setTexture(UNARMED);
		
		level_.dropItem(coord_, activeGun_);
		activeGun_ = nullptr;
	}
	
	void Sentient::shoot(Creature *victim) {
		if(activeGun_ == nullptr) return;
		
		activeGun_->shoot(victim);
	}
	
	ErrorCodes Sentient::shoot(Point &point, int randVar) {
		if(activeGun_ == nullptr) return ERROR;
		
		auto countHits = [](double agun, double aoper, double dist) -> int {
			double res = (dist - 2) / (agun * aoper);
			if(res < 0) res = 0;
			res = sqrt(res);
			return (int)res;
		};
		
		return activeGun_->shoot(level_, point, this, countHits(activeGun_->getAccuracy(), accuracyMultipl_, sqrt(pow(coord_.x - point.x, 2) + pow(coord_.y - point.y, 2))), randVar);
	}
	
	void Sentient::kill() {
		if(activeGun_ != nullptr) level_.dropItem(coord_, activeGun_);
		activeGun_ = nullptr;
		level_.killSentient(this);
	}
	
	ErrorCodes Sentient::move(int randVar){
		srand(randVar);
		ErrorCodes status = ERROR;
		
		if(!isBusy && activeGun_ == nullptr){//if no current task and unarmed => look for gun and make path to it
			Ptr<Point> nearItems = level_.getItemMap().getNear(coord_, viewRadius_);
			
			if(nearItems.amount != 0) {//check if there is gun and it is reachable
				for (int i = 0; i < nearItems.amount; ++i) {//for every point with items nearby
					if (std::find(unreachable.begin(), unreachable.end(), nearItems.ptr[i]) != unreachable.end()) {
						continue;//point was marked as unreachable и нет смысла проверять ее еще раз
					}
					if (std::find(useless.begin(), useless.end(), nearItems.ptr[i]) != useless.end()) {
						continue;//point has empty gun
					}
					
					Ptr<Item *> items = level_.getItemMap()[nearItems.ptr[i]];
					bool isGun = false;
					for (int j = 0; j < items.amount; ++j) {//check if point has a gun
						if (items.ptr[j]->getType() == GUN) {
							isGun = true;
							break;
						}
					}
					delete [] items.ptr;
					if (isGun) {//if there is a gun make path
						std::stack<Direction> newPath = level_.getPath(coord_, nearItems.ptr[i],
																	   [](const Cell &cell) -> bool {
																		   return cell.walkAble();
																	   });
						if (!newPath.empty()) {//reachable
							isBusy = true;
							path = std::move(newPath);
							delete [] nearItems.ptr;
							
							return OK;
						} else {//unreachable
							unreachable.push_back(nearItems.ptr[i]);
						}
					}
				}
			}
			delete [] nearItems.ptr;
			
			if(path.empty()){//can't go to gun => make random path
				path = level_.makePath(coord_, rand());
			}
			if(!path.empty()){
				status = walk(path.top());
				path.pop();
			}
		}
		else if(isBusy/* => unarmed */){//go to gun and take it
			if(!path.empty()){//go
				status = walk(path.top());
				path.pop();
			}
			else{//take
				receiveGun();
				if (activeGun_->getAmmoCurrent() <= 0) {//out of ammo
					throwGun();
					useless.push_back(coord_);//this point has empty gun
				}
				isBusy = false;
				
				return OK;
			}
		}
		else{//armed
			Ptr<Point> nearCreatures = level_.getCreatureMap().getNear(coord_, viewRadius_);//get visible points with creatures
			
			for(int i = 0;i < nearCreatures.amount;++i) {//for every point with creatures
				Ptr<Creature *> creatures = level_.getCreatureMap()[nearCreatures.ptr[i]];
				for (int j = 0; j < creatures.amount; ++j) {//check if point has operative
					if (creatures.ptr[j]->getType() == OPERATIVE) {//found operative
						bool canHit = true;
						level_.setRay(coord_, nearCreatures.ptr[i]);
						for (auto it: level_) {
							if (!it.getCell()->walkAble()) {//found obstacle
								canHit = false;
								break;
							}
						}
						
						if (canHit) {
							status = shoot(nearCreatures.ptr[i], rand());
							if(status == OK){
								level_.drawLine(coord_, nearCreatures.ptr[i]);
							}
							if (activeGun_->getAmmoCurrent() <= 0) {//out of ammo
								throwGun();
								useless.push_back(coord_);//this point has empty gun
							}
							delete [] creatures.ptr;
							delete [] nearCreatures.ptr;
							
							return status;
						}
						
						break;//can't hit current point, go to next
					}
				}
				delete [] creatures.ptr;
			}
			delete [] nearCreatures.ptr;
			
			if(path.empty()){//can't hit anyone => make random path
				path = level_.makePath(coord_, rand());
			}
			if(!path.empty()){
				status = walk(path.top());
				path.pop();
			}
		}
		
		if(status == ERROR){
			unreachable.clear();
			while(!path.empty()) path.pop();
			isBusy = false;
		}
		return status;
	}
	
	void Sentient::saveFile(std::ofstream &fs){
		fs << name_ << std::endl;
		fs << SENTIENT << ' ' << coord_.x << ' ' << coord_.y << ' ' << healthMax_ << ' ' << timeMax_ << ' ' << walkTime_ << ' ' << viewRadius_ << ' ' << accuracyMultipl_ << std::endl;
	}
}
