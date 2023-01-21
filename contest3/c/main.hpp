#include <iostream>
#include <list>
#include <vector>
#include <memory>
#include <algorithm>

struct Segment {
    bool allocated;
    int from;
    int to;
    int heap_ind;
    Segment* prev;
    Segment* next;
    bool fake_;

    Segment(bool is_alloc, int from, int to, int h_ind, Segment* prev_s, Segment* next_s,
            bool fake = false)
        : allocated(is_alloc),
          from(from),
          to(to),
          heap_ind(h_ind),
          prev(prev_s),
          next(next_s),
          fake_(fake) {
        if (prev) {
            prev->next = this;
        }
        if (next) {
            next->prev = this;
        }
    }

    bool IsAccessible() const {
        return !fake_ && !allocated;
    }

    int Size() {
        return to - from;
    }

    void Unlink() {
        prev->next = next;
        next->prev = prev;
    }
};

using Ptr = Segment*;

class MaxHeap {
    std::vector<Ptr> data_;

public:
    MaxHeap(int size);
    void Add(Ptr it);
    void Remove(int ind);
    void Update(int ind);
    int Size() const;
    Ptr Top();
    void Print(int vert = 0, std::string indent = "") const;

private:
    void SiftUp(int ind);
    void SiftDown(int ind);
    void Swap(int src, int dst);
};

class Memory {
    Ptr main_;
    int size_;

public:
    Memory(int size) : main_(new Segment(false, 0, size, -1, nullptr, nullptr)) {
        auto fake = new Segment(true, 0, 0, -1, main_, main_, true);
        main_->next = fake;
        main_->prev = fake;
        ++size_;
    }

    ~Memory() {
        auto fake = main_->next;
        while (size_ > 0) {
            auto prev = main_->prev;
            delete main_;
            main_ = prev;
            --size_;
        }
        delete fake;
    }

    Ptr Main() {
        return main_;
    }

    Ptr Alloc(Ptr segment, int mem_count) {
        auto seg =
            new Segment(true, segment->from, segment->from + mem_count, -1, segment->prev, segment);
        segment->from += mem_count;
        ++size_;
        return seg;
    }

    void Free(Ptr segment, MaxHeap* heap) {
        if (segment == main_) {
            main_ = main_->prev;
        }

        heap->Remove(segment->heap_ind);
        segment->Unlink();
        delete segment;
        --size_;
    }

    void EnlargeTo(Ptr enlarged, Ptr segment, MaxHeap* heap) {
        enlarged->to = segment->to;
        heap->Update(enlarged->heap_ind);
        Free(segment, heap);
    }

    void EnlargeFrom(Ptr enlarged, Ptr segment, MaxHeap* heap) {
        enlarged->from = segment->from;
        heap->Update(enlarged->heap_ind);
        Free(segment, heap);
    }

    void PrintSegment(Segment* seg) {
        std::cout << "Alloc: " << seg << " n->" << seg->next << std::endl;
    }
    void Print() {
        auto fake = main_->next;
        auto next = fake->next;
        while (next != fake) {
            std::cout << next << "->";
            next = next->next;
        }
        std::cout << std::endl;
    }
};

struct HashItem {
    int req_id;
    Ptr segment;
    HashItem(int req, Ptr seg) : req_id(req), segment(seg) {
    }
};

class HashTable {
    std::vector<std::list<HashItem>> data_;
    int size_;

public:
    HashTable(int size) : data_(size), size_(0) {
    }
    void Add(int req_id, Ptr segment) {
        data_[Hash(req_id) % data_.size()].emplace_back(req_id, segment);
        ++size_;
    }
    Ptr Remove(int req_id) {
        Ptr res = nullptr;
        auto& list = data_[Hash(req_id) % data_.size()];
        for (auto it = list.begin(); it != list.end();) {
            if (it->req_id == req_id) {
                res = it->segment;
                it = list.erase(it);
                --size_;
                break;
            } else {
                ++it;
            }
        }
        return res;
    }

private:
    uint64_t Hash(uint64_t req_id) {
        return req_id;
    }
};

class Manager {
public:
    Manager(int size);
    int Alloc(int mem_count);
    void Free(int req_id);

private:
    int req_id_;
    MaxHeap heap_;
    HashTable table_;
    Memory memory_;
};

Manager::Manager(int size) : req_id_(0), heap_(size), table_(size), memory_(size) {
    heap_.Add(memory_.Main());
}

