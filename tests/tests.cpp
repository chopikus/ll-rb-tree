#include "../src/set.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <set>
#include "testlib.h"

const int QUERIES = 100000;
const int A_LOT_OF_QUERIES = 1000000;
const int MAX_NUMBER = 1000000000;

using namespace llrb;

TEST(Set, InsertFindSimple) {
    Set<int> s;
    s.insert(3);
    EXPECT_FALSE(s.find(3) == s.end());
    EXPECT_EQ(s.find(2), s.end());
}

TEST(Set, InsertFindAdvanced) {
    std::set<int> std_set;
    Set<int> my_set;
    int n = QUERIES;
    while (n--) {
        int x = rnd.next(1000);
        my_set.insert(x);
        std_set.insert(x);
    }
    int queries = QUERIES;
    while (queries--) {
        int x = rnd.next(1000);
        bool stat1 = (my_set.find(x) != my_set.end());
        bool stat2 = std_set.count(x);
        if (stat1 != stat2) {
            stat1 = stat1;
        }
        EXPECT_EQ(stat1, stat2);
    }
}

TEST(Set, InsertFindErase) {
    Set<int> my_set;
    my_set.insert(3);
    EXPECT_TRUE(my_set.find(3) != my_set.end());
    EXPECT_FALSE(my_set.find(2) != my_set.end());
    my_set.insert(2);
    my_set.insert(4);
    my_set.insert(1);
    EXPECT_TRUE(my_set.find(2) != my_set.end());
    my_set.erase(2);
    EXPECT_TRUE(my_set.find(4) != my_set.end());
    EXPECT_FALSE(my_set.find(2) != my_set.end());
    my_set.erase(4);
    EXPECT_FALSE(my_set.find(4) != my_set.end());
    my_set.erase(1);
    EXPECT_FALSE(my_set.find(1) != my_set.end());
    EXPECT_TRUE(my_set.find(3) != my_set.end());
    my_set.erase(3);
}

TEST(Set, InsertFindEraseAdvanced) {
    std::set<int> std_set;
    Set<int> q;
    int n = 200;
    while (n--) {
        int x = rnd.next(1000);
        q.insert(x);
        std_set.insert(x);
    }
    int queries = 1000;
    while (queries--) {
        if (std_set.empty()) break;
        int x = rnd.next(1000);
        EXPECT_EQ((q.find(x) != q.end()), std_set.count(x));
        if (q.find(x) != q.end()) {
            q.erase(x);
            std_set.erase(x);
        }
    }
}

TEST(Set, LowerBoundSimple) {
    Set<int> my_set;
    my_set.insert(3);
    my_set.insert(4);
    my_set.insert(6);
    my_set.insert(8);
    my_set.insert(18);
    EXPECT_EQ(*my_set.lower_bound(2), 3);
    EXPECT_EQ(*my_set.lower_bound(3), 3);
    EXPECT_EQ(*my_set.lower_bound(4), 4);
    EXPECT_EQ(*my_set.lower_bound(5),  6);
    EXPECT_EQ(*my_set.lower_bound(6), 6);
    EXPECT_EQ(*my_set.lower_bound(7), 8);
    EXPECT_EQ(*my_set.lower_bound(8), 8);
    EXPECT_EQ(*my_set.lower_bound(9), 18);
    EXPECT_EQ(*my_set.lower_bound(18), 18);
}

TEST(Set, FullTest) {
    Set<int> my_set;
    EXPECT_TRUE(my_set.empty());
    my_set.insert(3);
    my_set.insert(1);
    my_set.insert(0);
    EXPECT_FALSE(my_set.find(0) == my_set.end());
    EXPECT_EQ(my_set.size(), 3);
    Set<int>::iterator it = my_set.begin();
    EXPECT_EQ(*it, 0);
    ++it;
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 3);
    it++;
    EXPECT_EQ(it, my_set.end());
    Set<int>::iterator it_lower = my_set.lower_bound(2);
    EXPECT_EQ(*it_lower, 3);
    EXPECT_EQ(my_set.lower_bound(100), my_set.end());
    --it;
    EXPECT_EQ(*it, 3);
    EXPECT_EQ(my_set.find(3), it);
    EXPECT_EQ(my_set.lower_bound(2), it);
    --it;
    EXPECT_EQ(my_set.find(1), it);
    EXPECT_EQ(my_set.lower_bound(1), it);
    --it;
    EXPECT_EQ(my_set.find(0), it);
}

