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
    std::vector<int> data(5);
    std::iota(data.begin(), data.end(), 1);

    Treap treap;

    for (auto num : data) {
        treap.Insert(num);
        treap.Insert(num);
    }
    treap.Print();
    // std::cout << treap.GetHeight() << std::endl;
    REQUIRE(true);
}
