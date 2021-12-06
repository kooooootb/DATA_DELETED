#ifndef LAB4_SENTIENT_H
#define LAB4_SENTIENT_H

namespace nodata{
	/*!
 * \file
 * \brief Заголовочный файл с описанием класса Sentient
 *
 * Данный файл содержит производный класс для описания поведения разумного существа
*/

/*! \addtogroup creature Описатели существ
 *	@{
 */

/*!
 * \brief Производный класс для описания Разумного существа
 *
 * Производный от класса Существо класс для описания поведения Разумного существа.
 * Разумное существо не имеет инвентаря, но может подбирать оружие и использовать его.
 */
	class Sentient : public Creature {
	private:
		Gun* activeGun_; ///< Текущее оружие оперативника, которое он несет в руках
		float accuracyMultipl_; ///< Коэффициент точности стрельбы
		
		std::stack<Direction> path;
		bool isBusy = false; ///< Занят ли подбором оружия
		std::vector<Point> unreachable; ///< Недосягаемые на этом ходу точки
		std::vector<Point> useless; ///< Точки с выкинутыми оружиями, у которых кончились патроны
	public:
		/*!
		 * Конструктор класса, инициализирующий его параметры
		 */
		Sentient(Level *level, std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, float accuracy);
		
		~Sentient() override;
		
		float getAccuracy() const { return accuracyMultipl_; }
		
		/*!
		 * Подбирает ближайшее оружие и делает его активным оружием разумного существа
		 */
		void receiveGun();
		/*!
		 * Выбрасывает оружие разумного существа
		 */
		void throwGun()
		/*!
		 * Стреляет в существо из активного оружия при его наличии
		 * @param victim указатель на существо-жертву
		 */;
		void shoot(Creature *victim);
		ErrorCodes shoot(Point &point, int randVar);
		/*!
		 *	Убивает существо и выбрасывает его активное оружие
		 */
		void kill() override;
		
		/*!
		 * Осуществляет ход существа
		 */
		ErrorCodes move(int randVar) override;
		
		void saveFile(std::ofstream &) override;
		
		CreatType getType() override { return SENTIENT; }
	};

/*! @} */
}

#endif //LAB4_SENTIENT_H
