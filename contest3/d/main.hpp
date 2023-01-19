#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <optional>

class IndexTable {
    std::vector<int> data_;
    int size_;

public:
    explicit IndexTable(int size) : data_(size, -1), size_(0) {
    }

    void Add(int ind, int ind_heap) {
        data_[ind] = ind_heap;
        ++size_;
    }
    int Remove(int ind) {
        auto ind_heap = data_[ind];
        data_[ind] = -1;
        --size_;
        return ind_heap;
    }
    bool Has(int ind) const {
        return data_[ind] != -1;
    }
    void Update(int ind, int ind_heap) {
        if (Has(ind)) {
            data_[ind] = ind_heap;
        }
    }
    int Size() const {
        return size_;
    }
    void Print() const {
        std::cout << "Print table" << std::endl;
        for (int i = 0; i < static_cast<int>(data_.size()); ++i) {
            if (Has(i)) {
                std::cout << i << " : " << data_[i] << std::endl;
            }
        }
        std::cout << "= = = = =" << std::endl;
    }
};

struct HeapItem {
    int val;
    int ind;
    HeapItem(int value, int index) : val(value), ind(index) {
    }
};

class Heap {
    std::vector<HeapItem> data_;
    IndexTable& table_;

public:
    Heap(int size, IndexTable& table) : data_(), table_(table) {
        data_.reserve(size);
    }
    void Add(int val, int ind) {
        table_.Add(ind, data_.size());
        data_.emplace_back(val, ind);
        SiftUp(data_.size() - 1);
    }
    std::optional<HeapItem> ExtractTop() {
        while (!data_.empty() && !table_.Has(data_[0].ind)) {
            RemoveTop();
        }
        if (data_.empty()) {
            return {};
        }

        auto res = data_[0];
        table_.Remove(res.ind);
        RemoveTop();

        return res;
    }

private:
    void SiftUp(int ind_heap) {
        auto parent = (ind_heap - 1) / 2;
        while (ind_heap != 0 && data_[parent].val > data_[ind_heap].val) {
            Swap(parent, ind_heap);
            ind_heap = parent;
            parent = (ind_heap - 1) / 2;
        }
    }
    void SiftDown(int ind_heap) {
        int size = data_.size();
        while (ind_heap < size / 2) {
            auto left = 2 * ind_heap + 1;
            auto right = 2 * ind_heap + 2;

            auto child = right < size && data_[right].val < data_[left].val ? right : left;

            if (data_[ind_heap].val <= data_[child].val) {
                break;
            }

            Swap(ind_heap, child);
            ind_heap = child;
        }
    }
    void Swap(int src, int dst) {
        table_.Update(data_[src].ind, dst);
        table_.Update(data_[dst].ind, src);
        std::swap(data_[src], data_[dst]);
    }
    void RemoveTop() {
        if (data_.size() == 1) {
            data_.pop_back();
        } else {
            Swap(0, data_.size() - 1);
            data_.pop_back();
            SiftDown(0);
        }
    }

public:
    void Print(int vert = 0, std::string indent = "") const {
        int size = data_.size();
        if (vert == 0) {
            std::cout << "HEAP Size: " << size << std::endl;
        }
        if (vert >= size) {
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

    /*
        void PrintData() const {
            for (int i = 0; i < size_; ++i) {
                std::cout << data_[i].val << " ";
            }
            std::cout << std::endl;
        }
     */
};

class HeapStatistic {
    std::vector<HeapItem> data_;
    IndexTable& table_;

public:
    HeapStatistic(int size, IndexTable& table) : data_(), table_(table) {
        data_.reserve(size);
    }
    void Add(int val, int ind) {
        table_.Add(ind, data_.size());
        data_.emplace_back(val, ind);
        SiftUp(data_.size() - 1);
    }
    int Top() {
        return data_[0].val;
    }
    HeapItem ExtractTopWithAdd(int val, int ind) {
        auto item = data_[0];
        table_.Remove(item.ind);
        table_.Add(ind, 0);
        data_[0] = {val, ind};
        SiftDown(0);
        return item;
    }
    void RemoveWithAdd(int ind_heap, Heap& heap) {
        auto item = heap.ExtractTop();
        if (item) {
            data_[ind_heap] = *item;
            table_.Add(item->ind, ind_heap);
        } else {
            Swap(ind_heap, data_.size() - 1);
            data_.pop_back();
            if (data_.empty()) {
                return;
            }
        }
        auto parent = (ind_heap - 1) / 2;
        if (data_[ind_heap].val > data_[parent].val) {
            SiftUp(ind_heap);
        } else {
            SiftDown(ind_heap);
        }
    }
    int Size() const {
        return data_.size();
    }

private:
    void SiftUp(int ind_heap) {
        auto parent = (ind_heap - 1) / 2;
        while (ind_heap != 0 && data_[ind_heap].val > data_[parent].val) {
            Swap(parent, ind_heap);
            ind_heap = parent;
            parent = (ind_heap - 1) / 2;
        }
    }

    void SiftDown(int ind_heap) {
        int size = data_.size();
        while (ind_heap < size / 2) {
            auto left = 2 * ind_heap + 1;
            auto right = 2 * ind_heap + 2;

            auto child = right < size && data_[right].val > data_[left].val ? right : left;

            if (data_[ind_heap].val >= data_[child].val) {
                break;
            }

            Swap(ind_heap, child);
            ind_heap = child;
        }
    }

    void Swap(int src, int dst) {
        table_.Update(data_[src].ind, dst);
        table_.Update(data_[dst].ind, src);
        std::swap(data_[src], data_[dst]);
    }

public:
    void Print(int vert = 0, std::string indent = "") const {
        int size = data_.size();
        if (vert == 0) {
            std::cout << "Size: " << size << std::endl;
        }
        if (vert >= size) {
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
};

std::vector<int> GetKStatistic(int k_stat, const std::string& directions,
                               const std::vector<int>& data) {
    auto left_i = 0;
    auto right_i = 0;
    std::vector<int> result;
    auto size = data.size();
    result.reserve(size);

    IndexTable table_st(size);
    IndexTable table(size);
    HeapStatistic heap_st(k_stat, table_st);
    Heap heap(size - k_stat, table);

    heap_st.Add(data[0], 0);

    for (char dir : directions) {
        if (dir == 'R') {
            ++right_i;

            auto val = data[right_i];
            if (heap_st.Size() < k_stat) {
                heap_st.Add(val, right_i);
            } else if (val < heap_st.Top()) {
                auto item = heap_st.ExtractTopWithAdd(val, right_i);
                heap.Add(item.val, item.ind);
            } else {
                heap.Add(val, right_i);
            }

        } else {
            if (table_st.Has(left_i)) {
                heap_st.RemoveWithAdd(table_st.Remove(left_i), heap);
            } else {
                table.Remove(left_i);
            }

            ++left_i;
        }
        auto answer = -1;
        if (heap_st.Size() == k_stat) {
            answer = heap_st.Top();
        }
        result.push_back(answer);
    }

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

    IndexTable table_st(size);
    IndexTable table(size);
    HeapStatistic heap_st(k_stat, table_st);
    Heap heap(size - k_stat, table);

    if (!data.empty()) {
        heap_st.Add(data[0], 0);
    }
    std::string directions;
    std::cin >> directions;

    for (auto dir : directions) {
        if (dir == 'R') {
            ++right_i;

            auto val = data[right_i];
            if (heap_st.Size() < k_stat) {
                heap_st.Add(val, right_i);
            } else if (val < heap_st.Top()) {
                auto item = heap_st.ExtractTopWithAdd(val, right_i);
                heap.Add(item.val, item.ind);
            } else {
                heap.Add(val, right_i);
            }

        } else {
            if (table_st.Has(left_i)) {
                heap_st.RemoveWithAdd(table_st.Remove(left_i), heap);
            } else {
                table.Remove(left_i);
            }

            ++left_i;
        }

        if (heap_st.Size() == k_stat) {
            std::cout << heap_st.Top() << '\n';
        } else {
            std::cout << -1 << '\n';
        }
    }

    return 0;
}

#endif
