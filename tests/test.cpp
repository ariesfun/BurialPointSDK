#include "gtest/gtest.h"
#include "include/buried.h"

TEST(BuriedBasicTest, Test1)
{
    EXPECT_EQ(BuriedTest(), 1);

    Buried_Create("E:/CPP ResumeProjects/LogTest");
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}