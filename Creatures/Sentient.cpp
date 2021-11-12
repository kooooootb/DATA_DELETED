#include "Table.h"
#include "Header_Creatures.h"
#include "Level.h"


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
	
	activeGun_->shoot(level, victim, this, activeGun_->countHits(accuracy_));
}

void Sentient::kill(Level *level) {
	level->getItemMap().addItem(coord_, activeGun_);
	activeGun_ = nullptr;
	level->killSentient(this);
}
