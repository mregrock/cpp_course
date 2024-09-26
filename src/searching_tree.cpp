#include <memory>
#include <stack>
#include <utility>
#include <vector>
#include <stdexcept>

template <class K, class V>
class SearchingTree {
public:
    struct Node {
        K key;
        V value;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        Node(const K& k, const V& v) : key(k), value(v), left(nullptr), right(nullptr) {}
    };

    class Iterator {
    public:
        Iterator() : current_(nullptr) {}

        Iterator(Node* root) {
            PushLeft(root);
            Advance();
        }

        bool operator!=(const Iterator& other) const {
            return current_ != other.current_;
        }

        bool operator==(const Iterator& other) const {
            return current_ == other.current_;
        }

        std::pair<const K&, V&> operator*() const {
            if (current_ == nullptr) {
                throw std::out_of_range("Iterator out of range");
            }
            return {current_->key, current_->value};
        }

        struct Proxy {
            std::pair<const K&, V&> pair;

            Proxy(const std::pair<const K&, V&>& p) : pair(p) {}

            std::pair<const K&, V&>* operator->() const {
                return const_cast<std::pair<const K&, V&>*>(&pair);
            }
        };

        Proxy operator->() const {
            return Proxy(operator*());
        }

        Iterator& operator++() {
            Advance();
            return *this;
        }

    private:
        std::stack<Node*> stack_;
        Node* current_;

        void PushLeft(Node* node) {
            while (node) {
                stack_.push(node);
                node = node->left.get();
            }
        }

        void Advance() {
            if (stack_.empty()) {
                current_ = nullptr;
                return;
            }
            current_ = stack_.top();
            stack_.pop();
            if (current_->right) {
                PushLeft(current_->right.get());
            }
        }

        friend class Range;
    };

    class Range {
    public:
        Range(const SearchingTree& tree, const K& a, const K& b) : tree_(tree), a_(a), b_(b) {}

        class RangeIterator {
        public:
            RangeIterator() : current_(nullptr), a_(nullptr), b_(nullptr) {}

            RangeIterator(Node* node, const K& a, const K& b) : a_(&a), b_(&b) {
                PushLeft(node);
                advanceToValid();
            }

            bool operator!=(const RangeIterator& other) const {
                return current_ != other.current_;
            }

            bool operator==(const RangeIterator& other) const {
                return current_ == other.current_;
            }

            std::pair<const K&, V&> operator*() const {
                if (current_ == nullptr) {
                    throw std::out_of_range("Iterator out of range");
                }
                return {current_->key, current_->value};
            }

            struct Proxy {
                std::pair<const K&, V&> pair;

                Proxy(const std::pair<const K&, V&>& p) : pair(p) {}

                std::pair<const K&, V&>* operator->() const {
                    return const_cast<std::pair<const K&, V&>*>(&pair);
                }
            };

            Proxy operator->() const {
                return Proxy(operator*());
            }

            RangeIterator& operator++() {
                if (stack_.empty()) {
                    current_ = nullptr;
                    return *this;
                }
                Node* node = stack_.top();
                stack_.pop();
                if (node->right) {
                    PushLeft(node->right.get());
                }
                advanceToValid();
                return *this;
            }

        private:
            std::stack<Node*> stack_;
            Node* current_;
            const K* a_;
            const K* b_;

            void PushLeft(Node* node) {
                while (node) {
                    stack_.push(node);
                    node = node->left.get();
                }
            }

            void advanceToValid() {
                current_ = nullptr;
                while (!stack_.empty()) {
                    Node* node = stack_.top();
                    stack_.pop();
                    if (a_ && b_ && *a_ <= node->key && node->key < *b_) {
                        current_ = node;
                        break;
                    }
                    if (a_ && b_ && node->key < *b_ && node->right) {
                        PushLeft(node->right.get());
                    }
                }
            }

            friend class SearchingTree;
        };

        RangeIterator begin() const {
            return RangeIterator(tree_.root_.get(), a_, b_);
        }

        RangeIterator end() const {
            return RangeIterator();
        }

    private:
        const SearchingTree& tree_;
        K a_;
        K b_;
    };

    Range range(const K& a, const K& b) const {
        return Range(*this, a, b);
    }

    SearchingTree() : root_(nullptr) {}

    void Insert(const K& key, const V& value) {
        root_ = Split(std::move(root_), key, value);
    }

    void Erase(const K& key) {
        root_ = Merge(std::move(root_), key);
    }

    Iterator Find(const K& key) const {
        Node* current = root_.get();
        while (current) {
            if (key < current->key) {
                current = current->left.get();
            } else if (current->key < key) {
                current = current->right.get();
            } else {
                return Iterator(current);
            }
        }
        return end();
    }

    Iterator begin() const {
        return Iterator(root_.get());
    }

    Iterator end() const {
        return Iterator();
    }

private:
    std::unique_ptr<Node> root_;

    std::unique_ptr<Node> Split(std::unique_ptr<Node> node, const K& key, const V& value) {
        if (!node) {
            return std::make_unique<Node>(key, value);
        }
        if (key < node->key) {
            node->left = Split(std::move(node->left), key, value);
        } else if (node->key < key) {
            node->right = Split(std::move(node->right), key, value);
        }
        return node;
    }

    std::unique_ptr<Node> Merge(std::unique_ptr<Node> node, const K& key) {
        if (!node) return node;
        if (key < node->key) {
            node->left = Merge(std::move(node->left), key);
        } else if (node->key < key) {
            node->right = Merge(std::move(node->right), key);
        } else {
            if (!node->left) {
                return std::move(node->right);
            }
            if (!node->right) {
                return std::move(node->left);
            }
            Node* minNode = FindMin(node->right.get());
            node->key = minNode->key;
            node->value = minNode->value;
            node->right = Merge(std::move(node->right), minNode->key);
        }
        return node;
    }

    Node* FindMin(Node* node) const {
        while (node->left) {
            node = node->left.get();
        }
        return node;
    }

    void InOrderRange(Node* node, const K& a, const K& b, std::vector<std::pair<const K&, V&>>& result) const {
        if (!node) return;
        if (a < node->key) {
            InOrderRange(node->left.get(), a, b, result);
        }
        if (a <= node->key && node->key < b) {
            result.emplace_back(node->key, node->value);
        }
        if (node->key < b) {
            InOrderRange(node->right.get(), a, b, result);
        }
    }
};