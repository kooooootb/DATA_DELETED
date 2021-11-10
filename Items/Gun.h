#ifndef LAB4_GUN_H
#define LAB4_GUN_H

#include "Item.h"
#include "Operative.h"

class Gun : public Item {
private:
	int damage_;
	int shootTime_;
	int reloadTime_;
	Ammunition ammoType_;
	int ammoCurrent_;
	int ammoMax_;
	int accuracy_;
	
	int calcAmmoWeightByType(int amount) const;
	int calcAmountByType() const;
public:
	
	Gun(std::string &name, int weight, int damage, int shootTime, int reloadTime, Ammunition ammoType, int ammoMax, int accuracy);
	
	~Gun() override = default;
	
	ErrorCodes use(Creature *creature) override;//take gun in hands
	
	Ammunition getAmmoType() const { return ammoType_; }
	int getAmmoMax() const { return ammoMax_; }
	int getAmmoCurrent() const { return ammoCurrent_; };
	
	int getReloadTime() const { return reloadTime_; }
	
	void setAmmoCurrent(int ammoCurrent);
	static void receiveDamage(Level *level, int damage, Operative *operative);
	void shoot(Level *level, Creature *victim, float hitsMultipl);
	
	float countHits(int operAccuracy) const;
//	void reload();//искать в таблице контейнер и использовать первый попавшийся?
};


#endif //LAB4_GUN_H
