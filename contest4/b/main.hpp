#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <string>
#include <random>
#include <numeric>

std::mt19937 gen(738547485u);

int GenRandomInt() {
    std::uniform_int_distribution<int> dist(0, std::numeric_limits<int>::max());
    return dist(gen);
}

class Treap {
    struct Node {
        int key;
        int prior;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        Node(int added_key)
            : key(added_key), prior(GenRandomInt()), left(nullptr), right(nullptr) {
        }
    };

    using NodePtr = std::shared_ptr<Node>;
    NodePtr root_;
public:
    Treap() : root_(nullptr) {
    }

    void Insert (int val) {
        auto pair = Split(root_, val);
        NodePtr new_node = std::make_shared<Node>(val);
        root_ = Merge(pair.first, Merge(new_node, pair.second));
    }

    void Remove(int val) {
        auto pair = Split(root_, val);
        auto elem = Find(val);
        if (elem) {
            elem = Merge(elem->left, elem->right);
        }
    }

    NodePtr Find(int val) {
        // ? ? ? ? ? ?
        auto elem = root_;
        while (elem && elem->key != val) {
            if (val > elem->key) {
                elem = elem->right;
                continue;
            }
            elem = elem->left;
        }
        return elem;
    }

    void Print() const {
        PrintImpl(root_);
    }

    int GetHeight() const {
        return GetHeightImpl(root_);
    }
private:
    NodePtr Merge(NodePtr left, NodePtr right) {
        if (!left || !right) {
            return left ? left : right;
        }
        if (left->prior > right->prior) {
            left->right = Merge(left->right, right);
            return left;
        }
        right->left = Merge(left, right->left);
        return right;
    }
    std::pair<NodePtr, NodePtr> Split(NodePtr node, int val) {
        if (!node) {
            return {};
        }
        if (node->key <= val) {
            auto pair = Split(node->right, val);
            node->right = pair.first;
            return {node, pair.second};
        }
        auto pair = Split(node->left, val);
        node->left = pair.second;
        return {pair.first, node};
    }
    void PrintImpl(NodePtr node, std::string indent = "") const {
        if (!node) {
            return;
        }
        if (node == root_) {
            std::cout << "Treap\n";
        }
        std::cout << indent << node->key << " (" << node->prior << ')' << '\n';
        indent = indent == "" ? "'-> " : "    " + indent;
        PrintImpl(node->left, indent);
        PrintImpl(node->right, indent);
    }
    int GetHeightImpl(NodePtr node) const {
        if (!node) {
            return 0;
        }
        auto left = GetHeightImpl(node->left);
        auto right = GetHeightImpl(node->right);
        return 1 + std::max(left, right);
    }
};

struct Rect {};

int GetExternalRect(__attribute__((unused)) const std::vector<Rect>& data) {
    return 0;
}

#ifndef LOCAL
int main() {
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    int count;
    std::cin >> count;

    std::vector<Rect> data;
    while (count > 0) {
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
    }

    std::cout << GetExternalRect(data) << '\n';
    return 0;
}

#endif
