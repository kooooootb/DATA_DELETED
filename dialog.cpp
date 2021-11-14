#include "dialog.h"
#include "Game.h"

const char *msgs[] = {"1 - Start game", "0 - Quit"};

void menu() {
	void (*fptr[])() = {nullptr, startGame};
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
	Game game;
	
	game.start();
}