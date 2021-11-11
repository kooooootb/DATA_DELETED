#ifndef LAB4_SENTIENT_H
#define LAB4_SENTIENT_H

class Sentient : public Creature {
private:
	Gun* activeGun_;
	int accuracy_;
public:
	Sentient(std::string &name, Point &coord, int accuracy);
	
	~Sentient() override;
	
	void receiveGun(Gun*);
	void throwGun(Level *level);
	void shoot(Level *level, Creature *victim);
	void kill(Level *level) override;
};

#endif //LAB4_SENTIENT_H
