#ifndef LAB4_MAP_H
#define LAB4_MAP_H

#include "../AllClasses.h"

template<class T>
class Map {
private:
	std::map<int, std::map<int, std::vector<T>>> vertAr;
public:
	Map() = default;
	
	~Map();
	
	void addItem(const Point &point, T item);
	void addItem(int x, int y, T item);
	
	void removeItem(const Point &point, T item);
	void removeItem(int x, int y, T item);
	
	std::vector<const std::vector<T>*> getLine(int y, int begin, int end) const;
	std::vector<T>* getItem(int x, int y);
	
	void print(std::ostream &s);
};


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
//		delete v[0];
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
//		delete v[0];
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
std::vector<T> *Map<T>::getItem(int x, int y) {
	if(vertAr.count(y) == 0) return nullptr;
	if(vertAr.at(y).count(x) == 0) return nullptr;
	
	return &vertAr.at(y).at(x);
}

#endif //LAB4_MAP_H
