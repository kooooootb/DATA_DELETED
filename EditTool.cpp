#include "EditTool.h"
#include <iostream>

namespace edittool{
	
	EditTool::EditTool() : window(sf::VideoMode(WINDOWWIDTH, WINDOWHEIGHT), "test") , coord(20,20)
	{
		window.setFramerateLimit(60);
		
		sf::Vector2f cellSize(CELLSIZE, CELLSIZE);
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
		
		cellsHoriz = level.getHorizCells();
		cellsVert = level.getVertCells();
		
		for(int i = 0;i < E_COUNT;++i){
			mesTips.emplace_back("", font, FONTSIZE);
		}
		mesTips[E_CELL].setPosition((float)WINDOWWIDTH * 3 / 4, 0);
		
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
	
	void EditTool::refreshMap(){
		for(int j = 0;j < WINDOWHEIGHT_AMOUNT;++j){
			const std::vector<const std::vector<Item*>*> items = level.getItemMap().getLine(j + coord.y - YOFFSET, coord.x - XOFFSET, coord.x - XOFFSET + WINDOWWIDTH_AMOUNT);
			const std::vector<const std::vector<Creature*>*> creatures = level.getCreatureMap().getLine(j + coord.y - YOFFSET, coord.x - XOFFSET, coord.x - XOFFSET + WINDOWWIDTH_AMOUNT);
			
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
	
	void EditTool::refreshInterface() {
		std::string mes("1 for WALL, 2 for FLOOR, 3 for GLASS, 4 for PARTITION\nR to refresh circle radius\nE to build circle\nF to build line\nT to refresh line length\nV to build row\nY to refresh row length");
		mesTips[E_CELL].setString(mes);
	}
	
	void EditTool::drawMap() {
		for(int i = 0;i < WINDOWWIDTH_AMOUNT;++i)
			for(int j = 0;j < WINDOWHEIGHT_AMOUNT;++j)
				window.draw(wCells[i][j]);
	}
	
	void EditTool::setCell(int x, int y){
		if(x < 0 || y < 0 || x >= cellsHoriz || y >= cellsVert) return;
		
		level.getCells()[x][y].setType(celltype);
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
					int x = sf::Mouse::getPosition(window).x/CELLSIZE - XOFFSET + coord.x, y = sf::Mouse::getPosition(window).y/CELLSIZE - YOFFSET + coord.y;
					if(x < 0 || y < 0 || x >= cellsHoriz || y >= cellsVert) break;
					level.getCells()[x][y].setType(celltype);
					
					refreshMap();
					redrawWindow();
					mousePressed = true;
				}
				if (event.type == sf::Event::MouseButtonReleased){
					mousePressed = false;
				}
				if (event.type == sf::Event::MouseMoved && mousePressed){
					int x = sf::Mouse::getPosition(window).x/CELLSIZE, y = sf::Mouse::getPosition(window).y/CELLSIZE;
					if(x - XOFFSET + coord.x < 0 || y - YOFFSET + coord.y < 0 || x - XOFFSET + coord.x >= cellsHoriz || y - YOFFSET + coord.y >= cellsVert) break;

					level.getCells()[x - XOFFSET + coord.x][y - YOFFSET + coord.y].setType(celltype);

					refreshMap();
					redrawWindow();
				}
				if (event.type == sf::Event::KeyPressed)
				{
					switch(event.key.code){
						case sf::Keyboard::LControl:
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
							celltype = WALL;
							break;
						case sf::Keyboard::Num2:
							celltype = FLOOR;
							break;
						case sf::Keyboard::Num3:
							celltype = GLASS;
							break;
						case sf::Keyboard::Num4:
							celltype = PARTITION;
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
	
	void EditTool::redrawWindow() {
		window.clear();
		drawMap();
		
		
		for(auto it = mesTips.begin();it != mesTips.end();++it){
			window.draw(*it);
		}
		
		window.display();
	}
	
	EditTool::~EditTool() {
		for(int i = 0;i < WINDOWWIDTH_AMOUNT;++i){
			delete [] wCells[i];
		}
		delete [] wCells;
	}
}
