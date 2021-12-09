#include <SFML/Graphics.hpp>
#include "Table.h"
#include "Creature.h"
#include "Item.h"
#include "Level.h"


namespace nodata{
	Operative::Operative(Level *level, std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, float reloadTime, int force, float accuracy) : Creature(level, name, coord, healthMax, timeMax, walkTime, viewRadius) , reloadTimeMultipl_(reloadTime) , force_(force) , accuracyMultipl_(accuracy) {
		activeGun_ = nullptr;
	}
	
	Operative::~Operative() {
		delete activeGun_;
	}
	
	ErrorCodes Operative::receiveItem(Item *item) {
		int sumWeight = item->getWeight() + itemTable_.getWeight();
		if(activeGun_ != nullptr)  sumWeight += activeGun_->getWeight();
		if(sumWeight > force_) return ERROR;//too heavy
		
		itemTable_.addItem(item);
		
		return OK;
	}
	
	void Operative::throwItem(int index) {
		if(index == -1){//throw active gun
			level_.dropItem(coord_, activeGun_);
			setTexture(UNARMED);
			activeGun_ = nullptr;
		}
		else{
			Item *item = itemTable_.deleteItem(index);
			if(item == nullptr) return;
			
			level_.dropItem(coord_, item);
		}
	}
	
	void Operative::useItem(int index) {
		if(index == -1){
			receiveItem(activeGun_);
			setTexture(UNARMED);
			activeGun_ = nullptr;
		}else{
			Item *item = itemTable_.getItem(index);
			if(item == nullptr) return;
			
			ErrorCodes status = item->use(this);
			
			switch(status){
				case TODELETE:
					delete itemTable_.deleteItem(index);
					break;
				case TOREMOVE:
					itemTable_.deleteItem(index);
					break;
				default:
					break;
			}
		}
	}
	
	void Operative::shoot(Creature *victim) {
		if(activeGun_ == nullptr || victim == this) return;
		
		activeGun_->shoot(victim);
	}
	
	void Operative::shoot(Point &point) {
		if(activeGun_ == nullptr) return;
		srand(timeCurrent_ * time(nullptr));
		
		auto countHits = [](double agun, double aoper, double dist) -> int {
			double res = (dist - 2) / (agun * aoper);
			if(res < 0) res = 0;
			res = sqrt(res);
			return (int)res;
		};
		
		activeGun_->shoot(level_, point, this, countHits(activeGun_->getAccuracy(), accuracyMultipl_, sqrt(pow(coord_.x - point.x, 2) + pow(coord_.y - point.y, 2))), rand());
	}
	
	void Operative::setActiveGun(Gun *gun) {
		activeGun_ = gun;
		setTexture(WITH_GUN);
	}
	
	void Operative::kill() {
		dropAllItems();
		level_.killOperative(this);
	}
	
	void Operative::dropAllItems() {
		itemTable_.dropAll(level_, coord_);
		if(activeGun_ != nullptr) level_.dropItem(coord_, activeGun_);
		activeGun_ = nullptr;
	}
	
	int Operative::getWeight() const {
		return itemTable_.getWeight() + (activeGun_ == nullptr ? 0 : activeGun_->getWeight());
	}
	
	void Operative::setForce(int force) {
		if(force <= 0) return;
		force_ = force;
	}
	
	void Operative::saveFile(std::ofstream &fs){
		fs << name_ << std::endl;
		fs << OPERATIVE << ' ' << coord_.x << ' ' << coord_.y << ' ' << healthMax_ << ' ' << timeMax_ << ' ' << walkTime_ << ' ' << viewRadius_ << ' ' << reloadTimeMultipl_ << ' ' << force_ << ' ' << accuracyMultipl_ << std::endl;
	}
	
	void Operative::saveCurrentState(std::ofstream &fs){
		fs << name_ << std::endl;
		fs << OPERATIVE << ' ' << coord_.x << ' ' << coord_.y << ' ' << healthMax_ << ' ' << timeMax_ << ' ' << walkTime_ << ' ' << viewRadius_ << ' ' << reloadTimeMultipl_ << ' ' << force_ << ' ' << accuracyMultipl_ << ' ' << healthCurrent_ << ' ' << timeCurrent_ << ' ' << (activeGun_ == nullptr ? 0 : 1) << ' ' << itemTable_.size() << std::endl;
		
		if(activeGun_ != nullptr){
			activeGun_->saveCurrentState(fs);
		}
		
		for(auto it : itemTable_.getVector()){
			it->saveCurrentState(fs);
		}
	}
}
