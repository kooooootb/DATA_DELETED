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
	WALL = 1, ///< Стена
	GLASS = 2, ///< Стекло
	PARTITION = 3, ///< Перегородка
	CELLSTYPE_COUNT ///< Количество типов клеток, оставлять последним
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
	TOREMOVE ///< Элемент нужно убрать из массива после завершения
};

/// Направления
enum Direction{
	LEFT, ///< Влево
	UP, ///< Вверх
	RIGHT, ///< Вправо
	DOWN ///< Вниз
};

///Структура точки или вектора
struct Point{
	int x,y; ///< координаты
	
	explicit Point(int X = 0, int Y = 0) : x(X), y(Y) {} ///< Инициализирующий конструктор
	
	bool operator!=(const Point &point) const{
		return (point.x != x || point.y != y);
	}
};

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
