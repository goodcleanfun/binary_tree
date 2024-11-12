#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "greatest/greatest.h"

#define BST_NAME binary_tree
#define BST_KEY_TYPE uint32_t
#define BST_VALUE_TYPE char *
#define BST_NODE_EXTRA \
    uint8_t height;
#include "binary_tree.h"
#undef BST_NAME
#undef BST_KEY_TYPE
#undef BST_VALUE_TYPE

TEST test_bst(void) {

    /*
    Make the following binary tree:

            4
          /   \
        2       6
       / \     / \
      1   3   5   7
     (a) (b) (c) (d)
    */

    binary_tree_node_t nodes[7];

    for (size_t i = 0; i < 7 ; i++) {
        nodes[i].key = i + 1;
        nodes[i].left = NULL;
        nodes[i].right = NULL;
    }

    binary_tree_node_t *node1 = &nodes[0];
    binary_tree_node_t *node2 = &nodes[1];
    binary_tree_node_t *node3 = &nodes[2];
    binary_tree_node_t *node4 = &nodes[3];
    binary_tree_node_t *node5 = &nodes[4];
    binary_tree_node_t *node6 = &nodes[5];
    binary_tree_node_t *node7 = &nodes[6];

    binary_tree_node_t *root = node4;
    root->left = node2;
    root->right = node6;

    node2->left = node1;
    node2->right = node3;

    node6->left = node5;
    node6->right = node7;

    char *a_value = "a";
    char *b_value = "b";
    char *c_value = "c";
    char *d_value = "d";

    node1->left = (binary_tree_node_t *)&a_value;
    node3->left = (binary_tree_node_t *)&b_value;
    node5->left = (binary_tree_node_t *)&c_value;
    node7->left = (binary_tree_node_t *)&d_value;

    char **a, **b, **c, **d;

    a = binary_tree_search(root, 1);
    ASSERT_STR_EQ(*a, "a");
    b = binary_tree_search(root, 3);
    ASSERT_STR_EQ(*b, "b");
    c = binary_tree_search(root, 5);
    ASSERT_STR_EQ(*c, "c");
    d = binary_tree_search(root, 7);
    ASSERT_STR_EQ(*d, "d");

    binary_tree_rotate_left(root);
    /*
    Now tree should be in this state:
    
              6
            /   \
           4     7
          / \   (d)
         2   5
        / \ (c)
       1   3
      (a) (b)
    */

    ASSERT_EQ(root->key, 6);
    ASSERT_EQ(root->left->key, 4);
    ASSERT_EQ(root->right->key, 7);
    ASSERT_EQ(root->left->left->key, 2);
    ASSERT_EQ(root->left->right->key, 5);
    ASSERT_EQ(root->left->left->left->key, 1);
    ASSERT_EQ(root->left->left->right->key, 3);

    a = binary_tree_search(root, 1);
    ASSERT_STR_EQ(*a, "a");
    b = binary_tree_search(root, 3);
    ASSERT_STR_EQ(*b, "b");
    c = binary_tree_search(root, 5);
    ASSERT_STR_EQ(*c, "c");
    d = binary_tree_search(root, 7);
    ASSERT_STR_EQ(*d, "d");


    binary_tree_rotate_right(root);
    // Tree should be in the original state

    ASSERT_EQ(root->key, 4);
    ASSERT_EQ(root->left->key, 2);
    ASSERT_EQ(root->right->key, 6);
    ASSERT_EQ(root->left->left->key, 1);
    ASSERT_EQ(root->left->right->key, 3);
    ASSERT_EQ(root->right->left->key, 5);
    ASSERT_EQ(root->right->right->key, 7);

    a = binary_tree_search(root, 1);
    ASSERT_STR_EQ(*a, "a");
    b = binary_tree_search(root, 3);
    ASSERT_STR_EQ(*b, "b");
    c = binary_tree_search(root, 5);
    ASSERT_STR_EQ(*c, "c");
    d = binary_tree_search(root, 7);
    ASSERT_STR_EQ(*d, "d");

    binary_tree_rotate_right(root);
    /*
    Now tree should be in this state:
    
              2
            /   \
           1     4
          (a)   / \
               3   6
              (b) / \
                 5   7
                (c) (d)
    */

    ASSERT_EQ(root->key, 2);
    ASSERT_EQ(root->left->key, 1);
    ASSERT_EQ(root->right->key, 4);
    ASSERT_EQ(root->right->left->key, 3);
    ASSERT_EQ(root->right->right->key, 6);
    ASSERT_EQ(root->right->right->left->key, 5);
    ASSERT_EQ(root->right->right->right->key, 7);

    a = binary_tree_search(root, 1);
    ASSERT_STR_EQ(*a, "a");
    b = binary_tree_search(root, 3);
    ASSERT_STR_EQ(*b, "b");
    c = binary_tree_search(root, 5);
    ASSERT_STR_EQ(*c, "c");
    d = binary_tree_search(root, 7);
    ASSERT_STR_EQ(*d, "d");

    PASS();
}



/* Add definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line options, initialization. */

    RUN_TEST(test_bst);

    GREATEST_MAIN_END();        /* display results */
}