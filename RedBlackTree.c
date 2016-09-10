#include "RedBlackTree.h"

RBT * rootNode;

int createRBTNode(RBT **makeNode, int value) {
    
    RBT * temp = (RBT *)calloc(sizeof(RBT), 1);
    
    if (temp == NULL) {
        fprintf(stderr, "Error in creating node");
        return FAILURE;
    }
    
    temp->value = value;
    temp->parent = NULL;
    temp->lchild = NULL;
    temp->rchild = NULL;
    
    temp->nodeColor = INVALID;
    temp->nodes_in_sub = 1;
    temp->red_nodes_in_sub = 1;
    (*makeNode) = temp;
    
    return SUCCESS;
}

RBT * searchNodeByKey(RBT * node, int key) {
    
    if (node == NULL){
        
        return NULL;
    }    
    if (node->value == key)
        return node;
    
    else if(node->value < key && node->rchild != NULL)
        return searchNodeByKey(node->rchild, key);
    
    else if (node->value > key && node->lchild != NULL)
        return searchNodeByKey(node->lchild, key);
    
    else
        return node;
    
    return NULL;
}

int max(int value1, int value2) {
    
    return value1 > value2 ? value1 : value2;
}

int heightOfNode(RBT * node) {
    
    if (node == NULL)
        return 0;
    
    if (node->lchild == NULL && node->rchild == NULL)
        return 0;
    
    return 1 + max(heightOfNode(node->lchild), heightOfNode(node->rchild));
}

int blackHeight(RBT * node) {
    
    int height = 0;
    
    if (node == NULL)
        return height;
    
    if (node->nodeColor == BLACK) {
        
        height = 1;
    }
    
    else{
        
        height = 0;
    }
    
    if (node->lchild != NULL)
        return height + blackHeight(node->lchild);
    
    else if (node->rchild != NULL)
        return height + blackHeight(node->rchild);
    
    else
        return height;
    
    return height;
}

RBT * predecessorNode(RBT * node) {
    
    if(node->lchild != NULL) {
        node = node->lchild;
        
        while ( node->rchild != NULL) {
            
            node = node->rchild;
        }
        
        return node;            
    }
    
    else if ( node->parent != NULL) {
        if (node->parent->rchild == node){
            node = node->parent;
        
        }
        else {
            
            while (node->parent->lchild == node) {
                
                node = node->parent;
            }
            
            node = node->parent;
        }
        
        return node;
    }
    
    return NULL;
}

RBT * successorNode(RBT * node) {
    
    if (node->rchild != NULL) {
        
        node = node->rchild;
    
        while (node->lchild != NULL) {
        
            node = node->lchild;
        }
        
        return node;
    }
    
    else if ( node->parent != NULL) {
        if (node->parent->lchild == node) {
        
            node = node->parent;
        }
        
        else {
            
            while (node->parent->rchild == node) {
                
                node = node->parent;
            }
            
            node = node->parent;
        }
        
        return node;
    }
    
    return NULL;
}

void updateNodeCount(RBT * node, int updateValue) {
    
    node->nodes_in_sub = node->nodes_in_sub + updateValue;
    
    if (node->parent != NULL)
        updateNodeCount(node->parent, updateValue);
}

void updateRedNodeCount(RBT * node, int updateValue) {

    node->red_nodes_in_sub += updateValue;
    
    if (node->parent != NULL)
        updateRedNodeCount(node->parent, updateValue);
}

int redRank(RBT * node, int key,int discarded) {
    
    if ( node->value == key) {
        
        if ( node->rchild != NULL)
            discarded = discarded + node->rchild->red_nodes_in_sub;
        
        return discarded + 1;
    }
    
    else if (node->value > key) {
        
        if (node->rchild != NULL)
            discarded = discarded + node->rchild->red_nodes_in_sub;
        
        if (node->nodeColor == RED)
            discarded += 1;
        
        if (node->lchild != NULL)
            return redRank(node->lchild, key, discarded);
        
        else
            return discarded + 1;
    }
    
    else if(node->value < key) {
        
        if (node->rchild != NULL)
            return redRank(node->rchild, key, discarded);
        
        else
            return discarded + 1;
    }
    
    else{
    }
    
    return -1;
}

int rank(RBT * node, int key, int discarded) {
    
    if ( node->value == key) {
        
        if ( node->rchild != NULL)
            discarded = discarded + node->rchild->nodes_in_sub;
        
        return discarded + 1;
    }
    
    else if (node->value > key) {
        
        if (node->rchild != NULL)
            discarded = discarded + node->rchild->nodes_in_sub;
        
        discarded += 1;
        
        if (node->lchild != NULL)
            return rank(node->lchild, key, discarded);
        
        else
            return discarded + 1;
    }
    
    else if(node->value < key) {
        
        if (node->rchild != NULL)
            return rank(node->rchild, key, discarded);
        
        else
            return discarded + 1;
    }
    
    else{
    }
    return -1;
}

