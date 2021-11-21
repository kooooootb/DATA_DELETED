#include "Level.h"
#include "../Parameters.h"
#include <iostream>

namespace nodata{
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
	
	Level::Level(const char *cellsFname) {
		turn = OPERATIVE;
		loadCells(cellsFname);
		activeCreature = nullptr;
		std::string name("default");
		Point coord(1, 1);
		spawnOperator(name, coord, 100, 100, 1, 10, 1, 100, 1);
		
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
			
			if(invalidArgs(x, y) || reloadTime < 0 || accuracy < 0)
				continue;
			
			auto *operative = new Operative(this, name, coord, healthMax, timeMax, walkTime, viewRadius, reloadTime, force, accuracy);
			
			operativeAr_.push_back(operative);
			creatureMap.addItem(coord, operative);
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
			
			if(invalidArgs(x, y))
				continue;
			
			auto *wild = new Wild(this, name, coord, healthMax, timeMax, walkTime, viewRadius, accuracy, damage);
			
			wildAr_.push_back(wild);
			creatureMap.addItem(coord, wild);
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
			
			if(invalidArgs(x, y))
				continue;
			
			auto *forager = new Forager(this, name, coord, healthMax, timeMax, walkTime, viewRadius, force);
			
			foragerAr_.push_back(forager);
			creatureMap.addItem(coord, forager);
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
			int x, y, weight, damage, shootTime, reloadTime, ammoTypeInt, ammoMax, switchTime;
			float accuracy;
			fs >> x >> y >> weight >> damage >> shootTime >> reloadTime >> ammoTypeInt >> ammoMax >> accuracy >> switchTime;
			Point coord(x, y);
			
			if(invalidArgs(x, y) || weight < 0 || damage < 0 || shootTime < 0 || reloadTime < 0 || ammoTypeInt < 0 || ammoTypeInt >= AMMUNITION_COUNT || ammoMax < 0 || accuracy < 0 || accuracy > 100 || switchTime < 0 )
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
			
			if(invalidArgs(x, y) || weight < 0 || healTime < 0 || healAmount < 0)
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
			
			if(invalidArgs(x, y) || weight < 0 || ammoTypeInt < 0 || ammoTypeInt >= AMMUNITION_COUNT || ammoMax < 0 )
				continue;
			
			auto ammoType = static_cast<Ammunition>(ammoTypeInt);
			
			switch(ammoType){
				case LARGE_CALIBER:
					name += "[LARGE CALIBER]";
					break;
				case MEDIUM_CALIBER:
					name += "[MEDIUM CALIBER]";
					break;
				case SMALL_CALIBER:
					name += "[SMALL CALIBER]";
					break;
				default:
					continue;
			}
			
			auto *acont = new AmmoContainer(name, weight, ammoType, ammoMax);
			
			itemMap.addItem(coord, acont);
		}
		
		fs.close();
	}
	
	void Level::setCell(int x, int y, CellType type) {
		cells_[x][y].setType(type);
	}
	
	void Level::setCell(const Point &point, CellType type) {
		cells_[point.x][point.y].setType(type);
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
		
		fs >> vertCells >> horizCells;
		
		cells_ = new Cell*[horizCells];
		
		for(int i = 0;i < horizCells;++i){
			cells_[i] = new Cell[vertCells];
		}
		
		int type;
		int x,y;
		while(fs >> type){
			fs >> x >> y;
			cells_[x][y].setType(static_cast<CellType>(type));
		}
		
		//goes after initialization to avoid retyping by save file
		for(int i = 0;i < horizCells;++i){
			cells_[i][0].setType(WALL);
			cells_[i][vertCells - 1].setType(WALL);
		}
		for(int i = 0;i < vertCells;++i){
			cells_[0][i].setType(WALL);
			cells_[vertCells - 1][i].setType(WALL);
		}
		
		fs.close();
	}
	
	ErrorCodes Level::getCell(int x, int y, Cell &cell) const {
		if(x < 0 || x >= horizCells || y < 0 || y >= vertCells)
			return ERROR;
		cell = cells_[x][y];
		return OK;
	}
	
	ErrorCodes Level::getCell(const Point &point, Cell &cell) const {
		int x = point.x, y = point.y;
		if(x < 0 || x >= horizCells || y < 0 || y >= vertCells)
			return ERROR;
		cell = cells_[x][y];
		return OK;
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
	
	bool Level::invalidArgs(int x, int y) const {
		return (x < 0 || x >= horizCells || y < 0 || y >= vertCells);
	}
	
	const std::vector<Creature *> &Level::getCurrentTeam() const {
		switch(turn){
			case OPERATIVE:
				return operativeAr_;
			case SENTIENT:
				return sentientAr_;
			case WILD:
				return wildAr_;
			case FORAGER:
				return foragerAr_;
			default:
				throw std::exception();
		}
	}
	
	std::vector<Creature *> &Level::getCurrentTeam() {
		switch(turn){
			case OPERATIVE:
				return operativeAr_;
			case SENTIENT:
				return sentientAr_;
			case WILD:
				return wildAr_;
			case FORAGER:
				return foragerAr_;
			default:
				throw std::exception();
		}
	}
	
	void Level::spawnOperator(std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, float reloadTime, int force, float accuracy) {
		if(invalidArgs(coord.x, coord.y) || reloadTime < 0 || accuracy < 0)
			return;
		
		auto *operative = new Operative(this, name, coord, healthMax, timeMax, walkTime, viewRadius, reloadTime, force, accuracy);
		
		operativeAr_.push_back(operative);
		creatureMap.addItem(coord, operative);
	}
	
	void Level::spawnSentient(std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius,float accuracy) {
		if(invalidArgs(coord.x, coord.y) || accuracy < 0)
			return;
		
		auto *sentient = new Sentient(this, name, coord, healthMax, timeMax, walkTime, viewRadius, accuracy);
		
		sentientAr_.push_back(sentient);
		creatureMap.addItem(coord, sentient);
	}
	
	void Level::spawnWild(std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, int accuracy, int damage) {
		if(invalidArgs(coord.x, coord.y))
			return;
		
		auto *wild = new Wild(this, name, coord, healthMax, timeMax, walkTime, viewRadius, accuracy, damage);
		
		wildAr_.push_back(wild);
		creatureMap.addItem(coord, wild);
	}
	
	void Level::spawnForager(std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, int force) {
		if(invalidArgs(coord.x, coord.y))
			return;
		
		auto *forager = new Forager(this, name, coord, healthMax, timeMax, walkTime, viewRadius, force);
		
		foragerAr_.push_back(forager);
		creatureMap.addItem(coord, forager);
	}
	
	CellIt Level::begin() const {
		return CellIt(rayBegin, rayEnd, *this);
	}
	
	CellIt Level::end() const {
		return CellIt(*this, rayBegin, rayEnd);
	}
	
	void Level::setRay(const Point &begin, const Point &end) {
		rayBegin = begin;
		rayEnd = end;
	}
	
	Level::~Level() {
		for(int i = 0;i < horizCells;++i){
			delete [] cells_[i];
		}
		delete [] cells_;
	}
	
	void Level::setTurn(CreatType cr){
		turn = cr;
	}
	
	void Level::resetTime() {
		for(auto it = getCurrentTeam().begin(), endIt = getCurrentTeam().end();it != endIt;++it){
			(*it)->resetTime();
		}
	}
}
