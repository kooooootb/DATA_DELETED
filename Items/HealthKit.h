#ifndef LAB4_HEALTHKIT_H
#define LAB4_HEALTHKIT_H

namespace nodata{
	/*!
 * \file
 * \brief Заголовочный файл с описанием класса HealthKit
 *
 * Данный файл содержит производный класс для описания Аптечки
*/

/*! \addtogroup item Описатели предметов
 *	@{
 */

/*!
 * \brief Производный класс для описания Аптечки
 *
 * Производный от класса Item класс для описания Аптечки.
 * Аптечка используется существом, которому необходимо восстановить здоровье.
 * Аптечка одноразовая, удаляется после использования
 */
	class HealthKit : public Item {
	private:
		int healAmount_; ///< Количество единиц здоровья, восстанавливаемых аптечкой
		int healTime_; ///< Количество времени, затрачиваемое на использование аптечки
	public:
		/*!
		 * Конструктор класса, инициализирующий его параметры
		 */
		HealthKit(std::string &name, int weight, const Point &point, int healAmount, int healTime);
		
		~HealthKit() override = default;
		
		/*!
		 * Возвращает healTime_
		 */
		int getHealTime() const { return healTime_; }
		
		/*!
		 * Восстанавливает здоровье существа на значение healAmount_
		 * @param creature указатель на существо, использующее аптечку
		 * @return TODELETE для удаления аптечки после использования
		 */
		ErrorCodes use(Creature *creature) override;
		
		void saveFile(std::ofstream&) override;
	};

/*! @} */
}


#endif //LAB4_HEALTHKIT_H
