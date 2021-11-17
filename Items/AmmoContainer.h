#ifndef LAB4_AMMOCONTAINER_H
#define LAB4_AMMOCONTAINER_H

/*!
 * \file
 * \brief Заголовочный файл с описанием класса AmmoContainer
 *
 * Данный файл содержит производный класс для описания Контейнера патронов
*/

/*! \addtogroup item Описатели предметов
 *	@{
 */

/*!
 * \brief Производный класс для описания Контейнера патронов
 *
 * Производный от класса Item класс для описания Контейнера патронов.
 * Используется для перезарядки оружия.
 */
class AmmoContainer : public Item {
private:
	Ammunition ammoType_; ///< Тип патронов, лежащих в контейнере
	int ammoCurrent_; ///< Текущее количество патронов в контейнере
	int ammoMax_; ///< Максимальное количество патронов в контейнере
	
	/*!
	 * Вспомогательная функция для расчета веса определенного количества патронов
	 * @param amount количество патронов
	 * @return вес патронов
	 */
	int calcAmmoWeightByType(int amount);
public:
	/*!
	 * Конструктор класса, инициализирующий его параметры
	 * @param weight вес контейнера без патронов
	 */
	AmmoContainer(std::string &name, int weight, Ammunition ammoType = MEDIUM_CALIBER, int ammoMax = 120);
	
	~AmmoContainer() override = default;
	
	/*!
	 * Возвращает ammoType_
	 */
	Ammunition getAmmoType() const { return ammoType_; }
	/*!
	 * Возвращает ammoCurrent_
	 */
	int getAmmoCurrent() const { return ammoCurrent_; }
	/*!
	 * Возвращает ammoMax_
	 */
	int getAmmoMax() const { return ammoMax_; }
	
	/*!
	 * Устанавливает ammoType_
	 */
	void setAmmoType(Ammunition);
	/*!
	 * Устанавливает ammoCurrent_
	 */
	void setAmmoCurrent(int);
	/*!
	 * Устанавливает ammoMax_
	 */
	void setAmmoMax(int);
	
	/*!
	 * Функция перезаряжает активное оружие существа, при наличии, и рассчитывает вес контейнера и активного оружия
	 * @param creature существо, перезаряжающее свое оружие
	 * @return ERROR при ошибке, TODELETE при опустошении контейнера поссле использования, OK при успешной операции
	 */
	ErrorCodes use(Creature *creature) override;
	
	/*!
	 * Метод для отображения контейнера на экране
	 */
	void drawCell(sf::RectangleShape &shape) override;
};

/*! @} */

#endif //LAB4_AMMOCONTAINER_H
