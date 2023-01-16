#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include <limits>
#include <util.h>

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
    HashTable() : data_(100000), size_(0) {
    }

    void Add(const HashItem& item) {
        data_[Hash(item) % data_.size()].push_back(item);
        size_++;
    }

    void Update(const HashItem& item) {
        auto& list = data_[Hash(item) % data_.size()];

        bool is_exist = false;


        int count = 0;
        for (auto it = list.begin(); it != list.end(); ++it) {
            if (it->global_i == item.global_i) {
                *it = item;
                is_exist = true;
                break;
            }
            count++;
        }

        if (count > 10) {
            std::cout << "T count: " << count << '\n';
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
                --size_;
                break;
            }
        }

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
        return ind;
        return (val * val + val * kPrime * ind + 42) % max_mod;
    }
};

struct HeapItem {
    int global_i;
    int val;
};

class KStatistic {
    std::vector<int> data_;
    bool valid_ = false;

public:
    KStatistic() {
    }

    KStatistic(int size, std::vector<HeapItem>& data) : data_(size) {
        for (int i = 0; i < size; ++i) {
            data_[i] = data[i].val;
        }
        BuildHeap();
    }

    int Max() const {
        return data_[0];
    }

    void UpdateTop(int val) {
        data_[0] = val;
        SiftDown(0);
    }

    void SetValid() {
        valid_ = true;
    }

    bool IsValid() {
        return valid_;
    }

    void Check(int val) {
        if (valid_ && val <= Max()) {
            valid_ = false;
        }
    }

private:
    void SiftDown(int ind) {
        int size = data_.size();
        while (ind < size / 2) {
            auto left = 2 * ind + 1;
            auto right = 2 * ind + 2;

            auto child = right < size && data_[right] > data_[left] ? right : left;

            if (data_[ind] >= data_[child]) {
                break;
            }

            std::swap(data_[ind], data_[child]);
            ind = child;
        }
    }

    void BuildHeap() {
        for (int i = data_.size() - 1; i >= 0; --i) {
            SiftDown(i);
        }
    }
};

class MaxHeap {
    std::vector<HeapItem> data_;
    HashTable table_;
    KStatistic statistic_;

public:
    MaxHeap() {
    }

    MaxHeap(const std::vector<HeapItem>& data) : data_(data), table_(), statistic_() {
        BuildHeap();
    }

    int Max() const {
        return data_[0].val;
    }

    int GetKStatistic(int k_term) {
        if (k_term > static_cast<int>(data_.size())) {
            return -1;
        }

        if (statistic_.IsValid()) {
            return statistic_.Max();
        }
        statistic_ = KStatistic(k_term, data_);
        for (int i = k_term; i < static_cast<int>(data_.size()); ++i) {
            if (data_[i].val > statistic_.Max()) {
                continue;
            }
            statistic_.UpdateTop(data_[i].val);
        }
        statistic_.SetValid();
        return statistic_.Max();
    }

