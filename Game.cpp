#include "Game.h"
#include <iostream>

namespace nodata{
	Game::Game() : window(sf::VideoMode(WINDOWWIDTH, WINDOWHEIGHT), "test") , interfaceWindow(sf::Vector2f((float)WINDOWWIDTH / 4, (float)WINDOWHEIGHT / 3)) , invWindow(sf::Vector2f((float)WINDOWWIDTH / 4, (float)WINDOWHEIGHT / 2))
	{
		nearItems = nullptr;
		cr = level.getActiveCreature();
		const Point &coord = cr->getPosition();
		
		window.setFramerateLimit(60);
		
		interfaceWindow.setFillColor(sf::Color(50,50,50,50));
		interfaceWindow.setPosition((float)WINDOWWIDTH * 3 / 4, 0);
		invWindow.setFillColor(sf::Color(50,50,50,50));
		invWindow.setPosition(0, 0);
		
		sf::Vector2f cellSize(CELLSIZE, CELLSIZE);
//	for(int i = 0;i < WINDOWWIDTH_AMOUNT;++i)
//		for(int j = 0;j < WINDOWHEIGHT_AMOUNT;++j){
//			wCells[i][j].setSize(cellSize);
//			wCells[i][j].setPosition((float)i * CELLSIZE, (float)j * CELLSIZE);
//		}
		wCells = new sf::RectangleShape*[WINDOWWIDTH_AMOUNT];
		for(int i = 0;i < WINDOWWIDTH_AMOUNT;++i){
			wCells[i] = new sf::RectangleShape[WINDOWHEIGHT_AMOUNT];
			for(int j = 0;j < WINDOWHEIGHT_AMOUNT;++j){
				wCells[i][j].setSize(cellSize);
				wCells[i][j].setPosition((float)i * CELLSIZE, (float)j * CELLSIZE);
			}
		}
		
		if(!font.loadFromFile("arial.ttf")){
			std::string fname;
			while(!font.loadFromFile(fname)) {
				std::cout << "Can't open font file. Input another font.ttf file name:";
				std::getline(std::cin, fname);
			}
		}
		
		for(int i = 0;i < T_COUNT;++i){
			mesTips.emplace_back("", font, FONTSIZE);
		}
		mesTips[T_ACTCREATURE].setPosition((float)WINDOWWIDTH * 3 / 4, 0);
		mesTips[T_TAKE].setPosition((float)WINDOWWIDTH * 3 / 4, FONTSIZE);
		mesTips[T_SHOOT].setPosition((float)WINDOWWIDTH * 3 / 4, 2 * FONTSIZE);
		mesTips[T_USE].setPosition((float)WINDOWWIDTH * 3 / 4, 3 * FONTSIZE);
		mesTips[T_THROW].setPosition((float)WINDOWWIDTH * 3 / 4, 4 * FONTSIZE);
		mesTips[T_ERROR].setPosition(0, WINDOWHEIGHT - FONTSIZE);
		mesTips[T_INTMES].setPosition((float)WINDOWWIDTH / 2, 0);
		
		for(int i = 0;i < S_COUNT;++i){
			mesStats.emplace_back("", font, FONTSIZE);
		}
		mesStats[S_TIME].setPosition((float)WINDOWWIDTH * 3 / 4, WINDOWHEIGHT - FONTSIZE);
		mesStats[S_HEALTH].setPosition((float)WINDOWWIDTH * 3 / 4, WINDOWHEIGHT - 2 * FONTSIZE);
		mesStats[S_AMMO].setPosition((float)WINDOWWIDTH * 3 / 4, WINDOWHEIGHT - 3 * FONTSIZE);
		mesStats[S_WEIGHT].setPosition((float)WINDOWWIDTH * 3 / 4, WINDOWHEIGHT - 4 * FONTSIZE);
		
		refreshMap();
		refreshInterface();
	}
	
