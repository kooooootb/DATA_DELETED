#ifndef LAB4_OPERATIVE_H
#define LAB4_OPERATIVE_H

class Operative : public Creature {
private:
	float reloadTime_;
	int force_;
	int accuracy_;
	Table itemTable_;
	Gun *activeGun_;
public:
	Operative(std::string &name, Point &coord, float reloadTime, int force, int accuracy);
	
	~Operative() override;
	
	Gun *getActiveGun() const { return activeGun_; }
	int getForce() const { return force_;}
	int getAccuracy() const { return accuracy_; }
	float getReloadTime() const { return reloadTime_; };
	Table &getTable() { return itemTable_; }
	
	void setActiveGun(Gun *gun);

//	void switchGun(int index);
	void kill(Level *level) override;
	ErrorCodes receiveItem(Item *item);
	void throwItem(Level *level,int index);
	void useItem(int index);
	void shoot(Level *level, Creature *victim);
	void dropAllItems(Level *level);
	
	void drawCell(sf::RectangleShape &shape) override;
};

#endif //LAB4_OPERATIVE_H
