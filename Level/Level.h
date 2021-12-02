#ifndef LAB4_LEVEL_H
#define LAB4_LEVEL_H

#include "../AllClasses.h"

#include "Item.h"
#include "Creature.h"
#include "Cell.h"
#include "CellIt.h"
#include "Map.h"

namespace nodata{
	/*!
	 * \file
	 * \brief Заголовочный файл с описанием класса Level
	 *
	 * Данный файл содержит класс для описание уровня, хранения существ и предметов, их загрузки из файлов
	 */

	/*! \defgroup level Описатели уровня
	 *	\brief Данный модуль предназначен для описания уровня
	 *	@{
	 */

	/*!
	 * \brief Класс для описания уровня
	 *
	 * Класс для описания уровня.
	 * Указатели на существа команд хранятся в соответствуюших векторах.
	 * Указатели на существа также хранятся в карте существ для их быстрого обнаружения.
	 * Указатели на предметы хранятся в карте предметов.
	*/
	class Level {
	private:
		std::vector<Creature *> operativeAr_; ///< Вектор содержащий всех оперативников на уровне
		std::vector<Creature *> wildAr_; ///< Вектор содержащий всех диких существ на уровне
		std::vector<Creature *> sentientAr_; ///< Вектор содержащий всех разумных существ на уровне
		std::vector<Creature *> foragerAr_; ///< Вектор содержащий всех фуражиров на уровне
		Cell **cells_; ///< Двумерный массив клеток, составляющих пространство, по которым передвигаются существа
		Creature *activeCreature; ///< Текущее активное существо
		Map<Item*> itemMap; ///< Карта выпавших предметов
		Map<Creature*> creatureMap; ///< Карта существ
		CreatType turn; ///< Определяет команду, совершающую текущих ход
		
		int horizCells, vertCells;
		
		friend class CellIt;
		Point rayBegin, rayEnd;
		
		sf::Texture cellText[CELLSTYPE_COUNT], creatText[CREATURES_COUNT], itemText[ITEMS_COUNT];
		
		/*!
		 * Проверяет корректность введенных координат
		 * @return true если коодинаты корректны, false в другом случае
		 */
		bool invalidArgs(int x, int y) const;
		/*!
		 * Передвигает курсор в файловом потоке, пропуская строки-комментарии(начинающиеся на '#')
		 * @param fs файловый поток
		 */
		static void skipComms(std::ifstream &fs);
	public:
		/*!
		 * Конструктор класса Level загружает клетки и существа из стандартных файлов, устанавливает активное существо и дает ход команде оперативников.
		 * Типы клеток загружаются из файла с названием CELLS_CFG.
		 * Оперативники загружаются из файла с названием OPERS_CFG.
		 * Разумные существа загружаются из файла с названием SENTS_CFG.
		 * Дикие существа загружаются из файла с названием WILDS_CFG.
		 * Фуражеры загружаются из файла с названием FORAGS_CFG.
		 * Все названия файлов опеределены в файле Parameters.h
		 */
		Level();
		/*!
		 * Конструктор класса Level загружает клетки из данного файла, создает оперативника по умолчанию, устанавливает активное существо и дает ход команде оперативников.
		 * @param cellsFname Название файла для загрузки клеток
		 */
		explicit Level(const char *cellsFname);
		
		~Level();
		
		typedef CellIt iterator;
		iterator begin() const;
		iterator end() const;
		
		void setRay(const Point &begin, const Point &end);
		
