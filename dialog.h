#ifndef LAB4_DIALOG_H
#define LAB4_DIALOG_H

#include <iostream>
#include <limits>

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
	
	void menu();
	int dialog();
	void startGame();
	void editMap();
}

#endif //LAB4_DIALOG_H
