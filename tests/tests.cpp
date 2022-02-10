//
// Created by Asus on 25.01.2022.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "../library/set.cpp"
#include<algorithm>
#include<random>
#include<set>


TEST_CASE("add_contains_simple") {
    Set<int> s;
    s.add(3);
    REQUIRE(s.contains(3));
    REQUIRE(!s.contains(2));
}

TEST_CASE("add_contains_advanced") {
    std::set<int> rightRealisation;
    Set<int> q;
    int n = 200;
    while (n--) {
        int x = rand() % 1000;
        q.add(x);
        rightRealisation.insert(x);
    }
    int queries = 1000;
    while (queries--) {
        int x = rand() % 1000;
        bool stat1 = q.contains(x);
        bool stat2 = rightRealisation.count(x);
        if (stat1 != stat2) {
            q.contains(x);
            stat1 = stat1;
        }
        //REQUIRE(q.getHonestSize() == 100);
        REQUIRE(stat1 == stat2);
    }
}

TEST_CASE("add_contains_erase") {
    Set<int> s;
    s.add(3);
    REQUIRE(s.contains(3));
    REQUIRE(!s.contains(2));
    s.add(2);
    s.add(4);
    s.add(1);
    REQUIRE(s.contains(2));
    s.erase(2);
    REQUIRE(s.contains(4));
    REQUIRE(!s.contains(2));
    s.erase(4);
    REQUIRE(!s.contains(4));
    s.erase(1);
    REQUIRE(!s.contains(1));
    REQUIRE(s.contains(3));
    s.erase(3);
}

TEST_CASE("add_contains_erase_advanced") {
    {
        std::set<int> rightRealisation;
        Set<int> q;
        int n = 200;
        while (n--) {
            int x = rand() % 1000;
            q.add(x);
            rightRealisation.insert(x);
        }
        int queries = 1000;
        while (queries--) {
            if (rightRealisation.empty()) break;
            int x = rand() % 1000;
            REQUIRE(q.contains(x) == rightRealisation.count(x));
            if (q.contains(x)) {
                q.erase(x);
                rightRealisation.erase(x);
            }
        }
    }
}

TEST_CASE("remove_node_test") {
    Set<int> q;
    q.root = new Set<int>::Node(3);
    q.root->left = new Set<int>::Node(2);
    q.root->left->parent = q.root;
    q.root->height = 1;
    q.root = q.removeMinVertex(q.root);
    REQUIRE(q.root->key == 3);
    REQUIRE(!q.root->left);
    REQUIRE(!q.root->right);
}

TEST_CASE("remove_one_node") {
    Set<int> q;
    q.root = new Set<int>::Node(3);
    q.root = q.removeMinVertex(q.root);
    REQUIRE(!q.root);
}

TEST_CASE("remove_one_check_right") {
    Set<int> q;
    q.root = new Set<int>::Node(4);
    q.root->left = new Set<int>::Node(2);
    q.root->left->parent = q.root;
    q.root->left->right = new Set<int>::Node(3);
    q.root->left->right->parent = q.root->left;
    q.root->height = 2;
    q.root->left->height = 1;
    q.root = q.removeMinVertex(q.root);
    REQUIRE(q.root->height == 1);
    REQUIRE(q.root->left);
    REQUIRE(q.root->key == 4);
    REQUIRE(q.root->left->key == 3);
    REQUIRE(q.root->left->parent == q.root);
}

TEST_CASE("lower_bound_simple") {
    Set<int> q;
    q.add(3);
    q.add(4);
    q.add(6);
    q.add(8);
    q.add(18);
    REQUIRE(*q.lower_bound(2) == 3);
    REQUIRE(*q.lower_bound(3) == 3);
    REQUIRE(*q.lower_bound(4) == 4);
    REQUIRE(*q.lower_bound(5) == 6);
    REQUIRE(*q.lower_bound(6) == 6);
    REQUIRE(*q.lower_bound(7) == 8);
    REQUIRE(*q.lower_bound(8) == 8);
    REQUIRE(*q.lower_bound(9) == 18);
    REQUIRE(*q.lower_bound(18) == 18);
}

