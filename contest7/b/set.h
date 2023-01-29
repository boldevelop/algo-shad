#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <string>
#include <random>
#include <numeric>
#include <limits>
#include <algorithm>

std::mt19937 gen(738547485u);

int GenRandomInt() {
    std::uniform_int_distribution<int> dist(std::numeric_limits<int>::min(),
                                            std::numeric_limits<int>::max());
    return dist(gen);
}
/*
void printHelper(NodePtr root, string indent, bool last) {
    // print the tree structure on the screen
    if (root != nullptr) {
        cout<<indent;
        if (last) {
            cout<<"└────";
            indent += "     ";
        } else {
            cout<<"├────";
            indent += "|    ";
        }

        cout<<root->data<<"("<<root->priority<<")"<<endl;

        printHelper(root->left, indent, false);
        printHelper(root->right, indent, true);
    }
}
 */
class Treap {
    struct Node {
        int key;
        int prior;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        explicit Node(int added_key)
            : key(added_key), prior(GenRandomInt()), left(nullptr), right(nullptr) {
        }
    };

    using NodePtr = std::shared_ptr<Node>;
    NodePtr root_;
    int size_;

public:
    Treap() : root_(nullptr), size_(0) {
    }

    void Insert(int val) {
        auto splitted = Split(root_, val);
        if (!splitted.elem) {
            NodePtr new_node = std::make_shared<Node>(val);
            root_ = Merge(Merge(splitted.left, new_node), splitted.right);
            ++size_;
        } else {
            root_ = Merge(Merge(splitted.left, splitted.elem), splitted.right);
        }
    }

    void Remove(int val) {
        auto splitted = Split(root_, val);
        root_ = Merge(splitted.left, splitted.right);
        if (splitted.elem) {
            --size_;
        }
    }

    NodePtr Find(int val) {
        auto splitted = Split(root_, val);
        root_ = Merge(Merge(splitted.left, splitted.elem), splitted.right);
        return splitted.elem;
    }

    NodePtr LowerBound(int val) {
        auto splitted = Split(root_, val);
        root_ = Merge(Merge(splitted.left, splitted.elem), splitted.right);
        if (splitted.elem) {
            return splitted.elem;
        }
        if (splitted.right) {
            auto elem = splitted.right;
            while (elem) {
                if (!elem->left || (elem->left && elem->left->key <= val)) {
                    break;
                }
                elem = elem->left;
            }
            return elem;
        }
        return nullptr;
    }

    NodePtr UpperBound(int val) {
        auto splitted = Split(root_, val);
        root_ = Merge(Merge(splitted.left, splitted.elem), splitted.right);
        auto elem = splitted.right;
        while (elem) {
            if (!elem->left || (elem->left && elem->left->key < val)) {
                break;
            }
            elem = elem->left;
        }
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
    int Size() const {
        return size_;
    }
    bool Empty() const {
        return size_ == 0;
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
        indent = indent.empty() ? "'-> " : "    " + indent;
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

template<class ValueType>
class Set {
    Treap treap_;

public:
/*
In a binary tree, to do operator++.

We need to know not only where we are,

but also how we got here.
 */
    class iterator {

    };

    Set() : treap_() {
    }

    template<class Iter>
    Set(Iter begin, Iter end) : treap_() {
        while (begin != end) {
            insert(*begin);
            begin = std::next(begin);
        }
    }
    Set(std::initializer_list<ValueType> list) : Set(list.begin(), list.end()) {
    }

    /* нужен итератор */
    // Set(const Set<ValueType>& src);
    // Set<ValueType> operator=(const Set<ValueType>& src);

    int size() const {
        return treap_.Size();
    }
    bool empty() const {
        return treap_.Empty();
    }
    void insert(ValueType elem) {
        treap_.Insert(elem);
    }
    void erase(ValueType elem) {
        treap_.Remove(elem);
    }
    ValueType find(ValueType elem) {
        auto node = treap_.Find(elem);
        if (node) {
            return elem;
        }
        return -1;
    }
    ValueType lower_bound(ValueType elem) {
        auto node = treap_.LowerBound(elem);
        if (node) {
            return node->key;
        }
        return -1;
    }

    void Print() {
        treap_.Print();
        treap_.PrintData();
    }

    iterator begin() const;
    iterator end() const;
};
