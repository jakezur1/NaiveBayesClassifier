// Project UID db1f506d06d84ab787baf250c265e24e

#include "BinarySearchTree.h"
#include "unit_test_framework.h"
#include <cstdlib>
#include <unordered_set>
using namespace std;


TEST(empty) {
    BinarySearchTree<int> tree;

    // assert empty
    ASSERT_TRUE(tree.empty())

    // assert not empty
    tree.insert(0);
    ASSERT_FALSE(tree.empty())
}


TEST(size) {
    BinarySearchTree<int> tree;

    // populate random values in set to comply with no duplicates invariant
    unordered_set<int> inputs;
    for (int i = 0; i < 1000; i++) {
        inputs.insert(rand() % 1000 - 500);
    }
    
    // assert tree is sorted after each insertion
    int count = 0;
    for (int input : inputs) {
        ASSERT_EQUAL(tree.size(), count++);
        tree.insert(input);
        
    }
    // account for last value that is inserted
    ASSERT_EQUAL(tree.size(), count);
}

TEST(height) {
    // empty
    BinarySearchTree<int> empty;
    ASSERT_EQUAL(empty.height(), 0);

    // single node
    BinarySearchTree<int> single_node;
    single_node.insert(1);
    ASSERT_EQUAL(single_node.height(), 1);

    // normal tree
    BinarySearchTree<int> balanced;
    vector<int> inputs = {10, 5, 15, 3, 8, 13, 17};
    for (int input : inputs) {
        balanced.insert(input);
    }
    ASSERT_EQUAL(balanced.height(), 3);

    // only left children
    BinarySearchTree<int> skewed_right;
    inputs = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    for (int input : inputs) {
        skewed_right.insert(input);
    }
    ASSERT_EQUAL(skewed_right.height(), 10);

    // tree with non uniform branching
    BinarySearchTree<int> random_tree;
    inputs = {20, 10, 30, 5, 15, 25, 35, 3, 8, 12, 18, 23, 27, 33, 38};
    for (int input : inputs) {
        random_tree.insert(input);
    }
    ASSERT_EQUAL(random_tree.height(), 4);
}

TEST(copy) {
    // empty
    BinarySearchTree<int> empty;
    BinarySearchTree<int> copy_ctor_empty(empty);
    BinarySearchTree<int> assign_empty = empty;
    string empty_string = empty.to_string();
    string copy_ctor_empty_string = copy_ctor_empty.to_string();
    string assign_empty_string = assign_empty.to_string();
    ASSERT_EQUAL(empty_string, copy_ctor_empty_string);
    ASSERT_EQUAL(empty_string, assign_empty_string);

    // single node
    BinarySearchTree<int> single_node;
    single_node.insert(10);
    BinarySearchTree<int> copy_ctor_single(single_node);
    BinarySearchTree<int> assign_single = single_node;
    string single_node_string = single_node.to_string();
    string copy_ctor_single_string = copy_ctor_single.to_string();
    string assign_single_string = assign_single.to_string();
    ASSERT_EQUAL(single_node_string, copy_ctor_single_string);
    ASSERT_EQUAL(single_node_string, assign_single_string);

    BinarySearchTree<int> random;
    // obtain random set
    unordered_set<int> inputs;
    for (int i = 0; i < 100; i++) {
        inputs.insert(rand() % 100 - 50);
    }
    // insert random set
    for (int input : inputs) {
        random.insert(input);
    }

    BinarySearchTree<int> copy_ctor_random(random);
    BinarySearchTree<int> assign_random = random;
    string random_string = random.to_string();
    string copy_ctor_random_string = copy_ctor_random.to_string();
    string assign_random_string = assign_random.to_string();
    ASSERT_EQUAL(random_string, copy_ctor_random_string);
    ASSERT_EQUAL(random_string, assign_random_string);
}

