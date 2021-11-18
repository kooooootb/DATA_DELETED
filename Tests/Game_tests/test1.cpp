#include "gtest/gtest.h"
#include "Level.h"
#include "Parameters.h"

nodata::Operative *initOperative(nodata::Level &level){
	std::string name = "test";
	Point coord(1,1);
	int i = (int)level.getCurrentTeam().size();
	level.spawnOperator(name, coord, 10,100,10,10,1,1000,1);
	level.setActive(i);
	auto *oper = dynamic_cast<nodata::Operative*>(level.getCurrentTeam()[i]);

	return oper;
}

nodata::Gun *initGun(){
	std::string name = "test";
	auto *gun = new nodata::Gun(name, 10,10,10,10,SMALL_CALIBER,20,1,10);

	return gun;
}

nodata::AmmoContainer *initACont(){
	std::string name = "test";
	auto *acont = new nodata::AmmoContainer(name, 10, SMALL_CALIBER, 60);

	return acont;
}

nodata::HealthKit *initHKit(){
	std::string name = "test";
	auto *hkit = new nodata::HealthKit(name, 10, 10, 10);

	return hkit;
}

TEST(Creatures, walk){
	nodata::Level level("testCells.cfg");
	nodata::Operative *oper = initOperative(level);
	Point coord(1,1);
	oper->walk(LEFT);//wall
	ASSERT_EQ(coord.x, oper->getPosition().x);
	ASSERT_EQ(coord.y, oper->getPosition().y);
	oper->walk(DOWN);//success
	ASSERT_EQ(coord.x, oper->getPosition().x);
	ASSERT_EQ(coord.y + 1, oper->getPosition().y);
	oper->setTimeMax(0);
	oper->walk(RIGHT);//no time
	ASSERT_EQ(coord.x, oper->getPosition().x);
	ASSERT_EQ(coord.y + 1, oper->getPosition().y);
	oper = initOperative(level);
	oper->walk(UP);//wall
	ASSERT_EQ(coord.x, oper->getPosition().x);
	ASSERT_EQ(coord.y, oper->getPosition().y);
	oper->walk(RIGHT);//success
	ASSERT_EQ(coord.x + 1, oper->getPosition().x);
	ASSERT_EQ(coord.y, oper->getPosition().y);
}

TEST(Creatures, heal) {
	nodata::Level level("testCells.cfg");
	nodata::Operative *oper = initOperative(level);
	Point coord(1, 1);
	
	oper->setHealthCurrent(10);
	oper->setHealthMax(20);
	ASSERT_EQ(oper->getHealthCurrent(), 10);
	oper->heal(10);
	ASSERT_EQ(oper->getHealthCurrent(), 20);
	oper->heal(10);
	ASSERT_EQ(oper->getHealthCurrent(), 20);
}

TEST(Creatures, kill) {
	nodata::Level level("testCells.cfg");
	nodata::Operative *oper = initOperative(level);
	Point coord(1, 1);
	ASSERT_EQ(level.getCurrentTeam().size(), 2);
	oper->kill();
	ASSERT_EQ(level.getCurrentTeam().size(), 1);
}

TEST(Creatures, spendTime) {
	nodata::Level level("testCells.cfg");
	nodata::Operative *oper = initOperative(level);
	Point coord(1, 1);
	int time = oper->getTimeCurrent();
	oper->walk(LEFT);
	ASSERT_EQ(oper->getTimeCurrent(), time);
	oper->walk(RIGHT);
	ASSERT_EQ(oper->getTimeCurrent(), time - oper->getWalkTime());
	
	oper = initOperative(level);
	auto *gun = initGun();
	oper->receiveItem(gun);
	oper->useItem(0);
	time = oper->getTimeCurrent();
	oper->shoot(level.getCurrentTeam().front());
	ASSERT_EQ(oper->getTimeCurrent(), time - AMOUNTSMALL * oper->getActiveGun()->getShootTime());
	nodata::AmmoContainer *acont = initACont();
	oper->receiveItem(acont);
	time = oper->getTimeCurrent();
	oper->useItem(0);
	ASSERT_EQ(oper->getTimeCurrent(), time - (oper->getActiveGun()->getReloadTime() * oper->getReloadTimeMultipl()));
	nodata::HealthKit *hkit = initHKit();
	oper->receiveItem(hkit);
	time = oper->getTimeCurrent();
	oper->useItem(1);
	ASSERT_EQ(oper->getTimeCurrent(), time - hkit->getHealTime());
}

