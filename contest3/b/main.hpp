#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <string>

struct HashData {
    long double hash;
    int vertex;
};

const uint64_t kPrime = 2 * 1'000'000'000 + 11;

class VertexHashTable {
    std::vector<std::list<HashData>> data_;
    int size_;

public:
    VertexHashTable() : data_(10000) {
    }

    void Add(long double key, int vertex) {
        data_[Hash(key) % 10000].push_back({key, vertex});
        size_++;
    }

    int Remove(long double key) {
        auto& list = data_[Hash(key) % 10000];
        int vertex = -1;

        for (auto it = list.begin(); it != list.end(); ++it) {
            if (it->hash == key) {
                vertex = it->vertex;
                list.erase(it);
                break;
            }
        }

        --size_;
        return vertex;
    }

private:
    uint Hash(long double key) const {
        uint hash = 0;
        std::memcpy(&key, &hash, sizeof(hash));
        return hash % kPrime;
    }
};

class Tree {
    std::vector<std::vector<int>> data_;
    int size_;
    std::vector<int> used_;
    std::vector<long double> hashes_;

public:
    explicit Tree(int size) : data_(size), size_(size) {
    }

    void AddEdge(int src, int dst) {
        data_[src - 1].push_back(dst - 1);
        data_[dst - 1].push_back(src - 1);
    }

    std::vector<int> GetCentroids() {
        std::vector<int> centroids;
        used_.resize(size_);
        DfsGetCentroids(0, -1, &centroids);
        used_.clear();
        return centroids;
    }

    long double Hash(int centroid, bool save_hashes = false) {
        if (save_hashes) {
            hashes_.resize(size_);
        }

        used_.resize(size_);
        auto hash = DfsHash(centroid, save_hashes);

        if (save_hashes) {
            hashes_[centroid] = hash;
        }

        used_.clear();
        return hash;
    }

    long double HashWithMap(int centroid, VertexHashTable* map) {
        used_.resize(size_);
        auto hash = DfsHashWithMap(centroid, map);

        map->Add(hash, centroid);

        used_.clear();
        return hash;
    }

    void Print(int vertex) {
        used_.resize(size_);
        DfsPrint(vertex, "");
        used_.clear();
    }

    bool IsIsomorphicWith(Tree& tree) {
        auto centroids = GetCentroids();
        auto another_centroids = tree.GetCentroids();
        std::cout << "centroids size: " << centroids.size() << std::endl;
        if (centroids.size() != another_centroids.size()) {
            std::cout << "-1" << std::endl;
            return false;
        }

        VertexHashTable map;
        auto first_hash = HashWithMap(centroids[0], &map);
        auto is_poly = first_hash == tree.Hash(another_centroids[0], true);

        if (is_poly) {
            std::cout << "H: " << first_hash << " " << tree.Hash(another_centroids[1], true) << std::endl;
            tree.PrintOldKeys(map);
            return true;
        }

        if (centroids.size() == 2 && first_hash == tree.Hash(another_centroids[1], true)) {
            tree.PrintOldKeys(map);
            return true;
        }

        std::cout << "-1" << std::endl;

        return false;
    }

    void PrintOldKeys(VertexHashTable& map) {
        for (auto hash : hashes_) {
            auto old_vert = map.Remove(hash);
            std::cout << (old_vert + 1) << std::endl;
        }
    }

private:
    long double DfsHash(int vertex, bool save_hashes) {
        used_[vertex] = 1;
        bool is_leaf = true;

        std::vector<long double> child_hashes;
        std::vector<int> vertexes;

        for (int child_v : data_[vertex]) {
            if (used_[child_v] != 0) {
                continue;
            }
            is_leaf = false;
            child_hashes.push_back(std::log(DfsHash(child_v, save_hashes)));
            vertexes.push_back(child_v);
        }

        if (is_leaf) {
            return 199.0;
        }

        if (save_hashes) {
            for (uint i = 0; i < child_hashes.size(); ++i) {
                hashes_[vertexes[i]] = child_hashes[i];
            }
        }

        std::sort(child_hashes.begin(), child_hashes.end());

        long double sum = 3.0;
        for (auto child_h : child_hashes) {
            sum += child_h;
        }

        return sum;
    }

    long double DfsHashWithMap(int vertex, VertexHashTable* map) {
        used_[vertex] = 1;
        bool is_leaf = true;

        std::vector<long double> child_hashes;
        std::vector<int> vertexes;

        for (int child_v : data_[vertex]) {
            if (used_[child_v] != 0) {
                continue;
            }
            is_leaf = false;
            child_hashes.push_back(std::log(DfsHashWithMap(child_v, map)));
            vertexes.push_back(child_v);
        }

        if (is_leaf) {
            return 199.0;
        }

        for (uint i = 0; i < child_hashes.size(); ++i) {
            map->Add(child_hashes[i], vertexes[i]);
        }

        std::sort(child_hashes.begin(), child_hashes.end());

        long double sum = 3.0;
        for (auto child_h : child_hashes) {
            sum += child_h;
        }

        return sum;
    }

    void DfsGetCentroids(int vert, int prev_vert, std::vector<int>* centroids) {
        used_[vert] = 1;
        bool is_centroid = true;

        for (auto child_v : data_[vert]) {
            if (child_v != prev_vert) {
                DfsGetCentroids(child_v, vert, centroids);
                used_[vert] += used_[child_v];

                if (used_[child_v] > size_ / 2) {
                    is_centroid = false;
                }
            }
        }

        if (size_ - used_[vert] > size_ / 2) {
            is_centroid = false;
        }

        if (is_centroid) {
            centroids->push_back(vert);
        }
    }

    void DfsPrint(int vertex, std::string indent = "") {
        used_[vertex] = 1;
        std::cout << indent << vertex << std::endl;
        for (int child_v : data_[vertex]) {
            if (used_[child_v] != 0) {
                continue;
            }
            DfsPrint(child_v, indent + "+- ");
        }
    }
};

// int main() {
//     std::cin.tie(nullptr);
//     std::ios_base::sync_with_stdio(false);

//     int size;
//     int a_src, b_dist;
//     std::cin >> size;

//     Tree first(size);
//     for (int i = 1; i < size; ++i) {
//         std::cin >> a_src >> b_dist;
//         first.AddEdge(a_src, b_dist);
//     }

//     Tree second(size);
//     for (int i = 1; i < size; ++i) {
//         std::cin >> a_src >> b_dist;
//         second.AddEdge(a_src, b_dist);
//     }

//     first.IsIsomorphicWith(second);
// }
