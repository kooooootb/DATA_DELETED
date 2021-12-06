#ifndef LAB4_ITEM_H
#define LAB4_ITEM_H

#include "../AllClasses.h"

namespace nodata{
	/*!
	 * \file
	 * \brief Заголовочный файл с описанием базового класса Item
	 *
	 * Данный файл содержит базовый класс для описания предметов, их общие методы и параметры
	 */

	/*! \defgroup item Описатели предметов
	 *	\brief Данный модуль предназначен для описания предметов
	 *
	 *	На данный момент доступно 3 вида предметов: Оружие, Аптечка и Контейнер патронов
	 *	@{
	 */

	/*!
	 * \brief Базовый класс для описания предметов
	 *
	 * Базовый абстрактный класс для описания предметов, который содержит их общие методы и параметры
	 */
	class Item {
	protected:
		std::string name_; ///< Название предмета
		int weight_; ///< Вес предмета
		Point coord_;
		
		sf::Sprite sprite;
	public:
		/*!
		 * Конструктор класса, инициализирующий его параметры
		 */
		explicit Item(std::string &name, int weight, const Point &point);
		
		virtual ~Item() = default;
		
		/*!
		 * Возвращает weight_
		 */
		int getWeight() const { return weight_; }
		/*!
		 * Возвращает константную ссылку на name_
		 */
		const std::string &getName() const { return name_; }
		
		/*!
		 * Устанавливает weight_
		 */
		void setWeight(int weight);
		/*!
		 * Виртуальный метод для использования предметов
		 * @param creature указатель на существо, необходимое при использовании предмета
		 * @return TODELETE если предмет надо удалить после использования, TOREMOVE если предмет надо убрать из таблицы инвентаря существа после использования, ERROR при возникновении ошибки
		 */
		virtual ErrorCodes use(Creature *creature) = 0;
		
		/*!
		 * Виртуальный метод для отображения предмета на экране
		 */
		void drawItem(sf::RenderWindow &window);
		void setDrawPosition(float x, float y);
		void setTexture(sf::Texture &texture);
		
		virtual void saveFile(std::ofstream&) = 0;
		
		virtual ItemType getType() const = 0;
	};

	/*! @} */
}

#include "Table.h"
#include "HealthKit.h"
#include "AmmoContainer.h"
#include "Gun.h"

#endif //LAB4_ITEM_H
