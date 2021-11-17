#ifndef LAB4_GUN_H
#define LAB4_GUN_H

/*!
 * \file
 * \brief Заголовочный файл с описанием класса Gun
 *
 * Данный файл содержит производный класс для описания Оружия
*/

/*! \addtogroup item Описатели предметов
 *	@{
 */

/*!
 * \brief Производный класс для описания Оружия
 *
 * Производный от класса Item класс для описания Оружия.
 * Оружие можно перезарядить используя Контейнер патронов.
 * Из оружия можно стрелять, за раз происходит несколько выстрелов в зависимости от типа патронов.
 * Точность стрельбы зависит от параметра точности стреляющего и точности оружия.
 */
class Gun : public Item {
private:
	int damage_; ///< Урон, наносимый оружием
	int shootTime_; ///< Время затрачиваемое при стрельбе
	int reloadTime_; ///< Время, затрачиваемое при перезарадке
	Ammunition ammoType_; ///< Тип патронов, используемый оружием
	int ammoCurrent_; ///< Текущее количество патронов в оружии
	int ammoMax_; ///< Максимальное количество патронов в оружии
	int accuracy_; ///< Точность стрельбы оружия
	int switchTime_; ///< Время, затрачиваемое при смене оружия
	
	/*!
	 * Вспомогательная функция для расчета веса определенного количества патронов
	 * @param amount количество патронов
	 * @return вес патронов
	 */
	int calcAmmoWeightByType(int amount) const;
	/*!
	 * Вспомогательная функция для расчета количества выстрелов из оружия за один вызов функции shoot() в зависимости от типа патронов
	 * @return количество выстрелов
	 */
	int calcAmountByType() const; ///<
public:
	/*!
	 * Конструктор класса, инициализирующий его параметры
	 */
	Gun(std::string &name, int weight, int damage, int shootTime, int reloadTime, Ammunition ammoType, int ammoMax, int accuracy, int switchTime);
	
	~Gun() override = default;
	
	/*!
	 * Делает данное оружие активным у существа и кладет прошлое активное оружие в инвентарь
	 * @param creature существо, у которого происходит замена активного оружия
	 * @return ERROR при отсутствии активного оружия у существа, TOREMOVE для удаления из инвентаря этого оружия
	 */
	ErrorCodes use(Creature *creature) override;//take gun in hands
	
	
	/*!
	 * Возвращает ammoType_
	 */
	Ammunition getAmmoType() const { return ammoType_; }
	/*!
	 * Возвращает ammoMax_
	 */
	int getAmmoMax() const { return ammoMax_; }
	/*!
	 * Возвращает ammoCurrent_
	 */
	int getAmmoCurrent() const { return ammoCurrent_; };
	/*!
	 * Возвращает reloadTime_
	 */
	int getReloadTime() const { return reloadTime_; }
	
	/*!
	 * Устанавливает ammoCurrent_
	 */
	void setAmmoCurrent(int ammoCurrent);
	
	/*!
	 * Наносит урон оперативнику
	 */
	static void receiveDamage(int damage, Operative *operative);
	/*!
	 * Производит несколько выстрелов из оружия по существу-жертве. Количество выстрелов зависит от типа патронов. Количество попаданий определяется переменной hitsMultipl
	 * @param victim существо-жертва
	 * @param shooter существо-стрелок
	 * @param hitsMultipl коэффициент попаданий
	 */
	void shoot(Creature *victim, Creature *shooter, float hitsMultipl);
	/*!
	 * Функция вычисляет коэффициент количества попаданий в зависимости от точности стрелявшего существа и точности оружия. Коэффициент не должен быть больше единицы.
	 * @param crAccuracy коэффициент точности стреляющего
	 * @return коэффициент количества попаданий
	 */
	float countHitsMultipl(float crAccuracy) const;
	
	/*!
	 * Метод для отображения оружия на экране
	 */
	void drawCell(sf::RectangleShape &shape) override;
};

/*! @} */


#endif //LAB4_GUN_H
