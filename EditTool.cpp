#include "EditTool.h"
#include <iostream>

namespace edittool{
	
	EditTool::EditTool(std::string &c_cfg, std::string &i_cfg, std::string &cr_cfg) :
		window(sf::VideoMode(WINDOWWIDTH, WINDOWHEIGHT), "test") ,
		coord(20,20) ,
		level(c_cfg, i_cfg, cr_cfg, nullptr) ,
		creatures_cfg(cr_cfg) ,
		cells_cfg(c_cfg) ,
		items_cfg(i_cfg)
	{
		window.setFramerateLimit(60);
		
		if(!font.loadFromFile("arial.ttf")){
			std::string fname;
			while(!font.loadFromFile(fname)) {
				std::cout << "Can't open font file. Input another font.ttf file name:";
				std::getline(std::cin, fname);
			}
		}
		
		cellsHoriz = level.getHorizCells();
		cellsVert = level.getVertCells();
		
		for(int i = 0;i < E_COUNT;++i){
			mesTips.emplace_back("", font, FONTSIZE);
		}
		mesTips[E_CELL].setPosition((float)WINDOWWIDTH * 3 / 4, 0);
		mesTips[E_ERROR].setPosition((float)WINDOWWIDTH / 2, FONTSIZE);
		mesTips[E_INPUT].setPosition((float)WINDOWWIDTH / 2, 0);
		
		refreshMap();
		refreshInterface();
	}
	
	int EditTool::getIntFromWindow(int amount){
		int choice = -1;
		
		sf::String sfInput;
		bool flag = false;
		while(window.isOpen()){
			sf::Event event;
			while (window.pollEvent(event)){
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::TextEntered){
					if(event.text.unicode == '\b'){
						if(sfInput.getSize() > 0) sfInput.erase(sfInput.getSize() - 1, 1);
					}else{
						sfInput += event.text.unicode;
					}
					mesTips[E_INPUT].setString(sfInput.toAnsiString());
					redrawWindow();
				}
				if (event.type == sf::Event::KeyPressed){
					if (event.key.code == sf::Keyboard::Enter) {
						bool correct = true;
						try {
							choice = std::stoi(sfInput.toAnsiString());
						} catch (std::invalid_argument &ia) {
							correct = false;
						}
						if (choice < 0 || (amount > 0 && choice > amount)) correct = false;
						if (!correct) {
							mesTips[E_INPUT].setString("");
							redrawWindow();
							sfInput = "";
							break;
						}
						
						flag = true;
					}
					if(event.key.code == sf::Keyboard::LControl){
						return ERROR;
					}
				}
			}
			if(flag) break;
		}
		mesTips[E_INPUT].setString("");
		redrawWindow();
		
