#include <iostream>
#include <vector>
#include <algorithm>
#include <optional>
#include <cmath>

struct TimeCost {
    int64_t val;
    bool no_solution;

    TimeCost() : val(0), no_solution(true) {
    }

    TimeCost(int64_t cost, bool no_sol = true) : val(cost), no_solution(no_sol) {
    }

    bool HasSolution() const {
        return !no_solution;
    }

    void SetTime(int64_t time) {
        val = time;
        no_solution = false;
    }
};

struct Coin {
    int64_t dist;
    int64_t time;

    Coin(int64_t di, int64_t ti) : dist(di), time(ti) {
    }

    int64_t Interval(const Coin& to) const {
        return std::abs(dist - to.dist);
    }

    int64_t IsEnoughTime(int64_t p_time) const {
        return time >= p_time;
    }
};

using TimeCostData = std::vector<std::vector<std::vector<std::optional<TimeCost>>>>;
using Coins = std::vector<Coin>;

TimeCost GetCostFromLR(const TimeCost& left, const TimeCost& right) {
    if (left.HasSolution() && right.HasSolution()) {
        return left.val < right.val ? left : right;
    } else if (left.HasSolution()) {
        return left;
    } else if (right.HasSolution()) {
        return right;
    }
    return {};
}

/* left = 0, right = 1 */
TimeCost GetTime(int first, int last, bool is_left, const Coins& coins, TimeCostData* time_cost) {
    if (is_left && (*time_cost)[first][last][0]) {
        return *(*time_cost)[first][last][0];
    }
    if (!is_left && (*time_cost)[first][last][1]) {
        return *(*time_cost)[first][last][1];
    }
    TimeCost left(0);
    TimeCost right(0);

    if (is_left) {
        auto time_cost_left = GetTime(first + 1, last, true, coins, time_cost);
        auto time_cost_right = GetTime(first + 1, last, false, coins, time_cost);

        if (time_cost_left.HasSolution() || time_cost_right.HasSolution()) {
            int64_t time = 0;
            if (time_cost_left.HasSolution()) {
                time = time_cost_left.val + coins[first].Interval(coins[first + 1]);
                if (coins[first].IsEnoughTime(time)) {
                    left.SetTime(time);
                }
            }
            if (time_cost_right.HasSolution()) {
                time = time_cost_right.val + coins[first].Interval(coins[last]);
                if (coins[first].IsEnoughTime(time)) {
                    right.SetTime(time);
                }
            }
        }
    } else {
        auto time_cost_left = GetTime(first, last - 1, true, coins, time_cost);
        auto time_cost_right = GetTime(first, last - 1, false, coins, time_cost);

        if (time_cost_left.HasSolution() || time_cost_right.HasSolution()) {
            int64_t time = 0;
            if (time_cost_left.HasSolution()) {
                time = time_cost_left.val + coins[last].Interval(coins[first]);
                if (coins[last].IsEnoughTime(time)) {
                    left.SetTime(time);
                }
            }
            if (time_cost_right.HasSolution()) {
                time = time_cost_right.val + coins[last].Interval(coins[last - 1]);
                if (coins[last].IsEnoughTime(time)) {
                    right.SetTime(time);
                }
            }
        }
    }

    (*time_cost)[first][last][is_left ? 0 : 1] = GetCostFromLR(left, right);
    return *(*time_cost)[first][last][is_left ? 0 : 1];
}

std::optional<int64_t> GetAns(Coins& coins) {
    std::sort(coins.begin(), coins.end(),
              [](const Coin& lhs, const Coin& rhs) { return lhs.dist < rhs.dist; });
    auto size = coins.size();
    TimeCostData time_cost(size, std::vector<std::vector<std::optional<TimeCost>>>(
                                     size, std::vector<std::optional<TimeCost>>(2)));

    for (int i = 0; i < static_cast<int>(size); ++i) {
        time_cost[i][i][0] = TimeCost(0, false);
        time_cost[i][i][1] = TimeCost(0, false);
    }

    auto res = GetCostFromLR(GetTime(0, coins.size() - 1, true, coins, &time_cost),
                             GetTime(0, coins.size() - 1, false, coins, &time_cost));
    if (res.HasSolution()) {
        return res.val;
    }
    return {};
}

#ifndef LOCAL
int main() {
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    int count;
    std::cin >> count;

    Coins coins;
    coins.reserve(count);

    auto iter = count;
    while (iter > 0) {
        int64_t dist, time;
        std::cin >> dist >> time;
        coins.emplace_back(dist, time);
        --iter;
    }

    auto ans = GetAns(coins);
    if (ans) {
        std::cout << (*ans) << '\n';
    } else {
        std::cout << "No solution" << '\n';
    }
    return 0;
}

#endif
