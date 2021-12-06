#include "dialog.h"
#include "Game.h"
#include "EditTool.h"

namespace Dialog{
	dialog::dialog() {
		msgs[0] = "1 - Start game";
		msgs[1] = "2 - Edit map";
		msgs[2] = "3 - Change cell cfg file, current is:" + cells_cfg;
		msgs[3] = "4 - Change items cfg file, current is:" + items_cfg;
		msgs[4] = "5 - Change creatures cfg file, current is:" + creatures_cfg;
		msgs[5] = "0 - Quit";
	}
	
	void dialog::menu() {
		void (dialog::*fptr[])() = {nullptr, &dialog::startGame, &dialog::editMap, &dialog::changeCellsFile, &dialog::changeItemsFile, &dialog::changeCreaturesFile};
		int rc = getOption();
		while(rc){
			void (dialog::*func)() = fptr[rc];
			(this->*func)();
			rc = getOption();
		}
	}
	
	int dialog::getOption() {
		const int Nmsgs = sizeof(msgs) / sizeof(msgs[0]);
		const char *errmsg = "";
		int rc;
		do {
			std::cout << errmsg;
			errmsg = "Error, repeat:\n";
			for (auto & msg : msgs)
				std::cout << msg << std::endl;
			std::cout << "Make your choice: ";
			if(getNum(rc, std::cin) < 0) throw(std::exception());
		} while (rc < 0 || rc >= Nmsgs);
		return rc;
	}
	
	void dialog::startGame(){
		nodata::Game game(cells_cfg, items_cfg, creatures_cfg);
		
		game.start();
	}
	
	void dialog::editMap(){
		edittool::EditTool edit(cells_cfg, items_cfg, creatures_cfg);
		
		edit.start();
	}
	
	void dialog::changeCellsFile() {
		std::cout << "Input new cells cfg filename:";
		std::cin.ignore(std::numeric_limits<int>::max(), '\n');
		std::getline(std::cin, cells_cfg);
		msgs[2] = "3 - Change cell cfg file, current is:" + cells_cfg;
	}
	
	void dialog::changeItemsFile() {
		std::cout << "Input new items cfg filename:";
		std::cin.ignore(std::numeric_limits<int>::max(), '\n');
		std::getline(std::cin, items_cfg);
		msgs[3] = "4 - Change items cfg file, current is:" + items_cfg;
	}
	
	void dialog::changeCreaturesFile() {
		std::cout << "Input new creatures cfg filename:";
		std::cin.ignore(std::numeric_limits<int>::max(), '\n');
		std::getline(std::cin, creatures_cfg);
		msgs[4] = "5 - Change creatures cfg file, current is:" + creatures_cfg;
	}
}