TEST(Set, TaskSample) {
    Set<int> q;
    q.insert(3);
    q.insert(1);
    q.insert(0);
    auto it = q.begin();
    EXPECT_EQ(q.find(0), it);
    EXPECT_EQ(*q.lower_bound(2), 3);
    EXPECT_EQ(q.lower_bound(4), q.end());
    q.erase(*q.begin());
    q.insert(2);
    q.insert(2);
    q.erase(3);
    q.insert(13);
    EXPECT_EQ(*q.lower_bound(4), 13);
}

TEST(Set, FindSizeInEmpty) {
    Set<int> q;
    EXPECT_TRUE(q.find(3) == q.end());
    EXPECT_TRUE(q.lower_bound(3) == q.end());
    Set<int> z = q;
    EXPECT_EQ(z.size(), q.size());
    EXPECT_EQ(z.size(), 0);
    EXPECT_EQ(q.size(), 0);
}

TEST(Set, AllFunctions) {
    std::set<int> std_set;
    Set<int> my_set;
    int queries = A_LOT_OF_QUERIES;
    while (queries--) {
        int type = rnd.next(4);
        if (type == 0) {
            int x = rnd.next(1000);
            my_set.insert(x);
            std_set.insert(x);
        } else if (type == 1) {
            int x = rnd.next(1000);
            my_set.erase(x);
            std_set.erase(x);
        } else if (type == 2) {
            int x = rnd.next(1000);
            if (std_set.lower_bound(x) != std_set.end()) {
                EXPECT_EQ(*std_set.lower_bound(x), *my_set.lower_bound(x));
            } else {
                EXPECT_EQ(my_set.lower_bound(x), my_set.end());
            }
        } else if (type == 3) {
            int x = rnd.next(1000);
            if (std_set.find(x) != std_set.end()) {
                EXPECT_EQ(*std_set.find(x), *my_set.find(x));
            } else {
                EXPECT_EQ(my_set.find(x), my_set.end());
            }
        }
    }
}

TEST(Set, Iterators) {
    std::set<int> std_set;
    Set<int> my_set;
    int queries = QUERIES;
    while (queries--) {
        int x = queries;
        std_set.insert(x);
        my_set.insert(x);
    }
    auto my_iter = my_set.begin();
    auto std_iter = std_set.begin();
    while (my_iter != my_set.end()) {
        EXPECT_EQ(*my_iter, *std_iter);
        my_iter++;
        std_iter++;
    }
    ASSERT_EQ(std_iter, std_set.end());
}

TEST(Set, IteratorsDecrease) {
    std::set<int> std_set;
    Set<int> my_set;
    int queries = QUERIES;
    while (queries--) {
        std_set.insert(queries);
        my_set.insert(queries);
    }
    auto my_iter = --my_set.end();
    auto std_iter = --std_set.end();
    while (my_iter != my_set.begin()) {
        EXPECT_EQ(*my_iter, *std_iter);
        my_iter--;
        std_iter--;
    }
}

TEST(Set, EmptySet) {
    Set<int> q;
    EXPECT_EQ(q.find(3), q.end());
    EXPECT_EQ(q.lower_bound(3), q.end());
    Set<int> z = q;
    EXPECT_EQ(z.begin(), z.end());
}

TEST(Set, CopySet) {
    Set<int> q{1, 2, 3};
    q = q;
    EXPECT_EQ(q.size(), 3);
}

TEST(Set, IteratorsSecond) {
    std::set<int> std_set;
    Set<int> my;
    int queries = QUERIES;
    while (queries--) {
        int x = rnd.next(MAX_NUMBER);
        std_set.insert(x);
        my.insert(x);
    }
    auto my_iter = my.begin();
    auto std_iter = std_set.begin();
    while (my_iter != my.end() && std_iter != std_set.end()) {
        EXPECT_EQ(*my_iter, *std_iter);
        my_iter++;
        std_iter++;
    }
    queries = QUERIES;
    while (queries--) {
        int x = rnd.next(MAX_NUMBER);
        std_set.insert(x);
        my.insert(x);
    }
    while (my_iter != my.end()) {
        EXPECT_EQ(*my_iter, *std_iter);
        my_iter++;
        std_iter++;
    }
    ASSERT_EQ(std_iter, std_set.end());
}

int main(int argc, char** argv) {
    registerGen(argc, argv, 1);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

