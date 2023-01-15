#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

// #define MAIN FakeMain
// #ifdef ANSWER
// #undef MAIN
// #define MAIN main
// #endif

class Heap {
    int size_;
    std::vector<int> data_;

public:
    Heap() : size_(0), data_() {
    }

    explicit Heap(const std::vector<int>& inp) : size_(inp.size()), data_(std::move(inp)) {
        BuildHeap();
    }

    void Recreate(const std::vector<int>& inp) {
        size_ = inp.size();
        data_ = std::move(inp);
        BuildHeap();
    }

    int Min() const {
        return data_[0];
    }

    int ExtractMin() {
        auto min = data_[0];
        std::swap(data_[0], data_[size_ - 1]);
        data_[size_ - 1] = -1;
        --size_;
        SiftDown(0);
        return min;
    }

    int GetKStatistic(int k_term) {
        int res = -1;
        if (k_term > size_) {
            return res;
        }
        while (k_term > 0) {
            res = ExtractMin();
            --k_term;
        }
        return res;
    }

    void Remove(int vert) {
        if (vert == 0) {
            ExtractMin();
            return;
        }
        std::swap(data_[vert], data_[size_ - 1]);
        data_[size_ - 1] = -1;
        --size_;
        if (size_ == 1) {
            return;
        }

        auto parent = (vert - 1) / 2;
        if (data_[parent] > data_[vert]) {
            SiftUp(vert);
        } else {
            SiftDown(vert);
        }
    }

    void Add(int val) {
        if (size_ == static_cast<int>(data_.size())) {
            data_.push_back(val);
        } else {
            data_[size_] = val;
        }
        size_++;
        SiftUp(size_ - 1);
    }

    bool Empty() const {
        return size_ == 0;
    }

    void Print(int vert = 0, std::string indent = "") const {
        if (vert == 0) {
            std::cout << "Size: " << size_ << std::endl;
        }
        if (vert >= size_) {
            return;
        }
        std::cout << indent << data_[vert] << '\n';
        indent = vert == 0 ? "'-> " : "    " + indent;
        Print(2 * vert + 1, indent);
        Print(2 * vert + 2, indent);
        if (vert == 0) {
            std::cout << "- - - - - - \n\n";
        }
    }

private:
    void SiftUp(int vert) {
        auto parent = (vert - 1) / 2;
        while (vert != 0 && data_[parent] > data_[vert]) {
            std::swap(data_[parent], data_[vert]);
            vert = parent;
            parent = (vert - 1) / 2;
        }
    }

    void SiftDown(int vert) {
        while (2 * vert < size_ - 1) {
            auto left = 2 * vert + 1;
            auto right = 2 * vert + 2;

            auto child = right < size_ && data_[right] < data_[left] ? right : left;

            if (data_[vert] <= data_[child]) {
                break;
            }

            std::swap(data_[vert], data_[child]);
            vert = child;
        }
    }

    void BuildHeap() {
        for (int i = size_; i >= 0; --i) {
            SiftDown(i);
        }
    }
};

#ifndef LOCAL
int main() {
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    int size;         // n
    int query_count;  // m
    int k_stat;       // k
    std::cin >> size >> query_count >> k_stat;
    std::vector<int> data(size);

    for (int i = 0; i < size; ++i) {
        int elem;
        std::cin >> elem;
        data[i] = elem;
    }

    std::string directions;
    std::cin >> directions;

    auto left_it = data.begin();
    auto right_it = data.begin();

    int dir_i = 0;
    while (dir_i < query_count) {
        auto dir = directions[dir_i];

        if (dir == 'R') {
            ++right_it;
        } else {
            ++left_it;
        }

        if ((right_it - left_it) + 1 < k_stat) {
            std::cout << -1 << '\n';
        } else {
            Heap heap({left_it, right_it + 1});
            std::cout << heap.GetKStatistic(k_stat) << '\n';
        }

        ++dir_i;
    }

    return 0;
}

#endif
