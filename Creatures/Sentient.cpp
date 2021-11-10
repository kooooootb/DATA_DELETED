#include "Sentient.h"

Sentient::Sentient(std::string &name, Point &coord, int accuracy) : Creature(name, coord) , accuracy_(accuracy){
	activeGun_ = nullptr;
}

Sentient::~Sentient() {
	delete activeGun_;
}

void Sentient::receiveGun(Gun *gun) {
	activeGun_ = gun;
}

void Sentient::throwGun(Level *level) {
	if(activeGun_ == nullptr) return;
	
	level->dropItem(coord_, activeGun_);
}

void Sentient::shoot(Level *level, Creature *victim) {
	if(activeGun_ == nullptr) return;
	
	activeGun_->shoot(level, victim, activeGun_->countHits(accuracy_));
}
