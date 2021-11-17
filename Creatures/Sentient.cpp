#include <SFML/Graphics.hpp>
#include "Table.h"
#include "Creature.h"
#include "Level.h"


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
	
	activeGun_->shoot(victim, this, activeGun_->countHitsMultipl(accuracyMultipl_));
}

void Sentient::kill() {
	if(activeGun_ != nullptr) level_.addItem(coord_, activeGun_);
	activeGun_ = nullptr;
	level_.killSentient(this);
}

void Sentient::drawCell(sf::RectangleShape &shape) {
	shape.setFillColor(sf::Color(210,100,0));
}
