#include "Game.h"
#include <iostream>

Game::Game() : window(sf::VideoMode(WINDOWWIDTH, WINDOWHEIGHT), "test") ,
interfaceWindow(sf::Vector2f((float)WINDOWWIDTH / 4, (float)WINDOWHEIGHT / 3))
{
	cr = level.getActiveCreature();
	const Point &coord = cr->getPosition();
	
	window.setFramerateLimit(60);
	
	interfaceWindow.setFillColor(sf::Color(50,50,50,50));
	interfaceWindow.setPosition((float)WINDOWWIDTH * 3 / 4, 0);
	
	sf::Vector2f cellSize(CELLSIZE, CELLSIZE);
	for(int i = 0;i < WINDOWWIDTH_AMOUNT;++i)
		for(int j = 0;j < WINDOWHEIGHT_AMOUNT;++j){
			wCells[i][j].setSize(cellSize);
			wCells[i][j].setPosition((float)i * CELLSIZE, (float)j * CELLSIZE);
		}
	
	if(!font.loadFromFile("arial.ttf")){
		std::string fname;
		while(!font.loadFromFile(fname)) {
			std::cout << "Can't open font file. Input another font.ttf file name:";
			std::getline(std::cin, fname);
		}
	}
	textActCreature.setFont(font);
	textActCreature.setCharacterSize(FONTSIZE);
	textActCreature.setPosition((float)WINDOWWIDTH * 3 / 4, 0);
	
	textItems.setFont(font);
	textItems.setCharacterSize(FONTSIZE);
	textItems.setPosition((float)WINDOWWIDTH * 3 / 4, FONTSIZE);
	
	nearItems = level.getItemMap().getItem(coord.x, coord.y);
	
	refreshMap();
	refreshInterface();
}

void Game::refreshMap(){
	const Point &coord = level.getActiveCreature()->getPosition();
	
	for(int j = 0;j < WINDOWHEIGHT_AMOUNT;++j){
		const std::vector<const std::vector<Item*>*> items = level.getItemMap().getLine(j + coord.y - YOFFSET, coord.x - XOFFSET, coord.x - XOFFSET + WINDOWWIDTH_AMOUNT);
		const std::vector<const std::vector<Creature*>*> creatures = level.getCreatureMap().getLine(j + coord.y - YOFFSET, coord.x - XOFFSET, coord.x - XOFFSET + WINDOWWIDTH_AMOUNT);
		
		nearItems = level.getItemMap().getItem(coord.x, coord.y);
		
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

void Game::refreshInterface(){
	std::string mes("Current creature:");
	mes += level.getActiveCreature()->getName();
	
	textActCreature.setString(mes);
	textActCreature.setPosition((float)WINDOWWIDTH * 3 / 4, 0);
	
	if(nearItems == nullptr){
		textItems.setString("No items available");
	}else{
		textItems.setString("Press F to take items");
	}
	textItems.setPosition((float)WINDOWWIDTH * 3 / 4, FONTSIZE);
}

void Game::start() {
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				switch(event.key.code){
					case sf::Keyboard::LControl:
						window.close();
						break;
					case sf::Keyboard::W:
						cr->walk(level, UP);
						break;
					case sf::Keyboard::D:
						cr->walk(level, RIGHT);
						break;
					case sf::Keyboard::S:
						cr->walk(level, DOWN);
						break;
					case sf::Keyboard::A:
						cr->walk(level, LEFT);
						break;
					case sf::Keyboard::Up:
						cr->walk(level, UP);
						break;
					case sf::Keyboard::Right:
						cr->walk(level, RIGHT);
						break;
					case sf::Keyboard::Down:
						cr->walk(level, DOWN);
						break;
					case sf::Keyboard::Left:
						cr->walk(level, LEFT);
						break;
					case sf::Keyboard::G:
						window.pollEvent(event);
						switchCreature();
						cr = level.getActiveCreature();
						break;
					case sf::Keyboard::F:
						window.pollEvent(event);
						takeItem();
						break;
					default :
						//display "wrong button"
						break;
				}
				refreshMap();
				refreshInterface();
			}
		}
		
		window.clear();
		drawMap();
		drawInterface();
		window.display();
	}
}

void Game::drawMap() {
	for(int i = 0;i < WINDOWWIDTH_AMOUNT;++i)
		for(int j = 0;j < WINDOWHEIGHT_AMOUNT;++j)
			window.draw(wCells[i][j]);
}

void Game::switchCreature() {
	const std::vector<Operative*> &opAr = level.getOperativeArray();
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(FONTSIZE);
	text.setPosition((float)WINDOWWIDTH * 3 / 4, CHOOSEOFFSET);
	int amount = -1;
	for(auto it = opAr.begin(), end = opAr.end();it != end;++it){
		amount++;
		text.setString(std::to_string(amount) + (*it)->getName());
		text.move(0,FONTSIZE);
		window.draw(text);
	}
	window.display();
	int choice = getIntFromWindow(amount);
	
	level.setActive(choice);
}

int Game::getIntFromWindow(int amount){
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
						std::cout << "Wrong option" << std::endl;
//					std::cout << "Wrong option, typed:" << sfInput.toAnsiString() << std::endl;
						sfInput = "";
						break;
					}
					
					flag = true;
				}
			}
		}
		if(flag) break;
	}
	
	return choice;
}

void Game::takeItem() {
	if(nearItems == nullptr) return;
	
	if(nearItems->size() == 1){
		auto *op = dynamic_cast<Operative*>(cr);
		op->receiveItem((*nearItems)[0]);
		level.getItemMap().removeItem(cr->getPosition(), (*nearItems)[0]);
		nearItems = nullptr;
	}
	else{
		int amount = -1;
		sf::Text text;
		text.setFont(font);
		text.setCharacterSize(FONTSIZE);
		text.setPosition((float)WINDOWWIDTH * 3 / 4, CHOOSEOFFSET);
		for(auto it = nearItems->begin(), end = nearItems->end();it != end;++it){
			amount++;
			text.setString(std::to_string(amount) + (*it)->getName());
			text.move(0,FONTSIZE);
			window.draw(text);
		}
		window.display();
		
		int choice = getIntFromWindow(amount);
		
		auto *op = dynamic_cast<Operative*>(cr);
		op->receiveItem((*nearItems)[choice]);
		(*nearItems).erase(nearItems->begin() + choice);
	}
	
	
}

void Game::drawInterface() {
	window.draw(interfaceWindow);
	window.draw(textActCreature);
	window.draw(textItems);
}
