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
namespace Simple {
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

        void Insert(int val) {
            auto splitted = Split(root_, val);
            NodePtr new_node = std::make_shared<Node>(val);
            root_ = Merge(Merge(splitted.left, new_node), splitted.right);
        }

        void Remove(int val) {
            auto splitted = Split(root_, val);
            root_ = Merge(splitted.left, splitted.right);
        }

        NodePtr Find(int val) {
            auto splitted = Split(root_, val);
            root_ = Merge(Merge(splitted.left, splitted.elem), splitted.right);
            return splitted.elem;
        }

        NodePtr UpperBound(int val) {
            auto splitted = Split(root_, val);
            root_ = Merge(Merge(splitted.left, splitted.elem), splitted.right);
            return splitted.right;
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
        struct Splitted {
            NodePtr left;
            NodePtr elem;
            NodePtr right;
        };

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

        Splitted Split(NodePtr node, int val) {
            if (!node) {
                return {nullptr, nullptr, nullptr};
            }
            if (node->key == val) {
                Splitted splitted{node->left, node, node->right};
                splitted.elem->left = nullptr;
                splitted.elem->right = nullptr;
                return splitted;
            }
            if (node->key < val) {
                auto splitted = Split(node->right, val);
                node->right = splitted.left;
                return {node, splitted.elem, splitted.right};
            }
            auto splitted = Split(node->left, val);
            node->left = splitted.right;
            return {splitted.left, splitted.elem, node};
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
}

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
