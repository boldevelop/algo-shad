#include <catch.hpp>
#include <main.hpp>
#include <util.h>

TEST_CASE("Treap", "Simple") {
    using namespace simple;
    SECTION("Height") {
        std::vector<int> data(100000);
        std::iota(data.begin(), data.end(), 1);

        simple::Treap treap;

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
            simple::Treap treap;

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

        simple::Treap treap;

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

using Data = std::vector<std::vector<int>>;

TEST_CASE("Пример 1") {
    Data data{
        {-3, -3, 3, 3},
        {-2, 2, 2, -2},
        {-1, -1, 1, 1}
    };

    REQUIRE(GetExternalRect(data) == 1);
}

TEST_CASE("Пример 2") {
    Data data{
        {0, 0, 3, 3},
        {1, 1, 2, 2},
        {100, 100, 101, 101},
        {200, 200, 201, 201}
    };

    REQUIRE(GetExternalRect(data) == 3);
}

TEST_CASE("Пример 3") {
    int max_rect = 100'000;
    int big = 1'000'000'000;
    int x1 = -big + 1;
    int x2 = -big + 2;
    int y1 = big - 2;
    int y2 = big - 1;

    SECTION("line") {
        Data data;
        data.push_back({-big, -big, big, big});
        for (int i = 0; i < max_rect / 2; ++i) {
            data.push_back({x1 + (i * 3), y1, x2 + (i * 3), y2});
            data.push_back({x1 + (i * 3), y1 - 3, x2 + (i * 3), y2 - 3});
        }

        Log l;
        auto answer = GetExternalRect(data);
        auto dur = l.GetDuration();
        std::cout << "Пример 3 line: " << dur << std::endl;
        REQUIRE(answer == 1);
    }
    SECTION("Диагональ") {
        Data data;
        data.push_back({-big, -big, big, big});
        for (int i = 0; i < max_rect / 2; ++i) {
            data.push_back({x1 + (i * 3), y1 - (i * 3),       x2 + (i * 3), y2 - (i * 3)});
            data.push_back({x1 + (i * 3), y1 - ((i + 1) * 3), x2 + (i * 3), y2 - ((i + 1) * 3)});
        }

        Log l;
        auto answer = GetExternalRect(data);
        auto dur = l.GetDuration();
        std::cout << "Пример 3 Диагональ: " << dur << std::endl;
        REQUIRE(answer == 1);
    }
    SECTION("Все внутри") {
        Data data;
        data.push_back({-big, -big, big, big});
        for (int i = 0; i < max_rect; ++i) {
            auto inc = i + 1;
            data.push_back({-big + inc, -big + inc, big - inc, big - inc});
        }

        Log l;
        auto answer = GetExternalRect(data);
        auto dur = l.GetDuration();
        std::cout << "Пример 3 Все внутри: " << dur << std::endl;
        REQUIRE(answer == 1);
    }
    SECTION("Все внутри 2") {
        Data data;
        int y1 = 0;
        int y2 = big;

        int xx1 = -big;
        int xx2 = -1;
        data.push_back({xx1, y1, xx2, y2});
        for (int i = 0; i < max_rect / 2 - 1; ++i) {
            auto inc = i + 1;
            data.push_back({xx1 + inc, y1 + inc, xx2 - inc, y2 - inc});
        }

        xx1 = 0;
        xx2 = big;
        data.push_back({xx1, y1, xx2, y2});
        for (int i = 0; i < max_rect / 2 - 1; ++i) {
            auto inc = i + 1;
            data.push_back({xx1 + inc, y1 + inc, xx2 - inc, y2 - inc});
        }

        Log l;
        auto answer = GetExternalRect(data);
        auto dur = l.GetDuration();
        std::cout << "Пример 3 Все внутри 2: " << dur << std::endl;
        REQUIRE(answer == 2);
    }
}
