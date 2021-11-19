#ifndef LAB4_MAP_H
#define LAB4_MAP_H

namespace nodata{
	/*!
 * \file
 * \brief Заголовочный файл с описанием шаблона класса Map
 *
 * Данный файл содержит шаблон класс для хранения элементов на местности и быстрого доступа к этим элементам через их координаты
*/

/*! \defgroup map Описатели карты
 *	\brief Данный модуль предназначен для описания карт
 *	@{
 */

#include "../AllClasses.h"

/*!
 * \brief Шаблон класса для описания карты
 *
 * Карта является подобием разреженной матрицы.
 * Карта содержит двумерный массив векторов с элементами.
 * Вектор элементов содержит все элементы, имеющие одинаковые координаты.
 * При отсутствии элементов на координате место в двумерном массиве под эту координату не выделяется
*/
	template<class T>
	class Map {
	private:
		std::map<int, std::map<int, std::vector<T>>> vertAr; ///< Двумерный массив векторов
	public:
		Map() = default;
		
		~Map();
		
		/*!
		 * Добавляет элемент в карту.
		 * Если до этого в карте не было элементов с данной координатой, создается новая ячейка в двумерном массиве и в нее кладётся элемент
		 * Если до этого в карте был элемент с данной координатой, элемент добавляется в конец вектора данной координаты
		 * @param point координаты
		 * @param item передаваемый элемент
		 */
		void addItem(const Point &point, T item);
		/*!
		 * Добавляет элемент в карту.
		 * Если до этого в карте не было элементов с данной координатой, создается новый вектор в двумерном массиве и в нее кладётся элемент
		 * Если до этого в карте был элемент с данной координатой, элемент добавляется в конец вектора данной координаты
		 * @param x, y координаты
		 * @param item передаваемый элемент
		 */
		void addItem(int x, int y, T item);
		
		/*!
		 * Удаляет данный предмет с карты.
		 * Если в карте больше нет элементов с данными координатами, удаляет также ячейку в двумерном массиве. В противном случае удаляет элемент из вектора данной координаты
		 * @param point координаты
		 * @param item удаляемый элемент
		 */
		void removeItem(const Point &point, T item);
		/*!
		 * Удаляет данный предмет с карты.
		 * Если в карте больше нет элементов с данными координатами, удаляет также ячейку в двумерном массиве. В противном случае удаляет элемент из вектора данной координаты
		 * @param x, y координаты
		 * @param item удаляемый элемент
		 */
		void removeItem(int x, int y, T item);
		
		/*!
		 * Создает вектор указателей на векторы элементов, лежащих между точками (begin, y) и (end, y)
		 * @param y горизонталь, на которой лежат элементы
		 * @param begin вертикаль, с которой начинается поиск элементов
		 * @param end вертикаль на которой заканчивается поиск элементов
		 * @return созданный вектор указателей на вектора элементов
		 */
		std::vector<const std::vector<T>*> getLine(int y, int begin, int end) const;
		/*!
		 * Возвращает указатель на вектор элементов, лежащих на данных координатах или nullptr если элементов с данными координатами нет
		 * @param point координаты
		 * @return указатель на вектор элементов или nullptr если он отсутствует
		 */
		const std::vector<T>* operator[](const Point &point) const;
		
		/*!
		 * Выводит данные о карте в поток вывода
		 * @param s поток вывода
		 */
		void print(std::ostream &s);
	};

/*! @} */
	
	template<class T>
	Map<T>::~Map() {
		for(auto itY = vertAr.begin(), endY = vertAr.end();itY != endY;++itY){
			std::map<int, std::vector<T>> &vCell = itY->second;
			for(auto itX = vCell.begin(), endX = vCell.end();itX != endX;++itX){
				std::vector<T> &v = itX->second;
				for(auto itV = v.begin(), endV = v.end();itV != endV;++itV){
					delete *itV;
				}
			}
		}
	}
	
	template<class T>
	void Map<T>::addItem(int x, int y, T item) {
		vertAr[y][x].push_back(item);
	}
	
	template<class T>
	void Map<T>::addItem(const Point &point, T item){
		int x = point.x, y = point.y;
		vertAr[y][x].push_back(item);
	}
	
	template<class T>
	void Map<T>::removeItem(int x, int y, T item) {
		std::vector<T> &v = vertAr[y][x];
		if(v.size() == 1){
			vertAr[y].erase(x);
			if(vertAr[y].size() == 0)
				vertAr.erase(y);
		}else{
			v.erase(std::find(v.begin(), v.end(), item));
		}
	}
	
	template<class T>
	void Map<T>::removeItem(const Point &point, T item){
		int x = point.x, y = point.y;
		std::vector<T> &v = vertAr[y][x];
		if(v.size() == 1){
			vertAr[y].erase(x);
			if(vertAr[y].size() == 0)
				vertAr.erase(y);
		}else{
			v.erase(std::find(v.begin(), v.end(), item));
		}
	}
	
	template<class T>
	std::vector<const std::vector<T>*> Map<T>::getLine(int y, int begin, int end) const{
		std::vector<const std::vector<T>*> res;
		res.resize(end - begin + 1);
		
		if(vertAr.count(y) == 0) return res;
		const std::map<int, std::vector<T>> &vCell = vertAr.at(y);
		
		for(auto it = vCell.begin(), itEnd = vCell.end();it != itEnd;++it){
			if(it->first >= begin && it->first <= end){
				res[it->first - begin] = &(it->second);
			}
		}
		
		return res;
	}
	
	template<class T>
	void Map<T>::print(std::ostream &s){
		for(auto itY = vertAr.begin(), endY = vertAr.end();itY != endY;++itY){
			s << "Y:" << itY->first << ':' << std::endl;
			std::map<int, std::vector<T>> &vCell = itY->second;
			for(auto itX = vCell.begin(), endX = vCell.end();itX != endX;++itX){
				s << "\tX:" << itX->first << ':' << std::endl;
				std::vector<T> &v = itX->second;
				for(auto itV = v.begin(), endV = v.end();itV != endV;++itV){
					s << "\t\tItem:" << *itV << std::endl;
				}
			}
		}
	}
	
	template<class T>
	const std::vector<T> *Map<T>::operator[](const Point &point) const{
		int x = point.x, y = point.y;
		if(vertAr.count(y) == 0) return nullptr;
		if(vertAr.at(y).count(x) == 0) return nullptr;
		
		return &vertAr.at(y).at(x);
	}
}

#endif //LAB4_MAP_H
