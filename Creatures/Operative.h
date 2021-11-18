#ifndef LAB4_OPERATIVE_H
#define LAB4_OPERATIVE_H

namespace nodata{
	/*!
 * \file
 * \brief Заголовочный файл с описанием класса Operative
 *
 * Данный файл содержит производный класс для описания поведения оперативника
*/

/*! \addtogroup creature Описатели существ
 *	@{
 */

/*!
 * \brief Производный класс для описания Оперативника
 *
 * Производный от класса Существо класс для описания поведения Оперативника.
 * Оперативник имеет инвентарь, может использовать оружие и предметы из инвентаря.
 * Оперативники должны уничтожить всех существ на уровне.
 */
	class Operative : public Creature {
	private:
		float reloadTimeMultipl_; ///< Коэффициент времени перезарядки оружия
		int force_; ///< Максимальный вес, который оперативник может нести
		float accuracyMultipl_; ///< Коэффициент точности стрельбы
		Table itemTable_; ///< Таблица инвентаря оперативника
		Gun *activeGun_; ///< Текущее оружие оперативника, которое он несет в руках
	public:
		/*!
		 * Конструктор класса, инициализирующий его параметры
		 */
		Operative(nodata::Level *level, std::string &name, Point &coord, int healthMax, int timeMax, int walkTime, int viewRadius, float reloadTime, int force, float accuracy);
		
		~Operative() override;
		
		/*!
		 * Возвращает activeGun_
		 */
		Gun *getActiveGun() const { return activeGun_; }
		/*!
		 * Возвращает force_
		 */
		int getForce() const { return force_;}
		/*!
		 * Возвращает accuracyMultipl_
		 */
		float getAccuracy() const { return accuracyMultipl_; }
		/*!
		 * Возвращает reloadTimeMultipl_
		 */
		float getReloadTimeMultipl() const { return reloadTimeMultipl_; };
		/*!
		 * Возвращает ссылку на itemTable_
		 */
		Table &getTable() { return itemTable_; }
		/*!
		 * Возвращает константную ссылку на itemTable_
		 */
		const Table &getTable() const { return itemTable_; }
		/*!
		 * Возвращает вес оперативника
		 */
		int getWeight() const;
		
		/*!
		 * Устанавливает текущее активное оружие оперативника
		 */
		void setActiveGun(Gun *gun);
		/*!
		 * Устанавливает force_
		 */
		void setForce(int force);
		
		/*!
		 *	Убивает оперативника и выбрасывает все вещи из его инвентаря
		 */
		void kill() override;
		/*!
		 * Добавляет предмет в инвентарь оперативника если возможно
		 * @param item Указатель на добавляемый предмет
		 * @return ERROR если предмет слишком тяжелый, OK при успешной операции
		 */
		ErrorCodes receiveItem(Item *item);
		/*!
		 * Выбрасывает предмет из инвентаря оперативника
		 * @param index номер предмета в таблице инвентаря
		 * @return ERROR при отсутствии предмета в инвентаре с данным индексом, OK при успешной операции
		 */
		void throwItem(int index);
		/*!
		 * Использует предмет из инвентаря оперативника. При необходимости удаляет предмет из таблицы(например, при использовании аптечки)
		 * @param index номер предмета в таблице инвентаря
		 */
		void useItem(int index);
		/*!
		 * Стреляет в существо из активного оружия при его наличии
		 * @param victim указатель на существо-жертву
		 */
		void shoot(Creature *victim);
		/*!
		 * Выбрасывает все предметы из инвентаря оперативника
		 */
		void dropAllItems();
		
		/*!
		 * Метод для отображения оперативника на экране
		 */
		void drawCell(sf::RectangleShape &shape) override;
	};

/*! @} */
}

#endif //LAB4_OPERATIVE_H
