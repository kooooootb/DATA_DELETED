#include "../Game.h"

#include <iostream>

namespace nodata{
	Level::Level(std::string &cells_cfg, std::string &items_cfg, std::string &creatures_cfg, Game *game) {//create new game
		curGame = game;
		turn = OPERATIVE;
		
		std::ifstream fs(cells_cfg);
		loadCells(fs);
		fs.close();
		
		loadTextures();
		
		fs.open(items_cfg);
		loadItems(fs);
		fs.close();
		
		fs.open(creatures_cfg);
		loadCreatures(fs);
		fs.close();
		
		if(operativeAr_.empty()){
			activeCreature = nullptr;
		}
		else{
			activeCreature = operativeAr_[0];
		}
	}
	
	Level::Level(std::string &save, Game *game) {//load game from save
		curGame = game;
		turn = OPERATIVE;
		loadTextures();
		
		loadSave(save);
		
		if(operativeAr_.empty()){
			activeCreature = nullptr;
		}
		else{
			activeCreature = operativeAr_[0];
		}
	}
	
	Level::Level(std::string &cellsFname) {
		turn = OPERATIVE;
		
		std::ifstream fs(cellsFname);
		if (!fs.is_open()) {
			throw std::runtime_error("No file named for Cell.cfg found");
		}
		
		loadCells(fs);
		
		fs.close();
		
		std::string name("default");
		Point coord(1, 1);
		spawnOperator(name, coord, 100, 100, 1, 10, 1, 100, 1);
		
		activeCreature = operativeAr_[0];
	}
	
