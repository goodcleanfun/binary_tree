#ifndef BST_H
#define BST_H

#include <stdbool.h>
#include <stddef.h>

#endif

#ifndef BST_NAME
#error "BST_NAME must be defined"
#endif

#ifndef BST_KEY_TYPE
#error "BST_KEY_TYPE must be defined"
#endif

#ifndef BST_MAX_HEIGHT
#define BST_MAX_HEIGHT 64
#endif

#define BST_CONCAT_(a, b) a ## b
#define BST_CONCAT(a, b) BST_CONCAT_(a, b)
#define BST_TYPED(name) BST_CONCAT(BST_NAME, _##name)
#define BST_FUNC(func) BST_CONCAT(BST_NAME, _##func)

#ifndef BST_NODE_TYPE
typedef struct BST_TYPED(node) {
    BST_KEY_TYPE key;
    struct BST_TYPED(node) *left;
    struct BST_TYPED(node) *right;
#ifdef BST_NODE_EXTRA
    BST_NODE_EXTRA
#endif
} BST_TYPED(node_t);

#define BST_NODE_TYPE BST_TYPED(node_t)
#endif

typedef struct BST_TYPED(stack) {
    BST_TYPED(node_t) *stack[BST_MAX_HEIGHT];
    size_t stack_size;
} BST_TYPED(stack_t);

bool BST_FUNC(stack_push)(BST_TYPED(stack_t) *stack, BST_TYPED(node_t) *node) {
    if (stack->stack_size >= BST_MAX_HEIGHT) return false;
    stack->stack[stack->stack_size++] = node;
    return true;
}

BST_TYPED(node_t) *BST_FUNC(stack_pop)(BST_TYPED(stack_t) *stack) {
    if (stack->stack_size == 0) return NULL;
    return stack->stack[--stack->stack_size];
}

bool BST_FUNC(stack_empty)(BST_TYPED(stack_t) *stack) {
    return stack->stack_size == 0;
}

void BST_FUNC(stack_clear)(BST_TYPED(stack_t) *stack) {
    stack->stack_size = 0;
}


#ifndef BST_KEY_LESS_THAN
static inline bool BST_TYPED(key_less_than)(BST_KEY_TYPE key, BST_KEY_TYPE node_key) {
    return key < node_key;
}
#define BST_KEY_LESS_THAN BST_TYPED(key_less_than)
#endif

#ifndef BST_KEY_EQUALS
static inline bool BST_TYPED(key_equal)(BST_KEY_TYPE key, BST_KEY_TYPE node_key) {
    return key == node_key;
}
#define BST_KEY_EQUALS BST_TYPED(key_equal)
#endif

static inline bool BST_FUNC(node_is_leaf)(BST_NODE_TYPE *node) {
    return node->right == NULL;
}

static inline bool BST_FUNC(node_is_internal)(BST_NODE_TYPE *node) {
    return node->right != NULL;
}

static inline BST_TYPED(node_t) *BST_FUNC(candidate_leaf)(BST_TYPED(node_t) *node, BST_KEY_TYPE key, BST_TYPED(stack_t) *stack) {
    BST_TYPED(node_t) *tmp_node = node;
    while (BST_FUNC(node_is_internal)(tmp_node)) {
        if (!BST_FUNC(stack_push(stack, tmp_node))) return NULL;
        if (BST_KEY_LESS_THAN(key, tmp_node->key)) {
            tmp_node = tmp_node->left;
        } else {
            tmp_node = tmp_node->right;
        }
    }
    return tmp_node;
}


void BST_FUNC(rotate_left)(BST_NODE_TYPE *node) {
    /*
    Left rotation (rotate a to the left)

          b                   c
        /   \               /   \
    [a,b)    c      =>     b   [c,d)
           /   \         /   \
        [b,c) [c,d)   [a,b)  [b,c)
    */

    BST_NODE_TYPE *tmp_node = node->left;
    BST_KEY_TYPE tmp_key = node->key;
    node->left = node->right;
    node->key = node->right->key;

    node->right = node->left->right;
    node->left->right = node->left->left;

    node->left->left = tmp_node;
    node->left->key = tmp_key;

}

void BST_FUNC(rotate_right)(BST_NODE_TYPE *node) {
    /*
    Left rotation (rotate a to the left)

            c                 b
          /   \             /   \ 
         b   [c,d)  =>  [a,b)    c
       /   \                   /   \
    [a,b)  [b,c)            [b,c)  [c,d)
    */

    BST_NODE_TYPE *tmp_node = node->right;
    BST_KEY_TYPE tmp_key = node->key;

    node->right = node->left;
    node->key = node->left->key;

    node->left = node->right->left;
    node->right->left = node->right->right;

    node->right->right = tmp_node;
    node->right->key = tmp_key;
}

#ifdef BST_VALUE_TYPE
BST_VALUE_TYPE *BST_FUNC(search)(BST_NODE_TYPE *node, BST_KEY_TYPE key) {
    BST_NODE_TYPE *tmp_node = node;
    BST_KEY_TYPE tmp_key = node->key;

    if (node->left == NULL) return NULL;
    while (BST_FUNC(node_is_internal)(tmp_node)) {
        if (BST_KEY_LESS_THAN(key, tmp_node->key)) {
            tmp_node = tmp_node->left;
        } else {
            tmp_node = tmp_node->right;
        }
    }

    if (BST_KEY_EQUALS(key, tmp_node->key)) {
        // value pointer is stored on the left key of a leaf node
        return (BST_VALUE_TYPE *)tmp_node->left;
    } else {
        return NULL;
    }
}
#endif


#undef BST_CONCAT_
#undef BST_CONCAT
#undef BST_TYPED
#undef BST_FUNC
