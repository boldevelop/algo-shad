#include <catch.hpp>
#include <main.hpp>
#include <util.h>

TEST_CASE("Контест") {
    SECTION("1 3 5 8 10") {
        Coins coins{
            {1, 3},
            {3, 1},
            {5, 8},
            {8, 19},
            {10, 15}
        };
        REQUIRE(*GetAns(coins) == 11);
    }

    SECTION("1 2 3 4 5") {
        Coins coins{
            {1, 5},
            {2, 1},
            {3, 4},
            {4, 2},
            {5, 3}
        };
        REQUIRE(!GetAns(coins));
    }
}

TEST_CASE("Свои") {
    SECTION("1 3 5") {
        {
            Coins coins{
                {1, 6},
                {3, 1},
                {5, 2}
            };

            REQUIRE(*GetAns(coins) == 6);
        }
        {
            Coins coins{
                {1, 6},
                {3, 5},
                {5, 2}
            };

            REQUIRE(*GetAns(coins) == 4);
        }
        {
            Coins coins{
                {1, 1},
                {3, 2},
                {5, 1}
            };

            REQUIRE(!GetAns(coins).has_value());
        }
    }

    SECTION("1 5 5") {
        {
            Coins coins{
                {1, 1},
                {5, 4},
                {5, 1}
            };

            REQUIRE(!GetAns(coins));
        }
        {
            Coins coins{
                {1, 1},
                {5, 1},
                {5, 4}
            };

            REQUIRE(!GetAns(coins));
        }
        {
            Coins coins{
                {1, 1},
                {5, 4},
                {5, 4}
            };

            REQUIRE(*GetAns(coins) == 4);
        }
    }

    {
        Coins coins{
            {5, 0},
            {5, 4},
            {5, 4}
        };

        REQUIRE(*GetAns(coins) == 0);
    }

    {
        Coins coins{
            {1, 0},
            {5, 4},
            {5, 4}
        };

        REQUIRE(*GetAns(coins) == 4);
    }
    {
        Coins coins{
            {1, 0},
            {5, 3},
            {5, 2}
        };

        REQUIRE(!GetAns(coins));
    }
}

TEST_CASE("Stress") {
    int size = 1000;
    std::vector<int64_t> help_data(size);
    std::iota(help_data.begin(), help_data.end(), 1);
    {
        Coins coins;
        for (auto num : help_data)  {
            coins.emplace_back(num, num);
        }

        Log log;
        auto res = GetAns(coins);
        auto dur = log.GetDuration();
        std::cout << "sorted: " << dur << std::endl;
        REQUIRE(*res == 999);
    }

    {
        Coins coins;
        for (auto num : help_data)  {
            coins.emplace_back(size - num + 1, size - num + 1);
        }

        Log log;
        auto res = GetAns(coins);
        auto dur = log.GetDuration();
        std::cout << "non sorted: "<< dur << std::endl;
        REQUIRE(*res == 999);
    }
}
