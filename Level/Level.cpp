#include "Level.h"

Level::Level() {
	cells_ = nullptr;
	activeCreature = nullptr;
	
	for(int i = 0;i < C_OPERATIVES;++i){
		std::string fn = "Op" + std::to_string(i);
		spawnOperative(fn);
	}
	for(int i = 0;i < C_SENTIENTS;++i){
		std::string fn = "Se" + std::to_string(i);
		spawnSentient(fn);
	}
	for(int i = 0;i < C_WILDS;++i){
		std::string fn = "Wi" + std::to_string(i);
		spawnWild(fn);
	}
	for(int i = 0;i < C_FORAGERS;++i){
		std::string fn = "Fo" + std::to_string(i);
		spawnForager(fn);
	}
	
}

void Level::spawnOperative(std::string &fname) {
	std::ifstream fs(fname);
	if (!fs.is_open())
		throw std::exception();//no file
	
	std::string name;
	std::getline(fs, name);
	
	int x, y, force, accuracy;
	float reloadTime;
	fs >> x >> y >> reloadTime >> force >> accuracy;
	Point coord(x, y);
	
	fs.close();
	
	auto *operative = new Operative(name, coord, reloadTime, force, accuracy);
	
	operativeAr_.push_back(operative);
}

void Level::spawnSentient(std::string &fname) {
	std::ifstream fs(fname);
	if (!fs.is_open())
		throw std::exception();//no file
	
	std::string name;
	std::getline(fs, name);
	
	int x, y, accuracy;
	fs >> x >> y >> accuracy;
	Point coord(x, y);
	
	fs.close();
	
	auto *sentient = new Sentient(name, coord, accuracy);
	
	sentientAr_.push_back(sentient);
}

void Level::spawnWild(std::string &fname) {
	std::ifstream fs(fname);
	if (!fs.is_open())
		throw std::exception();//no file
	
	std::string name;
	std::getline(fs, name);
	
	int x, y, damage, accuracy;
	fs >> x >> y >> accuracy >> damage;
	Point coord(x, y);
	
	fs.close();
	
	auto *wild = new Wild(name, coord, accuracy, damage);
	
	wildAr_.push_back(wild);
}

void Level::spawnForager(std::string &fname) {
	std::ifstream fs(fname);
	if(!fs.is_open())
		throw std::exception();//no file
	
	std::string name;
	std::getline(fs, name);
	
	int x, y, force;
	fs >> x >> y >> force;
	Point coord(x, y);
	
	fs.close();
	
	auto *forager = new Forager(name, coord, force);
	
	foragerAr_.push_back(forager);
}

void Level::setCell(int x, int y, Cell &cell) {
	cells_[x][y] = cell;
}

void Level::killOperative(Creature *cr) {
	operativeAr_.erase(std::find(operativeAr_.begin(), operativeAr_.end(), cr));
}
void Level::killSentient(Creature *cr) {
	sentientAr_.erase(std::find(sentientAr_.begin(), sentientAr_.end(), cr));
}
void Level::killWild(Creature *cr) {
	wildAr_.erase(std::find(wildAr_.begin(), wildAr_.end(), cr));
}
void Level::killForager(Creature *cr) {
	foragerAr_.erase(std::find(foragerAr_.begin(), foragerAr_.end(), cr));
}

void Level::dropItem(Point &point, Item *item) {
	itemMap.addItem(point.x, point.y, item);
}
