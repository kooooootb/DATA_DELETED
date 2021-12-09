#ifndef LAB4_DIALOG_H
#define LAB4_DIALOG_H

#include <iostream>
#include <limits>
#include <vector>

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
		std::string cells_cfg;
		std::string items_cfg;
		std::string creatures_cfg;
		
		std::vector<std::string> presets;
		
		std::string msgs[9];
		
		int curPreset = 0;
		
		void refreshDialog();
		void refreshFile();
	public:
		dialog();
		
		void menu();
		int getOption();
		void startGame();
		void editMap();
		void loadGame();
		
		void switchPreset();
		void editPreset();
		void addPreset();
		void deletePreset();
		void listPresets();
		
		void changeCellsFile();
		void changeCreaturesFile();
		void changeItemsFile();
	};
}

#endif //LAB4_DIALOG_H