int findRank(RBT * node, int rnk, int discarded) {
    
    if (rnk > node->nodes_in_sub)
        return -1;
    if(node->rchild != NULL && rnk == (node->rchild->nodes_in_sub + 1))
        return node->value;
    
    else if ( node->rchild != NULL && rnk <= node->rchild->nodes_in_sub)
        return findRank(node->rchild, rnk, discarded);
    
    else if ( node->rchild != NULL && rnk > (node->rchild->nodes_in_sub + 1)) {
        discarded = node->rchild->nodes_in_sub + 1;
        rnk = rnk - discarded;
        
        if (node->lchild != NULL)
            return findRank(node->lchild, rnk, discarded);
        
        return -1;
    }
    
    else if (node->rchild == NULL) {
            
            if (rnk == (node->nodes_in_sub - (node->lchild != NULL ? node->lchild->nodes_in_sub : 0)))
                return node->value;
            
            discarded = 1;
            rnk = rnk - discarded;
            if (node->lchild != NULL)
                return findRank(node->lchild, rnk, discarded);
    }
        
    return -1;
}

void rightRotation (RBT * root, RBT * pivot) {
    
    RBT * subB = pivot->rchild;
    
    pivot->parent = root->parent;
    if(pivot->parent != NULL) {
        
        if (root->parent->lchild == root)
            root->parent->lchild = pivot;
        
        else
            root->parent->rchild = pivot;
    }
    
    root->parent = pivot;
    pivot->rchild = root;
    
    root->lchild = subB;
    if(root->lchild != NULL)
        root->lchild->parent = root;
}

void leftRotation(RBT * root, RBT * pivot) {
    
    RBT * rootParent = root->parent;
    RBT * subB = pivot->lchild;
    
    pivot->parent = rootParent;
    if (pivot->parent != NULL) {
        
        if (rootParent->lchild == root)
            rootParent->lchild = pivot;
        
        else
            rootParent->rchild = pivot;
    }
    
    root->parent = pivot;
    pivot->lchild = root;
    
    root->rchild = subB;
    if (root->rchild != NULL)
        root->rchild->parent = root;
}

void leftLeftCase(RBT * root, RBT * pivot) {
    
    rightRotation (root, pivot);
}

void rightRightCase(RBT * root, RBT * pivot) {
    
    leftRotation(root, pivot);
}

void leftRightCase(RBT * root, RBT * pivot) {
    
    leftRotation(pivot, pivot->rchild);
    rightRotation(root, root->lchild);
}

void rightLeftCase(RBT * root, RBT * pivot) {
    
    rightRotation(pivot, pivot->lchild);
    leftRotation(root, root->rchild);
}

int insertNode(RBT * node, RBT * newNode) {
    
    RBT * grandParent;
    RBT * uncle;
    RBT * parentNode = NULL;
    if (node != NULL){
        parentNode = searchNodeByKey(node, newNode->value);
    }
    if (parentNode == NULL) {
        rootNode = newNode;
        newNode->nodeColor = BLACK;
        updateRedNodeCount(newNode, -1);
        return SUCCESS;
    }
    
    if (parentNode->value == newNode->value) {
        
        /*Do nothing*/
        return DUPLICATE;
    }
    
    if (parentNode->value < newNode->value)
        parentNode->rchild = newNode;
    else
        parentNode->lchild = newNode;
    
    newNode->parent = parentNode;
    newNode->nodeColor = RED;
    
    updateNodeCount(parentNode, 1);
    updateRedNodeCount(parentNode, 1);
    
    if (parentNode->nodeColor == RED) {
        grandParent = parentNode->parent;
        
        if (grandParent != NULL && grandParent->lchild == parentNode)
            uncle = grandParent->rchild;
        else
            uncle = grandParent != NULL ? grandParent->lchild : NULL;
        
        if (uncle == NULL || uncle->nodeColor == BLACK) {
            
            /*Case 1*/
            insertCase1 (grandParent, parentNode, newNode);
        }
        
        else {
            
            /*Case 2*/
            insertCase2 (grandParent, uncle, parentNode, newNode);
        }
    }
    
    return SUCCESS;
}

