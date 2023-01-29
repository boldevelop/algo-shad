#include <catch.hpp>
#include <set.h>
#include <util.h>

// TEST_CASE("Treap", "Simple") {
//     SECTION("Height") {
//         std::vector<int> data(100000);
//         std::iota(data.begin(), data.end(), 1);

//         simple::Treap treap;

//         Log l;
//         for (auto num : data) {
//             treap.Insert(num);
//         }
//         auto dur = l.GetDuration();
//         std::cout << treap.GetHeight() << ": " << dur << std::endl;
//         REQUIRE(treap.GetHeight() < 50);
//     }

//     SECTION("Correctness") {
//         std::vector<int> data(15);
//         std::iota(data.begin(), data.end(), 1);
//         RandomGenerator RandGen;

//         for (int i = 0; i < 5; ++i) {
//             simple::Treap treap;

//             for (auto num : data) {
//                 treap.Insert(num);
//                 REQUIRE(treap.Find(num)->key == num);
//                 if (i == 0) {
//                     REQUIRE(!treap.UpperBound(num));
//                     REQUIRE(treap.UpperBound(num - 1)->key == num);
//                 }
//             }

//             for (auto num : data) {
//                 REQUIRE(!treap.Find(num + 15));
//                 REQUIRE(treap.Find(num)->key == num);
//                 REQUIRE(!treap.Find(num - 15));
//                 treap.Remove(num + 16);
//             }

//             for (auto num : data) {
//                 treap.Remove(num);
//                 REQUIRE(!treap.Find(num));
//             }
//             REQUIRE(treap.GetHeight() == 0);
//             RandGen.Shuffle(data.begin(), data.end());
//         }
//     }

//     SECTION("Stress") {
//         std::vector<int> data(100000);
//         std::iota(data.begin(), data.end(), 1);

//         simple::Treap treap;

//         Log logger;
//         for (auto num : data) {
//             Log l;
//             treap.Insert(num);
//             REQUIRE(treap.Find(num)->key == num);
//             REQUIRE(!treap.UpperBound(num));
//             REQUIRE(treap.UpperBound(num - 1)->key == num);
//             auto dur = l.GetDuration();
//             if (dur > 500) {
//                 std::cout << treap.GetHeight() << ": " << dur << std::endl;
//             }
//         }
//         auto dur = logger.GetDuration();

//         std::cout << "All op: " << dur << std::endl;
//     }
// }

template<class T>
void Correctness(Set<T>& set, std::vector<T>& data) {
    for (auto num : data) {
        set.insert(num);
        REQUIRE(set.size() == 15);
        REQUIRE(set.find(num) == num);
        REQUIRE(set.lower_bound(num) == num);
        if (num == 1) {
            REQUIRE(set.lower_bound(num - 1) == num);
        } else {
            REQUIRE(set.lower_bound(num - 1) == num - 1);
        }
    }

    for (auto num : data) {
        REQUIRE(set.find(num + 15) == -1);
        REQUIRE(set.find(num) == num);
        REQUIRE(set.find(num - 15) == - 1);
        set.erase(num + 16);
        REQUIRE(set.size() == 15);
    }

    int size = 14;
    for (auto num : data) {
        set.erase(num);
        REQUIRE(set.find(num) == -1);
        REQUIRE(set.size() == size--);
    }
    REQUIRE(set.empty());
}

TEST_CASE("set") {
    // SECTION("Correctness") {
    //     std::vector<int> data(15);
    //     std::iota(data.begin(), data.end(), 1);
    //     RandomGenerator RandGen;

    //     for (int i = 0; i < 5; ++i) {
    //         Set<int> set;

    //         for (auto num : data) {
    //             set.insert(num);
    //             REQUIRE(set.find(num) == num);
    //             if (i == 0) {
    //                 REQUIRE(set.lower_bound(num) == num);
    //                 REQUIRE(set.lower_bound(num + 1) == -1);
    //                 if (num == 1) {
    //                     REQUIRE(set.lower_bound(num - 1) == num);
    //                 } else {
    //                     REQUIRE(set.lower_bound(num - 1) == num - 1);
    //                 }
    //             }
    //         }

    //         for (auto num : data) {
    //             REQUIRE(set.find(num + 15) == -1);
    //             REQUIRE(set.find(num) == num);
    //             REQUIRE(set.find(num - 15) == - 1);
    //             set.erase(num + 16);
    //         }

    //         int size = 14;
    //         for (auto num : data) {
    //             set.erase(num);
    //             REQUIRE(set.find(num) == -1);
    //             REQUIRE(set.size() == size--);
    //         }
    //         REQUIRE(set.empty());
    //         RandGen.Shuffle(data.begin(), data.end());
    //     }
    // }

    SECTION("Correctness init list") {
        std::vector<int> data(15);
        std::iota(data.begin(), data.end(), 1);
        Set<int> set{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        Correctness(set, data);
    }

    SECTION("Correctness array list") {
        std::vector<int> data(15);
        std::iota(data.begin(), data.end(), 1);
        Set<int> set(data.begin(), data.end());
        Correctness(set, data);
    }
}

TEST_CASE("set copy") {
    int elems[] = {3, 3, -3, 6, 0, 0, 17, -5, 4, 2};
    Set<int> s1(elems, elems + 10);
    Set<int> s2 = s1;
    s2.insert(5);
    s2.insert(18);
    s2.insert(-2);

    REQUIRE(s1.find(5) == -1);
    REQUIRE(s1.find(18) == -1);
    REQUIRE(s1.find(-2) == -1);
    for (auto num : elems) {
        REQUIRE(s1.find(num) == num);
        REQUIRE(s2.find(num) == num);
    }

    s1.erase(0);
    REQUIRE(s1.find(0) == -1);
    REQUIRE(s2.find(0) == 0);
}
