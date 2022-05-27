#include "gtest/gtest.h"

#define RECURSIVE
#include "randtree.hpp"


TEST(method, insert)
{
    randtree<int> rt;
    
    int item = 0;
    while (item < 10)
        ASSERT_EQ(*rt.insert(++item), item);

    ASSERT_EQ(rt.size(), item);

    item = 0;
    for (int n : rt)
        ASSERT_EQ(n, ++item);
}

TEST(method, erase)
{
    randtree<int> rt;
    
    int item = 0;
    while (item < 10)
        rt.insert(++item);

    item = 0;
    for (int n : rt)
        ASSERT_EQ(n, ++item);

    rt.erase(rt.find(7));
    ASSERT_EQ(rt.size(), 9);

    item = 0;
    for (int n : rt) {
        ASSERT_EQ(n, ++item);
        if (item + 1 == 7)
            ++item;
    }
}


int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