void insertCase1 (RBT * grandParent, RBT * parentNode, RBT * keyNode) {
    
    if (grandParent->rchild == parentNode) {
        
        if (parentNode->rchild == keyNode) {
            
            grandParent->nodeColor = RED;
            parentNode->nodeColor = BLACK;
            
            updateRedNodeCount(grandParent, 1);
            updateRedNodeCount(parentNode, -1);
            
            grandParent->nodes_in_sub -= parentNode->nodes_in_sub;
            grandParent->red_nodes_in_sub -= parentNode->red_nodes_in_sub;
            
            if (parentNode->lchild != NULL) {
                
                parentNode->nodes_in_sub -= parentNode->lchild->nodes_in_sub;
                grandParent->nodes_in_sub += parentNode->lchild->nodes_in_sub;
                
                parentNode->red_nodes_in_sub -= parentNode->lchild->red_nodes_in_sub;
                grandParent->red_nodes_in_sub += parentNode->lchild->red_nodes_in_sub;
                
            }
            parentNode->nodes_in_sub += grandParent->nodes_in_sub;
            
            parentNode->red_nodes_in_sub += grandParent->red_nodes_in_sub;
            
            if (grandParent == rootNode)
                rootNode = parentNode;
            
            rightRightCase (grandParent, parentNode);
        }
        
        else {
            
            grandParent->nodeColor = RED;
            keyNode->nodeColor = BLACK;
            
            updateRedNodeCount(grandParent, 1);
            updateRedNodeCount(keyNode, -1);
            
            grandParent->nodes_in_sub = grandParent->nodes_in_sub - parentNode->nodes_in_sub + (keyNode->lchild != NULL ? keyNode->lchild->nodes_in_sub: 0);
            parentNode->nodes_in_sub += ((keyNode->rchild != NULL ? keyNode->rchild->nodes_in_sub : 0) - keyNode->nodes_in_sub);
            keyNode->nodes_in_sub = grandParent->nodes_in_sub + parentNode->nodes_in_sub + 1;
            
            grandParent->red_nodes_in_sub = grandParent->red_nodes_in_sub - parentNode->red_nodes_in_sub + (keyNode->lchild != NULL ? keyNode->lchild->red_nodes_in_sub: 0);
            parentNode->red_nodes_in_sub += ((keyNode->rchild != NULL ? keyNode->rchild->red_nodes_in_sub : 0) - keyNode->red_nodes_in_sub);
            keyNode->red_nodes_in_sub = grandParent->red_nodes_in_sub + parentNode->red_nodes_in_sub + 1;
            
            
            if (grandParent == rootNode)
                rootNode = keyNode;
            
            rightLeftCase(grandParent, parentNode);
        }
    }
    
    else {
        
        if (parentNode->lchild == keyNode) {
            
            grandParent->nodeColor = RED;
            parentNode->nodeColor = BLACK;
            
            updateRedNodeCount(parentNode, -1);
            updateRedNodeCount(grandParent, 1);
            
            grandParent->nodes_in_sub += ((parentNode->rchild != NULL ? parentNode->rchild->nodes_in_sub : 0) - parentNode->nodes_in_sub);
            parentNode->nodes_in_sub += (grandParent->nodes_in_sub - (parentNode->rchild != NULL ? parentNode->rchild->nodes_in_sub : 0));
            
            grandParent->red_nodes_in_sub += ((parentNode->rchild != NULL ? parentNode->rchild->red_nodes_in_sub : 0) - parentNode->red_nodes_in_sub);
            parentNode->red_nodes_in_sub += (grandParent->red_nodes_in_sub - (parentNode->rchild != NULL ? parentNode->rchild->red_nodes_in_sub : 0));
            
            if (grandParent == rootNode)
                rootNode = parentNode;
            
            leftLeftCase(grandParent, parentNode);
        }
        
        else {
            
            grandParent->nodeColor = RED;
            keyNode->nodeColor = BLACK;
            
            updateRedNodeCount(keyNode, -1);
            updateRedNodeCount(grandParent, 1);
            
            grandParent->nodes_in_sub += ((keyNode->rchild != NULL ? keyNode->rchild->nodes_in_sub : 0) - parentNode->nodes_in_sub);
            parentNode->nodes_in_sub += ((keyNode->lchild != NULL ? keyNode->lchild->nodes_in_sub : 0) - keyNode->nodes_in_sub);
            keyNode->nodes_in_sub = parentNode->nodes_in_sub + grandParent->nodes_in_sub + 1;
            
            grandParent->red_nodes_in_sub += ((keyNode->rchild != NULL ? keyNode->rchild->red_nodes_in_sub : 0) - parentNode->red_nodes_in_sub);
            parentNode->red_nodes_in_sub += ((keyNode->lchild != NULL ? keyNode->lchild->red_nodes_in_sub : 0) - keyNode->red_nodes_in_sub);
            keyNode->red_nodes_in_sub = parentNode->red_nodes_in_sub + grandParent->red_nodes_in_sub + 1;
            
            
            if (grandParent == rootNode)
                rootNode = keyNode;
                
            leftRightCase(grandParent, parentNode);
        }
    }
}

void insertCase2 (RBT * grandParent, RBT * uncle, RBT * parentNode, RBT * keyNode) {
    
    grandParent->nodeColor = RED;
    uncle->nodeColor = BLACK;
    parentNode->nodeColor = BLACK;
    
    updateRedNodeCount(parentNode, -1);
    updateRedNodeCount(uncle, -1);
    updateRedNodeCount(grandParent, 1);
    
    if (grandParent->parent == NULL) {
        rootNode = grandParent;
        grandParent->nodeColor = BLACK;
        updateRedNodeCount(grandParent, -1);
        return;
    }
    
    if (grandParent->parent->nodeColor == BLACK) {
        return;
    }
    
    keyNode = grandParent;
    parentNode = grandParent->parent;
    grandParent = parentNode->parent;
    
    if (grandParent->lchild == parentNode)
        uncle = grandParent->rchild;
    else
        uncle = grandParent->lchild;
    
    if (uncle == NULL || uncle->nodeColor == BLACK) {
            
        /*Case 1*/
        insertCase1 (grandParent, parentNode, keyNode);
    }
    
    else {
        
        /*Case 2*/
        insertCase2 (grandParent, uncle, parentNode, keyNode);
    }
}

