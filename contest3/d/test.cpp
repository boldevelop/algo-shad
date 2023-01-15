#include <main.hpp>
#include <catch.hpp>
#include <util.h>

void FillHeap(Heap* p) {
    for (int i = 10; i > 0; --i) {
        p->Add(i);
        REQUIRE(p->Min() == i);
    }
}

TEST_CASE("Heap Construct from vector") {
    Heap p;
    RandomGenerator rand_gen;
    std::vector<int> data(10);
    std::iota(data.begin(), data.end(), 1);

    int count = 10;
    while (count > 0) {
        rand_gen.Shuffle(data.begin(), data.end());
        Heap p(data);
        REQUIRE(p.Min() == 1);

        for (int i = 0; i < 10; ++i) {
            auto min = p.ExtractMin();
            REQUIRE(min == i + 1);
        }
        --count;
    }

}

TEST_CASE("Heap extract min") {
    {
        Heap p;

        for (int i = 1; i <= 10; ++i) {
            p.Add(i);
            REQUIRE(p.Min() == 1);
        }

        for (int i = 0; i < 10; ++i) {
            auto min = p.ExtractMin();
            REQUIRE(min == i + 1);
        }
        REQUIRE(p.Empty());

        p.Add(4);
        REQUIRE(p.Min() == 4);

        p.Add(6);
        REQUIRE(p.Min() == 4);

        p.Add(1);
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

TEST_CASE("Heap remove") {
    Heap p;
    FillHeap(&p);

    for (int i = 0; i < 9; ++i) {
        p.Remove(1);
        REQUIRE(p.Min() == 1);
    }
    auto min = p.ExtractMin();
    REQUIRE(min == 1);
    REQUIRE(p.Empty());
}
