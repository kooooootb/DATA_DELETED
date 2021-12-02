#ifndef LAB4_WILD_H
#define LAB4_WILD_H

namespace nodata{
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
 * Производный от класса Существо класс для описания поведения Дикого существа.
 * Дикое существо не имеет инвентаря и не может подбирать оружие.
 * Оно аттакует в ближнем бою существа, которые находятся на соседних клетках.
 */
	class Wild : public Creature {
	private:
		int accuracy_; ///< Точность атаки существа
		int damage_; ///< Наносимый существом урон
		int attackTime_; ///< Время, затрачиваемое при атаке
		
		int penaltyMoves_ = 0;
		std::queue<Direction> path_;
	public:
		/*!
		 * Конструктор класса, инициализирующий его параметры
		 */
		Wild(Level *level, std::string &name, Point coord, int healthMax, int timeMax, int walkTime, int viewRadius, int accuracy, int damage, int attackTime);
		
		~Wild() override = default;
		
		int getAccuracy() const { return accuracy_; }
		int getDamage() const { return damage_; }
		int getAttackTime() const { return attackTime_; }
		
		/*!
		 * Атакует жертву при условии, что существо и жертва находятся на соседних клетках
		 * @param victim указатель на существо-жертву
		 */
		ErrorCodes attack(Creature *victim);
		/*!
		 *	Убивает существо
		 */
		void kill() override;
		
		 /*!
		  * Осуществляет ход существа
		  */
		ErrorCodes move(int randVar) override;
		
		void saveFile(std::ofstream &) override;
	};

/*! @} */
}

#endif //LAB4_WILD_H
