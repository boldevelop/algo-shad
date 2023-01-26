#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <string>
#include <random>
#include <numeric>

std::mt19937 gen(738547485u);

int GenRandomInt() {
    std::uniform_int_distribution<int> dist(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
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

/*
	if (!t)
		t = it;
	else if (it->prior > t->prior)
		split (t, it->key, it->l, it->r),  t = it;
	else
		insert (it->key < t->key ? t->l : t->r, it);

 */

    void Insert (int val) {
        auto pair = Split(root_, val);
        NodePtr new_node = std::make_shared<Node>(val);
        root_ = Merge(pair.first, Merge(new_node, pair.second));
    }

    void Remove(int val) {
        auto pair = Split(root_, val - 1);
        auto pair_second = Split(pair.second, val);
        std::cout << "Height: " << GetHeightImpl(pair_second.first) << std::endl;
        root_ = Merge(pair.first, pair_second.second);
    }

    NodePtr Find(int val) {
        auto pair = Split(root_, val - 1);
        return pair.second;
    }

    void Print() const {
        PrintImpl(root_);
    }

    int GetHeight() const {
        return GetHeightImpl(root_);
    }
    void PrintData(NodePtr node = nullptr) const {
        PrintDataImpl(node ? node : root_);
        std::cout << std::endl;
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
    void PrintDataImpl(NodePtr node) const {
        if (!node) {
            return;
        }
        PrintDataImpl(node->left);
        std::cout << node->key << " ";
        PrintDataImpl(node->right);
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