TEST(find) {
    BinarySearchTree<int>::Iterator default_it;

    // empty tree
    BinarySearchTree<int> empty;
    ASSERT_EQUAL(empty.find(0), default_it);
    
    // single node
    BinarySearchTree<int> single_node;
    BinarySearchTree<int>::Iterator ten = single_node.insert(10);
    ASSERT_EQUAL(single_node.find(10), ten);
    ASSERT_EQUAL(single_node.find(9), default_it);

    // unbalanced tree
    BinarySearchTree<int> unbalanced;
    vector<int> inputs = {10, 7, 15, 5, 9, 12, 18, 1, 6, 8, 11, 14, 17, 19};
    vector<BinarySearchTree<int>::Iterator> nodes;
    for (int input : inputs) {
        BinarySearchTree<int>::Iterator curr_node = unbalanced.insert(input);
        nodes.push_back(curr_node);
    }
    int count = 0;
    for (int input : inputs) {
        ASSERT_EQUAL(unbalanced.find(input), nodes[count++]);
    }


}

TEST(insert) {
    BinarySearchTree<int> tree;

    // obtain random set
    unordered_set<int> inputs;
    for (int i = 0; i < 1000; i++) {
        inputs.insert(rand() % 1000 - 500);
    }
    
    // insert random set and assert sorting invariant is upheld
    for (int input : inputs) {
        tree.insert(input);
        ASSERT_TRUE(tree.check_sorting_invariant());
    }
}

TEST(min_element) {
    BinarySearchTree<int>::Iterator default_it;

    // empty tree test
    BinarySearchTree<int> empty;
    ASSERT_EQUAL(empty.min_element(), default_it);

    // single node test
    BinarySearchTree<int> single_node;
    single_node.insert(10);
    ASSERT_EQUAL(*single_node.min_element(), 10);

    // all elements less than the root node
    BinarySearchTree<int> min_root;
    vector<int> inputs = {1, 2, 5, 3, 4};
    for (int input : inputs) {
        min_root.insert(input);
    }
    ASSERT_EQUAL(*min_root.min_element(), 1);

    // greatest element is at a leaf
    BinarySearchTree<int> min_leaf;
    inputs = {15, 5, 25, 8, 2, 35, 17, -1};
    for (int input : inputs) {
        min_leaf.insert(input);
    }
    ASSERT_EQUAL(*min_leaf.min_element(), -1);

    // umbalanced tree
    BinarySearchTree<int> unbalanced;
    inputs = {10, 7, 15, 5, 9, 12, 18, 1, 6, 8, 11, 14, 17, 19};
    for (int input : inputs) {
        unbalanced.insert(input);
    }
    ASSERT_EQUAL(*unbalanced.min_element(), 1);
} 

TEST(max_element) {
    BinarySearchTree<int>::Iterator default_it;

    // empty tree test
    BinarySearchTree<int> empty;
    ASSERT_EQUAL(empty.max_element(), default_it);

    // single node
    BinarySearchTree<int> single_node;
    single_node.insert(10);
    ASSERT_EQUAL(*single_node.max_element(), 10);

    // all elements less than the root node
    BinarySearchTree<int> max_root;
    vector<int> inputs = {5, 2, 3, 4, 1};
    for (int input : inputs) {
        max_root.insert(input);
    }
    ASSERT_EQUAL(*max_root.max_element(), 5);

    // greatest element is at a leaf
    BinarySearchTree<int> max_leaf;
    inputs = {15, 5, 25, 8, 2, 35, 17};
    for (int input : inputs) {
        max_leaf.insert(input);
    }
    ASSERT_EQUAL(*max_leaf.max_element(), 35);

    // umbalanced tree
    BinarySearchTree<int> unbalanced;
    inputs = {10, 7, 15, 5, 9, 12, 18, 1, 6, 8, 11, 14, 17, 19};
    for (int input : inputs) {
        unbalanced.insert(input);
    }
    ASSERT_EQUAL(*unbalanced.max_element(), 19);
}

