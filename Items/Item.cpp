#include "Item.h"

namespace nodata{
	Item::Item(std::string &name, int weight) : name_(name) , weight_(weight) {}
	
	void Item::setWeight(int weight) {
		weight_ = weight;
	}
}
