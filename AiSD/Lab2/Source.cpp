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
    std::stringstream ssTree, ssAVLTree, ssRBTree;

    //std::cout << "from 1 000 to 50 000:\n" << std::endl;
    //for (size_t i = 0; i < 50; i++) {
    //    for (size_t j = i * 1000; j < (i + 1) * 1000; j++) {
    //        tree.insert(j);
    //        avlTree.insert(j);
    //        rbTree.insert(j);
    //    }
    //    ssTree << tree.getHeight() << ", ";
    //    ssAVLTree << avlTree.getHeight() << ", ";
    //    ssRBTree << rbTree.getHeight() << ", ";
    //    //std::cout << "===" << (i + 1) * 1000 << "===" << std::endl;;
    //    //std::cout << "Tree: " << tree.getHeight() << std::endl;
    //    //std::cout << "AVLTree: " << avlTree.getHeight() << std::endl;
    //    //std::cout << "RBTree: " << rbTree.getHeight() << std::endl;
    //}
    //std::cout << "===END===" << std::endl;
    //std::cout << "Tree: " << ssTree.str() << std::endl;
    //std::cout << "AVLTree: " << ssAVLTree.str() << std::endl;
    //std::cout << "RBTree: " << ssRBTree.str() << std::endl;

    //tree.clear();
    //avlTree.clear();
    //rbTree.clear();
    //ssTree.str("");
    //ssAVLTree.str("");
    //ssRBTree.str("");

    //std::cout << "\n\n\nrandom:\n" << std::endl;
    //for (size_t i = 0; i < 50; i++) {
    //    std::unordered_set<size_t> data;
    //    data.reserve(1000);
    //    while(data.size() < 1000) {
    //        data.insert(std::rand()%5000);
    //    }
    //    for (const size_t& j : data) {
    //        tree.insert(j);
    //        avlTree.insert(j);
    //        rbTree.insert(j);
    //    }
    //    ssTree << tree.getHeight() << ", ";
    //    ssAVLTree << avlTree.getHeight() << ", ";
    //    ssRBTree << rbTree.getHeight() << ", ";
    //    //std::cout << "===" << (i + 1) * 1000 << "===" << std::endl;
    //    //std::cout << "Tree: " << tree.getHeight() << std::endl;
    //    //std::cout << "AVLTree: " << avlTree.getHeight() << std::endl;
    //    //std::cout << "RBTree: " << rbTree.getHeight() << std::endl;
    //}
    //std::cout << "===END===" << std::endl;
    //std::cout << "Tree: " << ssTree.str() << std::endl;
    //std::cout << "AVLTree: " << ssAVLTree.str() << std::endl;
    //std::cout << "RBTree: " << ssRBTree.str() << std::endl;


    //std::vector<size_t> V = {3787, 41, 24, 3467, 1334, 1500, 4169, 724, 1478, 4358, 3986};
    //for (const size_t& i : V) {
    //    avlTree.insert(i);
    //    std::cout << i << " -> " << avlTree.getHeight() << std::endl;
    //    std::cout << avlTree.levelOrder() << std::endl << std::endl;
    //}



    std::cout << "\n\n\nrandom:\n" << std::endl;
    for (size_t i = 0; i < 1; i++) {
        std::unordered_set<size_t> data;
        data.reserve(100);
        while (data.size() < 500) {
            data.insert(std::rand() % 500);
        }
        for (const size_t& j : data) {
            avlTree.insert(j);
            ssAVLTree << avlTree.getHeight() << ' ';
        }
        std::vector<size_t> vdata = { data.begin(), data.end() };
        std::cout << ssAVLTree.str() << std::endl;
        ssAVLTree.str("");
        while (!vdata.empty()) {
            const size_t& x = std::rand() % vdata.size();
            std::cout << vdata[x] <<std::endl;
            avlTree.del(vdata[x]);
            vdata.erase(vdata.begin() + x);
            ssAVLTree << avlTree.getHeight() << ' ';
        }
    }

    std::cout << "===END===" << std::endl;
    std::cout << "Tree: " << ssTree.str() << std::endl;
    std::cout << "AVLTree: " << ssAVLTree.str() << std::endl;
    std::cout << "RBTree: " << ssRBTree.str() << std::endl;

    return 0;
}