TEST_CASE("full_set_test") {
    Set<int> q;
    REQUIRE(q.empty());
    q.insert(3);
    q.insert(1);
    q.insert(0);
    REQUIRE(q.contains(0));
    REQUIRE(q.size() == 3);
    Set<int>::iterator it = q.begin();
    REQUIRE(*it == 0);
    ++it;
    REQUIRE(*it == 1);
    ++it;
    REQUIRE(*it == 3);
    it++;
    //Set<int>::iterator endIter = q.end();
    REQUIRE(it == q.end());
    Set<int>::iterator itlower = q.lower_bound(2);
    REQUIRE(*itlower == 3);
    REQUIRE(q.lower_bound(100) == q.end());
    --it;
    REQUIRE(*it == 3);
    REQUIRE(q.find(3) == it);
    REQUIRE(q.lower_bound(2) == it);
    --it;
    REQUIRE(q.find(1) == it);
    REQUIRE(q.lower_bound(1) == it);
    --it;
    REQUIRE(q.find(0) == it);
}

TEST_CASE("sample") {
    Set<int> q;
    q.insert(3);
    q.insert(1);
    q.insert(0);
    auto it = q.begin();
    REQUIRE(q.find(0) == it);
    REQUIRE(*q.lower_bound(2) == 3);
    REQUIRE(q.lower_bound(4) == q.end());
    q.erase(*q.begin());
    q.insert(2);
    q.insert(2);
    q.erase(3);
    q.insert(13);
    REQUIRE(*q.lower_bound(4) == 13);
}

TEST_CASE("find in empty") {
    Set<int> q;
    REQUIRE(q.find(3) == q.end());
    REQUIRE(q.lower_bound(3) == q.end());
    Set<int> z = q;
}

TEST_CASE("all_functions") {
    std::set<int> right;
    Set<int> my;
    int queries = 6e4;
    while (queries--) {
        int type = rand() % 4;
        if (type == 0) { //add
            int x = rand() % 1000;
            my.insert(x);
            right.insert(x);
        }
        if (type == 1) {
            int x = rand() % 1000;
            my.erase(x);
            right.erase(x);
        }
        if (type == 2) {
            int x = rand() % 1000;
            if (right.lower_bound(x) != right.end()) {
                REQUIRE(*right.lower_bound(x) == *my.lower_bound(x));
            } else {
                REQUIRE(my.lower_bound(x) == my.end());
            }
        }
        if (type == 3) {
            int x = rand() % 1000;
            if (right.find(x) != right.end()) {
                REQUIRE(*right.find(x) == *my.find(x));
            } else {
                REQUIRE(my.find(x) == my.end());
            }
        }
    }
}

TEST_CASE("iterators") {
    std::set<int> right;
    Set<int> my;
    int queries = 10000;
    while (queries--) {
        int x = queries;
        right.insert(x);
        my.insert(x);
    }
    auto myiter = my.begin();
    auto rightiter = right.begin();
    while (myiter != my.end()) {
        REQUIRE(*myiter == *rightiter);
        myiter++;
        rightiter++;
    }
    REQUIRE(rightiter == right.end());
}

TEST_CASE("iterators decrease") {
    std::set<int> right;
    Set<int> my;
    int queries = 10000;
    while (queries--) {
        int x = queries;
        right.insert(x);
        my.insert(x);
    }
    auto myiter = --my.end();
    auto rightiter = --right.end();
    while (myiter != my.begin()) {
        REQUIRE(*myiter == *rightiter);
        myiter--;
        rightiter--;
    }
    //REQUIRE(rightiter == right.end());
}

TEST_CASE("empty seet") {
    Set<int> q;
    REQUIRE(q.find(3) == q.end());
    REQUIRE(q.lower_bound(3) == q.end());
    Set<int> z = q;
    REQUIRE(z.begin() == z.end());
}

TEST_CASE("iterators 2") {
    std::set<int> right;
    Set<int> my;
    int queries = 10000;
    while (queries--) {
        int x = rand();
        right.insert(x);
        my.insert(x);
    }
    auto myiter = my.begin();
    auto rightiter = right.begin();
    while (myiter!=my.end() && rightiter!=right.end()) {
        REQUIRE(*myiter == *rightiter);
        myiter++;
        rightiter++;
    }
    queries = 10000;
    while (queries--) {
        int x = rand();
        right.insert(x);
        my.insert(x);
    }
    while (true) {
        REQUIRE(*myiter == *rightiter);
        myiter++;
        rightiter++;
    }
    REQUIRE(rightiter == right.end());
}
