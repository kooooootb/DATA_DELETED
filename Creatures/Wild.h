#ifndef LAB4_WILD_H
#define LAB4_WILD_H

/*!
 * \file
 * \brief Заголовочный файл с описанием класса Wild
 *
 * Данный файл содержит производный класс для описания поведения дикого существа
*/

/*! \addtogroup creature Описатели существ
 *	@{
 */

/*!
 * \brief Производный класс для описания Дикого существа
 *
 * Производный от класса Существо класс для описания поведения Дикого существа. Дикое существо не имеет инвентаря и не может подбирать оружие. Оно аттакует только в ближнем бою.
 */
class Wild : public Creature {
private:
	int accuracy_; ///< Точность атаки существа
	int damage_; ///< Наносимый существом урон
public:
	/*!
	 * Конструктор класса, инициализирующий его параметры
	 */
	Wild(Level *level, std::string &name, Point coord, int healthMax, int timeMax, int walkTime, int viewRadius, int accuracy, int damage);
	
	~Wild() override;
	
	/*!
	 * Атакует жертву
	 * @param victim указатель на существо-жертву
	 */
	void attack(Creature *victim);
	/*!
	 *	Убивает существо
	 */
	void kill() override;
	
	/*!
	 * Метод для отображения дикого существа на экране
	 */
	void drawCell(sf::RectangleShape &shape) override;
};

/*! @} */

#endif //LAB4_WILD_H
