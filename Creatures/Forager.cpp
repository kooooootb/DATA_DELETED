#include <SFML/Graphics.hpp>
#include "Table.h"
#include "Creature.h"
#include "Item.h"
#include "Level.h"

#include <iostream>

namespace nodata{
	Forager::Forager(Level *level, std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, int force) : Creature(level, name, coord, healthMax, timeMax, walkTime, viewRadius) , force_(force) {
	
	}
	
	ErrorCodes Forager::receiveItems() {
		Ptr<Item*> nearItems = level_.getItemMap()[coord_];
		for(int i = 0;i < nearItems.amount;++i){
			if(nearItems.ptr[i]->getWeight() + itemTable_.getWeight() > force_){
				continue;
			}//too heavy
			
			itemTable_.addItem(nearItems.ptr[i]);
			level_.getItemMap().removeItem(coord_, nearItems.ptr[i]);
		}
		delete [] nearItems.ptr;
		
		return OK;
	}
	
	ErrorCodes Forager::throwItem(int index) {
		Item *item = itemTable_.getItem(index);
		if(item == nullptr) return ERROR;
		
		level_.dropItem(coord_, item);
		
		return OK;
	}
	
	void Forager::kill() {
		dropAllItems();
		level_.killForager(this);
	}
	
	void Forager::dropAllItems() {
		itemTable_.dropAll(level_, coord_);
	}
	
	ErrorCodes Forager::move(int randVar){
		srand(randVar);
		ErrorCodes status = ERROR;
		
		if(!isBusy && itemTable_.empty()) {//if no items in inventory => look for item and make path to it
			Ptr<Point> nearItems = level_.getItemMap().getNear(coord_, viewRadius_);
			
			if (nearItems.amount != 0) {//check if there is item and it is reachable
				for (int i = 0; i < nearItems.amount; ++i) {//for every point with items nearby
					if (std::find(unreachable.begin(), unreachable.end(), nearItems.ptr[i]) != unreachable.end()) {
						continue;//point was marked as unreachable и нет смысла проверять ее еще раз
					}
					if (std::find(level_.getStorages().begin(), level_.getStorages().end(), nearItems.ptr[i]) !=
						level_.getStorages().end()) {
						continue;//point is a storage point
					}
					
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
			delete [] nearItems.ptr;
			
			if (path.empty()) {//can't find item => make random path
				path = level_.makePath(coord_, rand());
				if(path.empty()){
					status = ERROR;//stuck and can't find way => end turn
				}
			}
			if (!path.empty()) {
				status = walk(path.top());
				path.pop();
			}
		}
		else if(isBusy){//go to items and take it
			if(!path.empty()){//go
				status = walk(path.top());
				path.pop();
			}
			else{//take and create path to storage
				receiveItems();
				unreachable.push_back(coord_);
				
				isBusy = false;
				
				return OK;
			}
		}
		else if(goingToStorage) {//if going to storage => look for item nearby and go to storage
			Ptr<Point> nearItems = level_.getItemMap().getNear(coord_, viewRadius_);
			
			if (nearItems.amount != 0) {//check if there is item and it is reachable
				for (int i = 0; i < nearItems.amount; ++i) {//for every point with items nearby
					if (std::find(unreachable.begin(), unreachable.end(), nearItems.ptr[i]) != unreachable.end()) {
						continue;//point was marked as unreachable и нет смысла проверять ее еще раз
					}
					if (std::find(level_.getStorages().begin(), level_.getStorages().end(), nearItems.ptr[i]) !=
						level_.getStorages().end()) {
						continue;//point is a storage point
					}
					
					std::stack<Direction> newPath = level_.getPath(coord_, nearItems.ptr[i],
																   [](const Cell &cell) -> bool {
																	   return cell.walkAble();
																   });
					if (!newPath.empty()) {//reachable
						goingToStorage = false;
						isBusy = true;
						path = std::move(newPath);
						delete [] nearItems.ptr;
						
						return OK;
					} else {//unreachable
						unreachable.push_back(nearItems.ptr[i]);
					}
				}
			}
			delete [] nearItems.ptr;
			
			if(!path.empty()){//go
				status = walk(path.top());
				if(status != ERROR) path.pop();
			}
			else{//drop
				dropAllItems();
				goingToStorage = false;
				unreachable.clear();
				
				return OK;
			}
		}
		else if(!itemTable_.empty()){//find storage and create path to it
			const std::vector<Point> &storages = level_.getStorages();
			if(!storages.empty()){
				int dist = level_.getHorizCells() + level_.getVertCells();
				Point targetPoint;
				while(path.empty()){
					for(auto it : storages){
						if (std::find(unreachable.begin(), unreachable.end(), it) != unreachable.end()) {
							continue;//point was marked as unreachable и нет смысла проверять ее еще раз
						}
						
						int newDist = std::abs(it.x - coord_.x) + std::abs(it.y - coord_.y);
						if(newDist < dist){
							dist = newDist;
							targetPoint = it;
						}
					}
					
					std::stack<Direction> newPath = level_.getPath(coord_, targetPoint,
																   [](const Cell &cell) -> bool {
																	   return cell.walkAble();
																   });
					if (!newPath.empty()) {//reachable
						goingToStorage = true;
						path = std::move(newPath);
						
						return OK;
					} else {//unreachable
						unreachable.push_back(targetPoint);
					}
				}
			}
			
			if(path.empty()){//can't find path to storage => make random path
				path = level_.makePath(coord_, rand());
			}
			if(!path.empty()){
				status = walk(path.top());
				path.pop();
			}
		}
		
		if(status == ERROR){
			if(!goingToStorage){
				while(!path.empty()) path.pop();
				unreachable.clear();
			}
			
			isBusy = false;
		}
		return status;
	}
	
	void Forager::saveFile(std::ofstream &fs){
		fs << name_ << std::endl;
		fs << FORAGER << ' ' << coord_.x << ' ' << coord_.y << ' ' << healthMax_ << ' ' << timeMax_ << ' ' << walkTime_ << ' ' << viewRadius_ << ' ' << force_ << std::endl;
	}
}
