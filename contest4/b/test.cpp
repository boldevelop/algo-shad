#include <catch.hpp>
#include <main.hpp>
#include <util.h>

TEST_CASE("Treap Height") {
    std::vector<int> data(50000);
    std::iota(data.begin(), data.end(), 1);

    Treap treap;

    Log l;
    for (auto num : data) {
        treap.Insert(num);
        treap.Insert(num);
    }
    auto dur = l.GetDuration();
    std::cout << treap.GetHeight() << ": " << dur << std::endl;
    REQUIRE(treap.GetHeight() < 50);
}

TEST_CASE("Treap Insert") {
    std::vector<int> data(15);
    std::iota(data.begin(), data.end(), 1);

    Treap treap;

    for (auto num : data) {
        // treap.Insert(num);
        treap.Insert(num);
    }
    treap.PrintData();
    treap.Print();
    // treap.Remove(1);
    for (auto num : data) {
        std::cout << "REMOVE: " << num << std::endl;
        treap.Remove(num);
        treap.PrintData();
    }
    // treap.PrintData(treap.Find(2));
    // treap.PrintData(treap.Find(3));
    // treap.PrintData(treap.Find(4));
    // treap.PrintData(treap.Find(5));
    // treap.PrintData(treap.Find(6));
    std::cout << treap.GetHeight() << std::endl;
    REQUIRE(true);
}

TEST_CASE("Treap Insert 2") {
    std::vector<int> data{1, 2, 2, 2, 2};

    Treap treap;

    for (auto num : data) {
        treap.Insert(num);
    }
    treap.Print();
    // std::cout << treap.GetHeight() << std::endl;
    REQUIRE(true);
}
