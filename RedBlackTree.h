#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

#define SUCCESS 0
#define FAILURE 1
#define DUPLICATE 2
/**Colors of the node. INVALID initially**/

typedef enum {RED, BLACK, INVALID} color;

typedef struct TreeNode {
    int value;
    struct TreeNode * parent;
    struct TreeNode * lchild;
    struct TreeNode * rchild;
    color nodeColor; /*Takes one value from enum color, RED or BLACK*/
    int nodes_in_sub; /*Including itself*/
    int red_nodes_in_sub;
} RBT;


extern RBT * rootNode;
/**
** Creates and returns the node.
**/

int createRBTNode(RBT **makeNode, int value);

/**
** Searches for the key in tree.
** If a node with the same value is found, returns the node
** Else returns the last node visited
**/
RBT * searchNodeByKey(RBT * node, int key);

/**
** Inserts the new Node at the appropriate position
** in Red Black Tree. Nodes added are of colour Red
** It is done similar to BST. However, if parent node is also red,
** Two cases arises.
** Returns SUCCESS on success and FAILURE on failure of operation 
**/
int insertNode(RBT * node, RBT * newNode);

void insertCase1 (RBT * grandParent, RBT * parentNode, RBT * keyNode);

void insertCase2 (RBT * grandParent, RBT * uncle, RBT * parentNode, RBT * keyNode);

/**
** Takes as input, the node to be deleted.
** Deletes the node
** Has three cases. The case with black leaf node deletion
** has six sub-cases.
** Returns SUCCESS on successful deletion
** FAILURE otherwise
**/
int deleteNode(RBT * node);

void deletionCases(RBT * parentNode, RBT * sibling);

void deletionCase1_1(RBT * parentNode, RBT * sibling);

void deletionCase1_2(RBT * parentNode, RBT * sibling);

void deletionCase2_1_1(RBT * parentNode, RBT * sibling);

void deletionCase2_1_2(RBT * parentNode, RBT * sibling);

void deletionCase2_2_1(RBT * parentNode, RBT * sibling);

void deletionCase2_2_2(RBT * parentNode, RBT * sibling);

/**
** Returns the height of the given node.
** Leaf node is considered to be of height 0
**/
int heightOfNode(RBT * node);


/**
** Returns the black height of tree.
**/
int blackHeight(RBT * node);

/**
** Given a node, returns the predecessor, if present.
** Returns NULL otherwise.
**/
RBT * predecessorNode(RBT * node);

/**
** Given a node, returns the successor, if present.
** Returns NULL otherwise.
**/
RBT * successorNode(RBT * node);

/**
** Tree Rotations
**/

void rightRotation (RBT * root, RBT * pivot);

void leftRotation (RBT * root, RBT * pivot);

/**
** Rotation Cases
**/
void leftLeftCase(RBT * root, RBT * pivot);

void rightRightCase(RBT * root, RBT * pivot);

void leftRightCase(RBT * root, RBT * pivot);

void rightLeftCase(RBT * root, RBT * pivot);

/**
** Given the value, if present in tree,
** returns the rank.
**/
int rank(RBT * node, int key, int discarded);

int redRank(RBT * node, int key, int discarded);
/**
** Given the rank, find the element 
** corresponding to the rank.
**/
int findRank(RBT * node, int rnk, int discarded);


/**
** Returns the maximum value of the given two values
**/
int max(int value1, int value2);

void updateNodeCount(RBT * node, int updateValue);

void updateRedNodeCount(RBT * node, int updateValue);
/**
** Prints the tree as we traverse the tree.
** 1. Inorder
** 2. Preorder
** 3. Postorder
**/
void inorderTraversal(RBT * node);

void preorderTraversal(RBT * node);

void postorderTraversal(RBT * node);
