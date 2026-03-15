#include <iostream>
#include <unordered_set>
#include <random>
#include <ctime>
#include <thread>
#include <algorithm>
#include <numeric>

#include "AVLTree.h"
#include "RBTree.h"


#define NUM_OF_NODE 100000
#define CHASTOTA 1000



int main() {
    Tree tree;
    AVLTree avlTree;
    RBTree rbTree;
    std::stringstream ssTree, ssAVLTree, ssRBTree;
    std::vector<std::thread> vThr;
    vThr.reserve(3);
    std::vector<Tree*> vTree = { &tree,  &avlTree, &rbTree };
    std::vector<std::stringstream*> vssTree = { &ssTree,  &ssAVLTree, &ssRBTree };

    std::cout << "from 1 000 to " << NUM_OF_NODE << ":\n" << std::endl;
    for (char l = 0; l < 3; l++) {
        vThr.emplace_back([l, &vTree, &vssTree] {
            for (size_t i = 0; i < NUM_OF_NODE / CHASTOTA; i++) {
                for (size_t j = i * CHASTOTA; j < (i + 1) * CHASTOTA; j++) vTree[l]->insert(j);
                *vssTree[l] << vTree[l]->getHeight() << ", ";
            }
            });
    }
    for (std::thread& i : vThr)
        i.join();
    vThr.clear();
    std::cout << "===END===" << std::endl;
    std::cout << "Tree: " << ssTree.str() << std::endl;
    std::cout << "AVLTree: " << ssAVLTree.str() << std::endl;
    std::cout << "RBTree: " << ssRBTree.str() << std::endl;

    tree.clear();
    avlTree.clear();
    rbTree.clear();
    ssTree.str("");
    ssAVLTree.str("");
    ssRBTree.str("");

    std::cout << "\n\n\nrandom:\n" << std::endl;
    std::vector<size_t> vdata(NUM_OF_NODE);
    std::iota(vdata.begin(), vdata.end(), 0);
    std::shuffle(vdata.begin(), vdata.end(), std::default_random_engine(std::rand()));
    for (char l = 0; l < 3; l++) {
        vThr.emplace_back([l, &vdata, &vTree, &vssTree] {
            for (size_t i = 0; i < NUM_OF_NODE / CHASTOTA; i++) {
                for (size_t j = i * CHASTOTA; j < (i + 1) * CHASTOTA; j++) vTree[l]->insert(vdata[j]);
                *vssTree[l] << vTree[l]->getHeight() << ", ";
            }
            });
    }
    for (std::thread& i : vThr)
        i.join();
    vThr.clear();
    std::cout << "===END===" << std::endl;
    std::cout << "Tree: " << ssTree.str() << std::endl;
    std::cout << "AVLTree: " << ssAVLTree.str() << std::endl;
    std::cout << "RBTree: " << ssRBTree.str() << std::endl;

    system("pause");

    return 0;
}
