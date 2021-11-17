#ifndef LAB4_FORAGER_H
#define LAB4_FORAGER_H

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
 * Производный от класса Существо класс для описания поведения Одомашненного фуражира. Одомашенный фуражир имеет инвентарь, но не может использовать оружие и предметы. Фуражир должен искать на уровне выпавшие предметы и относить их на складские точки.
 */
class Forager : public Creature {
private:
	Table itemTable_; ///< Таблица инвентаря фуражира
	int force_; ///< Максимальный вес, который фуражир может нести
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
	 * Добавляет предмет в инвентарь фуражира если возможно
	 * @param item Указатель на добавляемый предмет
	 * @return ERROR если предмет слишком тяжелый, OK при успешной операции
	 */
	ErrorCodes receiveItem(Item *item);
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
	 * Метод для отображения фуражира на экране
	 */
	void drawCell(sf::RectangleShape &shape) override;
};

/*! @} */

#endif //LAB4_FORAGER_H
