#include "Level.h"
#include "../Parameters.h"
#include <iostream>

Level::Level() {
	turn = OPERATIVE;
	loadCells(CELLS_CFG);
	activeCreature = nullptr;
	
	loadOperative(OPERS_CFG);
	loadSentient(SENTS_CFG);
	loadWild(WILDS_CFG);
	loadForager(FORAGS_CFG);

	loadHKits(HKITS_CFG);
	loadAConts(ACONTS_CFG);
	loadGuns(GUNS_CFG);
	
	activeCreature = operativeAr_[0];
}

void Level::skipComms(std::ifstream &fs){
	while(!fs.eof()){
		char ch = (char)fs.peek();
		if(ch == '#') fs.ignore(std::numeric_limits<int>::max(), '\n');
		else{
			break;
		}
	}
}

void Level::loadOperative(const char * fname) {
	std::ifstream fs(fname);
	if (!fs.is_open()) {
		return;//no file
	}
	
	skipComms(fs);
	
	std::string name;
	while(std::getline(fs, name)){
		while(std::all_of(name.begin(), name.end(), isspace)){
			if(std::getline(fs, name).eof()) return;
		}
		//std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, float reloadTime, int force, float accuracy
		int x, y, force, healthMax, timeMax, walkTime, viewRadius;
		float reloadTime, accuracy;
		fs >> x >> y >> healthMax >> timeMax >> walkTime >> viewRadius >> reloadTime >> force >> accuracy;
		Point coord(x, y);
		
		if(invalidArgs(x, y, force) || reloadTime < 0 || accuracy < 0)
			continue;
		
		auto *operative = new Operative(this, name, coord, healthMax, timeMax, walkTime, viewRadius, reloadTime, force, accuracy);
		
		operativeAr_.push_back(operative);
		creatureMap.addItem(coord, operative);
		C_OPERATIVES += 1;
	}
	
	fs.close();
}

void Level::loadSentient(const char * fname) {
	std::ifstream fs(fname);
	if (!fs.is_open())
		return;//no file
	
	skipComms(fs);
	
	std::string name;
	while(std::getline(fs, name)){
		while(std::all_of(name.begin(), name.end(), isspace)){
			if(std::getline(fs, name).eof()) return;
		}
		//std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, float accuracy
		int x, y, healthMax, timeMax, walkTime, viewRadius;
		float accuracy;
		fs >> x >> y >> healthMax >> timeMax >> walkTime >> viewRadius >> accuracy;
		Point coord(x, y);
		
		if(invalidArgs(x, y) || accuracy < 0)
			continue;
		
		auto *sentient = new Sentient(this, name, coord, healthMax, timeMax, walkTime, viewRadius, accuracy);
		
		sentientAr_.push_back(sentient);
		creatureMap.addItem(coord, sentient);
		C_SENTIENTS += 1;
	}
	
	fs.close();
}

void Level::loadWild(const char * fname) {
	std::ifstream fs(fname);
	if (!fs.is_open())
		return;//no file
	
	skipComms(fs);
	
	std::string name;
	while(std::getline(fs, name)){
		while(std::all_of(name.begin(), name.end(), isspace)){
			if(std::getline(fs, name).eof()) return;
		}
		//std::string &name, Point coord, int healthMax, int timeMax, int walkTime, int viewRadius, int accuracy, int damage
		int x, y, damage, accuracy, healthMax, timeMax, walkTime, viewRadius;
		fs >> x >> y >> healthMax >> timeMax >> walkTime >> viewRadius >> accuracy >> damage;
		Point coord(x, y);
		
		if(invalidArgs(x, y, accuracy, damage))
			continue;
		
		auto *wild = new Wild(this, name, coord, healthMax, timeMax, walkTime, viewRadius, accuracy, damage);
		
		wildAr_.push_back(wild);
		creatureMap.addItem(coord, wild);
		C_WILDS += 1;
	}
	
	fs.close();
}

void Level::loadForager(const char * fname) {
	std::ifstream fs(fname);
	if (!fs.is_open())
		return;//no file
	
	skipComms(fs);
	
	std::string name;
	while(std::getline(fs, name)){
		while(std::all_of(name.begin(), name.end(), isspace)){
			if(std::getline(fs, name).eof()) return;
		}
		//std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, int force
		int x, y, force, healthMax, timeMax, walkTime, viewRadius;
		fs >> x >> y >> healthMax >> timeMax >> walkTime >> viewRadius >> force;
		Point coord(x, y);
		
		if(invalidArgs(x, y, force))
			continue;
		
		auto *forager = new Forager(this, name, coord, healthMax, timeMax, walkTime, viewRadius, force);
		
		foragerAr_.push_back(forager);
		creatureMap.addItem(coord, forager);
		C_FORAGERS += 1;
	}
	
	fs.close();
}

