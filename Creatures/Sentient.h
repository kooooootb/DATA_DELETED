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
	public:
		/*!
		 * Конструктор класса, инициализирующий его параметры
		 */
		Sentient(Level *level, std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, float accuracy);
		
		~Sentient() override;
		
		/*!
		 * Подбирает оружие и делает его активным оружием разумного существа
		 * @param gun Указатель на подбираемое оружие
		 */
		void receiveGun(Gun *gun);
		/*!
		 * Выбрасывает оружие разумного существа
		 */
		void throwGun()
		/*!
		 * Стреляет в существо из активного оружия при его наличии
		 * @param victim указатель на существо-жертву
		 */;
		void shoot(Creature *victim);
		/*!
		 *	Убивает существо и выбрасывает его активное оружие
		 */
		void kill() override;
		
		/*!
		 * Метод для отображения разумного существа на экране
		 */
		void drawCell(sf::RectangleShape &shape) override;
	};

/*! @} */
}

#endif //LAB4_SENTIENT_H
