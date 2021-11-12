#include "Level.h"
#include "../Parameters.h"

Level::Level() {
	loadCells(CELLS_CFG);
	activeCreature = nullptr;
	
	loadOperative(OPERS_CFG);
	loadSentient(SENTS_CFG);
	loadWild(WILDS_CFG);
	loadForager(FORAGS_CFG);
}

void Level::loadOperative(const char * fname) {
	std::ifstream fs(fname);
	if (!fs.is_open())
		throw std::exception();//no file
	
	std::string name;
	while(std::getline(fs, name)){
		while(std::all_of(name.begin(), name.end(), isspace)){
			if(std::getline(fs, name).eof()) return;
		}
		int x, y, force, accuracy;
		float reloadTime;
		fs >> x >> y >> reloadTime >> force >> accuracy;
		Point coord(x, y);
		
		auto *operative = new Operative(name, coord, reloadTime, force, accuracy);
		
		operativeAr_.push_back(operative);
		C_OPERATIVES += 1;
	}
	
	fs.close();
}

void Level::loadSentient(const char * fname) {
	std::ifstream fs(fname);
	if (!fs.is_open())
		throw std::exception();//no file
	
	std::string name;
	while(std::getline(fs, name)){
		while(std::all_of(name.begin(), name.end(), isspace)){
			if(std::getline(fs, name).eof()) return;
		}
		int x, y, accuracy;
		fs >> x >> y >> accuracy;
		Point coord(x, y);
		
		auto *sentient = new Sentient(name, coord, accuracy);
		
		sentientAr_.push_back(sentient);
		C_SENTIENTS += 1;
	}
	
	fs.close();
}

void Level::loadWild(const char * fname) {
	std::ifstream fs(fname);
	if (!fs.is_open())
		throw std::exception();//no file
	
	std::string name;
	while(std::getline(fs, name)){
		while(std::all_of(name.begin(), name.end(), isspace)){
			if(std::getline(fs, name).eof()) return;
		}
		int x, y, damage, accuracy;
		fs >> x >> y >> accuracy >> damage;
		Point coord(x, y);
		
		auto *wild = new Wild(name, coord, accuracy, damage);
		
		wildAr_.push_back(wild);
		C_WILDS += 1;
	}
	
	fs.close();
}

void Level::loadForager(const char * fname) {
	std::ifstream fs(fname);
	if (!fs.is_open())
		throw std::exception();//no file
	
	std::string name;
	while(std::getline(fs, name)){
		while(std::all_of(name.begin(), name.end(), isspace)){
			if(std::getline(fs, name).eof()) return;
		}
		int x, y, force;
		fs >> x >> y >> force;
		Point coord(x, y);
		
		auto *forager = new Forager(name, coord, force);
		
		foragerAr_.push_back(forager);
		C_FORAGERS += 1;
	}
	
	fs.close();
}

void Level::setCell(int x, int y, Cell &cell) {
	cells_[x][y] = cell;
}

void Level::killOperative(Creature *cr) {
	operativeAr_.erase(std::find(operativeAr_.begin(), operativeAr_.end(), cr));
	creatureMap.removeItem(cr->getPosition(), cr);
}
void Level::killSentient(Creature *cr) {
	sentientAr_.erase(std::find(sentientAr_.begin(), sentientAr_.end(), cr));
	creatureMap.removeItem(cr->getPosition(), cr);
}
void Level::killWild(Creature *cr) {
	wildAr_.erase(std::find(wildAr_.begin(), wildAr_.end(), cr));
	creatureMap.removeItem(cr->getPosition(), cr);
}
void Level::killForager(Creature *cr) {
	foragerAr_.erase(std::find(foragerAr_.begin(), foragerAr_.end(), cr));
	creatureMap.removeItem(cr->getPosition(), cr);
}

void Level::dropItem(Point &point, Item *item) {
	itemMap.addItem(point, item);
}

void Level::loadCells(const char *fname) {
	cells_ = new Cell*[CELLS_VERT];
	for(int i = 0;i < CELLS_VERT;++i){
		cells_[i] = new Cell[CELLS_HORIZ];
	}
	
	std::ifstream fs(fname);
	if (!fs.is_open())
		throw std::exception();//no file
	
	std::string type;
	while(fs >> type){
		fs >> type;
		
		
	}
	
	fs.close();
}
