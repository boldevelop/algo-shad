#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include <limits>

const uint64_t kPrime = 2 * 1'000'000'000 + 11;

struct HashItem {
    int global_i;
    int val;
    int heap_i;
};

class HashTable {
    std::vector<std::list<HashItem>> data_;
    int size_;

public:
    HashTable() : data_(5000), size_(0) {
    }

    void Add(const HashItem& item) {
        data_[Hash(item) % data_.size()].push_back(item);
        size_++;
    }

    void Update(const HashItem& item) {
        auto& list = data_[Hash(item) % data_.size()];

        bool is_exist = false;

        for (auto it = list.begin(); it != list.end(); ++it) {
            if (it->global_i == item.global_i) {
                *it = item;
                is_exist = true;
                break;
            }
        }

        if (!is_exist) {
            list.push_back(item);
            size_++;
        }
    }

    int Remove(const HashItem& item) {
        auto& list = data_[Hash(item) % data_.size()];

        int heap_ind = -1;

        for (auto it = list.begin(); it != list.end(); ++it) {
            if (it->global_i == item.global_i) {
                heap_ind = it->heap_i;
                list.erase(it);
                break;
            }
        }

        --size_;
        return heap_ind;
    }

    bool Has(const HashItem& item) {
        auto& list = data_[Hash(item) % data_.size()];

        for (auto it = list.begin(); it != list.end(); ++it) {
            if (it->global_i == item.global_i) {
                return true;
            }
        }

        return false;
    }

    int Size() const {
        return size_;
    }

private:
    uint64_t Hash(const HashItem& item) {
        uint64_t val = item.val;
        uint64_t ind = item.global_i + 1;
        uint64_t max_mod = std::numeric_limits<uint64_t>::max() / 2;

        return (val * val + val * kPrime * ind + 42) % max_mod;
    }
};

struct HeapItem {
    int global_i;
    int val;
};

class Heap {
    int size_;
    std::vector<HeapItem> data_;
    HashTable h_table_;
    int last_k_;

public:
    Heap() : size_(0), data_(), h_table_() {
    }

    explicit Heap(const std::vector<HeapItem>& inp)
        : size_(inp.size()), data_(std::move(inp)), h_table_(), last_k_(-1) {
        BuildHeap();
    }

    void Recreate(const std::vector<HeapItem>& inp) {
        size_ = inp.size();
        data_ = std::move(inp);
        BuildHeap();
    }

    int Min() const {
        return data_[0].val;
    }

    int ExtractMin() {
        auto min = data_[0].val;
        Swap(0, size_ - 1);
        RemoveLast();
        SiftDown(0);
        return min;
    }

    int GetKStatistic(int k_term) {
        int res = -1;
        if (k_term > size_) {
            last_k_ = -1;
            return res;
        }

        if (last_k_ == -1) {
            InplaceSort();
            last_k_ = data_[size_ - k_term].val;
        }

        return last_k_;
    }

    void Remove(HeapItem item) {
        if (item.val <= last_k_) {
            last_k_ = -1;
        }
        auto vert = h_table_.Remove({item.global_i, item.val, 0});
        if (vert == 0) {
            ExtractMin();
            return;
        }
        Swap(vert, size_ - 1);
        RemoveLast();

        if (size_ == 1) {
            return;
        }

        auto parent = (vert - 1) / 2;
        if (data_[parent].val > data_[vert].val) {
            SiftUp(vert);
        } else {
            SiftDown(vert);
        }
    }

    void Add(HeapItem item) {
        if (item.val < last_k_) {
            last_k_ = -1;
        }
        if (size_ == static_cast<int>(data_.size())) {
            data_.push_back(item);
        } else {
            data_[size_] = item;
        }

        h_table_.Add({item.global_i, item.val, size_});

        size_++;
        SiftUp(size_ - 1);
    }

    bool Empty() const {
        return size_ == 0;
    }

    void InplaceSort() {
        auto size = size_;
        while (size_ > 1) {
            Swap(0, size_ - 1);
            --size_;
            SiftDown(0);
        }
        size_ = size;
    }

    void Print(int vert = 0, std::string indent = "") const {
        if (vert == 0) {
            std::cout << "Size: " << size_ << std::endl;
        }
        if (vert >= size_) {
            return;
        }
        std::cout << indent << data_[vert].val << '\n';
        indent = vert == 0 ? "'-> " : "    " + indent;
        Print(2 * vert + 1, indent);
        Print(2 * vert + 2, indent);
        if (vert == 0) {
            std::cout << "- - - - - - \n\n";
        }
    }

    void PrintData() const {
        for (auto item : data_) {
            std::cout << item.val << " ";
        }
        std::cout << std::endl;
    }

    std::vector<HeapItem> GetData() {
        return data_;
    }

    std::vector<int> GetValueData() {
        std::vector<int> data;
        for (auto item : data_) {
            data.push_back(item.val);
        }
        return data;
    }

    int MapSize() const {
        return h_table_.Size();
    }

private:
    void SiftUp(int vert) {
        auto parent = (vert - 1) / 2;
        while (vert != 0 && data_[parent].val > data_[vert].val) {
            Swap(parent, vert);
            vert = parent;
            parent = (vert - 1) / 2;
        }
    }

    void SiftDown(int vert) {
        while (2 * vert < size_ - 1) {
            auto left = 2 * vert + 1;
            auto right = 2 * vert + 2;

            auto child = right < size_ && data_[right].val < data_[left].val ? right : left;

            if (data_[vert].val <= data_[child].val) {
                break;
            }

            Swap(vert, child);
            vert = child;
        }
    }

    void BuildHeap() {
        for (int i = size_ - 1; i >= 0; --i) {
            h_table_.Update({data_[i].global_i, data_[i].val, i});
            SiftDown(i);
        }
    }

    void Swap(int src, int dst) {
        HashItem src_item = {data_[src].global_i, data_[src].val, dst};
        HashItem dst_item = {data_[dst].global_i, data_[dst].val, src};
        h_table_.Update(src_item);
        h_table_.Update(dst_item);

        std::swap(data_[src], data_[dst]);
    }

    void RemoveLast() {
        --size_;
        h_table_.Remove({data_[size_].global_i, data_[size_].val, size_});
    }
};

void GetKStatistic(int k_stat, const std::string& directions, const std::vector<int>& data) {
    auto left_i = 0;
    auto right_i = 0;
    Heap heap({{0, data[0]}});

    for (char dir : directions) {
        if (dir == 'R') {
            ++right_i;
            heap.Add({right_i, data[right_i]});
        } else {
            heap.Remove({left_i, data[left_i]});
            ++left_i;
        }
        std::cout << heap.GetKStatistic(k_stat) << '\n';
    }
}

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

    auto left_i = 0;
    auto right_i = 0;
    Heap heap({{0, data[0]}});

    std::vector<int> result;
    result.reserve(query_count);

    while (query_count > 0) {
        char dir = std::cin.get();

        if (dir != 'R' && dir != 'L') {
            continue;
        }

        if (dir == 'R') {
            ++right_i;
            heap.Add({right_i, data[right_i]});
        } else {
            heap.Remove({left_i, data[left_i]});
            ++left_i;
        }

        result.push_back(heap.GetKStatistic(k_stat));

        --query_count;
    }

    for (auto num : result) {
        std::cout << num << '\n';
    }

    return 0;
}

#endif