		/*!
		 * Возвращает указатель на массив клеток
		 */
		Cell **getCells() { return cells_; }
		/*!
		 * Возвращает указатель на массив клеток
		 */
		Cell **getCells() const { return cells_; }
		/*!
		 * Передает тип клетки, находящейся на данных координатах, по ссылке cell.
		 * Если клетка не может находиться на данных координатах, возвращает ERROR
		 */
		ErrorCodes getCell(int x, int y, Cell *&cell) const;
		/*!
		 * Передает тип клетки, находящейся на данных координатах, по ссылке cell.
		 * Если клетка не может находиться на данных координатах, возвращает ERROR
		 */
		ErrorCodes getCell(const Point &point, Cell *&cell) const;
		/*!
		 * Возвращает ссылку на карту предметов
		 */
		Map<Item*> &getItemMap() { return itemMap; }
		/*!
		 * Возвращает константную ссылку на карту предметов
		 */
		const Map<Item*> &getItemMap() const { return itemMap; }
		/*!
		 * Возвращает константную ссылку на карту существ
		 */
		const Map<Creature*> &getCreatureMap() const { return creatureMap; }
		/*!
		 * Возвращает указатель на текущее активное существо
		 */
		Creature *getActiveCreature() const { return activeCreature; }
		/*!
		 * Возвращает количество клеток в одной горизонтали
		 */
		int getHorizCells() const { return horizCells; }
		/*!
		 * Возвращает количество клеток в одной горизонтали
		 */
		int getVertCells() const { return vertCells; }
		/*!
		 * Возвращает константную ссылку на массив существ команды, совершающей текущий ход
		 */
		const std::vector<Creature *> &getCurrentTeam() const;
		/*!
		 * Возвращает ссылку на массив существ команды, совершающей текущий ход
		 */
		std::vector<Creature *> &getCurrentTeam();
		/*!
		 * Возвращает указатель на клетку по данной координате
		 */
		 Cell *operator[](const Point &point);
		
		/*!
		 * Устанавливает тип type для клетки, находящейся на данных координатах x и y
		 */
		void setCell(int x, int y, CellType type);
		/*!
		 * Устанавливает тип type для клетки, находящейся на данных координатах point
		 */
		void setCell(const Point &point, CellType type);
		/*!
		 * Передает ход
		 */
		 void setTurn(CreatType cr);
		
		/*!
		 * Устанавливает текущее активное существо
		 * @param i индекс существа в соответствующем массиве команды, которая совершает текущий ход
		 */
		void setActive(int i);
		/*!
		 * Добавляет предмет на карту предметов
		 * @param point координата, на которую добавляется предмет
		 * @param item добавляемый предмет
		 */
		void dropItem(Point &point, Item *item);
		
		/*!
		 * Передвигает существо в данном направлении
		 * @param x, y координаты существа до передвижения
		 * @param creature указатель на существо
		 * @param direction направление
		 */
		void moveCreature(int x, int y, Creature *creature, Direction direction);
		/*!
		 * Восстанавливает текущее время у членов текущей команды
		 */
		void resetTime();
		 /*!
		  * Проверяет наличие живых существ-противников
		  */
		bool enemyDied() const;
		 /*!
		  * Проверяет наличие живых оперативников
		  */
	  	bool gameOver() const;
		
		void loadCells(const char *fname);
		
		void loadCreatures(const char *);
		void loadOperative(std::ifstream &fs, std::string &name);
		void loadSentient(std::ifstream &fs, std::string &name);
		void loadWild(std::ifstream &fs, std::string &name);
		void loadForager(std::ifstream &fs, std::string &name);
		
		void loadItems(const char *);
		void loadHKits(std::ifstream &fs, std::string &name);
		void loadAConts(std::ifstream &fs, std::string &name);
		void loadGuns(std::ifstream &fs, std::string &name);
		
		void loadTextures();
		
		/*!
		 * Убирает данного оперативника из карты существ и вектора оперативников
		 */
		void killOperative(Creature*);
		/*!
		 * Убирает данное разумное существо из карты существ и вектора разумных существ
		 */
		void killSentient(Creature*);
		/*!
		 * Убирает данное дикое существо из карты существ и вектора диких существ
		 */
		void killWild(Creature*);
		/*!
		 * Убирает данного фуражера из карты существ и вектора фуражеров
		 */
		void killForager(Creature*);
		
		void spawnOperator(std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, float reloadTime, int force, float accuracy);
		void spawnSentient(std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, float accuracy);
		void spawnWild(std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, int accuracy, int damage, int attackTime);
		void spawnForager(std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, int force);
		
		void spawnHKit(std::string &name, int weight, const Point &point, int healAmount, int healTime);
		void spawnACont(std::string &name, int weight, const Point &point, int ammoTypeInt, int ammoMax);
		void spawnGun(std::string &name, int weight, const Point &point, int damage, int shootTime, int reloadTime, int ammoTypeInt, int ammoMax, float accuracy, int switchTime);
	};

	/*! @} */
}


#endif //LAB4_LEVEL_H
