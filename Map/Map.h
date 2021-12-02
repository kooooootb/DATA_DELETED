#ifndef LAB4_MAP_H
#define LAB4_MAP_H

namespace nodata{
#include "../AllClasses.h"
	template<class T>
	class Map;
	template<class T>
	class MapIt;
	
	template<class T>
	class Map {
	private:
		friend class MapIt<T>;
		struct HorCell{
			int x = 0;
			int amount = 0;
			T *items = nullptr;
		};
		struct VertCell{
			int y;
			int amountX;
			HorCell *row;
		};
		VertCell *vertAr;
		int amountY;
		
		template<class CellT>
		CellT *createCell(int index, CellT *&ar, int &amount);
		template<class CellT>
		CellT *removeCell(int index, CellT *&ar, int &amount);
		
		void insert(HorCell &hcell, T item);
		void extract(HorCell &hcell, T item);
	public:
		Map() : vertAr(nullptr) , amountY(0) {}
		
		~Map();
		
		typedef MapIt<T> iterator;//goes through all items
		iterator begin() const{
			return iterator(vertAr, amountY, *this);
		}
		iterator end() const{
			return iterator(vertAr + amountY, *this);
		}
		
		void addItem(const Point &point, T item);
		void addItem(int x, int y, T item);
		
		void removeItem(const Point &point, T item);
		void removeItem(int x, int y, T item);
		
		const Ptr<T>* getLine(int y, int begin, int end) const;
		const Ptr<T>* getAll() const{}
		
		Ptr<T> operator[](const Point &point) const;
		
		void print(std::ostream &s);
	};
	
	template<class T>
	class MapIt{
	private:
		typename Map<T>::VertCell *vCell;
		typename Map<T>::HorCell *hCell;
		T *item;
		
		Map<T> &map;
		
		int curX, curItem, yAmount;
	public:
		explicit MapIt(typename Map<T>::VertCell *vertcell, int amount, Map<T> &Map) : map(Map) {//init begin
			vCell = vertcell;
			yAmount = amount;
			if(amount != 0){
				hCell = vertcell->row;
				item = hCell->items;
			}
			curX = 1;
			curItem = 1;
		}
		
		explicit MapIt(typename Map<T>::VertCell *vertcell, Map<T> &Map) : map(Map) {//init end
			vCell = vertcell;
			hCell = nullptr;
			curX = 1;
			curItem = 1;
			yAmount = 0;
		}
		
		void operator++(){
			if(curItem == hCell->amount) {//last item
				if (curX == vCell->amountX) {//last hCell
					vCell++;
					if(--yAmount == 0){
						return;
					}
					hCell = vCell->row;
					item = hCell->items;
					curX = 1;
					curItem = 1;
				}
				else{//go to next hCell
					hCell++;
					curX++;
					item = hCell->items;
					curItem = 1;
				}
			}
			else{//go to next item
				item++;
				curItem++;
			}
		}
		
		T operator*(){
			return *item;
		}
		
		bool operator!=(const MapIt<T> &a) const{
			return a.vCell != vCell;
		}
	};
	
	template<class T>
	void Map<T>::insert(HorCell &hcell, T item){
		T *newItems = new T[hcell.amount + 1];
		for(int i = 0;i < hcell.amount;++i){
			newItems[i] = hcell.items[i];
		}
		if(hcell.amount > 0) delete [] hcell.items;
		newItems[hcell.amount] = item;
		hcell.items = newItems;
		hcell.amount++;
	}
	
	template<class T>
	void Map<T>::extract(HorCell &hcell, T item){
		T *newItems = new T[hcell.amount - 1];
		int diff = 0;
		for(int i = 0;i < hcell.amount;++i){
			if(hcell.items[i] == item){
				i++;
				if(i >= hcell.amount) break;
				diff = -1;
			}
			newItems[i + diff] = hcell.items[i];
		}
		delete [] hcell.items;
		hcell.items = newItems;
		hcell.amount--;
	}
	
	template<class T>
	Map<T>::~Map() {
		for(int i = 0;i < amountY;++i){
			VertCell &vc = vertAr[i];
			for(int j = 0;j < vc.amountX;++j){
				T *v = vc.row[j].items;
				for(int k = 0;k < vc.row[j].amount;++k){
					delete v[k];
				}
				delete [] v;
			}
			delete [] vc.row;
		}
		delete [] vertAr;
	}
	
	template<class T>
	template<class CellT>
	CellT *Map<T>::createCell(int index, CellT *&ar, int &amount){
		auto *newAr = new CellT[amount + 1];
		for(int i = 0;i < index;++i){
			newAr[i] = ar[i];
		}
		
		for(int i = index;i < amount;++i){
			newAr[i + 1] = ar[i];
		}
		
		delete [] ar;
		ar = newAr;
		amount += 1;
		return (ar + index);
	}
	
