#ifndef LAB4_CELL_H
#define LAB4_CELL_H

#include "../AllClasses.h"

namespace nodata{
	/*!
	 * \file
	 * \brief Заголовочный файл с описанием класса Cell
	 *
	 * Данный файл содержит класс для описания Клеток уровня
	 */
	
	/*! \addtogroup level Описатели уровня
	 *	@{
	 */
	
	/*!
	 * \brief Класс для описания Клеток уровня
	 *
	 * На данный момент клетка может иметь один из 4 типов: Пол, Стена, Стекло и Перегородка.
	 */
	class Cell {
	private:
		CellType type_; ///< Тип клетки
		
		sf::Sprite sprite;
	public:
		/*!
		 * Конструктор класса Cell преобразует ее тип в Пол.
		 */
		Cell();
		
		/*!
		 * Возвращает тип клетки
		 */
		CellType getType() const { return type_; }
		
		/*!
		 * Устанавливает тип клетки и текстуру
		 */
		void setType(CellType type, sf::Texture *textures);
		/*!
		 * Устанавливает тип клетки
		 */
		void setType(CellType type);
		
		/*!
		 * Определяет, можно ли по клетке ходить
		 */
		bool walkAble() const;
		/*!
		 * Определяет, можно ли через клетку смотреть
		 */
		bool seeAble() const;
		
		/*!
		 * Устанавливает позицию спрайта клетки
		 * @param x, y координаты
		 */
		void setDrawPosition(float x, float y);
		/*!
		 * Рисует клетку
		 */
		void drawCell(sf::RenderWindow &window);
		/*!
		 * Устанавливает текстуру клетки
		 */
		void setTexture(sf::Texture &texture);
	};

	/*! @} */
}


#endif //LAB4_CELL_H
