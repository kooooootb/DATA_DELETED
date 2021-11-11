#include "Map.h"

template<class T>
template<class CellT>
CellT *Map<T>::createCell(int index, CellT *&ar, int &amount)  {
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
CellT *Map<T>::removeCell(int index, CellT *&ar, int &amount)  {
	auto *newAr = new CellT[amount - 1];
	for(int i = 0;i < index;++i){
		newAr[i] = ar[i];
	}
	
	for(int i = index;i < amount;++i){
		newAr[i] = ar[i + 1];
	}
	
	delete [] ar;
	ar = newAr;
	amount -= 1;
	return (ar + index);
}

template<class T>
void Map<T>::addItem(int x, int y, T *item) {
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
		vCell->row = new HorCell(x, item);
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
		
		hCell->items.push_back(item);
	}
}

template<class T>
std::vector<T> ** Map<T>::getLine(int y, int begin, int end) {
	auto ** res = new std::vector<T>*[end - begin + 1]();
	int i;
	VertCell *vCell = nullptr;
	for(i = 0;i < amountY;++i){
		if(vertAr[i].y == y){
			vCell = vertAr + i;
			break;
		}
	}
	if(vCell == nullptr) return res;
	
	HorCell *hCell = nullptr, *row = vCell->row;
	int amountX = vCell->amountX;
	for(i = 0;i < amountX;++i){
		if(row[i].x >= begin) break;
	}
	for(;i < amountX;++i){
		if(row[i].x > end) break;
		res[row[i].x - begin] = &row[i].item;
	}
	
	return res;
}

template<class T>
void Map<T>::removeItem(int x, int y, Item *item) {
	VertCell *vCell = nullptr;//100% that item will be there
	int iV, iH;
	for(iV = 0;iV < amountY;++iV){
		if(vertAr[iV].y == y){
			vCell = vertAr + iV;
			break;
		}
	}
	if(vCell == nullptr) throw std::exception();
	
	HorCell *&row = vCell->row, hCell = nullptr;
	int &amountX = vCell->amountX;
	for(iH = 0;iH < amountX;++iH){
		if(row[iH].x == x){
			hCell = row + iH;
			break;
		}
	}
	
	if(hCell == nullptr) throw std::exception();
	
	std::vector<T> &items = hCell.items;
	if(items.size() > 1){
		items.erase(std::find(items.begin(), items.end(), item));
	}
	else if(items.size() == 1){
		removeCell<HorCell>(iH, row, amountX);
		
		if(amountX == 0){
			removeCell<VertCell>(iV, vertAr, amountY);
		}
	}
	else throw std::exception();
}

template<class T>
void Map<T>::print(std::ostream &s) {
	for(int i = 0;i < amountY;++i){
		for(int j = 0;j < vertAr[i].amountX;++j){
			s << "x:" << vertAr[i].row[j].x;
			std::vector<T> &v = vertAr[i].row[j].items;
			for(int k = 0;k < v.size();++k){
				s << "Item " << k << ":" << v[k];
			}
			s << std::endl;
		}
	}
}

