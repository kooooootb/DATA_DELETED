#ifndef LAB4_CREATURE_H
#define LAB4_CREATURE_H

#include "../AllClasses.h"

namespace nodata{
	/*!
 * \file
 * \brief Заголовочный файл с описанием базового класса Creature
 *
 * Данный файл содержит базовый класс для описания существ, их общие методы и параметры
*/

/*! \defgroup creature Описатели существ
 *	\brief Данный модуль предназначен для описания существ
 *
 *	На данный момент доступно 4 вида существ: Оперативник, Разумное существо, Дикое существо и Одомашненный фуражер
 *	@{
 */

/*!
 * \brief Базовый класс для описания существ
 *
 * Базовый абстрактный класс для описания существ, который содержит их общие методы и параметры.
 * Любое существо может передвигаться по карте, имеет радиус обзора, координаты на уровне и показатели здоровья.
 * Некоторые действия невозможно выполнить без достаточного количества оставшегося времени хода.
 */
	class Creature {
	protected:
		std::string name_; ///< Имя существа
		int healthCurrent_; ///< Текущее количество единиц здоровья
		int healthMax_; ///< Максимальное количество единиц здоровья
		int timeCurrent_; ///< Текущее оставшееся время хода
		int timeMax_; ///< Максимальное время на 1 ход
		int walkTime_; ///< Время, затрачиваемое при каждом передвижении
		int viewRadius_; ///< Радиус обзора
		Point coord_; ///< Его координаты во время создания
		Level &level_; ///< Ссылка на уровень, содержащий существо
		
		sf::Sprite sprite;
	public:
		/*!
		 * Конструктор класса, инициализирующий его параметры
		 */
		Creature(Level *level, std::string &name, Point &coord, int healthMax = 100, int timeMax = 100, int walkTime = 5, int viewRadius = 5);
		
		virtual ~Creature();
		
		/*!
		 * Возвращает healthCurrent_
		 */
		int getHealthCurrent() const { return healthCurrent_; }
		/*!
		 * Возвращает healthMax_
		 */
		int getHealthMax() const { return healthMax_; }
		/*!
		 * Возвращает walkTime_
		 */
		int getWalkTime() const { return walkTime_; }
		/*!
		 * Возвращает viewRadius_
		 */
		int getViewRadius() const { return viewRadius_; }
		/*!
		 * Возвращает timeMax_
		 */
		int getTimeMax() const { return timeMax_; }
		/*!
		 * Возвращает timeCurrent_
		 */
		int getTimeCurrent() const { return timeCurrent_; }
		/*!
		 * Возвращает константную ссылку на coord_
		 */
		const Point &getPosition() const { return coord_; }
		/*!
		 * Возвращает константную ссылку на name_
		 */
		const std::string &getName() const { return name_; }
		
		/*!
		 * Устанавливает healthMax_
		 */
		void setHealthMax(int healthMax);
		/*!
		 * Устанавливает healthCurrent_
		 */
		void setHealthCurrent(int healthCurrent);
		/*!
		 * Устанавливает walkTime_
		 */
		void setWalkTime(int walkTime);
		/*!
		 * Устанавливает viewRadius_
		 */
		void setViewRadius(int viewRadius);
		/*!
		 * Устанавливает timeMax_
		 */
		void setTimeMax(int timeMax);
		
		/*!
		 * Увеличивает текущее количество единиц здоровья на healAmount, но не больше healthMax
		 * @param healAmount количество восстанавливаемого здоровья
		 */
		void heal(int healAmount);
		/*!
		 * Виртуальный метод, убивающий существо и выполняющий некоторые другие действия в зависимости от типа существа
		 */
		virtual void kill() = 0;
		/*!
		 * Уменьшает текущее оcтавшееся время существа
		 * @param time количество потраченного времени
		 */
		void spendTime(int time);
		/*!
		 * Выполняет перемещение существо в данном направлении
		 * @param direction направление
		 */
		ErrorCodes walk(Direction direction);
		/*!
		 * Уменьшает текущее количество здоровья существа и убивает его при недостаточном количестве единиц здоровья
		 * @param damage нанесенный урон
		 */
		void receiveDamage(int damage);
		void resetTime();
		
		/*!
		 * Виртуальный метод для отображения существа на экране
		 */
		void drawCreat(sf::RenderWindow &window);
		void setDrawPosition(float x, float y);
		void setTexture(sf::Texture &texture);
		
		virtual void saveFile(std::ofstream&) = 0;
		
		virtual ErrorCodes move(int randVar) = 0;
	};

/*! @} */
}

#include "Operative.h"
#include "Sentient.h"
#include "Wild.h"
#include "Forager.h"

#endif //LAB4_CREATURE_H
