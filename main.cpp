#include "src/searching_tree.cpp"
#include <iostream>
#include <cassert>

int main() {
    SearchingTree<int, std::string> tree;

    // Test insertion
    tree.Insert(10, "ten");
    tree.Insert(5, "five");
    tree.Insert(15, "fifteen");
    tree.Insert(3, "three");
    tree.Insert(7, "seven");
    tree.Insert(12, "twelve");
    tree.Insert(18, "eighteen");

    // Test search for existing key
    auto it = tree.Find(7);
    assert(it != tree.end());
    std::cout << "Found key 7: " << it->second << std::endl;

    // Test search for non-existing key
    it = tree.Find(20);
    assert(it == tree.end());
    std::cout << "Key 20 not found." << std::endl;

    // Test deletion of leaf node
    tree.Erase(3);
    it = tree.Find(3);
    assert(it == tree.end());
    std::cout << "Key 3 successfully deleted." << std::endl;

    // Test deletion of node with one child
    tree.Erase(18);
    it = tree.Find(18);
    assert(it == tree.end());
    std::cout << "Key 18 successfully deleted." << std::endl;

    // Test deletion of node with two children
    tree.Erase(10);
    it = tree.Find(10);
    assert(it == tree.end());
    std::cout << "Key 10 successfully deleted." << std::endl;

    // Test range query
    auto range = tree.range(5, 15);
    std::cout << "Range from 5 to 15:" << std::endl;
    for (const auto& pair : range) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // Iteration over the tree
    std::cout << "All tree elements:" << std::endl;
    for (const auto& pair : tree) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // Tests using structured bindings
    std::cout << "Iteration using structured bindings:" << std::endl;
    for (const auto& [k, v] : tree) {
        std::cout << k << ": " << v << std::endl;
    }

    std::cout << "Range iteration from 5 to 15 using structured bindings:" << std::endl;
    for (const auto& [k, v] : tree.range(5, 15)) {
        std::cout << k << ": " << v << std::endl;
    }

    std::cout << "All tests passed successfully." << std::endl;
    return 0;
}