	int Game::getIntFromWindow(int amount){
		setIntmes("Use keyboard to select element or LControl to exit");
		int choice = -1;
		
		sf::String sfInput;
		bool flag = false;
		while(window.isOpen()){
			sf::Event event;
			while (window.pollEvent(event)){
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::TextEntered){
					int keycode = event.key.code;
					if(keycode >= 48 && keycode <= 57) sfInput += event.text.unicode;
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
							showError("Invalid input, typed:" + sfInput.toAnsiString());
							sfInput.clear();
							break;
						}
						
						flag = true;
					}
					if(event.key.code == sf::Keyboard::LControl){
						clearIntmes();
						return ERROR;
					}
				}
			}
			if(flag) break;
		}
		clearIntmes();
		return choice;
	}
	
	void Game::refreshMap(){
		const Point &coord = level.getActiveCreature()->getPosition();
		
		nearItems = level.getItemMap()[coord];
		nearCreatures.clear();
		
		for(int j = 0;j < WINDOWHEIGHT_AMOUNT;++j){
			const std::vector<const std::vector<Item*>*> items = level.getItemMap().getLine(j + coord.y - YOFFSET, coord.x - XOFFSET, coord.x - XOFFSET + WINDOWWIDTH_AMOUNT);
			const std::vector<const std::vector<Creature*>*> creatures = level.getCreatureMap().getLine(j + coord.y - YOFFSET, coord.x - XOFFSET, coord.x - XOFFSET + WINDOWWIDTH_AMOUNT);
			
			for(auto itPtr = creatures.begin(), endPtr = creatures.end();itPtr != endPtr;++itPtr){
				if(*itPtr != nullptr){
					const std::vector<Creature*> &v = **itPtr;
					for(auto it = v.begin(), end = v.end();it != end;++it){
						nearCreatures.push_back(*it);
					}
				}
			}
			
			for(int i = 0;i < WINDOWWIDTH_AMOUNT;++i){
				Cell cell;
				ErrorCodes flag = level.getCell(coord.x - XOFFSET + i, coord.y - YOFFSET + j, cell);
				if(flag == OK){
					switch(cell.getType()){
						case FLOOR:
							wCells[i][j].setFillColor(sf::Color(25,25,25));
							if(items[i] != nullptr){
								for(auto it = (*items[i]).begin(), end = (*items[i]).end();it != end;++it)
									(*it)->drawCell(wCells[i][j]);
							}
							if(creatures[i] != nullptr){
								for(auto it = (*creatures[i]).begin(), end = (*creatures[i]).end();it != end;++it)
									(*it)->drawCell(wCells[i][j]);
							}
							break;
						case WALL:
							wCells[i][j].setFillColor(sf::Color(55,55,55));
							break;
						case GLASS:
							wCells[i][j].setFillColor(sf::Color(46,155,155));
							break;
						case PARTITION:
							wCells[i][j].setFillColor(sf::Color(80,30,0));
							break;
					}
				}
				else if(flag == ERROR) wCells[i][j].setFillColor(sf::Color::Black);
			}
		}
	}
	
	void Game::refreshActcreature() {
		std::string mes("Current creature:");
		mes += level.getActiveCreature()->getName();
		mesTips[T_ACTCREATURE].setString(mes);
	}
	
	void Game::refreshTake() {
		if(nearItems == nullptr){
			mesTips[T_TAKE].setString("No items available");
		}else{
			mesTips[T_TAKE].setString("Press F to take items");
		}
	}
	
	void Game::refreshShoot() {
		if(nearCreatures.empty()){
			mesTips[T_SHOOT].setString("No creatures in the area");
		}else{
			mesTips[T_SHOOT].setString("Press E to shoot");
		}
	}
	
	void Game::refreshUse() {
		mesTips[T_USE].setString("Press I to use item");
		
		auto op = dynamic_cast<Operative*>(cr);
		int i = 0;
		
		mesInv.clear();
		if(op->getActiveGun() != nullptr){
			mesInv.emplace_back("Active weapon:", font, FONTSIZE);
			mesInv.back().setPosition(0, (float)i++ * FONTSIZE);
			mesInv.emplace_back(op->getActiveGun()->getName(), font, FONTSIZE);
			mesInv.back().setPosition(0, (float)i++ * FONTSIZE);
		}
		mesInv.emplace_back("Inventory:", font, FONTSIZE);
		mesInv.back().setPosition(0, (float)i++ * FONTSIZE);
		const std::vector<Item*> &items = op->getTable().getVector();
		for(auto it = items.begin(), end = items.end();it != end;++it){
			mesInv.emplace_back((*it)->getName(), font, FONTSIZE);
			mesInv.back().setPosition(0, (float)i++ * FONTSIZE);
		}
	}
	
	void Game::refreshTime() {
		mesStats[S_TIME].setString("Time: " + std::to_string(cr->getTimeCurrent()));
	}
	
	void Game::refreshHealth() {
		mesStats[S_HEALTH].setString("HP: " + std::to_string(cr->getHealthCurrent()));
	}
	
	void Game::refreshAmmo() {
		auto op = dynamic_cast<Operative*>(cr);
		Gun *gun = op->getActiveGun();
		if(gun == nullptr){
			mesStats[S_AMMO].setString("No weapon");
		}
		else{
			mesStats[S_AMMO].setString("Ammo: " + std::to_string(gun->getAmmoCurrent()) + "/" + std::to_string(gun->getAmmoMax()));
		}
	}
	
	void Game::refreshWeight() {
		auto oper = dynamic_cast<Operative*>(cr);
		if(oper == nullptr) return;
		mesStats[S_WEIGHT].setString("Weight: " + std::to_string(oper->getWeight()) + '/' + std::to_string(oper->getForce()));
	}
	
	void Game::refreshThrow() {
		mesTips[T_THROW].setString("Press Q to throw item");
	}
	
	void Game::refreshInterface(){
		refreshActcreature();
		refreshTake();
		refreshShoot();
		refreshUse();
		refreshTime();
		refreshThrow();
		refreshHealth();
		refreshAmmo();
		refreshWeight();
	}
	
	void Game::drawMap() {
		for(int i = 0;i < WINDOWWIDTH_AMOUNT;++i)
			for(int j = 0;j < WINDOWHEIGHT_AMOUNT;++j)
				window.draw(wCells[i][j]);
	}
	
	void Game::drawInterface() {
		window.draw(interfaceWindow);
		window.draw(invWindow);
		
		for(auto it = mesTips.begin();it != mesTips.end();++it){
			window.draw(*it);
		}
		
		for(auto it = mesInv.begin(); it != mesInv.end(); ++it){
			window.draw(*it);
		}
		
		for(auto it = mesStats.begin(); it != mesStats.end(); ++it){
			window.draw(*it);
		}
	}
	
	void Game::showError(const std::string &mes){
		mesTips[T_ERROR].setString(mes);
		redrawWindow();
		mesTips[T_ERROR].setString("");
	}
	
	void Game::setIntmes(const std::string &mes){
		mesTips[T_INTMES].setString(mes);
		redrawWindow();
	}
	
	void Game::clearIntmes(){
		mesTips[T_INTMES].setString("");
		redrawWindow();
	}
	
	void Game::start() {
		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::MouseButtonPressed){
					auto *op = dynamic_cast<Operative*>(cr);
					if(op == nullptr || op->getActiveGun() == nullptr) break;
					Point point(sf::Mouse::getPosition(window).x/CELLSIZE - XOFFSET + cr->getPosition().x, sf::Mouse::getPosition(window).y/CELLSIZE - YOFFSET + cr->getPosition().y);
					op->shoot(point);
					refreshMap();
					refreshInterface();
					redrawWindow();
				}
				if (event.type == sf::Event::KeyPressed)
				{
					switch(event.key.code){
						case sf::Keyboard::LAlt:
							window.close();
							break;
						case sf::Keyboard::W:
							cr->walk(UP);
							break;
						case sf::Keyboard::D:
							cr->walk(RIGHT);
							break;
						case sf::Keyboard::S:
							cr->walk(DOWN);
							break;
						case sf::Keyboard::A:
							cr->walk(LEFT);
							break;
						case sf::Keyboard::Up:
							cr->walk(UP);
							break;
						case sf::Keyboard::Right:
							cr->walk(RIGHT);
							break;
						case sf::Keyboard::Down:
							cr->walk(DOWN);
							break;
						case sf::Keyboard::Left:
							cr->walk(LEFT);
							break;
						case sf::Keyboard::G:
							window.pollEvent(event);
							switchInt();
							cr = level.getActiveCreature();
							break;
						case sf::Keyboard::F:
							window.pollEvent(event);
							takeInt();
							break;
						case sf::Keyboard::E:
							window.pollEvent(event);
							shootInt();
							break;
						case sf::Keyboard::I:
							window.pollEvent(event);
							useInt();
							break;
						case sf::Keyboard::Q:
							window.pollEvent(event);
							throwInt();
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
	
	void Game::switchInt() {
		const std::vector<Creature *> &opAr = level.getCurrentTeam();
		
		int amount = -1;
		for(auto it = opAr.begin(), end = opAr.end();it != end;++it){
			amount++;
			
			mesTips.emplace_back(std::to_string(amount) + (*it)->getName(), font, FONTSIZE);
			mesTips.back().setPosition((float)WINDOWWIDTH * 3 / 4, (float)(CHOOSEOFFSET + amount * FONTSIZE));
		}
		redrawWindow();
		int choice;
		if((choice = getIntFromWindow(amount)) == ERROR){
			cleanTips(amount);
			return;
		}
		
		level.setActive(choice);
		for(int i = 0;i < amount + 1;++i){
			mesTips.pop_back();
		}
	}
	
	void Game::takeInt() {
		if(nearItems == nullptr) return;
		
		if(nearItems->size() == 1){
			auto *op = dynamic_cast<Operative*>(cr);
			if(op->receiveItem((*nearItems)[0]) == ERROR) {
				showError("Too heavy!");
			}
			else level.getItemMap().removeItem(cr->getPosition(), (*nearItems)[0]);
			nearItems = nullptr;
		}
		else{
			int amount = -1;
			
			for(auto it = nearItems->begin(), end = nearItems->end();it != end;++it){
				amount++;
				
				mesTips.emplace_back(std::to_string(amount) + (*it)->getName(), font, FONTSIZE);
				mesTips.back().setPosition((float)WINDOWWIDTH * 3 / 4, (float)(CHOOSEOFFSET + amount * FONTSIZE));
			}
			redrawWindow();
			int choice;
			if((choice = getIntFromWindow(amount)) == ERROR){
				cleanTips(amount);
				return;
			}
			
			auto *op = dynamic_cast<Operative*>(cr);
			if(op->receiveItem((*nearItems)[choice]) == ERROR) {
				showError("Too heavy!");
			}
			else level.getItemMap().removeItem(cr->getPosition(), (*nearItems)[choice]);
			
			for(int i = 0;i < amount + 1;i++){
				mesTips.pop_back();
			}
		}
	}
	
	void Game::shootInt(){
		if(nearCreatures.empty()) return;
		
		auto op = dynamic_cast<Operative*>(cr);
		if(op == nullptr) return;
		
		int amount = -1;
		for(auto it = nearCreatures.begin(), end = nearCreatures.end();it != end;++it){
			amount++;
			
			mesTips.emplace_back(std::to_string(amount) + (*it)->getName(), font, FONTSIZE);
			mesTips.back().setPosition((float)WINDOWWIDTH * 3 / 4, (float)(CHOOSEOFFSET + amount * FONTSIZE));
		}
		redrawWindow();
		int choice;
		if((choice = getIntFromWindow(amount)) == ERROR){
			cleanTips(amount);
			return;
		}
		
		op->shoot(nearCreatures[choice]);
		
		for(int i = 0;i < amount + 1;i++){
			mesTips.pop_back();
		}
	}
	
	void Game::useInt() {
		auto op = dynamic_cast<Operative*>(cr);
		if(op == nullptr) return;
		
		const std::vector<Item*> &items = op->getTable().getVector();
		int amount = -1;
		for(auto it = items.begin(), end = items.end();it != end;++it){
			amount++;
			
			mesTips.emplace_back(std::to_string(amount) + (*it)->getName(), font, FONTSIZE);
			mesTips.back().setPosition((float)WINDOWWIDTH * 3 / 4, (float)(CHOOSEOFFSET + amount * FONTSIZE));
		}
		redrawWindow();
		int choice;
		if((choice = getIntFromWindow(amount)) == ERROR){
			cleanTips(amount);
			return;
		}
		
		op->useItem(choice);
		
		cleanTips(amount);
	}
	
	void Game::throwInt() {
		auto op = dynamic_cast<Operative*>(cr);
		if(op == nullptr) return;
		
		const std::vector<Item*> &items = op->getTable().getVector();
		int amount = -1;
		for(auto it = items.begin(), end = items.end();it != end;++it){
			amount++;
			
			mesTips.emplace_back(std::to_string(amount) + (*it)->getName(), font, FONTSIZE);
			mesTips.back().setPosition((float)WINDOWWIDTH * 3 / 4, (float)(CHOOSEOFFSET + amount * FONTSIZE));
		}
		redrawWindow();
		int choice;
		if((choice = getIntFromWindow(amount)) == ERROR){
			cleanTips(amount);
			return;
		}
		
		op->throwItem(choice);
		
		cleanTips(amount);
	}
	
	void Game::redrawWindow() {
		window.clear();
		drawMap();
		drawInterface();
		window.display();
	}
	
	void Game::cleanTips(int amount) {
		for(int i = 0;i < amount + 1;++i){
			mesTips.pop_back();
		}
	}
	
	Game::~Game() {
		for(int i = 0;i < WINDOWWIDTH_AMOUNT;++i){
			delete [] wCells[i];
		}
		delete [] wCells;
	}
}
