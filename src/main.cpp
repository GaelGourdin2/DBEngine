#include "BTree.h"
#include "BTreePerf.h"
#include <string>
#include <variant>


int main() {
    PerformanceTest test;
    test.run_all_tests();

    // BTree tree(2);

    // for (int i = 0; i < 20; i++)
    // {
    //     tree.insert(i);
    //     tree.print();
    // }

    // BTreeNode *node = tree.search(7);
    // node->print();
    return 0;
}