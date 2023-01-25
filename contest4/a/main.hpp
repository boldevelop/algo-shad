#include <iostream>
#include <vector>
#include <algorithm>

int64_t mod = 123'456'789;

using Seq = std::vector<int64_t>;

class Catalan {
    Seq catalan_;

public:
    explicit Catalan(int max_num) : catalan_(max_num + 1) {
        GenerateCatalanNumbers();
    }
    void Print() {
        int num = 1;
        while (num < static_cast<int>(catalan_.size()) - 1) {
            std::cout << num << " " << GetCatalanNum(num) << std::endl;
            ++num;
        }
    }
    int64_t GetCatalanNum(int num) {
        return catalan_[num + 1];
    }

private:
    void GenerateCatalanNumbers() {
        catalan_[1] = 1;
        for (int i = 2; i < static_cast<int>(catalan_.size()); ++i) {
            for (int j = 1; j <= i - 1; ++j) {
                catalan_[i] = (catalan_[i] + ((catalan_[j]) * catalan_[i - j]) % mod) % mod;
            }
            catalan_[i] = catalan_[i] % mod;
        }
    }
};

int CountPotentialSize(int begin, int end, const Seq& data) {
    int size = 1;
    ++begin;
    while (begin < end) {
        if (data[begin] != data[begin - 1]) {
            ++size;
        }
        ++begin;
    }
    return size;
}

int64_t CountBST(int size, Seq* calculated) {
    if (size <= 1) {
        return 1;
    }
    if ((*calculated)[size] != -1) {
        return (*calculated)[size];
    }
    int64_t count = 0;
    for (int root = 1; root <= size; ++root) {
        int64_t left = CountBST(root - 1, calculated);
        int64_t right = CountBST(size - root, calculated);

        count += (left * right) % mod;
        count %= mod;
    }
    (*calculated)[size] = count;
    return count;
}

int CountBST(Seq& data) {
    int size = data.size();
    Catalan catalan(size + 1);
    std::sort(data.begin(), data.end());
    Seq calculated(size, -1);

    int64_t count = 0;
    for (int root = 0; root < size; ++root) {
        if (root != 0 && data[root] == data[root - 1]) {
            continue;
        }
        auto left = CountPotentialSize(0, root, data);
        int right_begin = root + 1;
        int next = root + 1;
        while (next < size && data[root] == data[next]) {
            ++right_begin;
            ++next;
        }
        auto right = CountPotentialSize(right_begin, size, data);
        count += (CountBST(left, &calculated) * CountBST(right, &calculated)) % mod;
        count %= mod;
    }
    return count;
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
