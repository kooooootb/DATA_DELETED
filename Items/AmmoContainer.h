#ifndef LAB4_AMMOCONTAINER_H
#define LAB4_AMMOCONTAINER_H

class AmmoContainer : public Item {
private:
	Ammunition ammoType_;
	int ammoCurrent_;
	int ammoMax_;
	
	int calcAmmoWeightByType(int amount);
public:
	AmmoContainer(std::string &name, int weight, Ammunition ammoType = MEDIUM_CALIBER, int ammoMax = 120);
	
	~AmmoContainer() override = default;
	
	Ammunition getAmmoType() const { return ammoType_; }
	int getAmmoCurrent() const { return ammoCurrent_; }
	int getAmmoMax() const { return ammoMax_; }
	void setAmmoType(Ammunition);
	void setAmmoCurrent(int);
	void setAmmoMax(int);
	ErrorCodes use(Creature *creature) override;
};

#endif //LAB4_AMMOCONTAINER_H
