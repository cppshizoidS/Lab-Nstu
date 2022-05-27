#include "gtest/gtest.h"

#include <array>

/* #define ITERATIVE */
#define RECURSIVE
#include "t234.hpp"


std::array<int, 10> items = {3, 1, 5, 4, 2, 9, 10, 8, 7, 6};

TEST(method, insert)
{
    t234 t;
    for (int item : items)
        ASSERT_EQ(item, *t.insert(item));
    ASSERT_EQ(t.size(), items.size());
}

TEST(method, find)
{
    t234 t;
    for (int item : items)
        t.insert(item);

    for (int item : items)
        ASSERT_EQ(*t.find(item), item);
}

TEST(method, erase)
{
    t234 t;
    for (int item : items)
        t.insert(item);

    auto iter = items.begin();
    for (size_t n = t.size(); t.size() > 0; --n, ++iter) {
        t.erase(t.find(*iter));
        ASSERT_EQ(t.size() + 1, n);
        ASSERT_EQ(t.find(*iter), t.end());
    }
    ASSERT_TRUE(t.empty());
}

TEST(method, clear)
{
    t234 t;
    for (int item : items)
        t.insert(item);
    t.clear();
    ASSERT_EQ(t.size(), 0);
    ASSERT_TRUE(t.empty());
}

TEST(iterator, forward)
{
    t234 t;
    for (int item : items)
        t.insert(item);

    int n = 0;
    for (auto it = t.begin(); it != t.end(); ++it)
        ASSERT_EQ(*it, ++n);
    ASSERT_EQ(n, t.size());
}

TEST(iterator, reverse)
{
    t234 t;
    for (int item : items)
        t.insert(item);

    int n    = t.size();
    auto rit = t.rbegin();
    for (; rit != t.rend(); --rit)
        ASSERT_EQ(*rit, n--);
    ASSERT_EQ(n, 0);
}


int
main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

