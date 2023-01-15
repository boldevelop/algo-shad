#include <main.hpp>
#include <catch.hpp>
#include <util.h>
#include <functional>

void FillHeap(Heap* p) {
    for (int i = 10; i > 0; --i) {
        p->Add({10 - i, i});
        REQUIRE(p->Min() == i);
    }
}

TEST_CASE("Heap Construct from vector") {
    Heap p;
    RandomGenerator rand_gen;
    std::vector<int> iota_v(10);
    std::iota(iota_v.begin(), iota_v.end(), 1);

    std::vector<HeapItem> data;
    for (int i = 0; i < static_cast<int>(iota_v.size()); ++i) {
        data.push_back({i, iota_v[i]});
    }

    int count = 10;
    while (count > 0) {
        rand_gen.Shuffle(data.begin(), data.end());
        Heap p(data);
        REQUIRE(p.Min() == 1);
        REQUIRE(p.MapSize() == 10);

        for (int i = 0; i < 10; ++i) {
            auto min = p.ExtractMin();
            REQUIRE(min == i + 1);
            REQUIRE(p.MapSize() == 10 - (i + 1));
        }
        --count;
    }
}

TEST_CASE("Heap extract min") {
    {
        Heap p;

        for (int i = 1; i <= 10; ++i) {
            p.Add({i - 1, i});
            REQUIRE(p.Min() == 1);
        }

        for (int i = 0; i < 10; ++i) {
            auto min = p.ExtractMin();
            REQUIRE(min == i + 1);
        }
        REQUIRE(p.Empty());

        p.Add({0, 4});
        REQUIRE(p.Min() == 4);

        p.Add({1, 6});
        REQUIRE(p.Min() == 4);

        p.Add({2, 1});
        REQUIRE(p.Min() == 1);
    }

    Heap p;
    FillHeap(&p);

    for (int i = 0; i < 10; ++i) {
        auto min = p.ExtractMin();
        REQUIRE(min == i + 1);
    }
    REQUIRE(p.Empty());
}

// TEST_CASE("Heap remove") {
//     Heap p;
//     FillHeap(&p);

//     for (int i = 0; i < 9; ++i) {
//         p.Remove(1);
//         REQUIRE(p.Min() == 1);
//     }
//     auto min = p.ExtractMin();
//     REQUIRE(min == 1);
//     REQUIRE(p.Empty());
// }

TEST_CASE("Heap k statistics") {
    {
        int k = 2;
        Heap p({{0, 4}});

        p.Add({1, 2});
        REQUIRE(p.GetKStatistic(k) == 4);

        p.Add({3, 1});
        REQUIRE(p.GetKStatistic(k) == 2);

        p.Remove({0, 4});
        REQUIRE(p.GetKStatistic(k) == 2);

        p.Remove({1, 2});
        REQUIRE(p.GetKStatistic(k) == -1);
    }

    {
        int k = 1;
        Heap p({{0, 1}});

        p.Add({1, 2});
        REQUIRE(p.GetKStatistic(k) == 1);

        p.Remove({0, 1});
        REQUIRE(p.GetKStatistic(k) == 2);

        p.Add({2, 3});
        REQUIRE(p.GetKStatistic(k) == 2);

        p.Remove({1, 2});
        REQUIRE(p.GetKStatistic(k) == 3);

        p.Add({3, 4});
        REQUIRE(p.GetKStatistic(k) == 3);

        p.Remove({2, 3});
        REQUIRE(p.GetKStatistic(k) == 4);
    }
}

TEST_CASE("Inplace Head sort") {
    RandomGenerator rand_gen;
    std::vector<int> correct(9);
    std::iota(correct.begin(), correct.end(), 1);
    std::sort(correct.begin(), correct.end(), std::greater<>());

    std::vector<int> initial_val = {7, 4, 2, 6, 1, 5, 3, 8, 9};
    std::vector<HeapItem> data;
    for (int i = 0; i < static_cast<int>(initial_val.size()); ++i) {
        data.push_back({i, initial_val[i]});
    }

    int count = 10;
    while (count > 0) {
        rand_gen.Shuffle(data.begin(), data.end());
        Heap p(data);
        REQUIRE(p.Min() == 1);
        REQUIRE(p.MapSize() == 9);

        p.InplaceSort();

        auto possible_correct = p.GetValueData();
        REQUIRE(possible_correct.size() == correct.size());
        for (int i = 0; i < static_cast<int>(possible_correct.size()); ++i) {
            REQUIRE(possible_correct[i] == correct[i]);
        }

        --count;
    }
}
