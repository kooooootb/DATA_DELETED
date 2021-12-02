#ifndef LAB4_ALLCLASSES_H
#define LAB4_ALLCLASSES_H

#include <string>
#include <vector>
#include <typeinfo>
#include <fstream>
#include <stdexcept>
#include <map>
#include <algorithm>
#include <stack>
#include <cmath>
#include <queue>

#include "SFML/Graphics.hpp"

/*!
* \file
* \brief Заголовочный файл с перечислениями, структурами и прототипами классов.
*
* Данный файл содержит стандартные структуры необходимые программе
*/

/// Набор существ
enum CreatType{
	OPERATIVE = 0, ///< Оперативник
	SENTIENT, ///< Разумное существо
	WILD, ///< Дикое существо
	FORAGER, ///< Одомашненный фуражер
	CREATURES_COUNT ///< Количество существ, оставлять последним
};

/// Типы клeток
enum CellType{
	FLOOR = 0, ///< Пол
	WALL, ///< Стена
	GLASS, ///< Стекло
	PARTITION, ///< Перегородка
	STORAGE, ///< Складская точка
	CELLSTYPE_COUNT ///< Количество типов клеток, оставлять последним
};

/// Набор предметов
enum ItemType{
	HKIT = 0,
	ACONT,
	GUN,
	ITEMS_COUNT
};

/// Типы патронов
enum Ammunition{
	LARGE_CALIBER = 0, ///< Крупный калибр
	MEDIUM_CALIBER, ///< Средний калибр
	SMALL_CALIBER, ///< Малый калибр
	AMMUNITION_COUNT ///< Количество типов, оставлять последним
};

/// Типы ошибок
enum ErrorCodes{
	ERROR = -1, ///< Ошибка
	OK, ///< Успех
	TODELETE, ///< Элемент нужно очистить после завершения
	TOREMOVE, ///< Элемент нужно убрать из массива после завершения
	SUCCESS, ///< Функция завершилась с успехом
	WARNING
};

/// Направления
enum Direction{
	UP, ///< Вверх
	RIGHT, ///< Вправо
	DOWN, ///< Вниз
	LEFT ///< Влево
};

template<class T>
struct Ptr{
	T *ptr;
	int amount;
	
	Ptr() : ptr(nullptr) , amount(0) {}
};

///Структура точки или вектора
struct Point{
	int x,y; ///< координаты
	
	explicit Point(int X = 0, int Y = 0) : x(X), y(Y) {} ///< Инициализирующий конструктор
	
	bool operator!=(const Point &point) const{
		return (point.x != x || point.y != y);
	}
	
	bool operator==(const Point &point) const{
		return (point.x == x && point.y == y);
	}
	
	Point operator+(const Point &point) const{
		return Point(x + point.x, y + point.y);
	}
	
	Direction getDirection(const Point &nextCell) const{
		if(nextCell.x > x) return RIGHT;
		if(nextCell.y > y) return DOWN;
		if(nextCell.x < x) return LEFT;
		return UP;
	}
};

static void createCircle(std::vector<Point> &res, int r) {
	int d = 3 - 2 * r;
	int x = 0, y = r;
	while (y >= x) {
		res.emplace_back(x, y);
		res.emplace_back(-x, y);
		res.emplace_back(x, -y);
		res.emplace_back(-x, -y);
		res.emplace_back(y, x);
		res.emplace_back(-y, x);
		res.emplace_back(y, -x);
		res.emplace_back(-y, -x);
		x++;
		
		if (d >= 0) {
			res.emplace_back(x, y);
			res.emplace_back(-x, y);
			res.emplace_back(x, -y);
			res.emplace_back(-x, -y);
			res.emplace_back(y, x);
			res.emplace_back(-y, x);
			res.emplace_back(y, -x);
			res.emplace_back(-y, -x);
			y--;
			d = d + 4 * (x - y) + 10;
		} else {
			d = d + 4 * x + 6;
		}
	}
}

namespace nodata{
	class Creature;
	class Forager;
	class Operative;
	class Sentient;
	class Wild;
	class AmmoContainer;
	class Gun;
	class HealthKit;
	class Item;
	class Table;
	class Cell;
	class Level;
}

namespace sf{
	class RectangleShape;
}


#endif //LAB4_ALLCLASSES_H
