#ifndef LAB4_FORAGER_H
#define LAB4_FORAGER_H

namespace nodata{
	/*!
 * \file
 * \brief Заголовочный файл с описанием класса Forager
 *
 * Данный файл содержит производный класс для описания поведения одомашненного фуражера
*/

/*! \addtogroup creature Описатели существ
 *	@{
 */

/*!
 * \brief Производный класс для описания Одомашненного фуражира
 *
 * Производный от класса Существо класс для описания поведения Одомашненного фуражира.
 * Одомашенный фуражир имеет инвентарь, но не может использовать оружие и предметы.
 * Фуражир должен искать на уровне выпавшие предметы и относить их на складские точки.
 */
	class Forager : public Creature {
	private:
		Table itemTable_; ///< Таблица инвентаря фуражира
		int force_; ///< Максимальный вес, который фуражир может нести
		
		std::stack<Direction> path; ///< Текущий путь
		std::vector<Point> unreachable; ///< Недостижимые на данном ходу точки
		std::vector<Point> cantTake; ///< Недостижимые на данном ходу точки
		
		bool isBusy = false; ///< Занят ли подбором предмета
		bool goingToStorage = false; ///< Идет ли к складсклй точке
	public:
		/*!
		 * Конструктор класса, инициализирующий его параметры
		 */
		Forager(Level *level, std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, int force);
		
		~Forager() override	= default;
		
		/*!
		 * Возвращает force_
		 */
		int getForce() const { return force_; }
		
		/*!
		 *	Убивает фуражира и выбрасывает все вещи из его инвентаря
		 */
		void kill() override;
		/*!
		 * Добавляет ближайший предмет в инвентарь фуражира если возможно
		 * @return ERROR если предмет слишком тяжелый, OK при успешной операции
		 */
		ErrorCodes receiveItems();
		/*!
		 * Выбрасывает предмет из инвентаря фуражира
		 * @param index номер предмета в таблице инвентаря
		 * @return ERROR при отсутствии предмета в инвентаре с данным индексом, OK при успешной операции
		 */
		ErrorCodes throwItem(int index);
		/*!
		 * Выбрасывает все предметы из инвентаря фуражира
		 */
		void dropAllItems();
		
		/*!
		 * Осуществляет ход существа
		 */
		 ErrorCodes move(int randVar) override;
		 
		 void saveFile(std::ofstream &) override;
		
		CreatType getType() override { return FORAGER; }
	};

/*! @} */
}

#endif //LAB4_FORAGER_H
