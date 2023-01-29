#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <string>
#include <random>
#include <numeric>
#include <limits>
#include <algorithm>
#include <sstream>

std::mt19937 gen(738547485u);

int GenRandomInt() {
    std::uniform_int_distribution<int> dist(std::numeric_limits<int>::min(),
                                            std::numeric_limits<int>::max());
    return dist(gen);
}

template <class ValueType>
class Treap {
    struct Node {
        ValueType key;
        int prior;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::weak_ptr<Node> parent;

        explicit Node(ValueType added_key)
            : key(added_key), prior(GenRandomInt()), left(nullptr), right(nullptr), parent() {
        }
    };

public:
    using NodePtr = std::shared_ptr<Node>;

private:
    NodePtr root_;
    int size_;

public:
    Treap() : root_(nullptr), size_(0) {
    }

    void Insert(ValueType val) {
        auto splitted = Split(root_, val);
        if (!splitted.elem) {
            NodePtr new_node = std::make_shared<Node>(val);
            root_ = Merge(Merge(splitted.left, new_node), splitted.right);
            ++size_;
        } else {
            root_ = Merge(Merge(splitted.left, splitted.elem), splitted.right);
        }
        UpdateParent(root_);
    }

    void Remove(ValueType val) {
        auto splitted = Split(root_, val);
        root_ = Merge(splitted.left, splitted.right);
        if (splitted.elem) {
            --size_;
        }
        UpdateParent(root_);
    }

    NodePtr Find(ValueType val) {
        auto splitted = Split(root_, val);
        root_ = Merge(Merge(splitted.left, splitted.elem), splitted.right);
        UpdateParent(root_);
        return splitted.elem;
    }
    NodePtr FindMin(NodePtr node = nullptr) const {
        node = node ? node : root_;
        while (node->left) {
            node = node->left;
        }
        return node;
    }
    NodePtr FindMax(NodePtr node = nullptr) const {
        node = node ? node : root_;
        while (node->right) {
            node = node->right;
        }
        return node;
    }

    NodePtr LowerBound(ValueType val) {
        auto splitted = Split(root_, val);
        root_ = Merge(Merge(splitted.left, splitted.elem), splitted.right);
        UpdateParent(root_);
        if (splitted.elem) {
            return splitted.elem;
        }
        if (splitted.right) {
            auto elem = splitted.right;
            while (elem) {
                if (!elem->left ||
                    (elem->left && (elem->left->key < val || Equal(elem->left->key, val)))) {
                    break;
                }
                elem = elem->left;
            }
            return elem;
        }
        return nullptr;
    }

    // NodePtr UpperBound(int val) {
    //     auto splitted = Split(root_, val);
    //     root_ = Merge(Merge(splitted.left, splitted.elem), splitted.right);
    //     UpdateParent(root_);
    //     auto elem = splitted.right;
    //     while (elem) {
    //         if (!elem->left || (elem->left && elem->left->key < val)) {
    //             break;
    //         }
    //         elem = elem->left;
    //     }
    //     return splitted.right;
    // }

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

    void UpdateParent(NodePtr& parent) {
        if (parent) {
            if (parent->left) {
                parent->left->parent = parent;
            }
            if (parent->right) {
                parent->right->parent = parent;
            }
        }
    }

    void AssignNode(NodePtr& parent, NodePtr child) {
        parent = child;
        UpdateParent(parent);
    }

    NodePtr Merge(NodePtr left, NodePtr right) {
        if (!left || !right) {
            return left ? left : right;
        }
        if (left->prior > right->prior) {
            AssignNode(left->right, Merge(left->right, right));
            return left;
        }
        AssignNode(right->left, Merge(left, right->left));
        return right;
    }

    bool Equal(const ValueType& lhs, const ValueType& rhs) const {
        return !(lhs < rhs) && !(rhs < lhs);
    }

    Splitted Split(NodePtr node, ValueType val) {
        if (!node) {
            return {nullptr, nullptr, nullptr};
        }
        if (Equal(node->key, val)) {
            // if (node->left) {
            //     node->left->parent = std::weak_ptr<Node>();
            // }
            // if (node->right) {
            //     node->right->parent = std::weak_ptr<Node>();
            // }
            Splitted splitted{node->left, node, node->right};
            splitted.elem->left = nullptr;
            splitted.elem->right = nullptr;
            return splitted;
        }
        if (node->key < val) {
            auto splitted = Split(node->right, val);
            AssignNode(node->right, splitted.left);
            // if (splitted.right) {
            //     splitted.right->parent = std::weak_ptr<Node>();
            // }
            return {node, splitted.elem, splitted.right};
        }
        auto splitted = Split(node->left, val);
        AssignNode(node->left, splitted.right);
        // if (splitted.left) {
        //     splitted.left->parent = std::weak_ptr<Node>();
        // }
        return {splitted.left, splitted.elem, node};
    }

