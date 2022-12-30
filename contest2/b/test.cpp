#include <catch.hpp>
#include <util.h>
#include <main.h>
#include <vector>
#include <iostream>

const int MAX_N = 100'000;

std::vector<Player> GeneratePlayers(const std::vector<int>& efficiencies) {
    std::vector<Player> res;
    int n = 1;
    for (auto eff : efficiencies) {
        res.push_back({n, eff});
        ++n;
    }
    return res;
}

TEST_CASE("Пример 1") {
    std::vector<Player> players = GeneratePlayers({3, 2, 5, 4, 1});
    auto res = FormFootballCommand(players);

    std::vector<int> expected = {1, 2, 3, 4};

    REQUIRE(14 == res.eff);
    REQUIRE(expected == res.players);
}

TEST_CASE("Пример 2") {
    std::vector<Player> players = GeneratePlayers({1, 2, 4, 8, 16});
    auto res = FormFootballCommand(players);

    std::vector<int> expected = {4, 5};

    REQUIRE(24 == res.eff);
    REQUIRE(expected == res.players);
}

TEST_CASE("Пример 3") {
    std::vector<int> efficiencies = {35, 20, 20};
    std::vector<int> expected;

    int64_t total_eff = 0;
    auto start = efficiencies.size();
    for (int i = 1; i < 9; ++i) {
        expected.push_back(start + i);
        auto eff = i == 1 ? 10 + 4 : 10;
        efficiencies.push_back(eff);
        total_eff += eff;
    }
    efficiencies.push_back(3);
    efficiencies.push_back(2);
    efficiencies.push_back(1);

    std::vector<Player> players = GeneratePlayers(efficiencies);
    auto res = FormFootballCommand(players);

    REQUIRE(total_eff == res.eff);
    REQUIRE(expected == res.players);
}

TEST_CASE("Крайние случаи") {
    auto eff = 10;

    SECTION("Один элемент") {
        std::vector<int> efficiencies = {eff};
        std::vector<int> expected = {1};
        std::vector<Player> players = GeneratePlayers(efficiencies);
        auto res = FormFootballCommand(players);
        REQUIRE(std::reduce(efficiencies.begin(), efficiencies.end()) == res.eff);
        REQUIRE(expected == res.players);
    }

    SECTION("Два элемента") {
        std::vector<int> expected = {1, 2};

        {
            std::vector<int> efficiencies = {eff, eff};
            std::vector<Player> players = GeneratePlayers(efficiencies);
            auto res = FormFootballCommand(players);
            REQUIRE(std::reduce(efficiencies.begin(), efficiencies.end()) == res.eff);
            REQUIRE(expected == res.players);
        }

        {
            std::vector<int> efficiencies = {eff, eff * 3};
            std::vector<Player> players = GeneratePlayers(efficiencies);
            auto res = FormFootballCommand(players);
            REQUIRE(std::reduce(efficiencies.begin(), efficiencies.end()) == res.eff);
            REQUIRE(expected == res.players);
        }
    }

    SECTION("Три элемента") {

        {
            std::vector<int> expected = {1, 2, 3};
            std::vector<int> efficiencies = {eff / 2, eff, eff / 2 + eff};
            std::vector<Player> players = GeneratePlayers(efficiencies);
            auto res = FormFootballCommand(players);
            REQUIRE(std::reduce(efficiencies.begin(), efficiencies.end()) == res.eff);
            REQUIRE(expected == res.players);
        }

        {
            std::vector<int> expected = {1, 2, 3};
            std::vector<int> efficiencies = {eff, eff, eff * 2};
            std::vector<Player> players = GeneratePlayers(efficiencies);
            auto res = FormFootballCommand(players);
            REQUIRE(std::reduce(efficiencies.begin(), efficiencies.end()) == res.eff);
            REQUIRE(expected == res.players);
        }

        {
            std::vector<int> expected = {2, 3};
            std::vector<int> efficiencies = {eff / 2, eff, eff * 2};
            std::vector<Player> players = GeneratePlayers(efficiencies);
            auto res = FormFootballCommand(players);
            REQUIRE(std::reduce(efficiencies.begin() + 1, efficiencies.end()) == res.eff);
            REQUIRE(expected == res.players);
        }

        {
            std::vector<int> expected = {1, 3};
            std::vector<int> efficiencies = {eff, eff / 2, eff * 2};
            std::vector<Player> players = GeneratePlayers(efficiencies);
            auto res = FormFootballCommand(players);
            REQUIRE(eff * 3 == res.eff);
            REQUIRE(expected == res.players);
        }
    }
}

TEST_CASE("Стресс 1") {
    std::vector<int> efficiencies;
    std::vector<int> expected;

    for (int i = 1; i <= MAX_N; ++i) {
        efficiencies.push_back(i);
        if (i >= MAX_N / 2) {
            expected.push_back(i);
        }
    }

    int64_t total_eff = 0;
    for (auto eff : expected) {
        total_eff += eff;
    }

    std::vector<Player> players = GeneratePlayers(efficiencies);
    Log log;
    auto res = FormFootballCommand(players);
    std::cout << "Стресс 1: " << log.GetDuration() << std::endl;

    REQUIRE(expected == res.players);
    REQUIRE(total_eff == res.eff);
}
