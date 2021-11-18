#include "gtest/gtest.h"
#include "Level.h"

initLevel(Level &level){

}

TEST(test,test){
	ASSERT_EQ(1,1);
}

int _tmain()
{
	::testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}