TEST(check_sorting_invariant) {
    // empty tree
    BinarySearchTree<int> empty;
    ASSERT_TRUE(empty.check_sorting_invariant());

    // single node
    BinarySearchTree<int> single_node;
    single_node.insert(10);
    ASSERT_TRUE(single_node.check_sorting_invariant());

    // normal tree
    BinarySearchTree<int> random;
    vector<int> inputs = {10, 5, 15, 3, 7, 13, 17}; 
    vector<BinarySearchTree<int>::Iterator> nodes;
    for (int input : inputs) {
        BinarySearchTree<int>::Iterator curr_node = random.insert(input);
        nodes.push_back(curr_node);
        ASSERT_TRUE(random.check_sorting_invariant());
    }

    // invalid test: change right child to be less than parent
    BinarySearchTree<int>::Iterator fifteen = nodes[2];
    *fifteen = 9;
    ASSERT_FALSE(random.check_sorting_invariant());
    *fifteen = 15;
    
    // invalid test: change left child to be greater than parent
    BinarySearchTree<int>::Iterator five = nodes[1];
    *five = 11;
    ASSERT_FALSE(random.check_sorting_invariant());
    *five = 5;

    // invalid test: left subtree contains a value less than the root 
    //               but greater than its direct parent
    BinarySearchTree<int>::Iterator thirteen = nodes[5];
    *thirteen = 9;
    ASSERT_FALSE(random.check_sorting_invariant());
    *thirteen = 13;

    // invalid test: left subtree contains a value less than the root 
    //               but greater than its direct parent
    BinarySearchTree<int>::Iterator seven = nodes[4];
    *seven = 11;
    ASSERT_FALSE(random.check_sorting_invariant());
    *seven = 7;
}

TEST(min_greater_than) {
    BinarySearchTree<int>::Iterator default_it;

    // empty tree 
    BinarySearchTree<int> empty;
    ASSERT_EQUAL(empty.min_greater_than(5), default_it);

    // single node 
    BinarySearchTree<int> single_node;
    single_node.insert(6);
    ASSERT_EQUAL(*single_node.min_greater_than(5), 6);
    ASSERT_EQUAL(single_node.min_greater_than(6), default_it);

    // all elements smaller than value
    BinarySearchTree<int> smaller_elts;
    vector<int> inputs = {5, 3, 7, 1, 4, 6, 8};
    for (int input : inputs) {
        smaller_elts.insert(input);
    }
    ASSERT_EQUAL(smaller_elts.min_greater_than(10), default_it);
}  

TEST(min_greater_than_2) {
    BinarySearchTree<int>::Iterator default_it;

    // all elements greater than value
    BinarySearchTree<int> greater_elts;
    vector<int> inputs = {11, 9, 13, 7, 10, 12, 16}; // {10, 8, 12, 6, 9, 11, 15}
    for (int input : inputs) {
        greater_elts.insert(input);
    }
    ASSERT_EQUAL(*greater_elts.min_greater_than(5), 7);
    ASSERT_EQUAL(*greater_elts.min_greater_than(6), 7);
    ASSERT_EQUAL(*greater_elts.min_greater_than(8), 9);
    ASSERT_EQUAL(*greater_elts.min_greater_than(10), 11);
    ASSERT_EQUAL(*greater_elts.min_greater_than(12), 13);
    ASSERT_EQUAL(*greater_elts.min_greater_than(13), 16);
    ASSERT_EQUAL(greater_elts.min_greater_than(16), default_it);

    // mixed elements (contains both greater and less than value)
    BinarySearchTree<int> mixed_elts;
    // added one
    inputs = {11, 6, 16, 4, 8, 14, 18, 2, 5, 7, 9, 12, 15, 17, 19};
    for (int input : inputs) {
        mixed_elts.insert(input);
    }
    ASSERT_EQUAL(*mixed_elts.min_greater_than(-1), 2);
    ASSERT_EQUAL(*mixed_elts.min_greater_than(16), 17);
    ASSERT_EQUAL(*mixed_elts.min_greater_than(3), 4);
    ASSERT_EQUAL(*mixed_elts.min_greater_than(9), 11);
    ASSERT_EQUAL(*mixed_elts.min_greater_than(2), 4);
    ASSERT_EQUAL(*mixed_elts.min_greater_than(11), 12);
}

TEST_MAIN()
