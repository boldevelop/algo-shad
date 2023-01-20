#include <iostream>
#include <list>
#include <vector>
#include <memory>
#include <algorithm>
#include <iterator>

struct Segment {
    bool allocated;
    int from;
    int to;
    int heap_ind;
    Segment* prev;
    Segment* next;

    Segment(bool is_alloc, int from, int to, int h_ind,  Segment* prev_s, Segment* next_s)
        : allocated(is_alloc), from(from), to(to), heap_ind(h_ind), prev(prev_s), next(next_s) {
        if (prev) {
            prev->next = this;
        }
        if (next) {
            next->prev = this;
        }
    }

    int Size() {
        return to - from;
    }

    Segment* Alloc(int mem_count) {
        auto seg = new Segment(true, from, from + mem_count, -1, prev, this);
        from += mem_count;
        return seg;
    }

    void Unlink() {
        if (prev) {
            prev->next = next;
        }
        if (next) {
            next->next = prev;
        }
    }
};

struct HashItem {
    int req_id;
    Segment* segment;
    HashItem(int req, Segment* seg) : req_id(req), segment(seg) {
    }
};

class HashTable {
    std::vector<std::list<HashItem>> data_;
    int size_;

public:
    HashTable(int size) : data_(size), size_(0) {
    }
    void Add(int req_id, Segment* segment) {
        data_[Hash(req_id) % data_.size()].emplace_back(req_id, segment);
        ++size_;
    }
    Segment* Remove(int req_id) {
        Segment* res = nullptr;
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
    using Ptr = Segment*;

    class MaxHeap {
        std::vector<Ptr> data_;

    public:
        MaxHeap(int size);
        void Add(Ptr it);
        void Remove(Ptr it);
        void Update(Ptr it);
        int Size() const;
        Ptr Top();
        void Print(int vert = 0, std::string indent = "") const;

    private:
        void SiftUp(int ind);
        void SiftDown(int ind);
        void Swap(int src, int dst);
    };

public:
    Manager(int size);
    int Alloc(int mem_count);
    void Free(int req_id);

private:
    int req_id_;
    MaxHeap heap_;
    HashTable table_;

private:
    void EnlargeTo(Ptr enlarged, Ptr segment);
    void EnlargeFrom(Ptr enlarged, Ptr segment);
};

Manager::Manager(int size) : req_id_(0), heap_(size), table_(size) {
    auto segment = new Segment(false, 0, size, -1, nullptr, nullptr);
    heap_.Add(segment);
}

int Manager::Alloc(int mem_count) {
    std::cout << "A " << mem_count << " i:" << req_id_ << std::endl;
    ++req_id_;
    int result = -1;
    if (heap_.Size() == 0) {
        return result;
    }
    auto longest_segment = heap_.Top();

    if (mem_count > longest_segment->Size()) {
        return result;
    }

    result = longest_segment->from + 1;
    auto new_segment = longest_segment->Alloc(mem_count);
    table_.Add(req_id_ - 1, new_segment);

    if (longest_segment->Size() > 0) {
        heap_.Update(longest_segment);
    } else {
        heap_.Remove(longest_segment);
        longest_segment->Unlink();
        delete longest_segment;
    }
    return result;
}

void Manager::Free(int req_id) {
    std::cout << "F " << req_id << " i:" << req_id_ << std::endl;

    ++req_id_;
    --req_id;
    if (req_id > req_id_ - 1 || req_id < 0) {
        return;
    }

    auto segment = table_.Remove(req_id);
    if (!segment) {
        return;
    }

    auto segment_prev = segment->prev;
    auto segment_next = segment->next;
    auto prev_freed = segment_prev && !segment_prev->allocated;
    auto next_freed = segment_next && !segment_next->allocated;

    if (!prev_freed && !next_freed) {
        segment->allocated = false;
        heap_.Add(segment);
        return;
    }

    if (!prev_freed) {
        EnlargeFrom(segment_next, segment);
        segment->Unlink();
        delete segment;
        return;
    }
    if (!next_freed) {
        EnlargeTo(segment_prev, segment);
        segment->Unlink();
        delete segment;
        return;
    }

    EnlargeTo(segment_prev, segment_next);
    segment->Unlink();
    delete segment;

    heap_.Remove(segment_next);
    segment_next->Unlink();
    delete segment_next;
}

void Manager::EnlargeTo(Ptr enlarged, Ptr segment) {
    enlarged->to = segment->to;
    heap_.Update(enlarged);
}

void Manager::EnlargeFrom(Ptr enlarged, Ptr segment) {
    enlarged->from = segment->from;
    heap_.Update(enlarged);
}

Manager::MaxHeap::MaxHeap(int size) : data_() {
    data_.reserve(size);
}

void Manager::MaxHeap::Add(Ptr segment) {
    segment->heap_ind = data_.size();
    data_.push_back(segment);
    SiftUp(data_.size() - 1);
}

void Manager::MaxHeap::Remove(Ptr segment) {
    auto ind = segment->heap_ind;
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

void Manager::MaxHeap::Update(Ptr segment) {
    auto ind = segment->heap_ind;
    auto parent = (ind - 1) / 2;
    if (data_[ind]->Size() > data_[parent]->Size()) {
        SiftUp(ind);
    } else {
        SiftDown(ind);
    }
}

int Manager::MaxHeap::Size() const {
    return data_.size();
}

Manager::Ptr Manager::MaxHeap::Top() {
    return data_[0];
}

void Manager::MaxHeap::SiftUp(int ind) {
    auto parent = (ind - 1) / 2;
    while (ind != 0 && data_[ind]->Size() > data_[parent]->Size()) {
        Swap(parent, ind);
        ind = parent;
        parent = (ind - 1) / 2;
    }
}

void Manager::MaxHeap::SiftDown(int ind) {
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

void Manager::MaxHeap::Swap(int src, int dst) {
    std::swap(data_[src], data_[dst]);
    std::swap(data_[src]->heap_ind, data_[dst]->heap_ind);
}

void Manager::MaxHeap::Print(int vert, std::string indent) const {
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
            std::cout << manager.Alloc(request) << std::endl;
        } else {
            manager.Free(-request);
        }
        --query_count;
    }
    return 0;
}

#endif