		return choice;
	}
	
	float EditTool::getFloatFromWindow(float amount){
		float choice = -1;
		
		sf::String sfInput;
		bool flag = false;
		while(window.isOpen()){
			sf::Event event;
			while (window.pollEvent(event)){
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::TextEntered){
					if(event.text.unicode == '\b'){
						if(sfInput.getSize() > 0) sfInput.erase(sfInput.getSize() - 1, 1);
					}else{
						sfInput += event.text.unicode;
					}
					mesTips[E_INPUT].setString(sfInput.toAnsiString());
					redrawWindow();
				}
				if (event.type == sf::Event::KeyPressed){
					if (event.key.code == sf::Keyboard::Enter) {
						bool correct = true;
						try {
							choice = std::stof(sfInput.toAnsiString());
						} catch (std::invalid_argument &ia) {
							correct = false;
						}
						if (choice < 0 || (amount > 0 && choice > amount)) correct = false;
						if (!correct) {
							mesTips[E_INPUT].setString("");
							redrawWindow();
							sfInput = "";
							break;
						}
						
						flag = true;
					}
					if(event.key.code == sf::Keyboard::LControl){
						return ERROR;
					}
				}
			}
			if(flag) break;
		}
		mesTips[E_INPUT].setString("");
		redrawWindow();
		
		return choice;
	}
	
	std::string EditTool::getStringFromWindow(){
		std::string str;
		
		sf::String sfInput;
		bool flag = false;
		while(window.isOpen()){
			sf::Event event;
			while (window.pollEvent(event)){
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::TextEntered){
					if(event.text.unicode == '\b'){
						if(sfInput.getSize() > 0) sfInput.erase(sfInput.getSize() - 1, 1);
					}else{
						sfInput += event.text.unicode;
					}
					mesTips[E_INPUT].setString(sfInput.toAnsiString());
					redrawWindow();
				}
				if (event.type == sf::Event::KeyPressed){
					if (event.key.code == sf::Keyboard::Enter) {
						str = sfInput.toAnsiString();
						if(str.empty()){
							mesTips[E_INPUT].setString("");
							redrawWindow();
							sfInput = "";
							break;
						}
						flag = true;
					}
				}
			}
			if(flag) break;
		}
		mesTips[E_INPUT].setString("");
		redrawWindow();
		
		return str;
	}
	
	void EditTool::refreshMap(){
		cellsOnScreen.clear();
		creaturesOnScreen.clear();
		itemsOnScreen.clear();
		
		for(int j = 0;j < WINDOWHEIGHT_AMOUNT;++j){
			const Ptr<Item*>* items = level.getItemMap().getLine(j + coord.y - YOFFSET, coord.x - XOFFSET, coord.x - XOFFSET + WINDOWWIDTH_AMOUNT);
			const Ptr<Creature*>* creatures = level.getCreatureMap().getLine(j + coord.y - YOFFSET, coord.x - XOFFSET, coord.x - XOFFSET + WINDOWWIDTH_AMOUNT);
			
			for(int i = 0;i < WINDOWWIDTH_AMOUNT;++i){
				Cell *cell;
				ErrorCodes flag = level.getCell(coord.x - XOFFSET + i, coord.y - YOFFSET + j, cell);

				if(flag == OK){
					cell->setDrawPosition((float)i, (float)j);
					
					if(cell->walkAble()) {
						if (items[i].ptr != nullptr) {
							for (int k = 0;k < items[i].amount;++k){
								items[i].ptr[k]->setDrawPosition((float)i, (float)j);
								itemsOnScreen.push_back(items[i].ptr[k]);
							}
						}
						if (creatures[i].ptr != nullptr) {
							for (int k = 0;k < creatures[i].amount;++k){
								creatures[i].ptr[k]->setDrawPosition((float)i, (float)j);
								creaturesOnScreen.push_back(creatures[i].ptr[k]);
							}
						}
					}
					cellsOnScreen.push_back(cell);
				}
			}
			delete [] items;
			delete [] creatures;
		}
	}
	
	void EditTool::refreshInterface() {
		std::string mes("1 for WALL, 2 for FLOOR, 3 for GLASS, 4 for PARTITION, 5 for storage\nR to refresh circle radius\nE to build circle\nF to build line\nT to refresh line length\nV to build row\nY to refresh row length\nO to create Operative\nP to create Sentient\nL to create Wild\nK to create Forager\nJ to clear creatures\nM to build HealthKit\nN to build AmmoContainer\nB to build Gun\nC to clear Items");
		mesTips[E_CELL].setString(mes);
	}
	
	void EditTool::drawMap() {
		for(auto it = cellsOnScreen.begin(), endIt = cellsOnScreen.end();it != endIt;++it){
			(*it)->drawCell(window);
		}
		
		for(auto it = itemsOnScreen.begin(), endIt = itemsOnScreen.end();it != endIt;++it){
			(*it)->drawItem(window);
		}
		
		for(auto it = creaturesOnScreen.begin(), endIt = creaturesOnScreen.end();it != endIt;++it){
			(*it)->drawCreat(window);
		}
	}
	
	void EditTool::buildCircle() {
		int xM = sf::Mouse::getPosition(window).x/CELLSIZE - XOFFSET + coord.x, yM = sf::Mouse::getPosition(window).y/CELLSIZE - YOFFSET + coord.y;
//		if(xM < 0 || yM < 0 || xM >= cellsHoriz || yM >= cellsVert) return;
		
		int r = Rad;
		int d = 3 - 2 * r;
		int x = 0, y = r;
		
		while(y >= x){
			setCell(xM + x,yM + y);
			setCell(xM - x, yM + y);
			setCell(xM + x, yM - y);
			setCell(xM - x, yM - y);
			setCell(xM + y,yM +  x);
			setCell(xM - y,yM +  x);
			setCell(xM + y, yM - x);
			setCell(xM - y, yM - x);
			x++;
			
			if(d >= 0){
				setCell(xM + x,yM + y);
				setCell(xM - x, yM + y);
				setCell(xM + x, yM - y);
				setCell(xM - x, yM - y);
				setCell(xM + y,yM +  x);
				setCell(xM - y,yM +  x);
				setCell(xM + y, yM - x);
				setCell(xM - y, yM - x);
				y--;
				d = d + 4 * (x - y) + 10;
			}else{
				d = d + 4 * x + 6;
			}
		}
	}
	
	void EditTool::buildColumn() {
		int xM = sf::Mouse::getPosition(window).x/CELLSIZE - XOFFSET + coord.x, yM = sf::Mouse::getPosition(window).y/CELLSIZE - YOFFSET + coord.y;
		
		for(int i = 0;i < colLength;++i){
			setCell(xM, yM);
			yM++;
		}
	}
	
	void EditTool::buildRow() {
		int xM = sf::Mouse::getPosition(window).x/CELLSIZE - XOFFSET + coord.x, yM = sf::Mouse::getPosition(window).y/CELLSIZE - YOFFSET + coord.y;
		
		for(int i = 0;i < rowLength;++i){
			setCell(xM, yM);
			xM++;
		}
	}
	
	void EditTool::start() {
		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::MouseButtonPressed){
					switch(building){
						case CREATURE:
							buildCreature();
							break;
						case CELL:
							buildCell();
							mousePressed = true;
							break;
						case ITEM:
							buildItem();
							break;
					}
					refreshMap();
					redrawWindow();
				}
				if (event.type == sf::Event::MouseButtonReleased){
					mousePressed = false;
				}
				if (event.type == sf::Event::MouseMoved && mousePressed){
					switch(building){
						case CREATURE:
							buildCreature();
							break;
						case CELL:
							buildCell();
							break;
						case ITEM:
							buildItem();
							break;
					}
					refreshMap();
					redrawWindow();
				}
				if (event.type == sf::Event::KeyPressed)
				{
					switch(event.key.code){
						case sf::Keyboard::LControl:
							saveLevel();
							window.close();
							break;
						case sf::Keyboard::E:
							buildCircle();
							break;
						case sf::Keyboard::F:
							buildColumn();
							break;
						case sf::Keyboard::V:
							buildRow();
							break;
						case sf::Keyboard::W:
							coord.y-=VIEWSTEP;
							break;
						case sf::Keyboard::A:
							coord.x-=VIEWSTEP;
							break;
						case sf::Keyboard::S:
							coord.y+=VIEWSTEP;
							break;
						case sf::Keyboard::D:
							coord.x+=VIEWSTEP;
							break;
						case sf::Keyboard::Num1:
							building = CELL;
							cellType = WALL;
							break;
						case sf::Keyboard::Num2:
							building = CELL;
							cellType = FLOOR;
							break;
						case sf::Keyboard::Num3:
							building = CELL;
							cellType = GLASS;
							break;
						case sf::Keyboard::Num4:
							building = CELL;
							cellType = PARTITION;
							break;
						case sf::Keyboard::Num5:
							building = CELL;
							cellType = STORAGE;
							break;
						case sf::Keyboard::O:
							building = CREATURE;
							creatType = OPERATIVE;
							break;
						case sf::Keyboard::P:
							building = CREATURE;
							creatType = SENTIENT;
							break;
						case sf::Keyboard::L:
							building = CREATURE;
							creatType = WILD;
							break;
						case sf::Keyboard::K:
							building = CREATURE;
							creatType = FORAGER;
							break;
						case sf::Keyboard::J:
							building = CREATURE;
							creatType = CREATURES_COUNT;
							break;
						case sf::Keyboard::M:
							building = ITEM;
							itemType = HKIT;
							break;
						case sf::Keyboard::N:
							building = ITEM;
							itemType = ACONT;
							break;
						case sf::Keyboard::B:
							building = ITEM;
							itemType = GUN;
							break;
						case sf::Keyboard::C:
							building = ITEM;
							itemType = ITEMS_COUNT;
							break;
						case sf::Keyboard::R:
							window.pollEvent(event);
							Rad = getIntFromWindow(100);
							break;
						case sf::Keyboard::T:
							window.pollEvent(event);
							colLength = getIntFromWindow(100);
							break;
						case sf::Keyboard::Y:
							window.pollEvent(event);
							rowLength = getIntFromWindow(100);
							break;
						default :
							//display "wrong button"
							break;
					}
					refreshMap();
					refreshInterface();
				}
			}
			redrawWindow();
		}
	}
	
	void EditTool::saveCells(){
		Cell **cells = level.getCells();
		
		std::ofstream fs(cells_cfg, std::ios::trunc);
		if (!fs.is_open()) {
			throw std::runtime_error("Can't create files");
		}
		fs << cellsHoriz << ' ' << cellsVert << ' ';
		for(int i = 1;i < cellsHoriz - 1;++i){
			for(int j = 1;j < cellsVert - 1;++j){
				if(cells[i][j].getType() != FLOOR){
					fs << cells[i][j].getType() << ' ' << i << ' ' << j << ' ';
				}
			}
		}
		
		fs.close();
	}
	
	void EditTool::saveCreatures(){
		std::ofstream fs(creatures_cfg, std::ios::trunc);
		if (!fs.is_open()) {
			throw std::runtime_error("Can't create save files for creatures");
		}
		
		for(Creature *it : level.getCreatureMap()){
			it->saveFile(fs);
		}
		
		fs.close();
	}
	
	void EditTool::saveItems(){
		std::ofstream fs(items_cfg, std::ios::trunc);
		if (!fs.is_open()) {
			throw std::runtime_error("Can't create save files for items");
		}
		
		for(Item *it : level.getItemMap()){
			it->saveFile(fs);
		}
		
		fs.close();
	}
	
	void EditTool::saveLevel(){
		int saveFlag = getInt("Input 1 to save changes", -1);
		if(saveFlag != 1) return;
		
		saveCells();
		saveCreatures();
		saveItems();
	}
	
	void EditTool::setCell(int x, int y){
		if(x < 0 || y < 0 || x >= cellsHoriz || y >= cellsVert) return;
		
		level.setCell(x, y, cellType);
	}
	
	void EditTool::buildCell(){
		int x = sf::Mouse::getPosition(window).x/CELLSIZE - XOFFSET + coord.x, y = sf::Mouse::getPosition(window).y/CELLSIZE - YOFFSET + coord.y;
		setCell(x, y);
	}
	
	void EditTool::buildCreature(){
		int x = sf::Mouse::getPosition(window).x/CELLSIZE - XOFFSET + coord.x, y = sf::Mouse::getPosition(window).y/CELLSIZE - YOFFSET + coord.y;
		if(x < 0 || y < 0 || x >= cellsHoriz || y >= cellsVert) return;
		
		switch(creatType){
			case OPERATIVE:
				buildOperative();
				break;
			case SENTIENT:
				buildSentient();
				break;
			case WILD:
				buildWild();
				break;
			case FORAGER:
				buildForager();
				break;
			default:
				clearCreatures();
				break;
		}
	}
	
	void EditTool::buildOperative() {
		//Operative::Operative(Level *level, std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, float reloadTime, int force, float accuracy)
		int x = sf::Mouse::getPosition(window).x/CELLSIZE - XOFFSET + coord.x, y = sf::Mouse::getPosition(window).y/CELLSIZE - YOFFSET + coord.y;
		
		std::string name = getString("input name and Enter to confirm");
		int healthMax = getInt("input health max and Enter to confirm", -1);
		int timeMax = getInt("input time max and Enter to confirm", -1);
		int walkTime = getInt("input walk max and Enter to confirm", -1);
		int viewRadius = getInt("input view radius and Enter to confirm", (int)sqrt(pow(level.getVertCells(), 2) + pow(level.getHorizCells(), 2)));
		float reloadTime = getFloat("input reload coefficient and Enter to confirm", 100);
		int force = getInt("input force and Enter to confirm", -1);
		float accuracy = getFloat("input accuracy coefficient and Enter to confirm", 100);
		clearError();
		Point point(x, y);
		
		level.spawnOperator(name, point, healthMax, timeMax, walkTime, viewRadius, reloadTime, force, accuracy);
	}
	
	void EditTool::buildSentient() {
//		std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, float accuracy
		int x = sf::Mouse::getPosition(window).x/CELLSIZE - XOFFSET + coord.x, y = sf::Mouse::getPosition(window).y/CELLSIZE - YOFFSET + coord.y;
		
		std::string name = getString("input name and Enter to confirm");
		int healthMax = getInt("input health max and Enter to confirm", -1);
		int timeMax = getInt("input time max and Enter to confirm", -1);
		int walkTime = getInt("input walk max and Enter to confirm", -1);
		int viewRadius = getInt("input view radius and Enter to confirm", (int)sqrt(pow(level.getVertCells(), 2) + pow(level.getHorizCells(), 2)));
		float accuracy = getFloat("input accuracy coefficient and Enter to confirm", 100);
		clearError();
		Point point(x, y);
		
		level.spawnSentient(name, point, healthMax, timeMax, walkTime, viewRadius, accuracy);
	}
	void EditTool::buildWild() {
//		std::string &name, Point coord, int healthMax, int timeMax, int walkTime, int viewRadius, int accuracy, int damage, int attackTime
		int x = sf::Mouse::getPosition(window).x/CELLSIZE - XOFFSET + coord.x, y = sf::Mouse::getPosition(window).y/CELLSIZE - YOFFSET + coord.y;
		
		std::string name = getString("input name and Enter to confirm");
		int healthMax = getInt("input health max and Enter to confirm", -1);
		int timeMax = getInt("input time max and Enter to confirm", -1);
		int walkTime = getInt("input walk time and Enter to confirm", -1);
		int viewRadius = getInt("input view radius and Enter to confirm", (int)sqrt(pow(level.getVertCells(), 2) + pow(level.getHorizCells(), 2)));
		int accuracy = getInt("input accuracy coefficient and Enter to confirm", 100);
		int damage = getInt("input damage and Enter to confirm", -1);
		int attackTime = getInt("input attack time and Enter to confirm", -1);
		clearError();
		Point point(x, y);
		
		level.spawnWild(name, point, healthMax, timeMax, walkTime, viewRadius, accuracy, damage, attackTime);
	}
	void EditTool::buildForager() {
//		std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, int force
		int x = sf::Mouse::getPosition(window).x/CELLSIZE - XOFFSET + coord.x, y = sf::Mouse::getPosition(window).y/CELLSIZE - YOFFSET + coord.y;
		
		std::string name = getString("input name and Enter to confirm");
		int healthMax = getInt("input health max and Enter to confirm", -1);
		int timeMax = getInt("input time max and Enter to confirm", -1);
		int walkTime = getInt("input walk max and Enter to confirm", -1);
		int viewRadius = getInt("input view radius and Enter to confirm", (int)sqrt(pow(level.getVertCells(), 2) + pow(level.getHorizCells(), 2)));
		int force = getInt("input force and Enter to confirm", -1);
		clearError();
		Point point(x, y);
		
		level.spawnForager(name, point, healthMax, timeMax, walkTime, viewRadius, force);
	}
	
	void EditTool::buildItem(){
		int x = sf::Mouse::getPosition(window).x/CELLSIZE - XOFFSET + coord.x, y = sf::Mouse::getPosition(window).y/CELLSIZE - YOFFSET + coord.y;
		if(x < 0 || y < 0 || x >= cellsHoriz || y >= cellsVert) return;
		
		switch(itemType){
			case HKIT:
				buildHKit();
				break;
			case ACONT:
				buildACont();
				break;
			case GUN:
				buildGun();
				break;
			default:
				clearItems();
				break;
		}
	}
	
	void EditTool::buildHKit() {
//		std::string &name, int weight, const Point &point, int healAmount, int healTime
		int x = sf::Mouse::getPosition(window).x/CELLSIZE - XOFFSET + coord.x, y = sf::Mouse::getPosition(window).y/CELLSIZE - YOFFSET + coord.y;
		
		std::string name = getString("input name and Enter to confirm");
		int weight = getInt("input weight and Enter to confirm", -1);
		int healAmount = getInt("input heal amount and Enter to confirm", -1);
		int healTime = getInt("input heal time and Enter to confirm", -1);
		clearError();
		Point point(x, y);
		
		level.spawnHKit(name, weight, point, healAmount, healTime);
	}
	void EditTool::buildACont() {
//		std::string &name, int weight, const Point &point, Ammunition ammoType, int ammoMax
		int x = sf::Mouse::getPosition(window).x/CELLSIZE - XOFFSET + coord.x, y = sf::Mouse::getPosition(window).y/CELLSIZE - YOFFSET + coord.y;
		
		std::string name = getString("input name and Enter to confirm");
		int weight = getInt("input weight and Enter to confirm", -1);
		int ammoTypeInt = getInt("input ammo type (1 = LARGE_CALIBER, 2 = MEDIUM_CALIBER, 3 = SMALL_CALIBER)\n and Enter to confirm", -1);
		int ammoMax = getInt("input ammo max and Enter to confirm", -1);
		clearError();
		Point point(x, y);
		ammoTypeInt--;
		if(ammoTypeInt < 0 || ammoTypeInt >= AMMUNITION_COUNT){
			mesTips[E_ERROR].setString("wrong ammo type");
			return;
		}
		
		switch (static_cast<Ammunition>(ammoTypeInt)) {
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
				return;
		}
		
		level.spawnACont(name, weight, point, ammoTypeInt, ammoMax);
	}
	void EditTool::buildGun() {
//		std::string &name, int weight, const Point &point, int damage, int shootTime, int reloadTime, Ammunition ammoType, int ammoMax, float accuracy, int switchTime
		int x = sf::Mouse::getPosition(window).x/CELLSIZE - XOFFSET + coord.x, y = sf::Mouse::getPosition(window).y/CELLSIZE - YOFFSET + coord.y;
		
		std::string name = getString("input name and Enter to confirm");
		int weight = getInt("input weight and Enter to confirm", -1);
		int damage = getInt("input damage and Enter to confirm", -1);
		int shootTime = getInt("input shoot time and Enter to confirm", -1);
		int reloadTime = getInt("input reload time and Enter to confirm", -1);
		int ammoTypeInt = getInt("input ammo type (1 = LARGE_CALIBER, 2 = MEDIUM_CALIBER, 3 = SMALL_CALIBER)\n and Enter to confirm", -1);
		int ammoMax = getInt("input ammo max and Enter to confirm", -1);
		float accuracy = getFloat("input accuracy and Enter to confirm", 100);
		int switchTime = getInt("input switch time and Enter to confirm", -1);
		clearError();
		Point point(x, y);
		ammoTypeInt--;
		if(ammoTypeInt < 0 || ammoTypeInt >= AMMUNITION_COUNT){
			mesTips[E_ERROR].setString("wrong ammo type");
			return;
		}
		
		level.spawnGun(name, weight, point, damage, shootTime, reloadTime, ammoTypeInt, ammoMax, accuracy, switchTime);
	}
	
	void EditTool::clearCreatures() {
		int x = sf::Mouse::getPosition(window).x/CELLSIZE - XOFFSET + coord.x, y = sf::Mouse::getPosition(window).y/CELLSIZE - YOFFSET + coord.y;
		Point point(x, y);
		Ptr<Creature*> creatures = level.getCreatureMap()[point];
		while(creatures.ptr != nullptr){
			creatures.ptr[0]->kill();
			delete [] creatures.ptr;
			creatures = level.getCreatureMap()[point];
		}
		delete [] creatures.ptr;
	}
	
	void EditTool::clearItems() {
		int x = sf::Mouse::getPosition(window).x/CELLSIZE - XOFFSET + coord.x, y = sf::Mouse::getPosition(window).y/CELLSIZE - YOFFSET + coord.y;
		Point point(x, y);
		Ptr<Item*> items = level.getItemMap()[point];
		while(items.ptr != nullptr){
			delete *items.ptr;
			level.getItemMap().removeItem(point, *items.ptr);
			delete [] items.ptr;
			items = level.getItemMap()[point];
		}
		delete [] items.ptr;
	}
	
	void EditTool::redrawWindow() {
		window.clear();
		drawMap();
		
		for(auto it = mesTips.begin();it != mesTips.end();++it){
			window.draw(*it);
		}
		
		window.display();
	}
	
	std::string EditTool::getString(const char *mes) {
		mesTips[E_ERROR].setString(mes);
		refreshInterface();
		redrawWindow();
		return getStringFromWindow();
	}
	
	float EditTool::getFloat(const char *mes, float amount) {
		mesTips[E_ERROR].setString(mes);
		refreshInterface();
		redrawWindow();
		return getFloatFromWindow(amount);
	}
	
	int EditTool::getInt(const char *mes, int amount) {
		mesTips[E_ERROR].setString(mes);
		refreshInterface();
		redrawWindow();
		return getIntFromWindow(amount);
	}
	
	void EditTool::clearError() {
		mesTips[E_ERROR].setString("");
		refreshInterface();
		redrawWindow();
	}
	
	EditTool::~EditTool() {
	}
}