	void Level::loadSave(std::string &save) {
		std::ifstream fs(save);
		if (!fs.is_open()) {
			throw std::runtime_error("No file with given name was found or file can't be opened");
		}
		
		int amount;
		fs >> amount;
		loadCells(fs, amount);
		
		fs >> amount;
		loadItems(fs, amount);
		
		fs >> amount;
		loadCreatures(fs, amount);
	
		fs.close();
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
	
	void Level::loadCreatures(std::ifstream &fs, int amount) {
		if (!fs.is_open()) {//no file
			std::string name;
			Point coord(5, 5);
			auto *operative = new Operative(this, name, coord, 100, 100, 1, 10, 1, 1000, 1);
			operative->setTexture(creatText);
			operativeAr_.push_back(operative);
			creatureMap.addItem(coord, operative);
			return;
		}
		
		skipComms(fs);
		
		std::string name;
		if(amount > -1){
			for(int i = 0;i < amount;++i){
				fs.ignore(std::numeric_limits<int>::max(), '\n');
				std::getline(fs, name);
				int typeInt;
				fs >> typeInt;
				auto type = static_cast<CreatType>(typeInt);
				switch (type) {
					case OPERATIVE:
						loadOperative(fs, name, true);
						break;
					case SENTIENT:
						loadSentient(fs, name, true);
						break;
					case WILD:
						loadWild(fs, name, true);
						break;
					case FORAGER:
						loadForager(fs, name, true);
						break;
					default:
						throw std::runtime_error("Invalid creatures save file");
				}
			}
		}
		else{
			while(std::getline(fs, name)){
				while(std::all_of(name.begin(), name.end(), isspace)){
					if(std::getline(fs, name).eof()) return;
				}
				int typeInt;
				fs >> typeInt;
				auto type = static_cast<CreatType>(typeInt);
				switch (type) {
					case OPERATIVE:
						loadOperative(fs, name);
						break;
					case SENTIENT:
						loadSentient(fs, name);
						break;
					case WILD:
						loadWild(fs, name);
						break;
					case FORAGER:
						loadForager(fs, name);
						break;
					default:
						throw std::runtime_error("Invalid creatures save file");
				}
			}
		}
	}
	
	void Level::loadOperative(std::ifstream &fs, std::string &name, bool loadCurrent) {
		int x, y, force, healthMax, timeMax, walkTime, viewRadius;
		float reloadTime, accuracy;
		fs >> x >> y >> healthMax >> timeMax >> walkTime >> viewRadius >> reloadTime >> force >> accuracy;
		Point coord(x, y);
		
		if (invalidArgs(x, y) || reloadTime < 0 || accuracy < 0)
			return;
		
		auto *operative = new Operative(this, name, coord, healthMax, timeMax, walkTime, viewRadius, reloadTime, force, accuracy);
		operative->setTexture(creatText);
		
		if(loadCurrent){
			int healthCurrent, timeCurrent, isArmed, invSize;
			fs >> healthCurrent >> timeCurrent >> isArmed >> invSize;
			
			operative->setHealthCurrent(healthCurrent);
			operative->setTimeCurrent(timeCurrent);
			
			if(isArmed == 1){
				fs.ignore(std::numeric_limits<int>::max(), '\n');
				std::getline(fs, name);
				fs >> x;//skip type
				
				operative->setActiveGun(loadGunCurrentState(fs, name));
			}
			
			if(invSize > 0){
				for(int i = 0;i < invSize;++i){
					fs.ignore(std::numeric_limits<int>::max(), '\n');
					std::getline(fs, name);
					int typeInt;
					fs >> typeInt;
					auto type = static_cast<ItemType>(typeInt);
					switch (type) {
						case HKIT:
							operative->receiveItem(loadHKitCurrentState(fs, name));
							break;
						case GUN:
							operative->receiveItem(loadGunCurrentState(fs, name));
							break;
						case ACONT:
							operative->receiveItem(loadAContCurrentState(fs, name));
							break;
						default:
							throw std::runtime_error("Invalid items save file");
					}
				}
			}
		}
		
		operativeAr_.push_back(operative);
		creatureMap.addItem(coord, operative);
	}
	
	void Level::loadSentient(std::ifstream &fs, std::string &name, bool loadCurrent) {
		int x, y, healthMax, timeMax, walkTime, viewRadius;
		float accuracy;
		fs >> x >> y >> healthMax >> timeMax >> walkTime >> viewRadius >> accuracy;
		Point coord(x, y);
		
		if (invalidArgs(x, y) || accuracy < 0)
			return;
		
		auto *sentient = new Sentient(this, name, coord, healthMax, timeMax, walkTime, viewRadius, accuracy);
		sentient->setTexture(creatText);
		
		if(loadCurrent){
			int healthCurrent, timeCurrent, isArmed;
			fs >> healthCurrent >> timeCurrent >> isArmed;
			
			sentient->setHealthCurrent(healthCurrent);
			sentient->setTimeCurrent(timeCurrent);
			
			if(isArmed == 1){
				fs.ignore(std::numeric_limits<int>::max(), '\n');
				std::getline(fs, name);
				fs >> x;//skip type
				
				sentient->setActiveGun(loadGunCurrentState(fs, name));
			}
		}
		
		sentientAr_.push_back(sentient);
		creatureMap.addItem(coord, sentient);
	}
	
	void Level::loadWild(std::ifstream &fs, std::string &name, bool loadCurrent) {
		int x, y, damage, accuracy, healthMax, timeMax, walkTime, viewRadius, attackTime;
		fs >> x >> y >> healthMax >> timeMax >> walkTime >> viewRadius >> accuracy >> damage >> attackTime;
		Point coord(x, y);
		
		if (invalidArgs(x, y))
			return;
		
		auto *wild = new Wild(this, name, coord, healthMax, timeMax, walkTime, viewRadius, accuracy, damage, attackTime);
		wild->setTexture(creatText);
		
		if(loadCurrent){
			int healthCurrent, timeCurrent;
			fs >> healthCurrent >> timeCurrent;
			
			wild->setHealthCurrent(healthCurrent);
			wild->setTimeCurrent(timeCurrent);
		}
		
		wildAr_.push_back(wild);
		creatureMap.addItem(coord, wild);
	}
	
	void Level::loadForager(std::ifstream &fs, std::string &name, bool loadCurrent) {
		int x, y, force, healthMax, timeMax, walkTime, viewRadius;
		fs >> x >> y >> healthMax >> timeMax >> walkTime >> viewRadius >> force;
		Point coord(x, y);
		
		if (invalidArgs(x, y))
			return;
		
		auto *forager = new Forager(this, name, coord, healthMax, timeMax, walkTime, viewRadius, force);
		forager->setTexture(creatText);
		
		if(loadCurrent){
			int healthCurrent, timeCurrent, invSize;
			fs >> healthCurrent >> timeCurrent >> invSize;
			
			forager->setHealthCurrent(healthCurrent);
			forager->setTimeCurrent(timeCurrent);
			
			if(invSize > 0){
				for(int i = 0;i < invSize;++i){
					fs.ignore(std::numeric_limits<int>::max(), '\n');
					std::getline(fs, name);
					int typeInt;
					fs >> typeInt;
					auto type = static_cast<ItemType>(typeInt);
					switch (type) {
						case HKIT:
							forager->receiveItem(loadHKitCurrentState(fs, name));
							break;
						case GUN:
							forager->receiveItem(loadGunCurrentState(fs, name));
							break;
						case ACONT:
							forager->receiveItem(loadAContCurrentState(fs, name));
							break;
						default:
							throw std::runtime_error("Invalid items save file");
					}
				}
			}
		}
		
		foragerAr_.push_back(forager);
		creatureMap.addItem(coord, forager);
	}
	
	void Level::loadItems(std::ifstream &fs, int amount) {
		if (!fs.is_open()) {
			throw std::runtime_error("No cfg file for items was found");
		}
		
		skipComms(fs);
		
		std::string name;
		if(amount > -1){
			for(int i = 0;i < amount;++i){
				fs.ignore(std::numeric_limits<int>::max(), '\n');
				std::getline(fs, name);
				int typeInt;
				fs >> typeInt;
				auto type = static_cast<ItemType>(typeInt);
				switch (type) {
					case HKIT:
						loadHKitsCurrentState(fs, name);
						break;
					case GUN:
						loadGunsCurrentState(fs, name);
						break;
					case ACONT:
						loadAContsCurrentState(fs, name);
						break;
					default:
						throw std::runtime_error("Invalid items save file");
				}
			}
		}else{
			while(std::getline(fs, name)){
				while(std::all_of(name.begin(), name.end(), isspace)){
					if(std::getline(fs, name).eof()) return;
				}
				int typeInt;
				fs >> typeInt;
				auto type = static_cast<ItemType>(typeInt);
				switch (type) {
					case HKIT:
						loadHKits(fs, name);
						break;
					case GUN:
						loadGuns(fs, name);
						break;
					case ACONT:
						loadAConts(fs, name);
						break;
					default:
						throw std::runtime_error("Invalid items save file");
				}
			}
		}
	}
	
	void Level::loadGuns(std::ifstream &fs, std::string &name) {
		int x, y, weight, damage, shootTime, reloadTime, ammoTypeInt, ammoMax, switchTime;
		float accuracy;
		fs >> x >> y >> weight >> damage >> shootTime >> reloadTime >> ammoTypeInt >> ammoMax >> accuracy >> switchTime;
		Point coord(x, y);
		
		if (invalidArgs(x, y) || weight < 0 || damage < 0 || shootTime < 0 || reloadTime < 0 || ammoTypeInt < 0 ||
			ammoTypeInt >= AMMUNITION_COUNT || ammoMax < 0 || accuracy < 0 || accuracy > 100 || switchTime < 0)
			return;
		
		auto ammoType = static_cast<Ammunition>(ammoTypeInt);
		
		auto *gun = new Gun(name, weight, coord, damage, shootTime, reloadTime, ammoType, ammoMax, accuracy,
							switchTime);
		gun->setTexture(gunText[ammoType]);
		
		itemMap.addItem(coord, gun);
	}
	
	void Level::loadHKits(std::ifstream &fs, std::string &name) {
		int x, y, weight, healAmount, healTime;
		fs >> x >> y >> weight >> healAmount >> healTime;
		Point coord(x, y);
		
		if (invalidArgs(x, y) || weight < 0 || healTime < 0 || healAmount < 0)
			return;
		
		auto *hkit = new HealthKit(name, weight, coord, healAmount, healTime);
		hkit->setTexture(itemText[HKIT]);
		
		itemMap.addItem(coord, hkit);
	}
	
	void Level::loadAConts(std::ifstream &fs, std::string &name) {
		int x, y, weight, ammoTypeInt, ammoMax;
		fs >> x >> y >> weight >> ammoTypeInt >> ammoMax;
		Point coord(x, y);
		
		if (invalidArgs(x, y) || weight < 0 || ammoTypeInt < 0 || ammoTypeInt >= AMMUNITION_COUNT || ammoMax < 0)
			return;
		
		auto ammoType = static_cast<Ammunition>(ammoTypeInt);
		
		auto *acont = new AmmoContainer(name, weight, coord, ammoType, ammoMax);
		acont->setTexture(itemText[ACONT]);
		
		itemMap.addItem(coord, acont);
	}
	
	void Level::loadGunsCurrentState(std::ifstream &fs, std::string &name) {
		Gun *gun = loadGunCurrentState(fs, name);
		itemMap.addItem(gun->getPosition(), gun);
	}
	
	void Level::loadHKitsCurrentState(std::ifstream &fs, std::string &name) {
		HealthKit *hkit = loadHKitCurrentState(fs, name);
		itemMap.addItem(hkit->getPosition(), hkit);
	}
	
	void Level::loadAContsCurrentState(std::ifstream &fs, std::string &name) {
		AmmoContainer *acont = loadAContCurrentState(fs, name);
		itemMap.addItem(acont->getPosition(), acont);
	}
	
	Gun *Level::loadGunCurrentState(std::ifstream &fs, std::string &name) {
		int x, y, weight, damage, shootTime, reloadTime, ammoTypeInt, ammoMax, switchTime, ammoCurrent;
		float accuracy;
		fs >> x >> y >> weight >> damage >> shootTime >> reloadTime >> ammoTypeInt >> ammoMax >> accuracy >> switchTime >> ammoCurrent;
		Point coord(x, y);
		
		if (invalidArgs(x, y) || weight < 0 || damage < 0 || shootTime < 0 || reloadTime < 0 || ammoTypeInt < 0 ||
			ammoTypeInt >= AMMUNITION_COUNT || ammoMax < 0 || accuracy < 0 || accuracy > 100 || switchTime < 0)
			return nullptr;
		
		auto ammoType = static_cast<Ammunition>(ammoTypeInt);
		
		auto *gun = new Gun(name, weight, coord, damage, shootTime, reloadTime, ammoType, ammoMax, accuracy,
							switchTime);
		gun->setTexture(gunText[ammoType]);
		gun->setAmmoCurrent(ammoCurrent);
		
		return gun;
	}
	
	HealthKit *Level::loadHKitCurrentState(std::ifstream &fs, std::string &name) {
		int x, y, weight, healAmount, healTime;
		fs >> x >> y >> weight >> healAmount >> healTime;
		Point coord(x, y);
		
		if (invalidArgs(x, y) || weight < 0 || healTime < 0 || healAmount < 0)
			return nullptr;
		
		auto *hkit = new HealthKit(name, weight, coord, healAmount, healTime);
		hkit->setTexture(itemText[HKIT]);
		
		return hkit;
	}
	
	AmmoContainer *Level::loadAContCurrentState(std::ifstream &fs, std::string &name) {
		int x, y, weight, ammoTypeInt, ammoMax, ammoCurrent;
		fs >> x >> y >> weight >> ammoTypeInt >> ammoMax >> ammoCurrent;
		Point coord(x, y);
		
		if (invalidArgs(x, y) || weight < 0 || ammoTypeInt < 0 || ammoTypeInt >= AMMUNITION_COUNT || ammoMax < 0)
			return nullptr;
		
		auto ammoType = static_cast<Ammunition>(ammoTypeInt);
		
		auto *acont = new AmmoContainer(name, weight, coord, ammoType, ammoMax);
		acont->setTexture(itemText[ACONT]);
		acont->setAmmoCurrent(ammoCurrent);
		
		return acont;
	}
	
	void Level::setCell(int x, int y, CellType type) {
		cells_[x][y].setType(type, cellText);
	}
	
	void Level::setCell(const Point &point, CellType type) {
		cells_[point.x][point.y].setType(type, cellText);
	}
	
	void Level::killOperative(Creature *cr) {
		operativeAr_.erase(std::find(operativeAr_.begin(), operativeAr_.end(), cr));
		creatureMap.removeItem(cr->getPosition(), cr);
		
		if(cr == activeCreature){
			if(!operativeAr_.empty()){
				activeCreature = operativeAr_[0];
			}
			else activeCreature = nullptr;
		}
		
		delete cr;
	}
	
	void Level::killSentient(Creature *cr) {
		sentientAr_.erase(std::find(sentientAr_.begin(), sentientAr_.end(), cr));
		creatureMap.removeItem(cr->getPosition(), cr);
		
		if(cr == activeCreature){
			if(!sentientAr_.empty()){
				activeCreature = sentientAr_[0];
			}
			else activeCreature = nullptr;
		}
		
		delete cr;
	}
	void Level::killWild(Creature *cr) {
		wildAr_.erase(std::find(wildAr_.begin(), wildAr_.end(), cr));
		creatureMap.removeItem(cr->getPosition(), cr);
		
		if(cr == activeCreature){
			if(!wildAr_.empty()){
				activeCreature = wildAr_[0];
			}
			else activeCreature = nullptr;
		}
		
		delete cr;
	}
	void Level::killForager(Creature *cr) {
		foragerAr_.erase(std::find(foragerAr_.begin(), foragerAr_.end(), cr));
		creatureMap.removeItem(cr->getPosition(), cr);
		
		if(cr == activeCreature){
			if(!foragerAr_.empty()){
				activeCreature = foragerAr_[0];
			}
			else activeCreature = nullptr;
		}
		
		delete cr;
	}
	
	void Level::dropItem(Point &point, Item *item) {
		itemMap.addItem(point, item);
		item->setPosition(point);
	}
	
	void Level::loadCells(std::ifstream &fs, int amount) {
		if (!fs.is_open()) {
			throw std::runtime_error("No cfg file for cells was found");
		}
		fs >> vertCells >> horizCells;
		
		cells_ = new Cell*[horizCells];
		
		for(int i = 0;i < horizCells;++i){
			cells_[i] = new Cell[vertCells];
			for(int j = 0;j < vertCells;++j){
				cells_[i][j].setTexture(cellText[FLOOR]);
			}
		}
		
		int type;
		int x,y;
		if(amount > -1){
			for(int i = 0;i < amount;++i){
				fs >> type >> x >> y;
				cells_[x][y].setType(static_cast<CellType>(type), cellText);
				if(type == STORAGE){
					storages.emplace_back(x, y);
				}
			}
		}else{
			while(fs >> type){
				fs >> x >> y;
				cells_[x][y].setType(static_cast<CellType>(type), cellText);
				if(type == STORAGE){
					storages.emplace_back(x, y);
				}
			}
		}
		
		//goes after initialization to avoid retyping by save file
		for(int i = 0;i < horizCells;++i){
			cells_[i][0].setType(WALL, cellText);
			cells_[i][vertCells - 1].setType(WALL, cellText);
		}
		for(int i = 0;i < vertCells;++i){
			cells_[0][i].setType(WALL, cellText);
			cells_[vertCells - 1][i].setType(WALL, cellText);
		}
	}
	
	ErrorCodes Level::getCell(int x, int y, Cell *&cell) const {
		if(x < 0 || x >= horizCells || y < 0 || y >= vertCells)
			return ERROR;
		cell = &cells_[x][y];
		return OK;
	}
	
	ErrorCodes Level::getCell(const Point &point, Cell *&cell) const {
		int x = point.x, y = point.y;
		if(x < 0 || x >= horizCells || y < 0 || y >= vertCells)
			return ERROR;
		cell = &cells_[x][y];
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
		operative->setTexture(creatText);
		
		operativeAr_.push_back(operative);
		creatureMap.addItem(coord, operative);
	}
	
	void Level::spawnSentient(std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius,float accuracy) {
		if(invalidArgs(coord.x, coord.y) || accuracy < 0)
			return;
		
		auto *sentient = new Sentient(this, name, coord, healthMax, timeMax, walkTime, viewRadius, accuracy);
		sentient->setTexture(creatText);
		
		sentientAr_.push_back(sentient);
		creatureMap.addItem(coord, sentient);
	}
	
	void Level::spawnWild(std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, int accuracy, int damage, int attackTime) {
		if(invalidArgs(coord.x, coord.y))
			return;
		
		auto *wild = new Wild(this, name, coord, healthMax, timeMax, walkTime, viewRadius, accuracy, damage, attackTime);
		wild->setTexture(creatText);
		
		wildAr_.push_back(wild);
		creatureMap.addItem(coord, wild);
	}
	
	void Level::spawnForager(std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, int force) {
		if(invalidArgs(coord.x, coord.y))
			return;
		
		auto *forager = new Forager(this, name, coord, healthMax, timeMax, walkTime, viewRadius, force);
		forager->setTexture(creatText);
		
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
		for(auto it : creatureMap){
			it->resetTime();
		}
	}
	
	bool Level::enemyDied() const{
		return sentientAr_.empty() && wildAr_.empty() && foragerAr_.empty();
	}
	
	void Level::loadTextures() {
		for(int i = 0;i != CELLSTYPE_COUNT;++i){
			if(!cellText[i].loadFromFile(CELLS_TEXTURES[i])){
				throw std::runtime_error("Can't find texture for cell");
			}
		}
		
		for(int i = 0;i != CREATTEXT_COUNT;++i){
			if(!creatText[i].loadFromFile(CREATURES_TEXTURES[i])){
				throw std::runtime_error("Can't find texture for creature");
			}
		}
		
		for(int i = 0;i != ITEMS_COUNT - 1;++i){
			if(!itemText[i].loadFromFile(ITEMS_TEXTURES[i])){
				throw std::runtime_error("Can't find texture for item");
			}
		}
		
		for(int i = 0;i != AMMUNITION_COUNT;++i){
			if(!gunText[i].loadFromFile(GUNS_TEXTURES[i])){
				throw std::runtime_error("Can't find texture for guns");
			}
		}
	}
	
	Cell *Level::operator[](const Point &point){
		return cells_[point.x] + point.y;
	}
	
	const Cell *Level::operator[](const Point &point) const{
		return cells_[point.x] + point.y;
	}
	
	void Level::spawnHKit(std::string &name, int weight, const Point &point, int healAmount, int healTime) {
		auto *hkit = new HealthKit(name, weight, point, healAmount, healTime);
		hkit->setTexture(itemText[HKIT]);
		
		itemMap.addItem(point, hkit);
	}
	
	void Level::spawnACont(std::string &name, int weight, const Point &point, int ammoTypeInt, int ammoMax) {
		auto ammoType = static_cast<Ammunition>(ammoTypeInt);
		
		auto *acont = new AmmoContainer(name, weight, point, ammoType, ammoMax);
		acont->setTexture(itemText[ACONT]);
		
		itemMap.addItem(point, acont);
	}
	
	void Level::spawnGun(std::string &name, int weight, const Point &point, int damage, int shootTime, int reloadTime, int ammoTypeInt, int ammoMax, float accuracy, int switchTime) {
		auto ammoType = static_cast<Ammunition>(ammoTypeInt);
		
		auto *gun = new Gun(name, weight, point, damage, shootTime, reloadTime, ammoType, ammoMax, accuracy, switchTime);
		gun->setTexture(gunText[ammoType]);
		
		itemMap.addItem(point, gun);
	}
	
	bool Level::gameOver() const {
		return operativeAr_.empty();
	}
	
	void Level::drawLine(const Point &from, const Point &to) {
		if(activeCreature == nullptr) return;
		
		Point fromOS((from.x - activeCreature->getPosition().x + XOFFSET) * CELLSIZEX, (from.y - activeCreature->getPosition().y + YOFFSET) * CELLSIZEY);
		Point toOS((to.x - activeCreature->getPosition().x + XOFFSET) * CELLSIZEX, (to.y - activeCreature->getPosition().y + YOFFSET) * CELLSIZEY);
		
		curGame->setLine(fromOS, toOS);
	}
	
	std::stack<Direction> Level::getPath(const Point &begin, const Point &end, bool (*checkFunc)(const Cell&)) const{//using a*
		std::stack<Direction> res;
		
		auto heuristic = [&end](const Point &from) -> double {//shows distance from current to goal point
			return std::abs(from.x - end.x) + std::abs(from.y - end.y);
		};
		
		auto getNeighbors = [&](const Point &p1) -> std::vector<Point> {//get current's 4 neighbors
			std::vector<Point> res;
			
			if(p1.x > 0 && checkFunc(*(*this)[Point(p1.x - 1, p1.y)])) res.emplace_back(p1.x - 1, p1.y);
			if(p1.y > 0 && checkFunc(*(*this)[Point(p1.x, p1.y - 1)])) res.emplace_back(p1.x, p1.y - 1);
			if(p1.x < this->horizCells - 1 && checkFunc(*(*this)[Point(p1.x + 1, p1.y)])) res.emplace_back(p1.x + 1, p1.y);
			if(p1.y < this->vertCells - 1 && checkFunc(*(*this)[Point(p1.x, p1.y + 1)])) res.emplace_back(p1.x, p1.y + 1);
			
			return res;
		};
		
		std::priority_queue<std::pair<double, Point>, std::vector<std::pair<double, Point>>, comparePairGreater<double, Point>> frontier;
		frontier.push(std::make_pair(0, begin));
		
		std::unordered_map<Point, Point, hashP> came_from;
		std::unordered_map<Point, double, hashP> cost_so_far;
		
		came_from[begin] = begin;
		cost_so_far[begin] = 0;
		
		while(!frontier.empty()){
			Point current = frontier.top().second;
			frontier.pop();
			
			if(current == end){
				break;
			}
			
			for(Point next : getNeighbors(current)){
				double new_cost = cost_so_far[current] + 1;//1 = distance between neighbors
				
				if(cost_so_far.find(next) == cost_so_far.end() || new_cost < cost_so_far[next]){
					cost_so_far[next] = new_cost;
					double priority = new_cost + heuristic(next);
					frontier.push(std::make_pair(priority, next));
					came_from[next] = current;
				}
			}
		}
		
		if(came_from.count(end) == 0){//no path?
			return res;
		}
		
//		std::cout << "graph with available built" << std::endl;
		
		Point current = end;
		while(current != begin){
			res.push(came_from[current].getDirection(current));
			current = came_from[current];
		}
		
//		std::cout << "path built" << std::endl;
		
		return res;
	}
	
	std::stack<Direction> Level::makePath(const Point &begin, int randVar) const{//make the longest possible path
		std::stack<Direction> res;
		srand(randVar);
		
		auto heuristic = [&begin](const Point &from) -> double {//shows distance from current to begin point
			return std::abs(from.x - begin.x) + std::abs(from.y - begin.y);
		};
		
		auto getNeighbors = [this](const Point &p1) -> std::vector<Point> {//get current's 4 neighbors
			std::vector<Point> res;
			
			if(p1.x > 0 && (*this)[Point(p1.x - 1, p1.y)]->walkAble()) res.emplace_back(p1.x - 1, p1.y);
			if(p1.y > 0 && (*this)[Point(p1.x, p1.y - 1)]->walkAble()) res.emplace_back(p1.x, p1.y - 1);
			if(p1.x < this->horizCells - 1 && (*this)[Point(p1.x + 1, p1.y)]->walkAble()) res.emplace_back(p1.x + 1, p1.y);
			if(p1.y < this->vertCells - 1 && (*this)[Point(p1.x, p1.y + 1)]->walkAble()) res.emplace_back(p1.x, p1.y + 1);
			
			std::swap(res[rand() % res.size()], res[rand() % res.size()]);
			
			return res;
		};
		
		std::priority_queue<std::pair<double, Point>, std::vector<std::pair<double, Point>>, comparePairGreater<double, Point>> frontier;
		frontier.push(std::make_pair(0, begin));
		
		std::unordered_map<Point, Point, hashP> came_from;
		std::unordered_map<Point, double, hashP> cost_so_far;
		
		came_from[begin] = begin;
		cost_so_far[begin] = 0;
		
		Point end;
		double new_cost;
		
		while(!frontier.empty()){
			end = frontier.top().second;
			frontier.pop();
			
			if(cost_so_far[end] == 100){
				break;
			}
			
			for(Point next : getNeighbors(end)){
				new_cost = cost_so_far[end] + 1;//1 = distance between neighbors
				
				if(cost_so_far.find(next) == cost_so_far.end() || new_cost < cost_so_far[next]){
					cost_so_far[next] = new_cost;
					double priority = new_cost + heuristic(next);
					frontier.push(std::make_pair(priority, next));
					came_from[next] = end;
				}
			}
		}
		
		if(came_from.count(end) == 0){//no path?
			return res;
		}
		
//		std::cout << "graph with available built" << std::endl;
		
		Point current = end;
		while(current != begin){
			res.push(came_from[current].getDirection(current));
			current = came_from[current];
		}
		
//		std::cout << "path built" << std::endl;
		
		return res;
	}
	
	void Level::saveGame(std::string &fname) {
		std::ofstream fs(fname, std::ios::trunc);
		if(!fs.is_open()){
			throw std::runtime_error("Can't create file");
		}
		
		int amount = 0;
		//save cells
		for(int i = 1;i < horizCells - 1;++i){
			for(int j = 1;j < vertCells - 1;++j){
				if(cells_[i][j].getType() != FLOOR){
					amount += 1;
				}
			}
		}
		
		fs << amount << ' ' << horizCells << ' ' << vertCells << ' ';
		for(int i = 1;i < horizCells - 1;++i){
			for(int j = 1;j < vertCells - 1;++j){
				if(cells_[i][j].getType() != FLOOR){
					fs << cells_[i][j].getType() << ' ' << i << ' ' << j << ' ';
				}
			}
		}
		
		//save items
		amount = 0;
		for(Item *it : itemMap){
			amount += 1;
		}
		fs << amount << std::endl;
		for(Item *it : itemMap){
			it->saveCurrentState(fs);
		}
		
		//save creatures
		amount = 0;
		for(Creature *it : creatureMap){
			amount += 1;
		}
		fs << amount << std::endl;
		for(Creature *it : creatureMap){
			it->saveCurrentState(fs);
		}
		
		fs.close();
	}
}
