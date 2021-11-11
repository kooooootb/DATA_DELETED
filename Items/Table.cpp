#include "Item.h"

Table::~Table() {
	for(auto it = items_.begin();it != items_.end();it++){
		delete *it;
	}
}

int Table::getAmount() const {
	return (int) items_.size();
}

void Table::addItem(Item *item){
	items_.push_back(item);
	weight_ += item->getWeight();
}

void Table::setItem(int num, Item *item) {
	if(num < items_.size()){
		delete items_[num];
		items_[num] = item;
	}
}

Item * Table::getItem(int num) {
	return items_[num];
}

int Table::getWeight() const {
	return weight_;
}

Item *Table::deleteItem(int index) {
	Item *item = items_[index];
	if(item == nullptr) return nullptr;
	
	weight_ -= item->getWeight();
	items_.erase(items_.begin() + index);
	return item;
}

void Table::changeWeight(int diff) {
	weight_ += diff;
}

void Table::dropAll(Level *level, Point &point) {
	//
}