int Manager::Alloc(int mem_count) {
    ++req_id_;
    int result = -1;
    if (heap_.Size() == 0) {
        return result;
    }
    auto longest_segment = heap_.Top();  // ? remove from table ?

    if (mem_count > longest_segment->Size()) {
        return result;
    }

    result = longest_segment->from + 1;
    auto new_segment = memory_.Alloc(longest_segment, mem_count);
    table_.Add(req_id_ - 1, new_segment);

    if (longest_segment->Size() > 0) {
        heap_.Update(longest_segment->heap_ind);
    } else {
        memory_.Free(longest_segment, &heap_);
    }
    return result;
}

void Manager::Free(int req_id) {
    ++req_id_;
    --req_id;
    if (req_id > req_id_ - 1 || req_id < 0) {
        return;
    }

    auto segment = table_.Remove(req_id);
    if (!segment) {  // ?
        return;
    }

    auto segment_prev = segment->prev;
    auto segment_next = segment->next;
    auto prev_freed = segment_prev->IsAccessible();
    auto next_freed = segment_next->IsAccessible();

    if (!prev_freed && !next_freed) {
        segment->allocated = false;
        heap_.Add(segment);
        return;
    }

    if (!prev_freed) {
        memory_.EnlargeFrom(segment_next, segment, &heap_);
        return;
    }
    if (!next_freed) {
        memory_.EnlargeTo(segment_prev, segment, &heap_);
        return;
    }

    memory_.EnlargeTo(segment_prev, segment_next, &heap_);
    memory_.Free(segment, &heap_);
}

MaxHeap::MaxHeap(int size) : data_() {
    data_.reserve(size);
}

void MaxHeap::Add(Ptr segment) {
    segment->heap_ind = data_.size();
    data_.push_back(segment);
    SiftUp(data_.size() - 1);
}

void MaxHeap::Remove(int ind) {
    if (ind < 0) {
        return;
    }
    Swap(ind, data_.size() - 1);
    data_.pop_back();
    if (ind >= static_cast<int>(data_.size())) {
        return;
    }
    auto parent = (ind - 1) / 2;
    if (data_[ind]->Size() > data_[parent]->Size()) {
        SiftUp(ind);
    } else {
        SiftDown(ind);
    }
}

void MaxHeap::Update(int ind) {
    auto parent = (ind - 1) / 2;
    if (data_[ind]->Size() > data_[parent]->Size()) {
        SiftUp(ind);
    } else {
        SiftDown(ind);
    }
}

int MaxHeap::Size() const {
    return data_.size();
}

Ptr MaxHeap::Top() {
    return data_[0];
}

void MaxHeap::SiftUp(int ind) {
    auto parent = (ind - 1) / 2;
    while (ind != 0 && data_[ind]->Size() > data_[parent]->Size()) {
        Swap(parent, ind);
        ind = parent;
        parent = (ind - 1) / 2;
    }
}

void MaxHeap::SiftDown(int ind) {
    int size = data_.size();
    while (ind < size / 2) {
        auto left = 2 * ind + 1;
        auto right = 2 * ind + 2;

        auto child = right < size && data_[right]->Size() > data_[left]->Size() ? right : left;

        if (data_[ind]->Size() >= data_[child]->Size()) {
            break;
        }

        Swap(ind, child);
        ind = child;
    }
}

void MaxHeap::Swap(int src, int dst) {
    std::swap(data_[src], data_[dst]);
    std::swap(data_[src]->heap_ind, data_[dst]->heap_ind);
}

void MaxHeap::Print(int vert, std::string indent) const {
    int size = data_.size();
    if (vert == 0) {
        std::cout << "Size: " << size << std::endl;
    }
    if (vert >= size) {
        return;
    }
    std::cout << indent << data_[vert]->from << " " << data_[vert]->to << '\n';
    indent = vert == 0 ? "'-> " : "    " + indent;
    Print(2 * vert + 1, indent);
    Print(2 * vert + 2, indent);
    if (vert == 0) {
        std::cout << "- - - - - - \n\n";
    }
}

#ifndef LOCAL
int main() {
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    int size;         // n
    int query_count;  // m
    std::cin >> size >> query_count;

    Manager manager(size);
    while (query_count > 0) {
        int request;
        std::cin >> request;
        if (request > 0) {
            std::cout << manager.Alloc(request) << '\n';
        } else {
            manager.Free(-request);
        }
        --query_count;
    }
    return 0;
}

#endif
