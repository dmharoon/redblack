#include<stdlib.h>
#include<stdio.h>

#include "RedBlackTree.h"

RBT * rootNode;

int main() {
    
    int count;
    int opType, value;
    RBT * result;
    int status;
    rootNode = NULL;
    FILE *fptr_1 = fopen("search_output", "w");
    FILE *fptr_2 = fopen("rank_output", "w");
    FILE *fptr_3 = fopen("findRank_output", "w");
    FILE *fptr_4 = fopen("insert_output", "w");
    FILE *fptr_5 = fopen("delete_output", "w");
    

    scanf("%d", &count);
    
    while(count > 0) {
        
        scanf("%d%d", &opType, &value);
        count--;
        switch(opType) {
            
            case 0:
                
                result = searchNodeByKey(rootNode, value);
                
                if (result == NULL)
                    fprintf(fptr_1, "%d Not found\n", value);
                
                else if (result->value != value)
                    fprintf(fptr_1, "%d Not found\n", value);
                    
                else
                    fprintf(fptr_1, "%d found\n", value);
                 break;   

            case 1:
      
                status = createRBTNode (&result, value);
                if (status == FAILURE) {
                    
                    fprintf(fptr_4, "Can't create node\n");
                    continue;
                }
                status = insertNode(rootNode, result);
                
                if (status == DUPLICATE || status == FAILURE) {
                    
                    fprintf(fptr_4, "Duplicate values not allowed %d\n", value);
                    continue;
                }
                fprintf(fptr_4,"%d Node inserted\n", value);
                //printf("In-order\n");
                //inorderTraversal(rootNode);
        
               break; 

            case 2:
                
                result = searchNodeByKey(rootNode, value);
                
                if (result == NULL || result->value != value)
                    fprintf(fptr_5, "%d No such node in tree\n", value);
                    
                else {
                    
                    status = deleteNode(result);
                    
                    if(status == SUCCESS)
                        fprintf(fptr_5, "%d deleted\n", value);
                    
                    else 
                        fprintf(fptr_5, "Error deleting %d \n", value);
                }
                //printf("In-order\n");
                //inorderTraversal(rootNode);
    
               break; 

             case 3:
             
                status = redRank(rootNode, value, 0);
                
                if (status == -1) {
                    
                    fprintf(fptr_2, "%d : No such element\n", value);
                    }
                else {
                    
                    fprintf(fptr_2, "%d : rank : %d \n", value, status);
                }
                break;

             case 4:
                
                status = findRank(rootNode, value, 0);
                
                if (status == -1)
                    fprintf(fptr_3, "Rank %d : Element Not found\n", value);
                    
                else
                    fprintf(fptr_3, "Rank %d : Element : %d\n", value, status);    
                    
                break;
        }
    }
    
    //printf("In-order\n");
    //inorderTraversal(rootNode);
    //printf("Pre-order\n");
    //preorderTraversal(rootNode);
    printf("Number of nodes: %d\n", rootNode->nodes_in_sub);
    printf("Height of tree: %d \n", heightOfNode(rootNode));
    
    close(fptr_1);
    close(fptr_2);
    close(fptr_3);
    
    return SUCCESS;
}
