#include <catch.hpp>
#include <main.hpp>
#include <util.h>

TEST_CASE("Treap", "Simple") {
    using namespace Simple;
    SECTION("Height") {
        std::vector<int> data(100000);
        std::iota(data.begin(), data.end(), 1);

        Treap treap;

        Log l;
        for (auto num : data) {
            treap.Insert(num);
        }
        auto dur = l.GetDuration();
        std::cout << treap.GetHeight() << ": " << dur << std::endl;
        REQUIRE(treap.GetHeight() < 50);
    }

    SECTION("Correctness") {
        std::vector<int> data(15);
        std::iota(data.begin(), data.end(), 1);
        RandomGenerator RandGen;

        for (int i = 0; i < 5; ++i) {
            Treap treap;

            for (auto num : data) {
                treap.Insert(num);
                REQUIRE(treap.Find(num)->key == num);
                if (i == 0) {
                    REQUIRE(!treap.UpperBound(num));
                    REQUIRE(treap.UpperBound(num - 1)->key == num);
                }
            }

            for (auto num : data) {
                REQUIRE(!treap.Find(num + 15));
                REQUIRE(treap.Find(num)->key == num);
                REQUIRE(!treap.Find(num - 15));
                treap.Remove(num + 16);
            }

            for (auto num : data) {
                treap.Remove(num);
                REQUIRE(!treap.Find(num));
            }
            REQUIRE(treap.GetHeight() == 0);
            RandGen.Shuffle(data.begin(), data.end());
        }
    }

    SECTION("Stress") {
        std::vector<int> data(100000);
        std::iota(data.begin(), data.end(), 1);

        Treap treap;

        Log logger;
        for (auto num : data) {
            Log l;
            treap.Insert(num);
            REQUIRE(treap.Find(num)->key == num);
            REQUIRE(!treap.UpperBound(num));
            REQUIRE(treap.UpperBound(num - 1)->key == num);
            auto dur = l.GetDuration();
            if (dur > 500) {
                std::cout << treap.GetHeight() << ": " << dur << std::endl;
            }
        }
        auto dur = logger.GetDuration();

        std::cout << "All op: " << dur << std::endl;
    }
}