    void Remove(HeapItem item) {
        statistic_.Check(item.val);
        Log l;
        auto vert = table_.Remove({item.global_i, item.val, 0}); // ?
        auto duration = l.GetDurationNano();
        if (duration > 1000)
            std::cout << "t.r: " << duration << '\n';
        Swap(vert, data_.size() - 1);
        RemoveLast();

        if (data_.empty()) {
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
        statistic_.Check(item.val);
        data_.push_back(item);
        int size = data_.size();
        Log l;
        table_.Add({item.global_i, item.val, size - 1});
        auto duration = l.GetDurationNano();
        if (duration > 1000)
            std::cout << "t.a: " << duration << '\n';
        SiftUp(size - 1);
    }

private:
    void SiftUp(int ind) {
        auto parent = (ind - 1) / 2;
        while (ind != 0 && data_[parent].val < data_[ind].val) {
            Swap(parent, ind);
            ind = parent;
            parent = (ind - 1) / 2;
        }
    }

    void SiftDown(int ind) {
        int size = data_.size();
        while (ind < size / 2) {
            auto left = 2 * ind + 1;
            auto right = 2 * ind + 2;

            auto child = right < size && data_[right].val > data_[left].val ? right : left;

            if (data_[ind].val >= data_[child].val) {
                break;
            }

            Swap(ind, child);
            ind = child;
        }
    }

    void BuildHeap() {
        for (int i = data_.size() - 1; i >= 0; --i) {
            Log l;
            table_.Update({data_[i].global_i, data_[i].val, i});
            auto duration = l.GetDurationNano();
            if (duration > 1000)
                std::cout << "t.u: " << duration << '\n';
            SiftDown(i);
        }
    }

    void Swap(int src, int dst) {
        HashItem src_item = {data_[src].global_i, data_[src].val, dst};
        HashItem dst_item = {data_[dst].global_i, data_[dst].val, src};
        {
            Log l;
            table_.Update(src_item);
            auto duration = l.GetDurationNano();
            if (duration > 1000)
                std::cout << "t.u: " << duration << '\n';
        }
        {
            Log l;
            table_.Update(dst_item);
            auto duration = l.GetDurationNano();
            if (duration > 1000)
                std::cout << "t.u: " << duration << '\n';
        }
        std::swap(data_[src], data_[dst]);
    }

    void RemoveLast() {
        auto& last = data_[data_.size() - 1];
        Log l;
        table_.Remove({last.global_i, last.val, static_cast<int>(data_.size()) - 1});
        auto duration = l.GetDurationNano();
        if (duration > 1000)
            std::cout << "t.r: " << duration << '\n';
        data_.pop_back();
    }

public:
    void Print(int vert = 0, std::string indent = "") const {
        if (vert == 0) {
            std::cout << "Size: " << data_.size() << std::endl;
        }
        if (vert >= static_cast<int>(data_.size())) {
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
        for (int i = 0; i < static_cast<int>(data_.size()); ++i) {
            std::cout << data_[i].val << " ";
        }
        std::cout << std::endl;
    }
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
        return Min();
        if (k_term > size_) {
            return -1;
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
        for (int i = 0; i < size_; ++i) {
            std::cout << data_[i].val << " ";
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

std::vector<int> GetKStatistic(int k_stat, const std::string& directions, const std::vector<int>& data) {
    auto left_i = 0;
    auto right_i = 0;
    std::vector<int> result;
    if (k_stat == 1) {
        Heap heap({{0, data[0]}});

        for (char dir : directions) {
            if (dir == 'R') {
                ++right_i;
                heap.Add({right_i, data[right_i]});
            } else {
                heap.Remove({left_i, data[left_i]});
                ++left_i;
            }

            result.push_back(heap.GetKStatistic(k_stat));
        }

        return result;
    }

    MaxHeap max_heap({{0, data[0]}});

    Log log;

    for (char dir : directions) {
        if (dir == 'R') {
            ++right_i;
            Log l;
            max_heap.Add({right_i, data[right_i]});
            auto duration = l.GetDuration();
            if (duration > 100000)
                std::cout << "max_heap.Add: " << duration << std::endl;
        } else {
            Log l;
            max_heap.Remove({left_i, data[left_i]});
            auto duration = l.GetDuration();
            if (duration > 300000)
                std::cout << "max_heap.Remove: " << duration << std::endl;
            ++left_i;
        }
        auto answer = -1;
        {
            Log l;
            answer = max_heap.GetKStatistic(k_stat);
            auto duration = l.GetDuration();
            if (duration > 100000) {
                std::cout << "max_heap GetKStatistic: " << duration << std::endl;
            }
        }
        {
            Log l;
            auto duration = l.GetDuration();
            result.push_back(answer);
            if (duration > 2000) {
                std::cout << "Push_back: " << duration << std::endl;
            }
        }
    }

    std::cout << "Log: " << log.GetDuration() << std::endl;

    return result;
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