int deleteNode(RBT * node) {
    
    RBT * successor;
    RBT * nextNode;
    RBT * sibling;
    RBT * parentNode;
    int temp;
    
    if(node == NULL) {
        return FAILURE;
        }
    if (node->lchild != NULL && node->rchild != NULL) {
        
        successor = successorNode(node);
        temp = successor->value;
        node->value = temp;
        
        deleteNode(successor);
    }
    
    else if (node->lchild != NULL || node->rchild != NULL) {
        if (node->lchild != NULL)
            nextNode = node->lchild;
            
        else 
            nextNode = node->rchild;
        
        if (node->parent != NULL) {
            
            if (node->parent->lchild == node) {
                
                node->parent->lchild = nextNode;
            }
            else {
                
                node->parent->rchild = nextNode;
            }
            
            nextNode->parent = node->parent;
            updateNodeCount(node->parent, -1);
            
        }
        
        else {
            
            rootNode = nextNode;
            nextNode->parent = NULL;
        }
        
        if (node->nodeColor == BLACK)
                nextNode->nodeColor = BLACK;
                
        updateRedNodeCount(nextNode, -1);        
        node->parent = NULL;    
        free(node);
    }
    
    else {
        
        if (node->nodeColor == RED) {
            
            if (node->parent->lchild == node)
                node->parent->lchild = NULL;
            
            else
                node->parent->rchild = NULL;
            
            updateNodeCount(node->parent, -1);
            updateRedNodeCount(node->parent, -1);
            
            node->parent = NULL;
            
            free(node);
        }
        
        else {
            if (rootNode == node) {
                
                rootNode = NULL;
                free(node);
                return SUCCESS;
            }
           
            parentNode = node->parent;
            if (parentNode->lchild == node) {
                parentNode->lchild = NULL;
                sibling = parentNode->rchild;
            }
            
            else {
                parentNode->rchild = NULL;
                sibling = parentNode->lchild;
            }
            
            updateNodeCount (parentNode, -1);
            node->parent = NULL;
            free(node);
            
            deletionCases(parentNode, sibling);
        }
    }
    
    return SUCCESS;
}

void deletionCases(RBT * parentNode, RBT * sibling) {
    
    if (parentNode->nodeColor == RED) {
        
        /*Case 1*/
        if ((sibling == NULL) || ((sibling->lchild == NULL || sibling->lchild->nodeColor == BLACK) && (sibling->rchild == NULL || sibling->rchild->nodeColor == BLACK))) {
            deletionCase1_2(parentNode, sibling);
        }
        else {
            deletionCase1_1(parentNode, sibling);
        }
    }
    
    else if (sibling != NULL && sibling->nodeColor == RED) {
        
        /*Case 2_1*/
        
        if ((sibling->lchild == NULL || sibling->lchild->nodeColor == BLACK) && (sibling->rchild == NULL || sibling->rchild->nodeColor == BLACK)) {
            
            /*Case 2_1_2*/
            deletionCase2_1_2 (parentNode, sibling);
        }
        
        else {
            
            /*Case 2_1_1*/
            deletionCase2_1_1(parentNode, sibling);
        }
    }
    
    else {
        
        /*Case 2_2*/
        if (sibling == NULL) {
            /*Something must be done to hieght*/
            return;
        }
            
        
        if ((sibling->lchild == NULL || sibling->lchild->nodeColor == BLACK) && (sibling->rchild == NULL || sibling->rchild->nodeColor == BLACK)) {
            
            /*Case 2_2_2*/
            deletionCase2_2_2(parentNode, sibling);
        }
        
        else {
            
            /*Case 2_2_1*/
            deletionCase2_2_1(parentNode, sibling);
        }
    }
}

