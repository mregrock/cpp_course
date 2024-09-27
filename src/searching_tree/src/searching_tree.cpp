#include <memory>
#include <stack>
#include <utility>
#include <vector>
#include <stdexcept>

template <class K, class V>
class SearchingTree {
public:
    struct Node {
        K Key;
        V Value;
        std::unique_ptr<Node> Left;
        std::unique_ptr<Node> Right;

        Node(const K& k, const V& v) : Key(k), Value(v), Left(nullptr), Right(nullptr) {}
    };

    class Iterator {
    public:
        Iterator() : Current(nullptr) {}

        Iterator(Node* root) {
            PushLeft(root);
            Advance();
        }

        bool operator!=(const Iterator& other) const {
            return Current != other.Current;
        }

        bool operator==(const Iterator& other) const {
            return Current == other.Current;
        }

        std::pair<K, V> operator*() const {
            if (Current == nullptr) {
                throw std::out_of_range("Iterator out of range");
            }
            return {Current->Key, Current->Value};
        }

        std::pair<K, V>* operator->() {
            if (Current == nullptr) {
                throw std::out_of_range("Iterator out of range");
            }
            Pair = {Current->Key, Current->Value};
            return &Pair;
        }

        Iterator& operator++() {
            Advance();
            return *this;
        }

    private:
        std::stack<Node*> Stack;
        Node* Current;
        std::pair<K, V> Pair;

        void PushLeft(Node* node) {
            while (node) {
                Stack.push(node);
                node = node->Left.get();
            }
        }

        void Advance() {
            if (Stack.empty()) {
                Current = nullptr;
                return;
            }
            Current = Stack.top();
            Stack.pop();
            if (Current->Right) {
                PushLeft(Current->Right.get());
            }
        }

        friend class Range;
    };

    class Range {
    public:
        Range(const SearchingTree& tree, const K& a, const K& b) : Tree(tree), A(a), B(b) {}

        class RangeIterator {
        public:
            RangeIterator() : Current(nullptr), A(nullptr), B(nullptr) {}

            RangeIterator(Node* node, const K& a, const K& b) : A(&a), B(&b) {
                PushLeft(node);
                AdvanceToValid();
            }

            bool operator!=(const RangeIterator& other) const {
                return Current != other.Current;
            }

            bool operator==(const RangeIterator& other) const {
                return Current == other.Current;
            }

            std::pair<K, V> operator*() const {
                if (Current == nullptr) {
                    throw std::out_of_range("Iterator out of range");
                }
                return {Current->Key, Current->Value};
            }

            std::pair<K, V>* operator->() {
                if (Current == nullptr) {
                    throw std::out_of_range("Iterator out of range");
                }
                Pair = {Current->Key, Current->Value};
                return &Pair;
            }

            RangeIterator& operator++() {
                if (Stack.empty()) {
                    Current = nullptr;
                    return *this;
                }
                Node* node = Stack.top();
                Stack.pop();
                if (node->Right) {
                    PushLeft(node->Right.get());
                }
                AdvanceToValid();
                return *this;
            }

        private:
            std::stack<Node*> Stack;
            Node* Current;
            const K* A;
            const K* B;
            std::pair<K, V> Pair;

            void PushLeft(Node* node) {
                while (node) {
                    Stack.push(node);
                    node = node->Left.get();
                }
            }

            void AdvanceToValid() {
                Current = nullptr;
                while (!Stack.empty()) {
                    Node* node = Stack.top();
                    Stack.pop();
                    if (A && B && *A <= node->Key && node->Key < *B) {
                        Current = node;
                        break;
                    }
                    if (A && B && node->Key < *B && node->Right) {
                        PushLeft(node->Right.get());
                    }
                }
            }

            friend class SearchingTree;
        };

        RangeIterator begin() const {
            return RangeIterator(Tree.Root.get(), A, B);
        }

        RangeIterator end() const {
            return RangeIterator();
        }

    private:
        const SearchingTree& Tree;
        K A;
        K B;
    };

    Range range(const K& a, const K& b) const {
        return Range(*this, a, b);
    }

    SearchingTree() : Root(nullptr) {}

    void Insert(const K& key, const V& value) {
        Root = Split(std::move(Root), key, value);
    }

    void Erase(const K& key) {
        Root = Merge(std::move(Root), key);
    }

    Iterator Find(const K& key) const {
        Node* current = Root.get();
        while (current) {
            if (key < current->Key) {
                current = current->Left.get();
            } else if (current->Key < key) {
                current = current->Right.get();
            } else {
                return Iterator(current);
            }
        }
        return end();
    }

    Iterator begin() const {
        return Iterator(Root.get());
    }

    Iterator end() const {
        return Iterator();
    }

private:
    std::unique_ptr<Node> Root;

    std::unique_ptr<Node> Split(std::unique_ptr<Node> node, const K& key, const V& value) {
        if (!node) {
            return std::make_unique<Node>(key, value);
        }
        if (key < node->Key) {
            node->Left = Split(std::move(node->Left), key, value);
        } else if (node->Key < key) {
            node->Right = Split(std::move(node->Right), key, value);
        }
        return node;
    }

    std::unique_ptr<Node> Merge(std::unique_ptr<Node> node, const K& key) {
        if (!node) return node;
        if (key < node->Key) {
            node->Left = Merge(std::move(node->Left), key);
        } else if (node->Key < key) {
            node->Right = Merge(std::move(node->Right), key);
        } else {
            if (!node->Left) {
                return std::move(node->Right);
            }
            if (!node->Right) {
                return std::move(node->Left);
            }
            Node* minNode = FindMin(node->Right.get());
            node->Key = minNode->Key;
            node->Value = minNode->Value;
            node->Right = Merge(std::move(node->Right), minNode->Key);
        }
        return node;
    }

    Node* FindMin(Node* node) const {
        while (node->Left) {
            node = node->Left.get();
        }
        return node;
    }

    void InOrderRange(Node* node, const K& a, const K& b, std::vector<std::pair<K, V>>& result) const {
        if (!node) return;
        if (a < node->Key) {
            InOrderRange(node->Left.get(), a, b, result);
        }
        if (a <= node->Key && node->Key < b) {
            result.emplace_back(node->Key, node->Value);
        }
        if (node->Key < b) {
            InOrderRange(node->Right.get(), a, b, result);
        }
    }
};