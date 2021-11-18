#include "dialog.h"
#include "Game.h"
#include "EditTool.h"

namespace Dialog{
	const char *msgs[] = {"1 - Start game", "2 - Edit map", "0 - Quit"};
	
	void menu() {
		void (*fptr[])() = {nullptr, startGame, editMap};
		int rc = dialog();
		while(rc){
			fptr[rc]();
			rc = dialog();
		}
	}
	
	int dialog() {
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
	
	void startGame(){
		nodata::Game game;
		
		game.start();
	}
	
	void editMap(){
		edittool::EditTool edit;
		
		edit.start();
	}
}