void deletionCase1_1(RBT * parentNode, RBT * sibling) {
    
    RBT * grandChild;
    
    if (parentNode->lchild == sibling) {
        
        if (sibling->lchild != NULL && sibling->lchild->nodeColor == RED) {
            
            sibling->nodeColor = RED;
            sibling->lchild->nodeColor = BLACK;
            parentNode->nodeColor = BLACK;
            
            updateRedNodeCount(sibling->lchild, -1);
            updateRedNodeCount(parentNode, -1);
            updateRedNodeCount(sibling, 1);
            
            parentNode->nodes_in_sub += ((sibling->rchild != NULL ? sibling->rchild->nodes_in_sub : 0) - sibling->nodes_in_sub);
            sibling->nodes_in_sub += (parentNode->nodes_in_sub - (sibling->rchild != NULL ? sibling->rchild->nodes_in_sub : 0));
            
            parentNode->red_nodes_in_sub += ((sibling->rchild != NULL ? sibling->rchild->red_nodes_in_sub : 0) - sibling->red_nodes_in_sub);
            sibling->red_nodes_in_sub += (parentNode->red_nodes_in_sub - (sibling->rchild != NULL ? sibling->rchild->red_nodes_in_sub : 0));
            
            
            if(parentNode == rootNode)
                rootNode = sibling;
            
            leftLeftCase(parentNode, sibling);
        }
        
        else {
            
            grandChild = sibling->rchild;
            parentNode->nodeColor = BLACK;
            
            updateRedNodeCount(parentNode, -1);
            
            parentNode->nodes_in_sub += ((grandChild->rchild != NULL ? grandChild->rchild->nodes_in_sub : 0) - sibling->nodes_in_sub);
            sibling->nodes_in_sub += ((grandChild->lchild != NULL ? grandChild->lchild->nodes_in_sub : 0) - grandChild->nodes_in_sub);
            grandChild->nodes_in_sub = parentNode->nodes_in_sub + sibling->nodes_in_sub + 1;
            
            parentNode->red_nodes_in_sub += ((grandChild->rchild != NULL ? grandChild->rchild->red_nodes_in_sub : 0) - sibling->red_nodes_in_sub);
            sibling->red_nodes_in_sub += ((grandChild->lchild != NULL ? grandChild->lchild->red_nodes_in_sub : 0) - grandChild->red_nodes_in_sub);
            grandChild->red_nodes_in_sub = parentNode->red_nodes_in_sub + sibling->red_nodes_in_sub + 1;
            
            
            if(parentNode == rootNode)
                rootNode = grandChild;
            
            leftRightCase(parentNode, sibling);
        }
    }
    
    else {
        
        if (sibling->rchild != NULL && sibling->rchild->nodeColor == RED) {
            
            parentNode->nodeColor = BLACK;
            sibling->rchild->nodeColor = BLACK;
            sibling->nodeColor = RED;
            
            updateRedNodeCount(parentNode, -1);
            updateRedNodeCount(sibling->rchild, -1);
            updateRedNodeCount(sibling, 1);
            
            parentNode->nodes_in_sub += ((sibling->lchild != NULL ? sibling->lchild->nodes_in_sub : 0) - sibling->nodes_in_sub);
            sibling->nodes_in_sub += (parentNode->nodes_in_sub - (sibling->lchild != NULL ? sibling->lchild->nodes_in_sub : 0));

            parentNode->red_nodes_in_sub += ((sibling->lchild != NULL ? sibling->lchild->red_nodes_in_sub : 0) - sibling->red_nodes_in_sub);
            sibling->red_nodes_in_sub += (parentNode->red_nodes_in_sub - (sibling->lchild != NULL ? sibling->lchild->red_nodes_in_sub : 0));

            
            if(parentNode == rootNode)
                rootNode = sibling;
                
            rightRightCase(parentNode, sibling);
        }
        
        else {
            
            grandChild = sibling->lchild;
            parentNode->nodeColor = BLACK;
            
            updateRedNodeCount(parentNode, -1);
            
            parentNode->nodes_in_sub += ((grandChild->lchild != NULL ? grandChild->lchild->nodes_in_sub : 0) - sibling->nodes_in_sub);
            sibling->nodes_in_sub += ((grandChild->rchild != NULL ? grandChild->rchild->nodes_in_sub : 0) - grandChild->nodes_in_sub);
            grandChild->nodes_in_sub = parentNode->nodes_in_sub + sibling->nodes_in_sub + 1;
            
            parentNode->red_nodes_in_sub += ((grandChild->lchild != NULL ? grandChild->lchild->red_nodes_in_sub : 0) - sibling->red_nodes_in_sub);
            sibling->red_nodes_in_sub += ((grandChild->rchild != NULL ? grandChild->rchild->red_nodes_in_sub : 0) - grandChild->red_nodes_in_sub);
            grandChild->red_nodes_in_sub = parentNode->red_nodes_in_sub + sibling->red_nodes_in_sub + 1;
            
            
            if(parentNode == rootNode)
                rootNode = grandChild;
            
            rightLeftCase(parentNode, sibling);
        }
    }
}

void deletionCase1_2(RBT * parentNode, RBT * sibling) {
    
    if(sibling != NULL){
        sibling->nodeColor = RED;
        updateRedNodeCount(sibling, 1);
    }
    parentNode->nodeColor = BLACK;
    
    updateRedNodeCount(parentNode, -1);
    
}

