#include "Game.h"
#include <iostream>

namespace nodata{
	Game::Game(std::string &cells_cfg, std::string &items_cfg, std::string &creatures_cfg) :
		window(sf::VideoMode(WINDOWWIDTH, WINDOWHEIGHT), "test") ,
		interfaceWindow(sf::Vector2f((float)WINDOWWIDTH / 4, (float)WINDOWHEIGHT / 3)) ,
		invWindow(sf::Vector2f((float)WINDOWWIDTH / 4, (float)WINDOWHEIGHT / 2)) ,
		level(cells_cfg, items_cfg, creatures_cfg, this)
	{
		cr = level.getActiveCreature();
		const Point &coord = cr->getPosition();
		
		window.setFramerateLimit(20);
		
		interfaceWindow.setFillColor(sf::Color(50,50,50,50));
		interfaceWindow.setPosition((float)WINDOWWIDTH * 3 / 4, 0);
		invWindow.setFillColor(sf::Color(50,50,50,50));
		invWindow.setPosition(0, 0);
		
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
		mesTips[T_ENDT].setPosition((float)WINDOWWIDTH * 3 / 4, 2 * FONTSIZE);
		mesTips[T_USE].setPosition((float)WINDOWWIDTH * 3 / 4, 3 * FONTSIZE);
		mesTips[T_THROW].setPosition((float)WINDOWWIDTH * 3 / 4, 4 * FONTSIZE);
		mesTips[T_ERROR].setPosition(0, WINDOWHEIGHT - FONTSIZE);
		mesTips[T_INTMES].setPosition((float)WINDOWWIDTH / 2 - FONTSIZE * 10, 0);
		mesTips[T_WIN].setPosition((float)WINDOWWIDTH * 2 / 5, FONTSIZE * 3);
		mesTips[T_TURN].setPosition((float)WINDOWWIDTH * 2 / 5, FONTSIZE * 3);
		
		mesTips[T_WIN].setScale(3, 3);
		mesTips[T_WIN].setString("");
		
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
		setIntmes("Use keyboard to select element and Enter to confirm or LControl to exit");
		int choice = -1;
		
		sf::String sfInput;
		bool flag = false;
		while(window.isOpen()){
			while (window.pollEvent(event)){
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::TextEntered){
					int keycode = event.key.code;
					if(keycode >= '0' && keycode <= '9') sfInput += event.text.unicode;
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

	void Game::drawCell(int xR, int yR) {
		Cell *cell;
		const Point &coord = cr->getPosition();
		Point pointR(xR, yR);//on the map
		int x = xR - coord.x + XOFFSET, y = yR - coord.y + YOFFSET;//on the screen
		if(x < 0 || y < 0 || x >= WINDOWWIDTH_AMOUNT || y >= WINDOWHEIGHT_AMOUNT){
			return;
		}
		Ptr<Item*> item = level.getItemMap()[pointR];
		Ptr<Creature*> creature = level.getCreatureMap()[pointR];
		
		ErrorCodes flag = level.getCell(pointR, cell);
		if (flag == OK) {
			cell->setDrawPosition((float)x, (float)y);
			
			if(cell->walkAble()) {
				if (item.ptr != nullptr) {
					for (int i = 0;i < item.amount;++i){
						item.ptr[i]->setDrawPosition((float)x, (float)y);
						itemsOnScreen.push_back(item.ptr[i]);
					}
				}
				if (creature.ptr != nullptr) {
					for (int i = 0;i < creature.amount;++i){
						(creature.ptr[i])->setDrawPosition((float)x, (float)y);
						creaturesOnScreen.push_back(creature.ptr[i]);
					}
				}
			}
		}
		delete [] item.ptr;
		delete [] creature.ptr;

		cellsOnScreen.push_back(cell);
	}
	
	void Game::drawRay(const Point &point) {
		const Point &coord = cr->getPosition();
		Point endP(coord + point);
		level.setRay(coord, endP);
//		for (Level::Iterator it = level.begin(), endIt = level.end(); it != endIt; ++it) {
		for (const CellIt &it : level) {
			drawCell(it.getPoint().x, it.getPoint().y);
			if(it.getCell()->getType() == WALL || it.getCell()->getType() == PARTITION){
				break;
			}
		}
	}
	
	void Game::refreshMap(){
		cr = level.getActiveCreature();
		cellsOnScreen.clear();
		creaturesOnScreen.clear();
		itemsOnScreen.clear();
		const Point &coord = cr->getPosition();
		
		cr->setDrawPosition(XOFFSET, YOFFSET);
		creaturesOnScreen.push_back(cr);
		level[coord]->setDrawPosition(XOFFSET, YOFFSET);
		cellsOnScreen.push_back(level[coord]);
		
		Ptr<Item*> item = level.getItemMap()[coord];
		Ptr<Creature*> creature = level.getCreatureMap()[coord];
		
		if (item.ptr != nullptr) {
			for (int i = 0;i < item.amount;++i){
				(item.ptr[i])->setDrawPosition(XOFFSET, YOFFSET);
				itemsOnScreen.push_back(item.ptr[i]);
			}
		}
		if (creature.ptr != nullptr) {
			for (int i = 0;i < creature.amount;++i){
				(creature.ptr[i])->setDrawPosition(XOFFSET, YOFFSET);
				creaturesOnScreen.push_back(creature.ptr[i]);
			}
		}
		delete [] item.ptr;
		delete [] creature.ptr;
		
		std::vector<Point> circle;
		createCircle(circle, cr->getViewRadius());
		for(auto it : circle){
			drawRay(it);
		}
		
	}
	
	void Game::refreshActcreature() {
		std::string mes("Current creature:");
		mes += cr->getName();
		mesTips[T_ACTCREATURE].setString(mes);
	}
	
	void Game::refreshTake() {
		delete [] nearItems.ptr;
		nearItems = level.getItemMap()[cr->getPosition()];
		if(nearItems.ptr == nullptr){
			mesTips[T_TAKE].setString("No items available");
		}else{
			mesTips[T_TAKE].setString("Press F to take items");
		}
	}
	
	void Game::refreshEndt() {
		mesTips[T_ENDT].setString("Press R to end turn");
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
		refreshEndt();
		refreshUse();
		refreshTime();
		refreshThrow();
		refreshHealth();
		refreshAmmo();
		refreshWeight();
	}
	
	void Game::drawMap() {
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
		ErrorCodes status;
		while (window.isOpen())
		{
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::MouseButtonPressed){
					shootInt();
				}
				if (event.type == sf::Event::KeyPressed)
				{
					switch(event.key.code){
						case sf::Keyboard::Escape:
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
						case sf::Keyboard::I:
							window.pollEvent(event);
							useInt();
							break;
						case sf::Keyboard::Q:
							window.pollEvent(event);
							throwInt();
							break;
						case sf::Keyboard::R:
							window.pollEvent(event);
							status = endInt();
							if(status == ERROR){
								window.close();
								return;
							}
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
		if(nearItems.ptr == nullptr) return;
		
		if(nearItems.amount == 1){
			auto *op = dynamic_cast<Operative*>(cr);
			if(op->receiveItem(*nearItems.ptr) == ERROR) {
				showError("Too heavy!");
			}
			else level.getItemMap().removeItem(cr->getPosition(), *nearItems.ptr);
			nearItems.ptr = nullptr;
		}
		else{
			int amount = -1;
			
			for(int i = 0;i < nearItems.amount;++i){
				amount++;
				
				mesTips.emplace_back(std::to_string(amount) + nearItems.ptr[i]->getName(), font, FONTSIZE);
				mesTips.back().setPosition((float)WINDOWWIDTH * 3 / 4, (float)(CHOOSEOFFSET + amount * FONTSIZE));
			}
			redrawWindow();
			int choice;
			if((choice = getIntFromWindow(amount)) == ERROR){
				cleanTips(amount);
				return;
			}
			
			auto *op = dynamic_cast<Operative*>(cr);
			if(op->receiveItem(nearItems.ptr[choice]) == ERROR) {
				showError("Too heavy!");
			}
			else level.getItemMap().removeItem(cr->getPosition(), nearItems.ptr[choice]);
			
			for(int i = 0;i < amount + 1;i++){
				mesTips.pop_back();
			}
		}
	}
	
	void Game::useInt() {
		auto op = dynamic_cast<Operative*>(cr);
		if(op == nullptr) return;
		
		const std::vector<Item*> &items = op->getTable().getVector();
		int amount = -1;
		
		if(op->getActiveGun() != nullptr){
			amount++;
			
			mesTips.emplace_back(std::to_string(amount) + op->getActiveGun()->getName(), font, FONTSIZE);
			mesTips.back().setPosition((float)WINDOWWIDTH * 3 / 4, (float)(CHOOSEOFFSET + amount * FONTSIZE));
		}
		
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
		
		if(op->getActiveGun() != nullptr){
			choice -= 1;
		}
		
		op->useItem(choice);
		
		cleanTips(amount);
	}
	
	void Game::throwInt() {
		auto op = dynamic_cast<Operative*>(cr);
		if(op == nullptr) return;
		
		const std::vector<Item*> &items = op->getTable().getVector();
		int amount = -1;
		
		if(op->getActiveGun() != nullptr){
			amount++;
			
			mesTips.emplace_back(std::to_string(amount) + op->getActiveGun()->getName(), font, FONTSIZE);
			mesTips.back().setPosition((float)WINDOWWIDTH * 3 / 4, (float)(CHOOSEOFFSET + amount * FONTSIZE));
		}
		
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
		
		if(op->getActiveGun() != nullptr){
			choice -= 1;
		}
		
		op->throwItem(choice);
		
		cleanTips(amount);
	}
	
	void Game::shootInt(){
		auto *op = dynamic_cast<Operative*>(cr);
		if(op == nullptr || op->getActiveGun() == nullptr) return;
		Point mousePos(sf::Mouse::getPosition(window).x / CELLSIZE - XOFFSET + cr->getPosition().x, sf::Mouse::getPosition(window).y / CELLSIZE - YOFFSET + cr->getPosition().y);
		
		op->shoot(mousePos);
		
		Point from(XOFFSET * CELLSIZE, YOFFSET * CELLSIZE);
		mousePos.x = (mousePos.x - cr->getPosition().x + XOFFSET) * CELLSIZE;
		mousePos.y = (mousePos.y - cr->getPosition().y + YOFFSET) * CELLSIZE;
		
		setLine(from, mousePos);
		
		refreshMap();
		refreshInterface();
		redrawWindow();
	}
	
	ErrorCodes Game::endInt() {
		mesTips[T_TURN].setString("Enemy's turn");
		srand(time(nullptr));
		
//		sentient's move
		level.setTurn(SENTIENT);
		for(auto it : level.getCurrentTeam()){
			while(it->move(rand()) != ERROR){
				if(level.gameOver()){
					std::cout << "Game over!" << std::endl;
					return ERROR;
				}
				refreshMap();
				redrawWindow();
				while(window.pollEvent(event));
			}
		}
		
//		wilds's move
		level.setTurn(WILD);
		for(auto it : level.getCurrentTeam()){
			while(it->move(rand()) != ERROR){
				if(level.gameOver()){
					std::cout << "Game over!" << std::endl;
					return ERROR;
				}
				refreshMap();
				redrawWindow();
				while(window.pollEvent(event));
			}
		}
		
//		forager's move
		level.setTurn(FORAGER);
		for(auto it : level.getCurrentTeam()){
			while(it->move(rand()) != ERROR){
				if(level.gameOver()){
					std::cout << "Game over!" << std::endl;
					return ERROR;
				}
				refreshMap();
				redrawWindow();
				while(window.pollEvent(event));
			}
		}
		
		level.setTurn(OPERATIVE);
		mesTips[T_TURN].setString("");
		if(level.enemyDied()){
			std::cout << "You won!" << std::endl;
			mesTips[T_WIN].setString("Victory!");
		}
		level.resetTime();
		return OK;
	}
	
	void Game::setLine(const Point &from, const Point &to) {
		clock.restart();
		
		line[0] = sf::Vertex(sf::Vector2f((float)from.x + ((float)CELLSIZE)/2, (float)from.y + ((float)CELLSIZE)/2));
		line[1] = sf::Vertex(sf::Vector2f((float)to.x + ((float)CELLSIZE)/2, (float)to.y + ((float)CELLSIZE)/2));
		
		period = sf::milliseconds(200);
	}
	
	void Game::redrawWindow() {
		window.clear();
		drawMap();
		drawInterface();
		
		if(clock.getElapsedTime() < period){
			window.draw(line, 2, sf::Lines);
		}
		
		window.display();
	}
	
	void Game::cleanTips(int amount) {
		for(int i = 0;i < amount + 1;++i){
			mesTips.pop_back();
		}
	}
	
	Game::~Game() {
		delete [] nearItems.ptr;
	}
}
