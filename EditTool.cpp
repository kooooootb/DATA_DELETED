#include "EditTool.h"
#include <iostream>

namespace edittool{
	
	EditTool::EditTool() : window(sf::VideoMode(WINDOWWIDTH, WINDOWHEIGHT), "test") , coord(20,20)
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
		mesTips[E_ERROR].setPosition((float)WINDOWWIDTH / 2, 0);
		
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
					sfInput += event.text.unicode;
				}
				if (event.type == sf::Event::KeyPressed){
					if (event.key.code == sf::Keyboard::Enter) {
						bool correct = true;
						try {
							choice = std::stoi(sfInput.toAnsiString());
						} catch (std::invalid_argument &ia) {
							correct = false;
						}
						if (choice < 0 || choice > amount) correct = false;
						if (!correct) {
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
					sfInput += event.text.unicode;
				}
				if (event.type == sf::Event::KeyPressed){
					if (event.key.code == sf::Keyboard::Enter) {
						bool correct = true;
						try {
							choice = std::stof(sfInput.toAnsiString());
						} catch (std::invalid_argument &ia) {
							correct = false;
						}
						if (choice < 0 || choice > amount) correct = false;
						if (!correct) {
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
					sfInput += event.text.unicode;
				}
				if (event.type == sf::Event::KeyPressed){
					if (event.key.code == sf::Keyboard::Enter) {
						str = sfInput.toAnsiString();
						if(str.empty()){
							sfInput = "";
							break;
						}
						flag = true;
					}
				}
			}
			if(flag) break;
		}
		
		return str;
	}
	
	void EditTool::refreshMap(){
		cellsOnScreen.clear();
		creaturesOnScreen.clear();
		itemsOnScreen.clear();
		
		for(int j = 0;j < WINDOWHEIGHT_AMOUNT;++j){
			const std::vector<const std::vector<Item*>*> items = level.getItemMap().getLine(j + coord.y - YOFFSET, coord.x - XOFFSET, coord.x - XOFFSET + WINDOWWIDTH_AMOUNT);
			const std::vector<const std::vector<Creature*>*> creatures = level.getCreatureMap().getLine(j + coord.y - YOFFSET, coord.x - XOFFSET, coord.x - XOFFSET + WINDOWWIDTH_AMOUNT);
			
			for(int i = 0;i < WINDOWWIDTH_AMOUNT;++i){
				Cell *cell;
				ErrorCodes flag = level.getCell(coord.x - XOFFSET + i, coord.y - YOFFSET + j, cell);

				if(flag == OK){
					cell->setDrawPosition((float)i, (float)j);
					
					if(cell->walkAble()) {
						if (items[i] != nullptr) {
							for (auto it = (*items[i]).begin(), end = (*items[i]).end(); it != end; ++it){
								(*it)->setDrawPosition((float)i, (float)j);
								itemsOnScreen.push_back(*it);
							}
						}
						if (creatures[i] != nullptr) {
							for (auto it = (*creatures[i]).begin(), end = (*creatures[i]).end(); it != end; ++it){
								(*it)->setDrawPosition((float)i, (float)j);
								creaturesOnScreen.push_back(*it);
							}
						}
					}
					
					cellsOnScreen.push_back(cell);
				}
			}
		}
	}
	
	void EditTool::refreshInterface() {
		std::string mes("1 for WALL, 2 for FLOOR, 3 for GLASS, 4 for PARTITION, 5 for storage\nR to refresh circle radius\nE to build circle\nF to build line\nT to refresh line length\nV to build row\nY to refresh row length\nO to create Operative\nP to create Sentient\nL to create Wild\nK to create Forager\nJ to clear creatures");
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
					if(drawingCell){
						buildCell();
						mousePressed = true;
					}
					else{
						buildCreature();
					}
					refreshMap();
					redrawWindow();
				}
				if (event.type == sf::Event::MouseButtonReleased){
					mousePressed = false;
				}
				if (event.type == sf::Event::MouseMoved && mousePressed){
					if(drawingCell){
						buildCell();
					}
					else{
						buildCreature();
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
							drawingCell = true;
							celltype = WALL;
							break;
						case sf::Keyboard::Num2:
							drawingCell = true;
							celltype = FLOOR;
							break;
						case sf::Keyboard::Num3:
							drawingCell = true;
							celltype = GLASS;
							break;
						case sf::Keyboard::Num4:
							drawingCell = true;
							celltype = PARTITION;
							break;
						case sf::Keyboard::Num5:
							drawingCell = true;
							celltype = STORAGE;
							break;
						case sf::Keyboard::O:
							drawingCell = false;
							creatType = OPERATIVE;
							break;
						case sf::Keyboard::P:
							drawingCell = false;
							creatType = SENTIENT;
							break;
						case sf::Keyboard::L:
							drawingCell = false;
							creatType = WILD;
							break;
						case sf::Keyboard::K:
							drawingCell = false;
							creatType = FORAGER;
							break;
						case sf::Keyboard::J:
							drawingCell = false;
							creatType = CREATURES_COUNT;
							break;
						case sf::Keyboard::R:
							Rad = getIntFromWindow(100);
							break;
						case sf::Keyboard::T:
							colLength = getIntFromWindow(100);
							break;
						case sf::Keyboard::Y:
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
	
	void EditTool::saveLevel(){
		mesTips[E_ERROR].setString("Input 1 to save changes");
		refreshInterface();
		redrawWindow();
		int saveFlag = getIntFromWindow(INT_MAX);
		if(saveFlag != 1) return;
		
		Cell **cells = level.getCells();
		
		std::ofstream fs(CELLS_CFG, std::ios::trunc);
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
	
	void EditTool::setCell(int x, int y){
		if(x < 0 || y < 0 || x >= cellsHoriz || y >= cellsVert) return;
		
		level.setCell(x, y, celltype);
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
		int healthMax = getInt("input health max and Enter to confirm", INT_MAX);
		int timeMax = getInt("input time max and Enter to confirm", INT_MAX);
		int walkTime = getInt("input walk max and Enter to confirm", INT_MAX);
		int viewRadius = getInt("input view radius and Enter to confirm", (int)sqrt(pow(level.getVertCells(), 2) + pow(level.getHorizCells(), 2)));
		float reloadTime = getFloat("input reload coefficient and Enter to confirm", 100);
		int force = getInt("input force and Enter to confirm", INT_MAX);
		float accuracy = getFloat("input accuracy coefficient and Enter to confirm", 100);
		clearError();
		Point point(x, y);
		
		level.spawnOperator(name, point, healthMax, timeMax, walkTime, viewRadius, reloadTime, force, accuracy);
	}
	
	void EditTool::buildSentient() {
//		std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, float accuracy
		int x = sf::Mouse::getPosition(window).x/CELLSIZE - XOFFSET + coord.x, y = sf::Mouse::getPosition(window).y/CELLSIZE - YOFFSET + coord.y;
		
		std::string name = getString("input name and Enter to confirm");
		int healthMax = getInt("input health max and Enter to confirm", INT_MAX);
		int timeMax = getInt("input time max and Enter to confirm", INT_MAX);
		int walkTime = getInt("input walk max and Enter to confirm", INT_MAX);
		int viewRadius = getInt("input view radius and Enter to confirm", (int)sqrt(pow(level.getVertCells(), 2) + pow(level.getHorizCells(), 2)));
		float accuracy = getFloat("input accuracy coefficient and Enter to confirm", 100);
		clearError();
		Point point(x, y);
		
		level.spawnSentient(name, point, healthMax, timeMax, walkTime, viewRadius, accuracy);
	}
	void EditTool::buildWild() {
//		std::string &name, Point coord, int healthMax, int timeMax, int walkTime, int viewRadius, int accuracy, int damage
		int x = sf::Mouse::getPosition(window).x/CELLSIZE - XOFFSET + coord.x, y = sf::Mouse::getPosition(window).y/CELLSIZE - YOFFSET + coord.y;
		
		std::string name = getString("input name and Enter to confirm");
		int healthMax = getInt("input health max and Enter to confirm", INT_MAX);
		int timeMax = getInt("input time max and Enter to confirm", INT_MAX);
		int walkTime = getInt("input walk max and Enter to confirm", INT_MAX);
		int viewRadius = getInt("input view radius and Enter to confirm", (int)sqrt(pow(level.getVertCells(), 2) + pow(level.getHorizCells(), 2)));
		int accuracy = getInt("input accuracy coefficient and Enter to confirm", 100);
		int damage = getInt("input damage and Enter to confirm", INT_MAX);
		clearError();
		Point point(x, y);
		
		level.spawnWild(name, point, healthMax, timeMax, walkTime, viewRadius, accuracy, damage);
	}
	void EditTool::buildForager() {
//		std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, int force
		int x = sf::Mouse::getPosition(window).x/CELLSIZE - XOFFSET + coord.x, y = sf::Mouse::getPosition(window).y/CELLSIZE - YOFFSET + coord.y;
		
		std::string name = getString("input name and Enter to confirm");
		int healthMax = getInt("input health max and Enter to confirm", INT_MAX);
		int timeMax = getInt("input time max and Enter to confirm", INT_MAX);
		int walkTime = getInt("input walk max and Enter to confirm", INT_MAX);
		int viewRadius = getInt("input view radius and Enter to confirm", (int)sqrt(pow(level.getVertCells(), 2) + pow(level.getHorizCells(), 2)));
		int force = getInt("input force and Enter to confirm", INT_MAX);
		clearError();
		Point point(x, y);
		
		level.spawnForager(name, point, healthMax, timeMax, walkTime, viewRadius, force);
	}
	
	void EditTool::clearCreatures() {
		int x = sf::Mouse::getPosition(window).x/CELLSIZE - XOFFSET + coord.x, y = sf::Mouse::getPosition(window).y/CELLSIZE - YOFFSET + coord.y;
		Point point(x, y);
		const std::vector<Creature*> *creatures = level.getCreatureMap()[point];
		while(creatures != nullptr){
			(*creatures)[0]->kill();
			creatures = level.getCreatureMap()[point];
		}
	}
	
	void EditTool::clearItems() {
		int x = sf::Mouse::getPosition(window).x/CELLSIZE - XOFFSET + coord.x, y = sf::Mouse::getPosition(window).y/CELLSIZE - YOFFSET + coord.y;
		Point point(x, y);
		const std::vector<Creature*> *creatures = level.getCreatureMap()[point];
		while(creatures != nullptr){
			(*creatures)[0]->kill();
			creatures = level.getCreatureMap()[point];
		}
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