void deletionCase2_1_1(RBT * parentNode, RBT * sibling) {
    
    RBT * grandChild;
    RBT * greatGrandChild;
    if (sibling == parentNode->lchild) {
        
        grandChild = sibling->rchild;
        
        if (grandChild->lchild != NULL && grandChild->lchild->nodeColor == RED) {
            
            greatGrandChild = grandChild->lchild;
            greatGrandChild->nodeColor = BLACK;
            
            updateRedNodeCount(greatGrandChild, -1);
            
            parentNode->nodes_in_sub += ((grandChild->rchild != NULL ? grandChild->rchild->nodes_in_sub : 0) - sibling->nodes_in_sub);
            sibling->nodes_in_sub += (greatGrandChild->nodes_in_sub - grandChild->nodes_in_sub);
            grandChild->nodes_in_sub = parentNode->nodes_in_sub + sibling->nodes_in_sub + 1;
            

            parentNode->red_nodes_in_sub += ((grandChild->rchild != NULL ? grandChild->rchild->red_nodes_in_sub : 0) - sibling->red_nodes_in_sub);
            sibling->red_nodes_in_sub += (greatGrandChild->red_nodes_in_sub - grandChild->red_nodes_in_sub);
            grandChild->red_nodes_in_sub = parentNode->red_nodes_in_sub + sibling->red_nodes_in_sub + 1;

            parentNode->lchild = grandChild;
            grandChild->parent = parentNode;
            sibling->parent = grandChild;
            sibling->rchild = greatGrandChild;
            greatGrandChild->parent = sibling;
            grandChild->lchild = sibling;
            
            if(parentNode == rootNode)
                rootNode = grandChild;
            
            leftLeftCase(parentNode, grandChild);
        }
        
        else {
            
            greatGrandChild = grandChild->rchild;
            greatGrandChild->nodeColor = BLACK;
            
            updateRedNodeCount(greatGrandChild, -1);
            
            parentNode->nodes_in_sub += ((greatGrandChild->rchild != NULL ? greatGrandChild->rchild->nodes_in_sub : 0) - sibling->nodes_in_sub);
            sibling->nodes_in_sub -= grandChild->nodes_in_sub;
            grandChild->nodes_in_sub += ((greatGrandChild->lchild != NULL ? greatGrandChild->lchild->nodes_in_sub : 0) - greatGrandChild->nodes_in_sub);
            sibling->nodes_in_sub += grandChild->nodes_in_sub;
            greatGrandChild->nodes_in_sub = parentNode->nodes_in_sub + sibling->nodes_in_sub + 1;
            
            parentNode->red_nodes_in_sub += ((greatGrandChild->rchild != NULL ? greatGrandChild->rchild->red_nodes_in_sub : 0) - sibling->red_nodes_in_sub);
            sibling->red_nodes_in_sub -= grandChild->red_nodes_in_sub;
            grandChild->red_nodes_in_sub += ((greatGrandChild->lchild != NULL ? greatGrandChild->lchild->red_nodes_in_sub : 0) - greatGrandChild->red_nodes_in_sub);
            sibling->red_nodes_in_sub += grandChild->red_nodes_in_sub;
            greatGrandChild->red_nodes_in_sub = parentNode->red_nodes_in_sub + sibling->red_nodes_in_sub + 1;


            grandChild->rchild = greatGrandChild->lchild;
            greatGrandChild->lchild->parent = grandChild;
            
            parentNode->lchild = greatGrandChild;
            greatGrandChild->parent = parentNode;
            
            greatGrandChild->lchild = sibling;
            sibling->parent = greatGrandChild;
            
            if(parentNode == rootNode)
                rootNode = greatGrandChild;
            
            leftLeftCase(parentNode, greatGrandChild);
        }
    }
    
    else {
        
        grandChild = sibling->lchild;
        
        if (grandChild->rchild != NULL && grandChild->rchild->nodeColor == RED) {
            
            greatGrandChild = grandChild->rchild;
            greatGrandChild->nodeColor = BLACK;
            
            updateRedNodeCount(greatGrandChild, -1);
            
            parentNode->nodes_in_sub += ((grandChild->lchild != NULL ? grandChild->lchild->nodes_in_sub : 0) - sibling->nodes_in_sub);
            sibling->nodes_in_sub += (greatGrandChild->nodes_in_sub - grandChild->nodes_in_sub);
            grandChild->nodes_in_sub = parentNode->nodes_in_sub + sibling->nodes_in_sub + 1;
            

            parentNode->red_nodes_in_sub += ((grandChild->lchild != NULL ? grandChild->lchild->red_nodes_in_sub : 0) - sibling->red_nodes_in_sub);
            sibling->red_nodes_in_sub += (greatGrandChild->red_nodes_in_sub - grandChild->red_nodes_in_sub);
            grandChild->red_nodes_in_sub = parentNode->red_nodes_in_sub + sibling->red_nodes_in_sub + 1;

            parentNode->rchild = grandChild;
            grandChild->parent = parentNode;
            sibling->parent = grandChild;
            sibling->lchild = greatGrandChild;
            greatGrandChild->parent = sibling;
            grandChild->rchild = sibling;
            
            if(parentNode == rootNode)
                rootNode = grandChild;
            
            rightRightCase(parentNode, grandChild);
        }
        
        else {
            
            greatGrandChild = grandChild->lchild;
            greatGrandChild->nodeColor = BLACK;
            
            updateRedNodeCount(greatGrandChild, -1);
            
            parentNode->nodes_in_sub += ((greatGrandChild->lchild != NULL ? greatGrandChild->lchild->nodes_in_sub : 0) - sibling->nodes_in_sub);
            sibling->nodes_in_sub -= grandChild->nodes_in_sub;
            grandChild->nodes_in_sub += ((greatGrandChild->rchild != NULL ? greatGrandChild->rchild->nodes_in_sub : 0) - greatGrandChild->nodes_in_sub);
            sibling->nodes_in_sub += grandChild->nodes_in_sub;
            greatGrandChild->nodes_in_sub = parentNode->nodes_in_sub + sibling->nodes_in_sub + 1;
            
            
            parentNode->red_nodes_in_sub += ((greatGrandChild->lchild != NULL ? greatGrandChild->lchild->red_nodes_in_sub : 0) - sibling->red_nodes_in_sub);
            sibling->red_nodes_in_sub -= grandChild->red_nodes_in_sub;
            grandChild->red_nodes_in_sub += ((greatGrandChild->rchild != NULL ? greatGrandChild->rchild->red_nodes_in_sub : 0) - greatGrandChild->red_nodes_in_sub);
            sibling->red_nodes_in_sub += grandChild->red_nodes_in_sub;
            greatGrandChild->red_nodes_in_sub = parentNode->red_nodes_in_sub + sibling->red_nodes_in_sub + 1;
            
            
            grandChild->lchild = greatGrandChild->rchild;
            greatGrandChild->rchild->parent = grandChild;
            
            parentNode->rchild = greatGrandChild;
            greatGrandChild->parent = parentNode;
            
            greatGrandChild->rchild = sibling;
            sibling->parent = greatGrandChild;
            
            if(parentNode == rootNode)
                rootNode = greatGrandChild;
            
            rightRightCase(parentNode, greatGrandChild);
        }
    }
}

