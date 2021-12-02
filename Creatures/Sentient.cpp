#include <SFML/Graphics.hpp>
#include "Table.h"
#include "Creature.h"
#include "Level.h"

namespace nodata{
	Sentient::Sentient(Level *level, std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, float accuracy) : Creature(level, name, coord, healthMax, timeMax, walkTime, viewRadius) ,
																																				  accuracyMultipl_(accuracy){
		activeGun_ = nullptr;
	}
	
	Sentient::~Sentient() {
		delete activeGun_;
	}
	
	void Sentient::receiveGun(Gun *gun) {
		activeGun_ = gun;
	}
	
	void Sentient::throwGun() {
		if(activeGun_ == nullptr) return;
		
		level_.dropItem(coord_, activeGun_);
	}
	
	void Sentient::shoot(Creature *victim) {
		if(activeGun_ == nullptr) return;
		
		activeGun_->shoot(victim);
	}
	
	void Sentient::kill() {
		if(activeGun_ != nullptr) level_.dropItem(coord_, activeGun_);
		activeGun_ = nullptr;
		level_.killSentient(this);
	}
	
	ErrorCodes Sentient::move(int randVar){
		
		
		return ERROR;
	}
	
	void Sentient::saveFile(std::ofstream &fs){
		fs << name_ << std::endl;
		fs << SENTIENT << ' ' << coord_.x << ' ' << coord_.y << ' ' << healthMax_ << ' ' << timeMax_ << ' ' << walkTime_ << ' ' << viewRadius_ << ' ' << accuracyMultipl_ << std::endl;
	}
}
