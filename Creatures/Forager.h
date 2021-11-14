#ifndef LAB4_FORAGER_H
#define LAB4_FORAGER_H

class Forager : public Creature {
private:
	Table itemTable_;
	int force_;
public:
	Forager(std::string &name, Point &coord, int force);
	
	~Forager() override	= default;
	
	int getForce() const { return force_; }
	
	void kill(Level *level) override;
	ErrorCodes receiveItem(Item *item);
	ErrorCodes throwItem(Level *level, int index);
	void dropAllItems(Level *level);
	
	void drawCell(sf::RectangleShape &shape) override;
};

#endif //LAB4_FORAGER_H
