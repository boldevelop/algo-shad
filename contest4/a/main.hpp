#include <iostream>
#include <vector>
#include <algorithm>

int64_t mod = 123'456'789;

using Seq = std::vector<int64_t>;

int64_t Count(int begin, int end, const Seq& data, std::vector<Seq>* saved) {
    if (end - begin == 1 || end == begin) {
        return 1;
    }

    if ((*saved)[begin][end - 1] != -1) {
        return (*saved)[begin][end - 1];
    }

    int first = begin;
    int64_t count = 0;
    while (begin < end) {
        if (begin != first && data[begin] == data[begin - 1]) {
            ++begin;
            continue;
        }
        auto left = Count(first, begin, data, saved);
        auto right = Count(begin + 1, end, data, saved);
        count += (left * right) % mod;
        count %= mod;
        ++begin;
    }
    count = count == 0 ? 1 : count;

    (*saved)[first][end - 1] = count;
    return (*saved)[first][end - 1];
}

int CountBST(Seq data) {
    int size = data.size();
    std::sort(data.begin(), data.end());
    std::vector<Seq> saved(size, Seq(size, -1));
    return Count(0, size, data, &saved);
}

#ifndef LOCAL
int main() {
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);
    int size;
    std::cin >> size;
    Seq data;
    data.reserve(size);
    while (size > 0) {
        int elem;
        std::cin >> elem;
        data.push_back(elem);
        --size;
    }
    std::cout << CountBST(data) << '\n';

    return 0;
}

#endif