void deletionCase2_1_2(RBT * parentNode, RBT * sibling) {
    
    if(parentNode == rootNode)
            rootNode = sibling;
            
    if (parentNode->lchild == sibling) {
        
        sibling->nodeColor = BLACK;
        sibling->rchild->nodeColor = RED;
        
        updateRedNodeCount(sibling, -1);
        updateRedNodeCount(sibling->rchild, 1);
        /* Update the nodes count*/
        parentNode->nodes_in_sub += ((sibling->rchild != NULL ? sibling->rchild->nodes_in_sub : 0) - sibling->nodes_in_sub);
        sibling->nodes_in_sub += (parentNode->nodes_in_sub - (sibling->rchild != NULL ? sibling->rchild->nodes_in_sub : 0));
        
        parentNode->red_nodes_in_sub += ((sibling->rchild != NULL ? sibling->rchild->red_nodes_in_sub : 0) - sibling->red_nodes_in_sub);
        sibling->red_nodes_in_sub += (parentNode->red_nodes_in_sub - (sibling->rchild != NULL ? sibling->rchild->red_nodes_in_sub : 0));


        leftLeftCase(parentNode, sibling);
    }
    
    else {
        
        sibling->nodeColor = BLACK;
        sibling->lchild->nodeColor = RED;
        
        updateRedNodeCount(sibling, -1);
        updateRedNodeCount(sibling->lchild, 1);
        /* Update the nodes count*/
        parentNode->nodes_in_sub += ((sibling->lchild != NULL ? sibling->lchild->nodes_in_sub : 0) - sibling->nodes_in_sub);
        sibling->nodes_in_sub += (parentNode->nodes_in_sub - (sibling->lchild != NULL ? sibling->lchild->nodes_in_sub : 0));
        
        parentNode->red_nodes_in_sub += ((sibling->lchild != NULL ? sibling->lchild->red_nodes_in_sub : 0) - sibling->red_nodes_in_sub);
        sibling->red_nodes_in_sub += (parentNode->red_nodes_in_sub - (sibling->lchild != NULL ? sibling->lchild->red_nodes_in_sub : 0));
        
        
        rightRightCase(parentNode, sibling);
    }
}