void Level::loadGuns(const char *fname) {
	std::ifstream fs(fname);
	if (!fs.is_open())
		return;//no file
	
	skipComms(fs);
	
	std::string name;
	while(std::getline(fs, name)){
		while(std::all_of(name.begin(), name.end(), isspace)){
			if(std::getline(fs, name).eof()) return;
		}
		int x, y, weight, damage, shootTime, reloadTime, ammoTypeInt, ammoMax, accuracy, switchTime;
		fs >> x >> y >> weight >> damage >> shootTime >> reloadTime >> ammoTypeInt >> ammoMax >> accuracy >> switchTime;
		Point coord(x, y);
		
		if(x < 0 || x >= CELLS_HORIZ || y < 0 || y >= CELLS_VERT || weight < 0 || damage < 0 || shootTime < 0 || reloadTime < 0 ||
			ammoTypeInt < 0 || ammoTypeInt >= AMMUNITION_COUNT || ammoMax < 0 || accuracy < 0 || accuracy > 100 || switchTime < 0 )
			continue;
		
		auto ammoType = static_cast<Ammunition>(ammoTypeInt);
		
		auto *gun = new Gun(name, weight, damage, shootTime, reloadTime, ammoType, ammoMax, accuracy, switchTime);
		
		itemMap.addItem(coord, gun);
	}
	
	fs.close();
}

void Level::loadHKits(const char *fname) {
	std::ifstream fs(fname);
	if (!fs.is_open())
		return;//no file
	
	skipComms(fs);
	
	std::string name;
	while(std::getline(fs, name)){
		while(std::all_of(name.begin(), name.end(), isspace)){
			if(std::getline(fs, name).eof()) return;
		}
		int x, y, weight, healAmount, healTime;
		fs >> x >> y >> weight >> healAmount >> healTime;
		Point coord(x, y);
		
		if(x < 0 || x >= CELLS_HORIZ || y < 0 || y >= CELLS_VERT || weight < 0 || healTime < 0 || healAmount < 0)
			continue;
		
		auto *hkit = new HealthKit(name, weight, healAmount, healTime);
		
		itemMap.addItem(coord, hkit);
	}
	
	fs.close();
}

void Level::loadAConts(const char *fname) {
	std::ifstream fs(fname);
	if (!fs.is_open())
		return;//no file
	
	skipComms(fs);
	
	std::string name;
	while(std::getline(fs, name)){
		while(std::all_of(name.begin(), name.end(), isspace)){
			if(std::getline(fs, name).eof()) return;
		}
		int x, y, weight, ammoTypeInt, ammoMax;
		fs >> x >> y >> weight >> ammoTypeInt >> ammoMax;
		Point coord(x, y);
		
		if(x < 0 || x >= CELLS_HORIZ || y < 0 || y >= CELLS_VERT || weight < 0 ||
			ammoTypeInt < 0 || ammoTypeInt >= AMMUNITION_COUNT || ammoMax < 0 )
			continue;
		
		auto ammoType = static_cast<Ammunition>(ammoTypeInt);
		
		auto *acont = new AmmoContainer(name, weight, ammoType, ammoMax);
		
		itemMap.addItem(coord, acont);
	}
	
	fs.close();
}

void Level::setCell(int x, int y, CellType type) {
	cells_[x][y].setType(type);
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
	std::ifstream fs(fname);
	if (!fs.is_open()) {
		throw std::runtime_error("No file named for Cell.cfg found");
	}
	
	fs >> CELLS_VERT >> CELLS_HORIZ;
	
	cells_.resize(CELLS_HORIZ);
	for(auto it = cells_.begin();it != cells_.end();++it){
		(*it).resize(CELLS_VERT);
	}
	
	int type;
	int x,y;
	while(fs >> type){
		fs >> x >> y;
		cells_[x][y].setType(static_cast<CellType>(type));
	}
	
	for(auto it = cells_[0].begin(), end = cells_[0].end();it != end;++it){
		(*it).setType(WALL);
	}
	for(auto it = cells_.back().begin(), end = cells_.back().end();it != end;++it){
		(*it).setType(WALL);
	}
	for(auto it = cells_.begin(), end = cells_.end();it != end;++it){
		(*it)[0].setType(WALL);
		(*it).back().setType(WALL);
	}
	
	fs.close();
}

ErrorCodes Level::getCell(int x, int y, Cell &cell) const {
	if(x < 0 || x >= CELLS_HORIZ || y < 0 || y >= CELLS_VERT)
		return ERROR;
	cell = cells_[x][y];
	return OK;
}

void Level::addItem(Point &point, Item *item) {
	itemMap.addItem(point, item);
}

void Level::moveCreature(int x, int y, Creature *creature, Direction direction) {
	creatureMap.removeItem(x, y, creature);
	switch (direction) {
		case UP:
			creatureMap.addItem(x, y - 1, creature);
			break;
		case RIGHT:
			creatureMap.addItem(x + 1, y, creature);
			break;
		case DOWN:
			creatureMap.addItem(x, y + 1, creature);
			break;
		case LEFT:
			creatureMap.addItem(x - 1, y, creature);
			break;
	}
}

void Level::setActive(int i) {
	switch(turn){
		case OPERATIVE:
			activeCreature = operativeAr_[i];
			break;
		case SENTIENT:
			activeCreature = sentientAr_[i];
			break;
		case WILD:
			activeCreature = wildAr_[i];
			break;
		case FORAGER:
			activeCreature = foragerAr_[i];
			break;
	}
	
}

bool Level::invalidArgs(int x, int y, int int1, int int2) const {
	return (x < 0 || x >= CELLS_VERT || y < 0 || y >= CELLS_HORIZ || int1 < 0 || int2 < 0);
}
