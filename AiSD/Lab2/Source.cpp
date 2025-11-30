#include <iostream>
#include <unordered_set>
#include <random>
#include <ctime>

#include "AVLTree.h"
#include "RBTree.h"

int main() {
    Tree tree;
    AVLTree avlTree;
    RBTree rbTree;

    std::cout << "from 1 000 to 10 000:\n" << std::endl;

    for (size_t i = 0; i < 10; i++) {
        for (size_t j = i * 1000; j < (i + 1) * 1000; j++) {
            tree.insert(j);
            avlTree.insert(j);
            rbTree.insert(j);
        }
        std::cout << "===" << (i + 1) * 1000 << "===" << std::endl;;
        std::cout << "Tree: " << tree.getHeight() << std::endl;
        std::cout << "AVLTree: " << avlTree.getHeight() << std::endl;
        std::cout << "RBTree: " << rbTree.getHeight() << std::endl;
    }
    
    tree.clear();
    avlTree.clear();
    rbTree.clear();

    std::cout << "\n\n\nrandom:\n" << std::endl;

    for (size_t i = 0; i < 10; i++) {
        std::unordered_set<size_t> data;
        data.reserve(1000);
        while(data.size() < 1000) {
            data.insert(std::rand() % 100000);
        }
        for (const size_t& j : data) {
            tree.insert(j);
            avlTree.insert(j);
            rbTree.insert(j);
        }
        std::cout << "===" << (i + 1) * 1000 << "===" << std::endl;
        std::cout << "Tree: " << tree.getHeight() << std::endl;
        std::cout << "AVLTree: " << avlTree.getHeight() << std::endl;
        std::cout << "RBTree: " << rbTree.getHeight() << std::endl;
    }

    return 0;
}