void deletionCase2_2_1(RBT * parentNode, RBT * sibling) {
    
    RBT * grandChild;
    
    if(sibling == parentNode->lchild) {
        if (sibling->lchild != NULL && sibling->lchild->nodeColor == RED) {
            
            grandChild = sibling->lchild;
            grandChild->nodeColor = BLACK;
            
            updateRedNodeCount(grandChild, -1);
            
            parentNode->nodes_in_sub += ((sibling->rchild != NULL ? sibling->rchild->nodes_in_sub : 0) - sibling->nodes_in_sub);
            sibling->nodes_in_sub += (parentNode->nodes_in_sub - (sibling->rchild != NULL ? sibling->rchild->nodes_in_sub : 0));
            
            parentNode->red_nodes_in_sub += ((sibling->rchild != NULL ? sibling->rchild->red_nodes_in_sub : 0) - sibling->red_nodes_in_sub);
            sibling->red_nodes_in_sub += (parentNode->red_nodes_in_sub - (sibling->rchild != NULL ? sibling->rchild->red_nodes_in_sub : 0));


            if(parentNode == rootNode)
                rootNode = sibling;
            
            leftLeftCase(parentNode, sibling);
        }
        
        else {
            
            grandChild = sibling->rchild;
            grandChild->nodeColor = BLACK;
            
            updateRedNodeCount(grandChild, -1);
            
            parentNode->nodes_in_sub += ((grandChild->rchild != NULL ? grandChild->rchild->nodes_in_sub : 0) - sibling->nodes_in_sub);
            sibling->nodes_in_sub += ((grandChild->lchild != NULL ? grandChild->lchild->nodes_in_sub : 0) - grandChild->nodes_in_sub);
            grandChild->nodes_in_sub = parentNode->nodes_in_sub + sibling->nodes_in_sub + 1;

            parentNode->red_nodes_in_sub += ((grandChild->rchild != NULL ? grandChild->rchild->red_nodes_in_sub : 0) - sibling->red_nodes_in_sub);
            sibling->red_nodes_in_sub += ((grandChild->lchild != NULL ? grandChild->lchild->red_nodes_in_sub : 0) - grandChild->red_nodes_in_sub);
            grandChild->red_nodes_in_sub = parentNode->red_nodes_in_sub + sibling->red_nodes_in_sub + 1;

            
            if(parentNode == rootNode)
                rootNode = grandChild;
            
            leftRightCase(parentNode, sibling);
        }
    }
    
    else {
        
        if (sibling->rchild != NULL && sibling->rchild->nodeColor == RED) {
            
            grandChild = sibling->rchild;
            grandChild->nodeColor = BLACK;
            
            updateRedNodeCount(grandChild, -1);
            
            parentNode->nodes_in_sub += ((sibling->lchild != NULL ? sibling->lchild->nodes_in_sub : 0) - sibling->nodes_in_sub);
            sibling->nodes_in_sub += (parentNode->nodes_in_sub - (sibling->lchild != NULL ? sibling->lchild->nodes_in_sub : 0));
            
            parentNode->red_nodes_in_sub += ((sibling->lchild != NULL ? sibling->lchild->red_nodes_in_sub : 0) - sibling->red_nodes_in_sub);
            sibling->red_nodes_in_sub += (parentNode->red_nodes_in_sub - (sibling->lchild != NULL ? sibling->lchild->red_nodes_in_sub : 0));


            if(parentNode == rootNode)
                rootNode = sibling;
            
            rightRightCase(parentNode, sibling);
        }
        
        else {
            
            grandChild = sibling->lchild;
            grandChild->nodeColor = BLACK;
            
            updateRedNodeCount(grandChild, -1);
            
            parentNode->nodes_in_sub += ((grandChild->lchild != NULL ? grandChild->lchild->nodes_in_sub : 0) - sibling->nodes_in_sub);
            sibling->nodes_in_sub += ((grandChild->rchild != NULL ? grandChild->rchild->nodes_in_sub : 0) - grandChild->nodes_in_sub);
            grandChild->nodes_in_sub = parentNode->nodes_in_sub + sibling->nodes_in_sub + 1;


            parentNode->red_nodes_in_sub += ((grandChild->lchild != NULL ? grandChild->lchild->red_nodes_in_sub : 0) - sibling->red_nodes_in_sub);
            sibling->red_nodes_in_sub += ((grandChild->rchild != NULL ? grandChild->rchild->red_nodes_in_sub : 0) - grandChild->red_nodes_in_sub);
            grandChild->red_nodes_in_sub = parentNode->red_nodes_in_sub + sibling->red_nodes_in_sub + 1;
            
            
            if(parentNode == rootNode)
                rootNode = grandChild;
            
            rightLeftCase(parentNode, sibling);
        }
    }
}

void deletionCase2_2_2(RBT * parentNode, RBT * sibling) {
    
    RBT * temp;
    sibling->nodeColor = RED;
    
    updateRedNodeCount(sibling, 1);
    
    if (parentNode != NULL && parentNode->parent != NULL) {
        
        temp = parentNode;        
        parentNode = parentNode->parent;
        
        if(temp == parentNode->lchild)
            sibling = parentNode->rchild;
        
        else
            sibling = parentNode->lchild;
        
        deletionCases(parentNode, sibling);
    }
}

void preorderTraversal(RBT * node) {
    
    if (node == NULL) {
        
        return;
    }
    printf("%d :%d", node->value, node->nodes_in_sub);
    if(node->nodeColor == RED)
        printf("R\n");
    
    else
        printf("B\n");

    if(node->lchild != NULL)
        preorderTraversal(node->lchild);
    
    if(node->rchild != NULL)
        preorderTraversal(node->rchild);
}

void inorderTraversal(RBT * node) {
    
    if(node->lchild != NULL)
        inorderTraversal(node->lchild);
    
    if (node == NULL) {        
        return;
    }
    
    printf("%d :%d :%d", node->value, node->nodes_in_sub, node->red_nodes_in_sub);
    if(node->nodeColor == RED)
        printf("R\n");
    
    else
        printf("B\n");

    
    if(node->rchild != NULL)
        inorderTraversal(node->rchild);
}

void postorderTraversal(RBT * node) {
    
    if(node->lchild != NULL)
        postorderTraversal(node->lchild);
    
    if(node->rchild != NULL)
        postorderTraversal(node->rchild);

    if (node == NULL) {    
        return;
    }
    printf("%d :%d", node->value, node->nodes_in_sub);
    if(node->nodeColor == RED)
        printf("R\n");
    
    else
        printf("B\n");
}