    std::string GetAddr(NodePtr node) const {
        std::stringstream ssstream;
        ssstream << node.get();
        std::string addr = ssstream.str();
        return addr.substr(addr.size() - 6);
    }

    void PrintParent(NodePtr node) const {
        std::cout << GetAddr(node);
        auto parent = node->parent.lock();
        if (parent) {
            std::cout << ", " << GetAddr(parent);
        }
    }

    void PrintImpl(NodePtr node, std::string indent = "") const {
        if (!node) {
            std::cout << indent << "X" << '\n';
            return;
        }
        if (node == root_) {
            std::cout << "Treap\n";
        }
        std::cout << indent << node->key << " (";
        PrintParent(node);
        std::cout << ')' << '\n';
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

template <class ValueType>
class Set {
    mutable Treap<ValueType> treap_;

    using TreeNode = typename Treap<ValueType>::NodePtr;
    using Tree = Treap<ValueType>;

public:
    class BstIterator : public std::iterator<std::bidirectional_iterator_tag, const ValueType> {
    private:
        friend class Set<ValueType>;
        TreeNode node_;
        const Tree* treap_;

        BstIterator(TreeNode node, Tree* treap) : node_(node), treap_(treap) {
        }

    public:
        BstIterator() : node_(), treap_() {
        }
        bool operator==(const BstIterator& rhs) const {
            return node_ == rhs.node_;
        }
        bool operator!=(const BstIterator& rhs) const {
            return !(*this == rhs);
        }
        const ValueType& operator*() const {
            return node_->key;
        }
        ValueType* operator->() const {
            return &node_->key;
        }
        BstIterator& operator++() {
            if (node_->right) {
                node_ = node_->right;
                node_ = treap_->FindMin(node_);
            } else {
                TreeNode parent = node_->parent.lock();
                while (parent && node_ == parent->right) {
                    node_ = parent;
                    parent = parent->parent.lock();
                }
                node_ = parent;
            }

            return *this;
        }
        BstIterator operator++(int) {
            BstIterator result(*this);
            ++(*this);
            return result;
        }
        BstIterator operator--() {
            if (!node_) {
                node_ = treap_->FindMax();
            } else {
                if (node_->left) {
                    node_ = node_->left;
                    node_ = treap_->FindMax(node_);
                } else {
                    TreeNode parent;
                    parent = node_->parent.lock();
                    while (parent && node_ == parent->left) {
                        node_ = parent;
                        parent = parent->parent.lock();
                    }
                    node_ = parent;
                }
            }
            return *this;
        }
        BstIterator operator--(int) {
            BstIterator result(*this);
            --(*this);
            return result;
        }
    };

    using const_iterator = BstIterator;
    using iterator = const_iterator;

    Set() : treap_() {
    }
    template <class Iter>
    Set(Iter begin, Iter end) : treap_() {
        while (begin != end) {
            insert(*begin);
            begin = std::next(begin);
        }
    }
    Set(std::initializer_list<ValueType> list) : Set(list.begin(), list.end()) {
    }

    Set(const Set<ValueType>& src) {
        Tree treap;
        for (auto elem : src) {
            treap.Insert(elem);
        }
        treap_ = treap;
    }
    Set<ValueType> operator=(const Set<ValueType>& src) {
        Tree treap;
        for (auto elem : src) {
            treap.Insert(elem);
        }
        treap_ = treap;
        return *this;
    }

    void insert(ValueType elem) {
        treap_.Insert(elem);
    }
    void erase(ValueType elem) {
        treap_.Remove(elem);
    }
    const_iterator find(ValueType elem) const {
        return BstIterator(treap_.Find(elem), &treap_);
    }
    const_iterator lower_bound(ValueType elem) const {
        return BstIterator(treap_.LowerBound(elem), &treap_);
    }
    const_iterator begin() const {
        return BstIterator(treap_.FindMin(), &treap_);
    };
    const_iterator end() const {
        return BstIterator(nullptr, &treap_);
    }
    int size() const {
        return treap_.Size();
    }
    bool empty() const {
        return treap_.Empty();
    }

    void Print() const {
        treap_.Print();
        treap_.PrintData();
    }
};
