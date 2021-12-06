#ifndef LAB4_DIALOG_H
#define LAB4_DIALOG_H

#include <iostream>
#include <limits>

#include "Parameters.h"

namespace Dialog{
	template<class T>//шаблон для ввода чисел
	int getNum(T &a, std::istream &s) {
		while(true){
			s >> a;
			if(s.good() == 1)
				break;
			if(std::cin.eof() == 1)
				return -1;
			std::cout << "Error, repeat:";
			s.clear();
			s.ignore(std::numeric_limits<int>::max(), '\n');
		}
		return 1;
	}
	
	class dialog{
	private:
		std::string cells_cfg = CELLS_CFG;
		std::string items_cfg = ITEMS_CFG;
		std::string creatures_cfg = CREATURES_CFG;
		
		std::string msgs[6];
	public:
		dialog();
		
		void menu();
		int getOption();
		void startGame();
		void editMap();
		void changeCellsFile();
		void changeCreaturesFile();
		void changeItemsFile();
	};
}

#endif //LAB4_DIALOG_H