TEST(Creature, force){
	nodata::Level level("testCells.cfg");
	nodata::Operative *oper = initOperative(level);
	Point coord(1, 1);
	
	oper->setForce(10);
	nodata::AmmoContainer *acont = initACont();
	ASSERT_EQ(oper->receiveItem(acont), ERROR);
}

TEST(Creature, receivedamage){
	nodata::Level level("testCells.cfg");
	nodata::Operative *oper = initOperative(level);
	
	oper->setHealthCurrent(100);
	oper->receiveDamage(10);
	ASSERT_EQ(oper->getHealthCurrent(), 90);
	
	oper->receiveDamage(1000);
	ASSERT_EQ(level.getCurrentTeam().size(), 1);
}

TEST(Creature, throwItem){
	nodata::Level level("testCells.cfg");
	nodata::Operative *oper = initOperative(level);
	
	nodata::Gun *gun = initGun();
	oper->receiveItem(gun);
	ASSERT_EQ(oper->getTable().getVector().size(), 1);
	oper->throwItem(0);
	ASSERT_EQ(oper->getTable().getVector().size(), 0);
}

TEST(Map, add_receive){
	nodata::Level level("testCells.cfg");
	nodata::Operative *oper = initOperative(level);
	Point point(1,1);
	nodata::Gun *gun = initGun();
	oper->receiveItem(gun);
	oper->throwItem(0);
	nodata::Item *item = level.getItemMap()[point][0][0];
	ASSERT_EQ(item, gun);
	oper->receiveItem(gun);
	level.getItemMap().removeItem(point, gun);
	std::vector<nodata::Item*>*nitem = level.getItemMap()[point];
	ASSERT_EQ(nitem, nullptr);
}

TEST(Item, Gun_shoot){
	nodata::Level level("testCells.cfg");
	nodata::Operative *oper = initOperative(level);
	nodata::Operative *victim = initOperative(level);
	Point point(1,1);
	
	nodata::Gun *gun = initGun();
	oper->receiveItem(gun);
	oper->useItem(0);
	ASSERT_EQ(level.getCurrentTeam().size(), 3);
	oper->shoot(victim);
	ASSERT_EQ(level.getCurrentTeam().size(), 2);
}

TEST(Item, ACont_use){
	nodata::Level level("testCells.cfg");
	nodata::Operative *oper = initOperative(level);
	nodata::Operative *victim = initOperative(level);
	Point point(1,1);
	
	nodata::Gun *gun = initGun();
	oper->receiveItem(gun);
	oper->useItem(0);
	oper->shoot(victim);
	ASSERT_EQ(oper->getActiveGun()->getAmmoMax() - AMOUNTSMALL, oper->getActiveGun()->getAmmoCurrent());
	nodata::AmmoContainer *acont = initACont();
	oper->receiveItem(acont);
	oper->useItem(0);
	ASSERT_EQ(oper->getActiveGun()->getAmmoMax(), oper->getActiveGun()->getAmmoCurrent());
}

TEST(Item, Gun_use){
	nodata::Level level("testCells.cfg");
	nodata::Operative *oper = initOperative(level);
	nodata::Operative *victim = initOperative(level);
	Point point(1,1);
	
	ASSERT_EQ(oper->getActiveGun(), nullptr);
	nodata::Gun *gun = initGun();
	oper->receiveItem(gun);
	oper->useItem(0);
	ASSERT_EQ(oper->getActiveGun(), gun);
}

TEST(Item, HKit_use) {
	nodata::Level level("testCells.cfg");
	nodata::Operative *oper = initOperative(level);
	Point coord(1, 1);
	
	oper->setHealthCurrent(100);
	oper->receiveDamage(10);
	ASSERT_EQ(oper->getHealthCurrent(), oper->getHealthMax() - 10);
	nodata::HealthKit *hkit = initHKit();
	oper->receiveItem(hkit);
	oper->useItem(0);
	ASSERT_EQ(oper->getHealthCurrent(), oper->getHealthMax());
}

int _tmain()
{
	std::ofstream fs("testCells.cfg", std::ios::trunc);
	fs << 10 << 10;
	fs.close();
	
	::testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}
