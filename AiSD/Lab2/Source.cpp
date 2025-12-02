#include <iostream>
#include <unordered_set>
#include <random>
#include <ctime>
#include <thread>
#include <algorithm>
#include <numeric>

#include "AVLTree.h"
#include "RBTree.h"


#define NUM_OF_NODE 50000
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





    std::cout << "from 1 000 to 50 000:\n" << std::endl;
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


    //std::vector<size_t> V = {3787, 41, 24, 3467, 1334, 1500, 4169, 724, 1478, 4358, 3986};
    //for (const size_t& i : V) {
    //    avlTree.insert(i);
    //    std::cout << i << " -> " << avlTree.getHeight() << std::endl;
    //    std::cout << avlTree.levelOrder() << std::endl << std::endl;
    //}


    //for (size_t i = 0; i !=-1 ; i++) {
    //    srand(i);
    //    avlTree.clear();
    //    std::unordered_set<size_t> data;
    //    data.reserve(5000);
    //    while (data.size() < 5000) {
    //        data.insert(std::rand() % 5000);
    //    }
    //    for (const size_t& j : data) {
    //        avlTree.insert(j);
    //        ssAVLTree << avlTree.getHeight() << ' ';
    //    }
    //    std::vector<size_t> vdata = { data.begin(), data.end() };
    //    ssAVLTree.str("");
    //    while (!vdata.empty()) {
    //        const size_t& x = std::rand() % vdata.size();
    //        avlTree.del(vdata[x]);
    //        vdata.erase(vdata.begin() + x);
    //    }
    //}

    return 0;
}
