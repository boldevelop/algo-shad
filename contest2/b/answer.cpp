#include <algorithm>
#include <vector>
#include <iostream>

struct Player {
    int n;
    int eff;
};

struct Command {
    int64_t eff;
    std::vector<Player>::iterator begin;
    std::vector<Player>::iterator end;
};

struct Result {
    int64_t eff;
    std::vector<int> players;
};

Result FormFootballCommand(const std::vector<Player>& players) {
    auto sorted_players = players;
    std::sort(sorted_players.begin(), sorted_players.end(),
              [](const Player& lhs, const Player& rhs) { return lhs.eff > rhs.eff; });
    std::vector<Command> commands;

    auto lazy_it = sorted_players.begin();
    auto fast_it = sorted_players.begin();
    int64_t total_eff = 0;

    while (true) {
        if (fast_it == sorted_players.end()) {
            commands.emplace_back() = {total_eff, lazy_it, fast_it};
            total_eff = 0;
            break;
        }

        if (fast_it - lazy_it < 2) {
            total_eff += fast_it->eff;
            fast_it++;
            continue;
        }

        auto prev = fast_it - 1;
        if (fast_it->eff + prev->eff < lazy_it->eff) {
            commands.emplace_back() = {total_eff, lazy_it, fast_it};
            total_eff = 0;
            lazy_it = fast_it;
        }
        total_eff += fast_it->eff;
        ++fast_it;
    }

    std::sort(commands.begin(), commands.end(),
              [](const Command& lhs, const Command& rhs) { return lhs.eff > rhs.eff; });

    std::vector<int> res;

    auto beg = commands[0].begin;
    while (beg < commands[0].end) {
        res.push_back(beg->n);
        ++beg;
    }

    std::sort(res.begin(), res.end());

    return {commands[0].eff, res};
}

int main() {
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);
    int n_size;
    std::vector<Player> players;
    std::cin >> n_size;

    for (int i = 0; i < n_size; ++i) {
        int eff;
        std::cin >> eff;
        players.push_back({i + 1, eff});
    }

    auto res = FormFootballCommand(players);
    std::cout << res.eff << '\n';
    for (auto pos : res.players) {
        std::cout << pos << " ";
    }
    std::cout << '\n';
}
