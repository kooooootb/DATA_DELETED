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
	public:
		/*!
		 * Конструктор класса Cell преобразует ее тип в Пол.
		 */
		Cell() : type_(FLOOR) {}
		
		/*!
		 * Возвращает тип клетки
		 */
		CellType getType() const { return type_; }
		
		/*!
		 * Устанавливает тип клетки
		 */
		void setType(CellType type);
	};

	/*! @} */
}


#endif //LAB4_CELL_H
