



#ifndef ALGORITHM_AVLTREE_H
#define ALGORITHM_AVLTREE_H

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _AVLTree AVLTree;

#ifdef TEST_ALTERNATE_VALUE_TYPES
#include "alt-value-type.h"
#else


typedef void *AVLTreeKey;


typedef void *AVLTreeValue;


#define AVL_TREE_NULL ((void *) 0)

#endif 


typedef struct _AVLTreeNode AVLTreeNode;


typedef enum {
	AVL_TREE_NODE_LEFT = 0,
	AVL_TREE_NODE_RIGHT = 1
} AVLTreeNodeSide;


typedef int (*AVLTreeCompareFunc)(AVLTreeKey value1, AVLTreeKey value2);


AVLTree *avl_tree_new(AVLTreeCompareFunc compare_func);


void avl_tree_free(AVLTree *tree);


AVLTreeNode *avl_tree_insert(AVLTree *tree, AVLTreeKey key, AVLTreeValue value);


void avl_tree_remove_node(AVLTree *tree, AVLTreeNode *node);


int avl_tree_remove(AVLTree *tree, AVLTreeKey key);


AVLTreeNode *avl_tree_lookup_node(AVLTree *tree, AVLTreeKey key);


AVLTreeValue avl_tree_lookup(AVLTree *tree, AVLTreeKey key);


AVLTreeNode *avl_tree_root_node(AVLTree *tree);


AVLTreeKey avl_tree_node_key(AVLTreeNode *node);


AVLTreeValue avl_tree_node_value(AVLTreeNode *node);


AVLTreeNode *avl_tree_node_child(AVLTreeNode *node, AVLTreeNodeSide side);


AVLTreeNode *avl_tree_node_parent(AVLTreeNode *node);


int avl_tree_subtree_height(AVLTreeNode *node);


AVLTreeKey *avl_tree_to_array(AVLTree *tree);


unsigned int avl_tree_num_entries(AVLTree *tree);

#ifdef __cplusplus
}
#endif

#endif 