	template<class T>
	template<class CellT>
	CellT *Map<T>::removeCell(int index, CellT *&ar, int &amount){
		if(amount == 1){
			delete [] ar;
			ar = nullptr;
			amount -= 1;
			return nullptr;
		}
		
		auto *newAr = new CellT[amount - 1];
		for(int i = 0;i < index;++i){
			newAr[i] = ar[i];
		}
		
		for(int i = index;i < amount - 1;++i){
			newAr[i] = ar[i + 1];
		}
		
		delete [] ar;
		ar = newAr;
		amount -= 1;
		return (ar + index);
	}
	
	template<class T>
	void Map<T>::addItem(int x, int y, T item) {
		int i;
		VertCell *vCell = nullptr;
		for(i = 0;i < amountY;++i){
			if(vertAr[i].y > y) break;
			if(vertAr[i].y == y){
				vCell = vertAr + i;
				break;
			}
		}
		
		if(vCell == nullptr){
			vCell = createCell<VertCell>(i, vertAr, amountY);
			vCell->y = y;
			vCell->amountX = 1;
			vCell->row = new HorCell[1];
			vCell->row->x = x;
			insert(*(vCell->row), item);
		}
		else{
			HorCell *hCell = nullptr, *&row = vCell->row;
			for(i = 0;i < vCell->amountX;++i){
				if(row[i].x > x) break;
				if(row[i].x == x){
					hCell = row + i;
					break;
				}
			}
			
			if(hCell == nullptr){
				hCell = createCell<HorCell>(i, row, vCell->amountX);
				hCell->x = x;
			}
			
			insert(*hCell, item);
		}
	}
	
	
	template<class T>
	void Map<T>::addItem(const Point &point, T item){
		int x = point.x, y = point.y;
		addItem(x, y, item);
	}
	
	template<class T>
	void Map<T>::removeItem(int x, int y, T item) {
		VertCell *vCell = nullptr;
		int iV, iH;
		for(iV = 0;iV < amountY;++iV){
			if(vertAr[iV].y == y){
				vCell = vertAr + iV;
				break;
			}
		}
		if(vCell == nullptr) throw std::exception();
		
		HorCell *&row = vCell->row, *hCell = nullptr;
		int &amountX = vCell->amountX;
		for(iH = 0;iH < amountX;++iH){
			if(row[iH].x == x){
				hCell = row + iH;
				break;
			}
		}
		
		if(hCell == nullptr) throw std::exception();
		
		if(hCell->amount > 1){
			extract(*hCell, item);
		}
		else if(hCell->amount == 1){
			if(removeCell<HorCell>(iH, row, amountX) == nullptr){
				removeCell<VertCell>(iV, vertAr, amountY);
			}
		}
		else throw std::exception();
	}
	
	template<class T>
	void Map<T>::removeItem(const Point &point, T item){
		int x = point.x, y = point.y;
		removeItem(x, y, item);
	}
	
	template<class T>
	const Ptr<T>* Map<T>::getLine(int y, int begin, int end) const{
		Ptr<T>* res;
		res = new Ptr<T>[end - begin + 1];
		for(int i = 0;i < end - begin + 1;++i){
			res[i].ptr = nullptr;
		}
		
		int i;
		VertCell *vCell = nullptr;
		for(i = 0;i < amountY;++i){
			if(vertAr[i].y == y){
				vCell = vertAr + i;
				break;
			}
		}
		if(vCell == nullptr) return res;
		
		HorCell *row = vCell->row;
		int amountX = vCell->amountX;
		for(i = 0;i < amountX;++i){
			if(row[i].x >= begin) break;
		}
		for(;i < amountX;++i){
			if(row[i].x > end) break;
			res[row[i].x - begin].ptr = row[i].items;
			res[row[i].x - begin].amount = row[i].amount;
		}
		
		return res;
	}
	
	template<class T>
	void Map<T>::print(std::ostream &s){
		for(int i = 0;i < amountY;++i){
			VertCell &vc = vertAr[i];
			s << "Y:" << vc.y << std::endl;
			for(int j = 0;j < vc.amountX;++j){
				s << "\tX:" << vc.row[j].x;
				T *v = vc.row[j].items;
				s << "\tsize:" << vc.row[j].amount;
				for(int k = 0;k < vc.row[j].amount;++k){
					s << "\tItem" << k+1 << ":" << v[k];
				}
				s << std::endl;
			}
		}
	}
	
	template<class T>
	Ptr<T> Map<T>::operator[](const Point &point) const {
		int x = point.x, y = point.y;
		
		Ptr<T> res;
		res.ptr = nullptr;
		res.amount = 0;
		
		int i;
		VertCell *vCell = nullptr;
		for(i = 0;i < amountY;++i){
			if(vertAr[i].y == y){
				vCell = vertAr + i;
				break;
			}
		}
		if(vCell == nullptr){
			return res;
		}
		
		HorCell *row = vCell->row;
		int amountX = vCell->amountX;
		for(i = 0;i < amountX;++i){
			if(row[i].x == x){
				res.ptr = row[i].items;
				res.amount = row[i].amount;
				return res;
			}
		}
		
		return res;
	}
}

#endif //LAB4_MAP_H